"""
SB_test_cases.resource
Author: Florentin LEPELTIER
Date: 9 June, 2023

Main file describing the automated tests cases of the SWARMBOTS project.
Ce fichier est basé sur les scénarios de validation définis dans le Plan de Test - ProSE_A2_PdT (v2.01 - Révision 10)
https://172.24.2.6/projects/se2024-a2/repository/doc/revisions/master/raw/gestion_projet/client/documents_signes/recette/%5BProSE%5D_A2_PDT_2024.pdf

MIT License
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Copyright (c) 2023, Prose A2 2023
"""
*** Settings ***
Resource    resources/android/CU_scenarios.resource

#Suite Setup         start Swarmbots
#Suite Teardown      stop Swarmbots

*** Test Cases ***
#Ici, on essaie de tester le maximum de choses avec 1 seul lancement de l'application sans redémarrage
Piloter un essaim de robots[un robot]
    Validation Manuelle Utilisateur    DÉMARRAGE DU TEST POUR 1 ROBOT
    #---INIT BEFORE STARTING APP---
    Utilisateur démarre SB_C    ${ID_ROBOT_1}
    SB_C indique qu’il est en attente de connexion à Utilisateur    ${ID_ROBOT_1}
    #------------------------------
    Start Swarmbots
    [P0]Connexion d'un robot
    #---DISPLAY COMMAND SCREEN---
    Utilisateur demande à afficher Ecran_Commande
    SB_IHM affiche Ecran_Commande
    Sleep   4s      #Attendre l'initialisation des fragments vidéo
    #----------------------------
    _[P1]_Transmission d'un flux caméra
    _[P1]_Sélection d'un robot
    _[P1]_Déplacement d'un robot
    _[P1]_Déplacement d'un robot[obstacle présent][buzzer et radar on]
    #_[P1]_Journalisation d'évènements[en console sur SB_IHM] --> Difficile à vérifier sans android studio mais fonctionnel
    _[P1]_Journalisation d'évènements[en console sur SB_C]
    _[P2]_Commande des périphériques[désactivation]
    #On redéplace le robot afin de vérifier les activations et desactivations du radar et buzzer
    _[P1]_Déplacement d'un robot[obstacle présent][buzzer et radar off]
    _[P2]_Commande des périphériques[activation]
    #On redéplace le robot afin de vérifier les activations et desactivations du radar et buzzer
    _[P1]_Déplacement d'un robot[obstacle présent][buzzer et radar on]
    _[P3]_Journalisation d'évènements[en mémoire sur SB_C]
    #_[P3]_Journalisation d'évènements[en mémoire sur SB_IHM] --> Non fonctionnel
    #_[P4]_Robustesse de connexion --> PopUp d'erreur
    #---DISPLAY LOGS SCREEN---
    Utilisateur demande à afficher Ecran_Logs
    SB_IHM affiche Ecran_Logs
    #-------------------------
    Sleep   3s
    _[P4]_Consultation des logs[selection d'une source de logs]
    #_[P5]_Consultation des logs[transfert et stockage des logs robot] --> Non fonctionnel
    #_[P5]_Consultation des logs[effacement des logs SB_C] --> Non fonctionnel
    #_[P6]_Export des logs --> Non fonctionnel
    Stop Swarmbots
    Sleep   3s

Piloter un essaim de robots[deux robots]
    Validation Manuelle Utilisateur    DÉMARRAGE DU TEST POUR 2 ROBOTS
    #---INIT BEFORE STARTING APP---
    Utilisateur démarre SB_C    ${ID_ROBOT_1}
    SB_C indique qu’il est en attente de connexion à Utilisateur    ${ID_ROBOT_1}
    Utilisateur démarre SB_C    ${ID_ROBOT_2}
    SB_C indique qu’il est en attente de connexion à Utilisateur    ${ID_ROBOT_2}
    #-------------------------------
    Start Swarmbots
    _[P0]_Connexion de plusieurs robots
    #---DISPLAY COMMAND SCREEN---
    Utilisateur demande à afficher Ecran_Commande
    SB_IHM affiche Ecran_Commande
    #----------------------------
    Sleep   8s      #Attendre l'initialisation des fragments vidéo
    _[P1]_Transmission de plusieurs flux caméra
    _[P1]_Sélection d'un robot
    _[P1]_Sélection d'un robot[robot déjà sélectionné]
    Sleep   2s
    Stop swarmbots
