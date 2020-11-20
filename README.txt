            Projet CS351 : Émulateur MIPS
    Réflexion sur la structure de la mémoire

Tout d'abord, MIPS est une architecture big endian. C'est-à-dire que les bits de poids forts sont stockés aux adresses les plus basses. Nous avons déterminé qu'il était possible d'utiliser un tableau de int ou de char. Cependant, l'architecture big endian nous a poussé à choisir d'utiliser un tableau de char. 

Ensuite, pour la structure de la mémoire, nous avons décidé d'utiliser une liste chaînée. Son fonctionnement nous semblait très adapté à la tâche demandée. Il sera facile de la réaliser en se basant sur le modèle réalisé en TP avant le projet. Nous aurons besoin d'un champ pour l'adresse et d'un champ pour les données. Lors de l'insertion, nous trierons les champs de données par leur adresse. Cela permet d'allouer seulement la mémoire nécessaire plutôt que les 4Go prévus. Par exemple, stocker deux mots n'allouera que 8 octets de mémoire.

    Réflexion sur la structure des registres

Enfin, vis-à-vis des registres, il nous suffira de créer des tableaux de 32 int. Cependant, il ne faut pas oublier que certains registres sont reservés ou spéciaux (comme le $29 pour le stack pointer ou le $0 qui est toujours nul). Ainsi, le module des registres ne se chargera pas de les initialiser. L'initialisation des registres spéciaux se fera dans le code.
Nous nommerons les registres généraux de $0 à $31 conformément à l'annexe 1. Les registres pour le compteur programme PC et les registres HI et LO seront nommés respectivement $33, $34 et $35. HI et LO servent au résultat d'une multiplication des valeurs de deux registres.
Nous nécessiterons  deux fonctions pour utiliser les registres. La première servira à rentrer une valeur dans un registre et la seconde à récupérer la valeur d'un registre. De cette manière, nous pourrons initialiser les registres et réaliser les opérations voulues.