#include "graphe.h"

static arete swap_sommets(arete a);

void init_graphe(graphe *g, FILE *fichier, char *nom)
{
    fichier = fopen(nom, "r");
    if (fichier == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE); // permet d'arreter le code sans que ça fasse un [1]    1759 segmentation fault  ./main
                            // En vrai c'est useless mais c'est pour garder le code clean ~Antoine
    }
    // On fait l'hypothèse qu'il peut y avoir que des switch ou que des stations donc on fait on malloc du nombre de machines
    // Pas optimisé, mais ce n'est pas ce qu'on cherche ~Antoine
    fscanf(fichier, "%lu %u", &g->ordre, &g->aretes_capacite);
    g->swListe = calloc(g->ordre, sizeof(Switch)); // Ce qui nous permettra de stocker toutes les stations et switchs pour les retrouver
    g->stListe = calloc(g->ordre, sizeof(station));
    g->nb_stations = 0;
    g->nb_switch = 0;
    g->nb_aretes = 0;
    g->conditionArret = false;
    for (size_t i = 0; i < g->ordre; i++)
    {
        // exemple :
        /*
        2;01:45:23:a6:f7:ab;8;1024
        1;54:d6:a6:82:c5:23;130.79.80.21
        */
        int tmp; // pour connaitre le type (A RENOMMER POV JAI LA FLEMME IL EST 1H DU MAT ET ❤️ LE C)
        fscanf(fichier, "%d;", &tmp);
        if (tmp == 2) // 2 : switch (le plus dur). Je commence par 2 vu que dans le fichier config les switchs sont toujours en premier (aucun rapport)
        {
            // Switch *sw = malloc(sizeof(Switch));
            Switch *sw = g->swListe + g->nb_switch;
            fscanf(fichier, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx;%d;%d", &sw->_mac._Macadr[0], &sw->_mac._Macadr[1], &sw->_mac._Macadr[2],
                   &sw->_mac._Macadr[3], &sw->_mac._Macadr[4], &sw->_mac._Macadr[5], &sw->_nbre_ports, &sw->_priorite); // Initialise addr mac, nbre de ports et priorite
            printf("TEST %x\n", sw->_mac._Macadr[0]);
            sw->_tableCommutation = NULL;
            // NULL vu qu'on initialise juste le graphe
            sw->_longueurTable = 0;
            // 0 vu qu'on initnialise juste le grapge
            // g->swListe[g->nb_switch] = *sw;
            // Ajoute le nom su switch
            // char nom[STATION_NOM_LEN] = "sw"; + g->nb_switch (impossible en c lol, sinon ça serait trop beau pour être vrai);
            //  pas dérangeant de mettre STATION_NOM vu qu'il est de 255 de meme pour le nom de switch
            sprintf(g->swListe[g->nb_switch]._nom, "sw%ld", g->nb_switch + 1);
            // On le met a son emplacement dans notre liste
            g->nb_switch++;
            // Et on incremente pour le malloc pour tout retrouver apres
            // free(sw);
        }
        if (tmp == 1)
        {
            station *st = malloc(sizeof(station));
            fscanf(fichier, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx;%hhu.%hhu.%hhu.%hhu", &st->_mac._Macadr[0], &st->_mac._Macadr[1], &st->_mac._Macadr[2],
                   &st->_mac._Macadr[3], &st->_mac._Macadr[4], &st->_mac._Macadr[5], &st->_ip._IPadr[0], &st->_ip._IPadr[1],
                   &st->_ip._IPadr[2], &st->_ip._IPadr[3]);
            // Buffer qu'on a appris en S21, comme un strcpy mais avec des int etc... en gros strcpy = surcoté
            g->stListe[g->nb_stations] = *st;
            sprintf(g->stListe[g->nb_stations]._nom, "st%ld", g->nb_stations + 1);
            g->nb_stations++;
            free(st);
        }
    }
    /*Allocation pour la table de commutation des switchs*/
    for (size_t i = 0; i < g->nb_switch; i++)
    {
        g->swListe[i]._tableCommutation = calloc(g->ordre, sizeof(TableCommutation)); // On alloue pour la table de commutation
    }

    // On passe a l'init. des aretes. Vu que strasbourg est écologique (la grosse blague)
    // on réutilise ajouter_arete(graphe *g, arete a) pour être ecoresponsable.💚 (vert pour l'ecologie)
    /*0;1;4
    0;2;19      Exemple pour ne pas avoir de faire d'alt tab toutes les 2s
    0;3;4*/
    g->aretes = malloc(g->aretes_capacite * sizeof(arete));
    for (int i = 0; i < g->aretes_capacite; i++)
    {
        arete a;
        sommet s1;
        sommet s2;
        cout c;
        arete b;
        fscanf(fichier, "%ld;%ld;%ld", &s1, &s2, &c);
        a.s1 = s1;
        a.s2 = s2;
        a.c = c;
        // Swap si la sommet 1 est plus grand que le sommet 2, ça permet de bien voir les aretes dans l'affichage (on par du switch jusq'a la station)
        if (a.s1 > a.s2)
        {
            b = swap_sommets(a);
            ajouter_arete(g, b);
        }
        else
        {
            ajouter_arete(g, a);
        }
    }

    for (size_t i = 0; i < g->nb_switch; i++)
    {
        g->swListe[i]._sommetArrive = -1;
    }

    fclose(fichier);
}

void afficherTMP(graphe *g)
{
    printf("---SWITCH---\n");
    for (size_t i = 0; i < g->nb_switch; i++)
    {
        printf("%ld. %s\n", i, g->swListe[i]._nom);
        afficherAdrMac(&g->swListe[i]._mac);
        printf("Priorite : %d\n", g->swListe[i]._priorite);
        printf("Nombre de ports : %d\n", g->swListe[i]._nbre_ports);
    }
    printf("---STATION---\n");
    for (size_t i = 0; i < g->nb_stations; i++)
    {
        printf("%ld. %s\n", i + g->nb_switch, g->stListe[i]._nom);
        afficherAdrMac(&g->stListe[i]._mac);
        afficherAdrIP(&g->stListe[i]._ip);
    }
    printf("--ARETES : COUT--\n");
    for (size_t i = 0; i < nb_aretes(g); i++)
    {
        printf("%ld - %ld : %ld\n", g->aretes[i].s1, g->aretes[i].s2, g->aretes[i].c);
    }
}

void free_graphe(graphe *g)
{
    // libère la mémoire qui avait été allouée dans la fonction init_graphe
    // réinitialise les champs internes du graphe g
    for (size_t i = 0; i < g->nb_switch; i++)
    {
        free(g->swListe[i]._tableCommutation);
    }
    free(g->swListe);
    free(g->stListe);
    free(g->aretes);

    /*for (int i = 0; i < (int)g->nb_switch; i++)
    {
        free(g->swListe[i]._tableCommutation);
    }

    free(g->stListe);
    free(g->swListe);
    free(g->aretes);

    g->stListe = NULL;
    g->swListe = NULL;
    g->aretes = NULL;

    g->aretes_capacite = 0;
    g->ordre = 0;
    g->nb_aretes = 0;
    g->nb_switch = 0;
    g->nb_stations = 0;*/
    /*
    arete *aretes;*/
}

size_t switchDebutTrame(graphe *g, station *st)
{
    int numstation = index_Station(g, st);
    int numswitch;
    for (size_t i = 0; i < g->ordre; i++)
    {
        numswitch = index_Switch(g, &g->swListe[i]);
        if ((int)g->aretes[i].s1 == numstation)
        {
            return numswitch; // peut etre faux
        }
    }
    return -1;
}

void getStationAvecIndice(graphe *g, int indice, station *st)
{
    *st = g->stListe[indice];
}

void getStationAvecMAC(graphe *g, adrMac *mac, station *st)
{
    for (size_t i = 0; i < g->nb_stations; i++)
    {
        if (macEquals(&g->stListe[i]._mac, mac) == 1)
        {
            *st = g->stListe[i];
        }
    }
}

/*void envoyerTrame(trame *tr, graphe *g)
{
    station stTMP;
    int indicePremier = (int)index_MAC(g, &tr->source);
    // getSwitchAvecIndice(g, indicePremier, swTMP);
    getStationAvecIndice(g, indicePremier, &stTMP);
    int premierswitch = switchDebutTrame(g, &stTMP);
    printf("Le chemin commence par le switch : %d\n", premierswitch);
    for (int i = 0; i < (int)g->nb_switch; i++)
    {
        for (int j = 0; j < (int)g->nb_aretes; j++)
        {
        }
    }
}*/
/*void envoyerTrame(trame *tr, graphe *g)
{
    station st;
    station stDestination;
    getStationAvecMAC(g, &tr->source, &st);
    getStationAvecMAC(g, &tr->destination, &stDestination);
    int SwitchPassage = switchDebutTrame(g, &st);
    for (int i = 0; i < (int)g->ordre; i++) // Tous les appareils du graphe
    {
        for (int j = 0; j < g->swListe[i]._longueurTable; j++) // On verifie que pour le switch actuelle, il n'y a pas de trame déjà existante
        {
            if (!existeDansTable(&g->swListe[j], &tr->source))
            {
                for (int x = 0; x < g->swListe[j]._longueurTable; x++)
                {
                    if (estNULL(&g->swListe[j]._tableCommutation[x]._mac))
                    {
                        cpyAdrMac(&g->swListe[j]._tableCommutation[x]._mac, &tr->source);
                        g->swListe[j]._longueurTable++;
                    }
                }
            }
        }
    }
    if (g->nb_switch == 1)
{
    for (int i = 0; i < g->swListe->_nbre_ports; i++)
    {
        if (estNULL(&g->swListe[0]._tableCommutation[i]._mac) == 1)
        {
            cpyAdrMac(&g->swListe[0]._tableCommutation[i]._mac, &tr->source);
            g->swListe[0]._longueurTable++;
            break;
        }
    }
}

    afficherTableCommutation(&g->swListe[0]);
}*/

void envoyerTrame(trame *tr, graphe *g)
{
    printf("######################\n");
    printf("#  DEBUT DE L'ENVOIE #\n");
    printf("######################\n");

    station stSource;
    getStationAvecMAC(g, &tr->source, &stSource); // On recupere la station source du message
    station stDestination;
    getStationAvecMAC(g, &tr->destination, &stDestination); // Recup la station de dest (au cas ou)

    int SwitchPassage = index_Station(g, &stSource);
    int StationDebut = index_Station(g, &stSource) + g->nb_switch;
    sommet tmp[g->nb_switch];
    sommets_adjacents(g, SwitchPassage + g->nb_switch, tmp);
    SwitchPassage = tmp[0];
    // Pour chaque tout dans le graphe y compris station
    sommet switchInterroge;                                            // Sert a connaitre le switch actuel interroge
    size_t nbresaSwitchInterroge;                                      // Permet de connaitre le nombre de switch/Station adjacente
    switchInterroge = index_Switch(g, &g->swListe[SwitchPassage]);     // On recup l'index du switch interroge
    sommet saSwitchInterroge[g->swListe[switchInterroge]._nbre_ports]; // Sommets adjacent du switch interrogé (ici premier switch apres la station)
    setTrameInSwitch(&g->swListe[switchInterroge], tr);                // On dépose la trame de stSource -> prochain switch
    printf("Num switch : %ld\n", switchInterroge);
    sommet switchFin = index_MAC(g, &tr->destination); // On active le mode dieu pour JUSTE print le message de "je vais ver %ld"
    printf("Je pars du switch %ld!\nJe vais vers la station %ld\n", switchInterroge, switchFin + g->nb_switch);
    for (int j = 0; j < g->swListe[switchInterroge]._nbre_ports; j++) // Pour le nombre de ports
    {
        // On le creer si il n'est pas dans la table de comm
        if (!existeDansTable(&g->swListe[switchInterroge], &stSource._mac) && estNULL(&g->swListe[switchInterroge]._tableCommutation[j]._mac))
        {
            cpyAdrMac(&g->swListe[switchInterroge]._tableCommutation[j]._mac, &tr->source); // On rajoute dans la table de commutation l'adresse mac source
            g->swListe[switchInterroge]._tableCommutation[j]._port = StationDebut;          // Et le switch de passage pour y acceder (système de port simplifié)
            g->swListe[switchInterroge]._sommetArrive = switchInterroge;                    // on met l'index du switch d'arrive pour ne pas retourner dessus et faire de boucles infini
            g->swListe[switchInterroge]._longueurTable++;                                   // vu qu'on rajoute une donnee dans la table de comm, on incremente son compteur
            printf("Je suis à %ld\n", switchInterroge);
            break; // On break vu que ça ne sert plus a rien de retourner de nouveau dans la tabke
        }
    }
    nbresaSwitchInterroge = sommets_adjacents(g, switchInterroge, saSwitchInterroge);
    for (size_t z = 0; z < nbresaSwitchInterroge; z++)
    {

        // if (saSwitchInterroge[z] < g->nb_switch)
        //{
        /*for (size_t _ = 0; _ < nbresaSwitchInterroge; _++)
        {
            if (existeDansTable(&g->swListe[saSwitchInterroge[_]], &tr->destination))
            {
                // printf("Existe, donc %ld -> %ld\n", switchInterroge, saSwitchInterroge[_]);
                envoyerTrameRecursive(tr, g, saSwitchInterroge[_]);
                // afficherTableCommutation(&g->swListe[switchInterroge]);
            }
        }*/
        // printf("Existe pas donc, %ld ->%ld\n", switchInterroge, saSwitchInterroge[z]);
        envoyerTrameRecursive(tr, g, saSwitchInterroge[z], switchInterroge); // On envoie à tout les switchs et machines adjacentes

        //}
    }
    for (size_t _ = 0; _ < g->nb_switch; _++)
    {
        g->swListe[_]._sommetArrive = -1;
    }
}
bool estStation(graphe const *g, sommet s)
{
    return s > g->nb_switch - 1; // True si c'est une station comprise dans l'interval ]nbreSwitch;nbreStation + nbreSwitch]
}
void envoyerTrameRecursive(trame *tr, graphe *g, sommet switchInterroge, sommet ancienSwitch)
{
    if (estStation(g, switchInterroge)) // Voici "condition de fin" sans en etre une. Cette partie est importante pour ne pas renvoyer de nouveau la trame en arrière
    {
        printf("Trame passe par :  %ld\n", switchInterroge);
        station stDestination;
        getStationAvecMAC(g, &tr->destination, &stDestination); // Recup la station de dest (au cas ou)

        if (macEquals(&g->stListe[switchInterroge - g->nb_switch]._mac, &stDestination._mac) == 1)
        {

            // Ajout du decodage
            afficherAdrMac(&g->stListe[switchInterroge - g->nb_switch]._mac);
            printf("Bien arrivé ! (Station : %ld)\n", switchInterroge);
            decoderTrame(tr);
            //  exit(EXIT_SUCCESS); //Peut toujours servir si on est aussi flemmard pour remplir le restant des tables de commutation que la presse française (Référence a la non représentation de Jean Lasalle aux européenes de 2024 sur les plateaux télé)
        }
    }
    if ((!estStation(g, switchInterroge) || switchInterroge == 0) && g->swListe[switchInterroge]._sommetArrive != switchInterroge)
    {
        g->swListe[switchInterroge]._sommetPrecedent = ancienSwitch;
        setTrameInSwitch(&g->swListe[switchInterroge], tr);
        sommet saSwitchInterroge[g->swListe[switchInterroge]._nbre_ports];
        size_t nbresaSwitchInterroge;
        station stSource;
        getStationAvecMAC(g, &tr->source, &stSource); // On recupere la station source du message
        for (int j = 0; j < g->swListe[switchInterroge]._nbre_ports; j++)
        {
            if (!existeDansTable(&g->swListe[switchInterroge], &tr->source) && estNULL(&g->swListe[switchInterroge]._tableCommutation[j]._mac)) // On regarde si il n'est pas dans la table ET qu'il n'est pas nul (car adresse non renseigné = NULL Comme certains hommes politique (B*))
            {
                cpyAdrMac(&g->swListe[switchInterroge]._tableCommutation[j]._mac, &tr->source);
                g->swListe[switchInterroge]._sommetArrive = switchInterroge;           // Evite les boucles infini. et permet de stocker l'index. Si il y a un chiffre, alors il est considéré comme visité, sinon non
                g->swListe[switchInterroge]._tableCommutation[j]._port = ancienSwitch; // Et le switch de passage pour y acceder (système de port simplifié)
                g->swListe[switchInterroge]._longueurTable++;
                // g->swListe[switchInterroge]._sommetPrecedent = switc;
                break;
            }
        }
        nbresaSwitchInterroge = sommets_adjacents(g, switchInterroge, saSwitchInterroge);
        if (existeDansTable(&g->swListe[switchInterroge], &tr->destination)) // Evite les boucles infini. et permet de stocker l'index. Si il y a un chiffre, alors il est considéré comme visité, sinon non
        {
            bool tmp = false;
            bool tmp2 = false;

            int SwitchSuivantAvecTable;
            for (size_t l = 0; l < nbresaSwitchInterroge; l++) // Permet de vérifier si le switch est adjacent a la station de destination
            {
                for (int tablecomm = 0; tablecomm < g->swListe[switchInterroge]._longueurTable; tablecomm++)
                {
                    if ((int)saSwitchInterroge[l] == g->swListe[switchInterroge]._tableCommutation[tablecomm]._port && saSwitchInterroge[l] != ancienSwitch)
                    {
                        tmp2 = true;
                        SwitchSuivantAvecTable = saSwitchInterroge[l];
                    }
                }
                if (saSwitchInterroge[l] == index_MAC(g, &tr->destination) + g->nb_switch)
                {
                    tmp = true;
                    printf("J'ai retrouvé mon chemin grâce aux Tables de commutations !\n");
                }
            }
            if (tmp)
            {
                g->swListe[switchInterroge]._sommetArrive = switchInterroge; // Evite les boucles infini. et permet de stocker l'index. Si il y a un chiffre, alors il est considéré comme visité, sinon non
                printf("Trame passe par :  %ld\n", switchInterroge);

                envoyerTrameRecursive(tr, g, index_MAC(g, &tr->destination) + g->nb_switch, switchInterroge);
                return;
            }
            if (tmp2)
            {
                printf("Je sus à %ld, je vais à %d\n", switchInterroge, SwitchSuivantAvecTable);
                envoyerTrameRecursive(tr, g, SwitchSuivantAvecTable, switchInterroge);
            }
        }
        else
        {
            printf("Trame passe par :  %ld\n", switchInterroge);
            for (size_t z = 0; z < nbresaSwitchInterroge; z++)
            {
                envoyerTrameRecursive(tr, g, saSwitchInterroge[z], switchInterroge); // On envoie à tout les switchs et machines adjacentes
                g->swListe[switchInterroge]._trEnCours = NULL;
            }
        }
    }
}

size_t index_Station(graphe *g, station *st)
{

    for (size_t i = 0; i < g->nb_stations; i++)
    {

        if (macEquals(&st->_mac, &g->stListe[i]._mac) == 1)
        {
            return i;
        }
    }
    return -1;
}
size_t index_MAC(graphe *g, adrMac *mac)
{

    for (size_t i = 0; i < g->ordre; i++)
    {
        if (macEquals(mac, &g->swListe[i]._mac) == 1)
        {
            return i;
        }
        if (macEquals(mac, &g->stListe[i]._mac) == 1)
        {
            return i;
        }
    }

    printf("La machine n'a pas été trouvée dans le graphe\n");
    return -1;
}
size_t index_Switch(graphe *g, Switch *sw)
{

    for (size_t i = 0; i < g->nb_switch; i++)
    {

        if (macEquals(&sw->_mac, &g->swListe[i]._mac) == 1)
        {
            return i;
        }
    }

    printf("Le switch n'a pas été trouvée dans le graphe\n");
    return -1;
}

size_t ordre(graphe const *g)
{
    return g->ordre;
}

size_t nb_aretes(graphe const *g)
{
    return g->nb_aretes;
}

void ajouter_sommet(graphe *g)
{
    g->ordre += 1;
}

static arete swap_sommets(arete a)
{
    return (arete){a.s2, a.s1, a.c};
}
// une fonction locale "static arete swap_sommets(arete a)" pourra être utile
// cette fonction retourne une nouvelle arête dont les sommets sont les même que l'arête reçue mais inversés

bool existe_arete(graphe const *g, arete a)
{
    // retourne true si l'arête a est contenue dans le graphe g, false sinon
    // /!\ l'arête (s1,s2) et l'arête (s2,s1) sont considérées équivalentes
    if (a.s1 > a.s2)
        a = swap_sommets(a);
    for (size_t i = 0; i < g->nb_aretes; i++)
    {
        if (g->aretes[i].s1 == a.s1 && g->aretes[i].s2 == a.s2)
            return true;
    }
    return false;
}

bool ajouter_arete(graphe *g, arete a)
{
    // l'arête a n'est ajoutée que si les conditions suivantes sont remplies :
    //  - les sommets s1 et s2 de a existent dans g
    //  - les sommets s1 et s2 de a sont distincts fait
    //  - l'arête a n'existe pas dans g fait

    // /!\ si la capacité actuelle du tableau d'arêtes n'est pas suffisante,
    // /!\ il faut le réallouer.
    // /!\ on peut par exemple doubler la capacité du tableau actuel.

    // retourne true si l'arête a bien été ajoutée, false sinon
    if (!existe_arete(g, a) && a.s1 != a.s2 && (a.s1 < g->ordre && a.s2 < g->ordre))
    {
        if ((int)g->nb_aretes == g->aretes_capacite)
        {
            g->aretes_capacite *= 2;
            g->aretes = realloc(g->aretes, (g->aretes_capacite) * sizeof(arete));
        }
        g->aretes[g->nb_aretes] = a;
        g->nb_aretes++;
        return true;
    }

    return false;
}

size_t index_arete(graphe const *g, arete a)
{
    // retourne l'index de l'arête au sein du tableau d'arêtes de g si l'arête a existe dans g,
    // la valeur UNKNOWN_INDEX sinon
    if (a.s1 > a.s2)
        a = swap_sommets(a);
    for (size_t i = 0; i < g->nb_aretes; i++)
    {
        if (g->aretes[i].s1 == a.s1 && g->aretes[i].s2 == a.s2)
            return i;
    }
    return UNKNOWN_INDEX;
}

size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[])
{
    // remplit le tableau sa avec les sommets adjacents de s dans g
    // et retourne le nombre de sommets ainsi stockés
    // (on suppose que s fait bien partie du graphe g)
    // (le tableau sa est supposé assez grand pour contenir les sommets adjacents de s)
    size_t nb = 0;
    for (size_t i = 0; i < g->nb_aretes; i++)
    {
        if (g->aretes[i].s1 == s)
        {
            sa[nb] = g->aretes[i].s2;
            nb++;
        }
        if (g->aretes[i].s2 == s)
        {
            sa[nb] = g->aretes[i].s1;
            nb++;
        }
    }
    return nb;
}

sommet determineRacine(graphe *g)
{
    Switch tmp = g->swListe[0];
    for (size_t i = 1; i < g->nb_switch; i++)
    {
        if (g->swListe[i]._priorite == tmp._priorite)
        {
            if (adrMacInf(&g->swListe[i]._mac, &tmp._mac) == 1)
            {
                tmp = g->swListe[i];
            }
        }
        if (g->swListe[i]._priorite < tmp._priorite)
        {
            tmp = g->swListe[i];
        }
    }
    return index_Switch(g, &tmp);
}

void dijkstra(graphe *g, sommet *pred, cout *dist)
{

    bool *fixe = malloc(ordre(g) * sizeof(bool));

    for (size_t i = 0; i < g->ordre; i++)
    {
        dist[i] = INFINI;
        fixe[i] = false;
    }

    sommet racine = determineRacine(g);
    pred[racine] = racine;
    dist[racine] = 0;

    for (size_t i = 0; i < g->ordre; i++)
    {

        cout min_distance = INFINI;
        sommet s = (sommet)-1;

        for (sommet j = 0; j < g->ordre; j++)
        {
            if (fixe[j] == false && dist[j] < min_distance)
            { // On cherche le sommet le plus proche
                min_distance = dist[j];
                s = j;
            }
        }

        if (s == (sommet)-1)
        { // Si on a pas trouvé de sommet adjacent
            break;
        }

        fixe[s] = true;
        sommet *adj = malloc(ordre(g) * sizeof(sommet));
        size_t nb_adj = sommets_adjacents(g, s, adj);

        for (size_t k = 0; k < nb_adj; k++)
        {
            sommet adj_sommet = adj[k];

            if (!fixe[adj_sommet])
            {
                size_t index = index_arete(g, (arete){s, adj_sommet, 0});
                if (index != UNKNOWN_INDEX)
                {
                    cout distance = dist[s] + g->aretes[index].c; // On ajoute le cout de l'arete
                    if (distance < dist[adj_sommet])
                    {
                        dist[adj_sommet] = distance;
                        pred[adj_sommet] = s;
                    }
                }
            }
        }
        free(adj);
    }
    free(fixe);
    // for(int i = 0; i < (int)g->ordre; i++){
    //     printf("Distance de %ld à %d : %ld\n", predecesseur[i], i, distance[i]);
    // }
}
void afficherSwitchRoute(graphe *g)
{
    sommet racine = determineRacine(g);
    printf("---SWITCH RACINE---\n");
    afficherSwitch(&g->swListe[racine]);
}
void stp(graphe *g)
{
    sommet *pred = calloc(ordre(g), sizeof(sommet));
    cout *dist = malloc(ordre(g) * sizeof(cout));
    dijkstra(g, pred, dist);

    arete *lstNewAretes = malloc(ordre(g) * sizeof(arete));
    size_t nbNewAretes = 0;

    for (sommet i = 0; i < g->ordre; i++)
    {

        for (sommet j = 0; j < g->ordre; j++)
        {

            if (pred[j] == i && i != j && existe_arete(g, (arete){i, j, 0}))
            {
                lstNewAretes[nbNewAretes] = (arete){i, j, dist[j]};
                nbNewAretes++;
            }
        }
    }
    free(pred);
    free(dist);
    free(g->aretes);
    g->aretes = malloc(nbNewAretes * sizeof(arete));
    g->nb_aretes = 0;

    for (size_t i = 0; i < nbNewAretes; i++)
    {
        printf("Arete %ld - %ld : %ld\n", lstNewAretes[i].s1, lstNewAretes[i].s2, lstNewAretes[i].c);
        ajouter_arete(g, lstNewAretes[i]);
    }
    free(lstNewAretes);
}