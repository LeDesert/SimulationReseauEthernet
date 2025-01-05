#include "mac.h"
void setAdrMac(adrMac *adresseMac, char macEnHexa[6][3])
{
    umac tmp[ADDR_MAC_LEN];
    for (int i = 0; i < ADDR_MAC_LEN; i++)
    {
        /* Note pour les autres : strtol prend en param 1 une chaine de char. en 2 le pointeur qui pointe jusqu'à ou il va (si erreur). en 3 la base
        ~ Antoine
        */
        int val = strtol(macEnHexa[i], NULL, 16);
        tmp[i] = (umac)val; // Je caste pour éviter que le compilateur fasse un warning et pour que ça soit plus lisible ~ Antoine
    }

    for (int i = 0; i < ADDR_MAC_LEN; i++)
    {
        adresseMac->_Macadr[i] = tmp[i];
    }
}
void cpyAdrMac(adrMac *adresseMacCopie, adrMac const *adresseMac)
{
    umac tmp[ADDR_MAC_LEN];
    for (int i = 0; i < ADDR_MAC_LEN; i++)
    {
        tmp[i] = adresseMac->_Macadr[i];
        adresseMacCopie->_Macadr[i] = tmp[i];
    }
}
void afficherAdrMac(adrMac const *adresseMac)
{
    printf("MAC : ");
    for (int i = 0; i < ADDR_MAC_LEN; i++)
    {
        printf("%02x", adresseMac->_Macadr[i]);
        if (i != ADDR_MAC_LEN - 1)
        {
            printf(".");
        }
    }
    printf("\n");
}

int macEquals(adrMac const *mac1, adrMac const *mac2)
{
    for (int i = 0; i < 6; i++)
    {
        if (mac1->_Macadr[i] != mac2->_Macadr[i])
        {
            return 0;
        }
    }
    return 1; // flemme de incldue stdbool
}

int estNULL(adrMac const *mac1)
{
    if (mac1 == NULL)
    {
        return 1;
    }
    else
    {
        for (int i = 0; i < 6; i++)
        {
            if (mac1->_Macadr[i] != 0)
            {
                return 0;
            }
        }
    }
    return 1;
}
int adrMacInf(adrMac *mac1, adrMac *mac2)
{
    for (int i = 0; i < 6; i++)
    {
        if (mac1->_Macadr[i] < mac2->_Macadr[i])
        {
            return 1;
        }
        if (mac1->_Macadr[i] > mac2->_Macadr[i])
        {
            return 0;
        }
    }
    return 0;
}