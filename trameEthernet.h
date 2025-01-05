#pragma once
#ifndef __TRAME_H
#define __TRAME_H
#include "station.h"
#include <stdint.h>
#include <stdio.h>
typedef int8_t upreambule; // int8_t est sur 1 octet
typedef int8_t usfd;       // de meme et en + on a seulement besoin d'un octet pour le sfd
typedef uint16_t utype;    // uint16 vu que le type est sur 2 octets
typedef int8_t udata;
typedef uint32_t ufcs;
typedef struct
{
    upreambule preambule[7];
    usfd sfd;
    adrMac destination;
    adrMac source;
    usfd type;
    udata data[1500];
    ufcs fcs;
    int longueurChaine;
} trame;

void afficherEthernet(trame *tr);

void decoderTrame(trame *tr);

void setTrame(trame *tr, station *destination, station *source);

void message(trame *tr, char *message);

#endif