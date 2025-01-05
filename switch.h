#ifndef __SWITCH_H
#define __SWITCH_H
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "ip.h"
#include "mac.h"
#include "station.h"
#include <stdbool.h>
#include "trameEthernet.h"

#define SWITCH_NOM_LEN 256

typedef char unom;
typedef int upriorite;
typedef int unbrePorts;
typedef int ulongueurTable;
typedef int uport;
typedef size_t sommet;

typedef struct
{
    adrMac _mac;
    uport _port;
} TableCommutation;

typedef struct
{
    adrMac _mac;
    unbrePorts _nbre_ports;
    upriorite _priorite;
    TableCommutation *_tableCommutation;
    ulongueurTable _longueurTable;
    unom _nom[SWITCH_NOM_LEN];
    trame *_trEnCours;
    sommet _sommetArrive;
    sommet _sommetPrecedent;
} Switch;

void setSwitch(Switch *station, adrMac const *adresseMac, unom const nom[SWITCH_NOM_LEN], unbrePorts nbreports, upriorite priorite);
void afficherSwitch(Switch const *sw);
adrMac getSwitchMac(Switch const *sw);
int getSwitchNbrePorts(Switch const *sw);
int getSwitchPriorite(Switch const *sw);
ulongueurTable getSwitchLongueurTable(Switch const *sw);
void getSwitchNom(Switch const *sw, unom nom[SWITCH_NOM_LEN]);
void afficherTableCommutation(Switch *sw);
bool existeDansTable(Switch const *sw, adrMac const *mac);
void setTrameInSwitch(Switch *sw, trame *tr);
void removeTrameInSwitch(Switch *sw);
void setPortArriveSwitch(Switch *sw, sommet s);
int getTablePort(Switch const *sw, adrMac const *mac);

#endif
