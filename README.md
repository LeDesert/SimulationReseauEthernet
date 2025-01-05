# Simulateur de Réseau Ethernet avec Switches et Stations

## Description

Ce projet est une implémentation en C d'un réseau de commutation avec des stations et des switches fait en première année d'études. Il permet de simuler l'envoi de trames Ethernet entre différentes stations à travers un réseau de switches en local.

### Fonctionnalités principales :
- Simulation de l'envoi de trames Ethernet entre des stations via des switches.
- Gestion des adresses IP et MAC pour chaque station et switch.
- Construction dynamique du réseau sous forme de graphes.
- Gestion de la **table ARP** (Address Resolution Protocol) pour associer les adresses IP aux adresses MAC dans le réseau.

## Structure du projet

- `graphe.c` et `graphe.h` : Gestion des graphes représentant le réseau.
- `ip.c` et `ip.h` : Gestion des adresses IP.
- `mac.c` et `mac.h` : Gestion des adresses MAC.
- `station.c` et `station.h` : Gestion des stations.
- `switch.c` et `switch.h` : Gestion des switches.
- `trameEthernet.c` et `trameEthernet.h` : Gestion des trames Ethernet.
- `main.c` : Point d'entrée du programme.
- `makefile` : Fichier pour compiler le projet.

## Compilation

Pour compiler le projet, utilisez la commande suivante :
```sh
make
```

Pour nettoyer les fichiers objets et l'exécutable, utilisez :
```sh
make clean
```

## Utilisation

Après compilation, exécutez le programme avec :
```sh
./main
```

## Auteur

- Antoine

---

# Ethernet Network Simulator with Switches and Stations

## Description

This project is a C implementation of a switching network with stations and switches. It simulates the sending of Ethernet frames between different stations through a network of switches.

### Key Features:
- Simulation of Ethernet frame transmission between stations through switches.
- Management of IP and MAC addresses for each station and switch.
- Dynamic construction of the network topology as graphs.
- Management of the ARP table (Address Resolution Protocol) to map IP addresses to MAC addresses in the network.

## Project Structure

- `graphe.c` and `graphe.h` : Management of graphs representing the network.
- `ip.c` and `ip.h` : Management of IP addresses.
- `mac.c` and `mac.h` : Management of MAC addresses.
- `station.c` and `station.h` : Management of stations.
- `switch.c` and `switch.h` : Management of switches.
- `trameEthernet.c` and `trameEthernet.h` : Management of Ethernet frames.
- `main.c` : Entry point of the program.
- `makefile` : File to compile the project.

## Compilation

To compile the project, use the following command:
```sh
make
```

To clean the object files and the executable, use:
```sh
make clean
```

## Usage

After compilation, run the program with:
```sh
./main
```

## Author

- Antoine