---
title: "CS351 Projet Émulateur MIPS"
subject: "Bilan final"
author: ***REMOVED*** ***REMOVED*** & ***REMOVED*** ***REMOVED***
date: "14/12/2020"
geometry: margin=2.5cm
papersize: a4
titlepage: true,
titlepage-text-color: "FFFFFF"
titlepage-rule-color: "FFFFFF"
titlepage-rule-height: 3
titlepage-background: "bg.pdf"
...
# Introduction
Dans le cadre du projet de programmation C, l'objectif est de réaliser un émulateur MIPS capable d'assembler et d'exécuter des instructions en assembleur. Il devra comporter 3 modes : intératif, pas-à-pas et non-intéractif. Nous avons à notre disposition de la documentation décrivant le fonctionnement du processeur MIPS et la structure de ces instructions. Le but étant d'avoir un émulateur ayant un comportement le plus proche de la réalité possible. 

# Choix techniques

Suite au rendu de l'étape du 2 du projet nous avons finalement modifié quelques-uns de nos choix techniques. Nous allons donc vous présenter rapidement la structure du logiciel module par module.

## Module mémoire

Nous avons retenu la plupart des concepts formulés lors de l'étape précédente, à savoir une liste chaînée triée par adresse. Un choix judicieux puisque que la mémoire physique est allouée dynamiquement en fonction de l'utilisation.

Néanmoins nous avons dû nous résoudre à simplifier l'implémentation de la gestion mémoire en stockant mot à mot et non octet par octet. Le structure de la liste chaînée a été renommée de `byteMem` à `wordMem`, le type du champ `data` est passé de `char` à `int` et les fonctions inutiles ont été supprimées. La complexité de notre code est ainsi bien moindre mais en l'état la mémoire n'est plus adressable directement par octet.

## Module registre

Nous avons implémenté ce module de la même manière que nous l'avions imaginé, avec un tableau de 35 `int`. De l'index 0 à 31, sont positionnés les registres classiques et de l'index 32 à 34 les registres spéciaux (PC, HI et LO).

Le tableau contenant les registres est modifié et lu via les fonctions `storeRegister` et `getRegister` pour éviter les erreurs et les opérations interdites (ie: modifier la valeur du registre $0).

## Module stream

Ce module contient simplement deux fonctions qui permettent d'ouvrir et de fermer de manière sécurisée des fichiers.

## Module main

Ce module contient simplement le `main.c`.

## Module assembleur (codage/décodage)

Premier module à avoir été codé dans le cadre du projet. Nous avons réalisé une refonte de celui-ci pour qu'il puisse offrir correctement l'ensemble des fonctions permettant la traduction d'instructions sous forme de chaînes de caractères en hexadécimal, mais également l'extraction de l'opcode et des instructions.

Nous avons défini un tableau de structures contenant toutes les informations sur chaque fonction implémentée. Voici la structure de base :

```
typedef struct instruction instruction;
struct instruction{
    char name[MAX_OPCODE_SIZE+1];
    int (*fct)(int args[]);
    int type;
    int opcode;
    int function;
};
```
Cette structure comporte un pointeur de fonction permettant de réaliser l'appel de la fonction associée à l'instruction à traiter de manière générique.

Le champ type est également très intéressant car il permet d'identifier proprement chaque instruction et les arguments qu'il faut lui fournir. Voici quelques exemples :

|\color{red}**Opcode**| INDEX | BASE | RS | ROTATE | RT | OFFSET | IMM | RD | SA | HINT | FCT |\color{blue}**Type**|
| ------------------- | ----- | ---- | -- | ------ | -- | ------ | --- | -- | -- | ---- | --- | ------------------ |
| **ADDI**            | 0     | 0    | 0  | 0      | 0  | 1      | 0   | 1  | 0  | 0    | 1   | **0x149**          |
| **BGTZ**            | 0     | 0    | 1  | 0      | 0  | 1      | 0   | 0  | 0  | 0    | 0   | **0x120**          |
| **ROTR**            | 0     | 0    | 0  | 1      | 1  | 0      | 0   | 1  | 1  | 0    | 1   | **0x0CD**          |


## Module fonction

Ce module est composé des fonctions réalisant les actions respectives (écritures des régistres/mémoire) de chaque instruction MIPS prise en charge par l'émulateur. Elles sont toutes appelées par un pointeur de fonction définit dans le module assembleur. Cette partie du code est assez simple. En effet, nous avons déporté toute la complexité dans les autres modules, notamment avec le champ `type` dans le tableau de structures `instruction`.

## Module mode

Ce module est la clef de voûte de notre programme, en effet il gère les trois différents modes d'exécution de l'emulateur. Les fonctions qui le constituent, utilisent les modules précédemment cités. Il implémente en quelque sorte la pipeline de processeur MIPS en appelant successivement des fonctions pour lire, encoder, stocker, décoder, et exécuter les instructions que l'on fournit.


# Progression et répartition du travail

## Étape 1

Nous avons réalisé la première partie du projet (module assembleur + tests) à quatre mains grâce à un outil de écriture de code partagé et git.

## Étape 2

Nous avons ensuite défini ensemble un découpage en modules pour pouvoir créer les prototypes demandés et nous répartir le travail. Avant même de coder nous avons défini précisemment tout ce que nous allions devoir faire, ce qui nous a permis de gagner du temps en mettant à profit les travaux réalisés dans les précédents TP.

## Étape 3
C'est à partir de cette étape que nous avons commencé à travailler chacun de notre côté.

- ***REMOVED*** : Modules Registre, Mémoire, et Modes (+ refonte Assembleur)
- ***REMOVED*** : Module fonction, Écriture de tests supplémentaires pour le débogage

# Conclusion

Lors de ce projet nous avons eu l'occasion de mettre en application l'ensemble du spectre de nos connaissances en programmation C et certaines notions d'électronique numérique. Cette réalisation à l'envergure bien plus importante que ce nous avions pu faire auparavant, nous a permis de mettre en œuvre de la programmation modulaire afin de pouvoir mieux structurer notre code et partager la charge de travail convenablement. Nous avons réussi à terminer le travail attendu ainsi que quelques fonctions supplémentaires qui pourraient être utilisées dans l'implémentation des étiquettes.

# Addendum
## Description des éléments du rendu final (archive)
- L'ensemble du code source est contenu dans le dossier `src`
- Les tests (codes assembleur) `in?.txt` et des résultats `out?.txt` sont dans le dossier `test`
