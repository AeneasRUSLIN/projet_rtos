Ce projet a pour objectif de programmer une carte arduino en utilisant FreeRTOS afin d'accomplir plusieurs tâches.

La première tâche récupère la valeur analogique du potentiomètre et l'envoie à la tâche 3.
La deuxième tâche récupère la valeur numérique de l'addition des entrées des pin 3 et 4. Des boutons poussoirs permettent de faire l'addition. Cette valeur est envoyée à la tâche 3.
La troisième tâche récupère les valeurs des tâches 1 et 2. Ces valeurs sont ensuite insérées dans une structure.
La tâche 4 reçoit la structure de la tâche 4 puis utilise le port série pour afficher les valeurs.
La tâche 5 consiste à convertir la valeur du temps (ms) en minutes.
