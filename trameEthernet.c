#include "trameEthernet.h"
void afficherEthernet(trame *tr)
{
    printf("Préambule : ");
    for (int i = 0; i < 7; i++)
    {
        udata tmp = tr->preambule[i];
        for (int j = 7; j >= 0; j--)
        {
            printf("%x", (tmp >> j) & 1);               // Permet d'afficher bit a bit le préambule avec un tempon AU CAS OU
            j % 4 == 0 && j % 8 != 0 ? printf(" ") : 0; // je me suis amusé à faire du ternaire
            // vu que c'est incompréhensible, ici j'affiche des espaces "tout les 4 j", a chaque demi-octet
        }
        i < 6 ? (printf(".")) : 0; // Je met un point tout les octets et je fais à 6 sinon ça met un point a la fin
    }
    printf("\n");
    printf("SFD : ");
    for (int i = 7; i >= 0; i--)
    {
        printf("%x", (tr->sfd) >> i & 1);
    }
    printf("\n");
    printf("DESTINATION ");
    afficherAdrMac(&tr->destination);
    printf("SOURCE ");
    afficherAdrMac(&tr->source);
    printf("Type : %4x\n", tr->type);
    printf("Data : ");
    for (int i = 0; i < tr->longueurChaine; i++)
    {
        printf("%02x ", tr->data[i]);
    }
    printf("\n");
    // printf("FCS : %08x \n", tr->fcs);
}
/*
    upreambule preambule[7]; //fait
    usfd sfd;// fait
    adrMac destination; //fait
    adrMac source; //fait
    usfd type; //fait
    udata data[1500];
    ufcs fcs;
    int longueurChaine;*/

// Prend un destinatair eet une source. SERT UNIQUEMENT A PREPAPRER LA TRAME. MET LE TYPE A NULL, DATA AUSSI ET FCS AUSSI
// POUR CONFIGURER LA TRAM COMME PING OU MESSAGE UTILISER LES DEUX AUTRES
void setTrame(trame *tr, station *destination, station *source)
{
    for (int i = 0; i < 7; i++)
    {
        tr->preambule[i] = 0x55;
    }
    tr->sfd = 171;
    tr->destination = destination->_mac;
    tr->source = source->_mac;
}

void decoderTrame(trame *tr)
{
    printf("################################\n");
    printf("#     DECODAGE DE LA TRAME     #\n");
    printf("################################\n");
    printf("Préambule : ✅"); // Pas besoin de le vérifier car on est dans un environnement stable sans colision(s) possible(s) 🐀 (le rat vu qu'il prend bénéfice de tout ce qui l'avantage)
    printf("\n");
    printf("SFD : ✅");
    printf("\n");
    printf("DESTINATION ");
    afficherAdrMac(&tr->destination);
    printf("SOURCE ");
    afficherAdrMac(&tr->source);
    printf("Type : %4x\n", tr->type);
    printf("Data : ");
    for (int i = 0; i < tr->longueurChaine; i++)
    {
        printf("%c", tr->data[i]); // Print en char et pas en 02x vu qu'on veut le traduire🐀
    }
    printf("\n");
}

void message(trame *tr, char *message)
{
    int longeurMessage;
    tr->type = 16;
    longeurMessage = strlen(message);
    if (longeurMessage < 46)
    {
        memcpy(tr->data, message, longeurMessage);
        memset(tr->data + longeurMessage, 00, 46 - longeurMessage);
        tr->longueurChaine = 46 - longeurMessage;
    }
    else if (longeurMessage <= 1500)
    {
        memcpy(tr->data, message, longeurMessage);
        tr->longueurChaine = longeurMessage;
    }
    else
    {
        perror("Message trop long ! \n###############\n# Trame rejetée     #\n###############");
        exit(EXIT_FAILURE);
    }
}