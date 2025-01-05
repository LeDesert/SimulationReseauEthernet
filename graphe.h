#pragma once
#ifndef __GRAPHE_H
#define __GRAPHE_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "switch.h"
#include "trameEthernet.h"
#define NOMFICHIER_LONGUEUR 255
#define INFINI INT16_MAX
typedef size_t sommet;
typedef size_t cout;

typedef struct arete
{
    sommet s1;
    sommet s2;
    cout c;
} arete;
// on garde cette mise en forme car pour differencier un switch et une station, le switch est compris entre 0 et (ordre-nb_stations)

typedef struct graphe
{
    size_t ordre;
    int aretes_capacite;
    size_t nb_aretes;

    size_t nb_switch;
    Switch *swListe;
    size_t nb_stations;
    station *stListe;

    arete *aretes;

    trame *lstTrameSwitch;
    bool conditionArret;
} graphe;

/*typedef struct areteGraphe
{
    graphe g1;
    graphe g2;
} areteGraphe;*/

static const size_t UNKNOWN_INDEX = -1;

void init_graphe(graphe *g, FILE *fichier, char *nom);
void afficherTMP(graphe *g);
void free_graphe(graphe *g);

size_t ordre(graphe const *g);
size_t nb_aretes(graphe const *g);

void ajouter_sommet(graphe *g);
bool existe_arete(graphe const *g, arete a);
bool ajouter_arete(graphe *g, arete a);
size_t index_arete(graphe const *g, arete a);

size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[]);
size_t index_Station(graphe *g, station *st);
size_t index_Switch(graphe *g, Switch *sw);
size_t index_MAC(graphe *g, adrMac *mac);
void getStationAvecIndice(graphe *g, int indice, station *st);
void getStationAvecMAC(graphe *g, adrMac *mac, station *st);
void envoyerTrame(trame *tr, graphe *g);
size_t switchDebutTrame(graphe *g, station *st);
void afficher_chemin(trame *tr, graphe *g, sommet s1, sommet s2);
int cout_max(graphe *g);
bool estStation(graphe const *g, sommet s);
void envoyerTrameRecursive(trame *tr, graphe *g, sommet switchInterroge, sommet ancienSwitch);
void initSTP(graphe *g);
void seekLeRootDuReseau(graphe *g, Switch *switchRouteur);
sommet determineRacine(graphe *g);
void dijkstra(graphe *g, sommet *pred, cout *dist);
void stp(graphe *g);
void afficherSwitchRoute(graphe *g);
#endif