# Servotron

Robot mobile à deux roues piloté par un ESP32. Il se commande depuis une interface web, trace des formes (carré, triangle, cercle), suit une ligne au sol et évite les obstacles.

Ce dépôt contient tout le nécessaire pour construire une réplique du robot et tester son code, ainsi qu'une simulation Webots.

## Organisation

| Dossier | Contenu |
|---|---|
| `robot-reel/cao` | Pièces du robot (STL et SolidWorks) |
| `robot-reel/ihm` | Interface web, lisible séparément |
| `robot-reel/code-final` | Programme complet du robot |
| `robot-reel/tests-unitaires` | Un programme par capteur, actionneur et fonction |
| `simulation` | Projet Webots complet |

## Robot réel

### Câblage

| Élément | Broches ESP32 |
|---|---|
| Moteur 1 | DIR 25, PWM 26 |
| Moteur 2 | DIR 5, PWM 17 |
| Capteurs de ligne (gauche, centre, droit) | 32, 35, 33 |
| Ultrason 1 | TRIG 23, ECHO 22 |
| Ultrason 2 | TRIG 21, ECHO 15 |
| LED RVB (rouge, vert, bleu) | 2, 19, 12 |
| Bouton suivi de ligne | 13 |
| Bouton obstacle | 14 |
| Bouton carré | 27 |
| Bouton stop | 18 |

Les moteurs sont montés en miroir : pour avancer, DIR du moteur 1 est à l'état bas et DIR du moteur 2 à l'état haut.

### Installer le code final

1. Installer l'Arduino IDE et le paquet ESP32 (cœur Arduino 3.x).
2. Ouvrir `robot-reel/code-final/Servotron/Servotron.ino`.
3. Renseigner `WIFI_SSID` et `WIFI_MDP` en haut du fichier.
4. Téléverser le programme, puis lire l'adresse de l'IHM dans le moniteur série (115200 bauds).

Si le réseau est introuvable, le robot crée le sien : `Servotron`, mot de passe `servotron`.

### Utilisation

| Commande | Comportement | Couleur de la LED |
|---|---|---|
| Flèches et Stop | Le robot roule tant que le bouton reste appuyé | Éteinte |
| Suivi de ligne | Suit une ligne noire avec les 3 capteurs | Vert |
| Évitement d'obstacle | S'arrête à moins de 20 cm, puis tourne du côté le plus dégagé | Rouge |
| Carré | Quatre côtés de 2 s, virages de 690 ms | Bleu |
| Triangle | Trois côtés et deux virages chronométrés | Violet |
| Cercle | Roues à vitesses différentes pendant 4,5 s | Jaune |

Le robot n'a pas d'encodeurs : les formes sont chronométrées. Il faut ajuster les durées selon le sol et la charge de la batterie.

### Tests unitaires

Chaque test est indépendant : on l'ouvre dans l'Arduino IDE et on le téléverse. Les tests des fonctions démarrent 3 secondes après la mise sous tension, le temps de poser le robot.

| Dossier | Tests |
|---|---|
| `capteurs` | `test_ultrason`, `test_capteurs_ligne` |
| `moteurs` | `test_moteurs` |
| `interface` | `test_boutons`, `test_led` |
| `fonctions` | `avancer`, `reculer`, `tourner_gauche`, `tourner_droite`, `carre`, `triangle`, `cercle`, `suivi_ligne`, `evitement_obstacle` |

Je conseille de commencer par `test_moteurs`, roues levées, pour vérifier le câblage et le sens de rotation.

## Simulation Webots

Réplique du robot dans une arène de 5 m sur 5 m, avec une piste, une zone de dessin et un enclos d'obstacles. Elle diffère du robot réel sur deux points : elle utilise l'odométrie pour les formes, et ses capteurs d'obstacle sont un ultrason et deux capteurs infrarouges.

Pour la lancer avec Webots R2025a :

1. Ouvrir `simulation/worlds/servotron_arena.wbt`.
2. Ouvrir `servotron.cpp` dans l'éditeur de Webots et le compiler.
3. Faire un clic droit sur le robot, puis **Show Robot Window**.

Résultats mesurés en simulation :

| Test | Résultat |
|---|---|
| Carré de 60 cm | Écart de 2 mm à l'arrivée |
| Triangle de 70 cm | Écart de 2 mm à l'arrivée |
| Cercle de 35 cm de rayon | Écart de 4 cm à l'arrivée |
| Suivi de ligne pendant 70 s | Ligne vue 100 % du temps |

## Auteur

Manuel Parker NGOUNOU
