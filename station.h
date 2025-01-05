#ifndef __STATION_H
#define __STATION_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ip.h"
#include "mac.h"

#define STATION_NOM_LEN 256

typedef char unom;

typedef struct
{
    adrMac _mac;
    adrIP _ip;
    unom _nom[STATION_NOM_LEN];
} station;

void setStation(station *station, adrMac const *adresseMac, adrIP const *adresseIP, unom const nom[STATION_NOM_LEN]);
void afficherStation(station const *station); // Renommé pour éviter les conflits
void getStationMac(station const *station, adrMac *adressemac);
void getStationIP(station const *station, adrIP *adresseIP);
void getStationNom(station const *station, unom nom[STATION_NOM_LEN]);

#endif
