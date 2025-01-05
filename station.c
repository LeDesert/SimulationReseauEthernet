#include "station.h"

void setStation(station *st, adrMac const *adresseMac, adrIP const *adresseIP, unom const nom[STATION_NOM_LEN])
{
    st->_ip = *adresseIP;
    st->_mac = *adresseMac;
    /*J'ai modifié un simple strcpy en condition pour éviter les erreurs valgrind ~ Antoine
==29567== Source and destination overlap in strncpy(0x1ffeffef7d, 0x1ffeffef7d, 255)
==29567==    at 0x484F084: strncpy (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==29567==    by 0x109D38: setStation (in /mnt/c/Users/antoi/OneDrive - etu.unistra.fr/Etude/IUT/Illkirch/S2/SAEs22/main)
==29567==    by 0x1097DF: main (in /mnt/c/Users/antoi/OneDrive - etu.unistra.fr/Etude/IUT/Illkirch/S2/SAEs22/main)
==29567==
==29567== Source and destination overlap in strncpy(0x1ffefff08d, 0x1ffefff08d, 255)
==29567==    at 0x484F084: strncpy (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==29567==    by 0x109D38: setStation (in /mnt/c/Users/antoi/OneDrive - etu.unistra.fr/Etude/IUT/Illkirch/S2/SAEs22/main)
==29567==    by 0x10980B: main (in /mnt/c/Users/antoi/OneDrive - etu.unistra.fr/Etude/IUT/Illkirch/S2/SAEs22/main)
==29567==
                                    Corrigé !✅
*/
    if (st->_nom != nom)
    {
        memmove(st->_nom, nom, STATION_NOM_LEN - 1);
        // memmove Copie un bloc de mémoire d'une région à une autre, en gérant correctement le chevauchement entre la source et la destination.
        //(Wikipedia.) au niveau ou on est on s'en fou, seulement a retenir que c'est une sorte de strcpy mais en mieux
        st->_nom[STATION_NOM_LEN - 1] = '\0';
    }
}

void afficherStation(station const *st)
{
    printf("NOM : %s\n", st->_nom);
    afficherAdrMac(&st->_mac);
    afficherAdrIP(&st->_ip);
}

void getStationMac(station const *st, adrMac *adressemac)
{
    *adressemac = st->_mac;
}

void getStationIP(station const *st, adrIP *adresseIP)
{
    *adresseIP = st->_ip;
}

void getStationNom(station const *st, unom nom[STATION_NOM_LEN])
{
    strncpy(nom, st->_nom, STATION_NOM_LEN);
}
