#include "ip.h"

void setAdrIP(adrIP *adresseIP, uip adresse[ADDR_IP_LEN])
{
    for (int i = 0; i < ADDR_IP_LEN; i++)
    {
        adresseIP->_IPadr[i] = adresse[i];
    }
}
void cpyAdrIP(adrIP *adresseIPCopie, adrIP const *adresseIP)
{
    uip tmp[ADDR_IP_LEN];
    for (int i = 0; i < ADDR_IP_LEN; i++)
    {
        tmp[i] = adresseIP->_IPadr[i];
        adresseIPCopie->_IPadr[i] = tmp[i];
    }
}
void afficherAdrIP(adrIP const *adresseIP)
{
    printf("IP : ");
    for (int i = 0; i < ADDR_IP_LEN; i++)
    {
        printf("%d", adresseIP->_IPadr[i]);
        if (i != ADDR_IP_LEN - 1)
        {
            printf(".");
        }
    }
    printf("\n");
}
