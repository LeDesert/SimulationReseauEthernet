// MEMO DES LIBREAIRIES
#include <stdlib.h> //Pur les mallocs
#include <stdio.h>  // Pour les printf etc.. entree-sortie
#include <stdint.h> // Pour les uint16 etc
#include <string.h> //Pour strlen au cas ou
#include <unistd.h> // Pour utiliser sleep
#include "switch.h"
#include "graphe.h"
#include "trameEthernet.h"
int main()
{
    printf("----TEST IP------\n");
    adrIP adrip;
    uip ip[4] = {10, 10, 10, 10};
    setAdrIP(&adrip, ip);
    afficherAdrIP(&adrip);

    adrMac adrmac;
    char adresse[6][3] = {"a6", "0d", "a5", "d6", "c5", "f6"};
    setAdrMac(&adrmac, adresse);
    afficherAdrMac(&adrmac);

    unom nom[STATION_NOM_LEN] = "Salut";
    printf("----TEST STATION----\n");

    station st;
    setStation(&st, &adrmac, &adrip, nom);
    afficherStation(&st);
    printf("-----TEST SWITCH-----\n");
    Switch sw;
    setSwitch(&sw, &adrmac, nom, 4, 8);
    afficherSwitch(&sw);
    // 1;54:d6:a6:82:c5:01;130.79.80.1
    printf("-----TEST TABLE-----\n");
    TableCommutation tc;
    tc._mac = adrmac;
    tc._port = 2;
    printf("LA\n");
    sw._tableCommutation = &tc;
    sw._longueurTable++;
    afficherSwitch(&sw);
    printf("-----TEST GRAPHE-----\n");

    graphe g;
    FILE fichier;
    // init_graphe(&g, &fichier, "mylan_no_cycle.lan.txt");
    //  init_graphe(&g, &fichier, "test.txt");
    // init_graphe(&g, &fichier, "mylan.lan.txt"); // boucle : 0-1 0-2 1-4 2-4
    init_graphe(&g, &fichier, "stp_test");
    // init_graphe(&g, &fichier, "petit_stp");
    printf("Ordre : %lu\n", g.ordre);
    printf("aretes : %d\n", g.aretes_capacite);
    printf("AFFICHER GRAPHE \n");
    afficherTMP(&g);
    //  heures passé dessus jusqu'ici : 9 heures
    stp(&g);
    trame tr;
    station st1 = g.stListe[0]; // 7
    station st3 = g.stListe[7]; // 8
    setStation(&st1, &st1._mac, &st1._ip, st1._nom);
    setStation(&st3, &st3._mac, &st3._ip, st3._nom);
    setTrame(&tr, &st3, &st1);
    message(&tr, "Auguste a vote Bardella alors qu'il est de gauche");
    printf("---AFFICHAGE TRAME--- \n");
    afficherEthernet(&tr);
    printf("st1 : %ld \nst3 : %ld \n", index_Station(&g, &st1) + g.nb_switch, index_Station(&g, &st3) + g.nb_switch);
    afficherAdrMac(&st1._mac);
    afficherAdrMac(&st3._mac);
    printf("---ENVOIE TRAME--- \n");
    envoyerTrame(&tr, &g);
    printf("---ENVOIE TRAME--- \n");
    trame tr2;
    setTrame(&tr2, &st1, &st3);
    message(&tr2, "C'est un mensonge ?");
    envoyerTrame(&tr2, &g);
    for (size_t i = 0; i < g.nb_switch; i++)
    {
        printf("Table commut de %ld\n", i);
        afficherTableCommutation(&g.swListe[i]);
    }
    afficherSwitchRoute(&g);
    // afficherTMP(&g);

    free_graphe(&g);
    // Total heures : ~30-40h

    return 0;
}
