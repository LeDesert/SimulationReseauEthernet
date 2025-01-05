#ifndef __MAC_H
#define __MAC_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define ADDR_MAC_LEN 6

typedef uint8_t umac;

typedef struct
{
    umac _Macadr[ADDR_MAC_LEN];
} adrMac;

void setAdrMac(adrMac *adresseMac, char macEnHexa[6][3]);
void cpyAdrMac(adrMac *adresseMacCopie, adrMac const *adresseMac);
void afficherAdrMac(adrMac const *adresseMac);
int macEquals(adrMac const *mac1, adrMac const *mac2);
int estNULL(adrMac const *mac1);
int adrMacInf(adrMac *mac1, adrMac *mac2);

#endif
