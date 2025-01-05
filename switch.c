#include "switch.h"

void setSwitch(Switch *sw, adrMac const *adresseMac, unom const nom[SWITCH_NOM_LEN], unbrePorts nbreports, upriorite priorite)
{
    sw->_mac = *adresseMac;
    sw->_nbre_ports = nbreports;
    sw->_priorite = priorite;
    strcpy(sw->_nom, nom);
    sw->_longueurTable = 0;
    sw->_tableCommutation = NULL;
}

void afficherSwitch(Switch const *sw)
{
    printf("NOM : %s\n", sw->_nom);
    printf("NOMBRE DE PORTS : %d\n", sw->_nbre_ports);
    printf("LONGUEUR TABLE : %d\n", sw->_longueurTable);
    printf("PRIORITE : %d\n", sw->_priorite);
    afficherAdrMac(&sw->_mac);
}

adrMac getSwitchMac(Switch const *sw)
{
    return sw->_mac;
}

int getSwitchNbrePorts(Switch const *sw)
{
    return sw->_nbre_ports;
}

int getSwitchPriorite(Switch const *sw)
{
    return sw->_priorite;
}

ulongueurTable getSwitchLongueurTable(Switch const *sw)
{
    return sw->_longueurTable;
}

void getSwitchNom(Switch const *sw, unom nom[SWITCH_NOM_LEN])
{
    strncpy(nom, sw->_nom, SWITCH_NOM_LEN);
}

void afficherTableCommutation(Switch *sw)
{
    printf("---TABLE DE COMMUTATION---\n");
    for (int i = 0; i < sw->_longueurTable; i++)
    {
        afficherAdrMac(&sw->_tableCommutation[i]._mac);
        printf("Port (Via le switch id): %d\n", sw->_tableCommutation[i]._port);
    }
    printf("--------------------------\n");
}

bool existeDansTable(Switch const *sw, adrMac const *mac)
{
    for (int i = 0; i < sw->_longueurTable; i++)
    {
        if (memcmp(&sw->_tableCommutation[i]._mac, mac, sizeof(adrMac)) == 0)
        {
            return true;
        }
    }
    return false;
}
int getTablePort(Switch const *sw, adrMac const *mac)
{
    for (int i = 0; i < sw->_longueurTable; i++)
    {
        if (memcmp(&sw->_tableCommutation[i]._mac, mac, sizeof(adrMac)) == 0)
        {
            return sw->_tableCommutation[i]._port;
        }
    }
    return -1;
}
void setTrameInSwitch(Switch *sw, trame *tr)
{
    sw->_trEnCours = tr;
}
void removeTrameInSwitch(Switch *sw)
{
    sw->_trEnCours = NULL;
}

void setPortArriveSwitch(Switch *sw, sommet s)
{
    sw->_sommetArrive = s;
}