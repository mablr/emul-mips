---
title: "CS351 Projet Émulateur MIPS : Étape 2"
subject: "Réflexion sur les modules"
author: ***REMOVED*** ***REMOVED*** & ***REMOVED*** ***REMOVED***
date: "20/11/2020"
papersize: a4
...
# CS351 Projet Émulateur MIPS

## Réflexion sur le module de gestion de la mémoire

Tout d'abord, MIPS est une architecture big endian. C'est-à-dire que les bits de poids forts sont stockés aux adresses les plus basses. Nous avons déterminé qu'il était possible d'utiliser un tableau de int ou de char. Cependant, au vu de la structure de la mémoire décrite dans l'annexe du sujet, nous avons choisi d'utiliser un tableau de char. De cette manière, il sera possible d'adresser sur 32 bits chaque emplacement mémoire (octet), les adresses des mots iront donc de 4 en 4. C'est ainsi la structure la plus proche de la réalité

Ensuite, pour gérer cette mémoire, nous avons décidé d'utiliser une liste chaînée. Son fonctionnement nous semble très adapté à la tâche demandée, car assez léger et flexible. Et pour cause allouer 4 Go de mémoire avec un tableau de 2^32 int est inimaginable, inefficace, et contre-productif.

Il sera facile de réaliser ce système en s'insipirant des listes chaînées réalisés en TP avant le projet. En outre, nous aurons besoin d'un champ pour l'adresse et d'un champ pour les données.

Lors d'une allocation de mémoire (insertion dans la liste chainée), la liste gardera chacun de ses éléments (adresse + octet de données) triés selon leur addresse, ce qui permettra de réduire la complexité pour lire ou libérer la mémoire ensuite.

Voici la structure que nous envisageons d'utiliser pour la mémoire :
```c
typedef struct byteMem byteMem;
struct byteMem{
    unsigned int address;
    unsigned char data;
    byteMem * nextByteMem;
};
typedef byteMem * memory;

```

Nous écrirons dans ce module les fonctions suivantes :

- `insertByteMem` et `insertWord` pour stocker respectivement un octet et un mot (4 octets) en mémoire à une adresse donnée.
- `getByteMem` et `getWord` pour récupérer respectivement la valeur de l'octet et du mot en mémoire à une adresse donnée.
- `deleteByteMem` pour libérer l'emplacement mémoire à une adresse donnée.
- `isAllocatedByteMem` pour savoir si l'emplacement mémoire à l'adresse donnée est alloué.

## Réflexion sur le module de gestion des registres

Concernant la gestion des registres, nous avons opté pour une solution plus simple, il nous suffira de créer un tableau d'entiers.

Cependant, il ne faut pas oublier que certains registres sont reservés ou spéciaux, par exemple, le registre $29 contient le stack pointer et le registre $0 qui est toujours nul. Ainsi, la fonction ayant pour but de charger des valeurs dans les registres devra être assez robuste pour ne pas réaliser d'opérations interdites ou engendrer des erreurs.

Les registres généraux de $0 à $31, seront stockés de manière transparente dans le tableau des registres du rang 0 au rang 31.

Les registres pour le compteur programme PC et les registres HI et LO seront stockés respectivement aux rangs 33, 34 et 35.

Le module sera donc composé de seulement deux fonctions :

- `storeRegister` pour stocker une valeur dans un registre.
- `getRegister` pour récupérer la valeur d'un registre.