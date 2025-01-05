#ifndef __IP_H
#define __IP_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define ADDR_IP_LEN 4

typedef uint8_t uip;

typedef struct
{
    uip _IPadr[ADDR_IP_LEN];
} adrIP;

// void setAdrIP(adrIP *adresseIP, uip adresse[ADDR_IP_LEN]);
// void cpyAdrIP(adrIP *adresseIPCopie, adrIP const *adresseIP);
void afficherAdrIP(adrIP const *adresseIP);
void setAdrIP(adrIP *adresseIP, uip adresse[ADDR_IP_LEN]);
void cpyAdrIP(adrIP *adresseIPCopie, adrIP const *adresseIP);

#endif