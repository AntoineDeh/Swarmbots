"""
CU_scenarios.resource
Author: Florentin LEPELTIER
Date: 9 June, 2023

Ce fichier est basé sur les scénarios de validation définis dans le Plan de Test - ProSE_A2_PdT (v2.01 - Révision 10)
https://172.24.2.6/projects/se2024-a2/repository/doc/revisions/master/raw/gestion_projet/client/documents_signes/recette/%5BProSE%5D_A2_PDT_2024.pdf
Il comporte une phase d'initialisation des préconditions pour chaque test.

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
Resource        ../tools_config.resource
Resource        ../defs.resource
Resource        ../User_interactions.resource
Resource        ../SB_C.resource
Resource        ../SB_IHM.resource

*** Keywords ***
'''-----------------------------------------SCENARIOS DE VALIDATION-----------------------------------------'''

'''---------------------SET UP---------------------'''
Initialisation Ecran_Commande
    SB_IHM affiche Ecran_Accueil
    sleep    8s    #Wait for at least one robot to connect
    Utilisateur demande à afficher Ecran_Commande
    SB_IHM affiche Ecran_Commande
'''------------------------------------------------'''


'''---------------------CONNEXION DES ROBOTS---------------------'''
#Lors de l'exécution du test, au démarrage, l'application essaie automatiquement de se connecter aux robots,
#l'utilisateur n'a donc pas vriament le temps de valider l'état d'attente de connexion car le robot se connecte rapidement.
#L'idée est de démarrer le robot en amont du test afin de faire ces vérifications
#Éléments du Plan de Test : Commandes de gestion de la couleur des LEDs
Connexion d'un robot
    Utilisateur démarre SB_C
    SB_C indique qu’il est en attente de connexion à Utilisateur    ${ID_ROBOT_1}
    Utilisateur démarre SB_IHM
    SB_IHM affiche Ecran_Accueil
    #Vérifier affichage message accueil     ${MESSAGE_ACCUEIL}
    SB_C indique qu’il est connecté à Utilisateur  ${ID_ROBOT_1}
    #Vérifier affichage message accueil     ${MESSAGE_ACCUEIL_SUCCES}
    #idRobot ?
    #Pastille verte --> Ergo ?


Connexion de plusieurs robots
    #TODO


'''---------------------SELECTION D'UN ROBOT---------------------'''
#Éléments du Plan de Test : Commandes de gestion de la couleur des LEDs, Retour de sélection sur l'IHM
Sélection d'un robot
    #--------------Préconditions--------------#
    #Le robot ayant pour id 1 doit être allumé et en attente de connexion (SB_C démarré)
    Initialisation Ecran_Commande
    SB_IHM indique qu'aucun robot n'est sélectionné à Utilisateur
    #Verif que les btns de commande de périph et déplacement sont masquées ?
    #------------------Test------------------#
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_1}
    SB_IHM indique que le robot est sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_1}
    SB_IHM indique que le robot n'est pas sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_2}
    #Verif que les btns de commande de périph et déplacement sont affichées ?
    SB_C indique qu’il est sélectionné à Utilisateur        ${ID_ROBOT_1}

Sélection d'un robot[robot déjà sélectionné]
   #--------------Préconditions--------------#
    #Robot 1 est déjà sélectionné
    Initialisation Ecran_Commande
    SB_IHM indique que le robot est sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_1}
    #------------------Test------------------#
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_2}
    SB_IHM indique que le robot est sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_2}
    SB_IHM indique que le robot n'est pas sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_1}
    #Verif que les btns de commande de périph et déplacement sont affichées ?
    SB_C indique qu’il est connecté à Utilisateur       ${ID_ROBOT_1}
    SB_C indique qu’il est sélectionné à Utilisateur    ${ID_ROBOT_2}

'''---------------------DEPLACEMENT DES ROBOTS---------------------'''
Effectuer un déplacement
    [Arguments]     ${ID_ROBOT}     ${BTN_CMD}
    Utilisateur demande à déplacer un robot    ${ID_ROBOT}    ${BTN_CMD}
    #SB_IHM indique la commande à Utilisateur
    SB_C exécute le déplacement cmd_deplacement     ${ID_ROBOT}    ${BTN_CMD}
    #Utilisateur demande à stopper un robot --> Détection du relachement du btn auto ?
    #SB_IHM indique la commande à Utilisateur
    SB_C exécute le déplacement cmd_arret   ${ID_ROBOT}

#Éléments du Plan de Test : Commandes de déplacement, Retour de commande IHM
Déplacement d'un robot
    #--------------Préconditions--------------#
    #Le robot ayant pour id 1 doit être allumé et en attente de connexion (SB_C démarré)
    Initialisation Ecran_Commande
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_1}
    Validation Manuelle Utilisateur     Vérifiez qu'aucun obstacle n'est présent autour du robot
    #------------------Test------------------#
    Effectuer un déplacement    ${ID_ROBOT_1}    ${BTN_CMD_FORWARD}
    Sleep   1s
    Effectuer un déplacement    ${ID_ROBOT_1}    ${BTN_CMD_BACKWARD}
    Sleep   1s
    Effectuer un déplacement    ${ID_ROBOT_1}    ${BTN_CMD_RIGHT}
    Sleep   1s
    Effectuer un déplacement    ${ID_ROBOT_1}    ${BTN_CMD_LEFT}
    Sleep   1s

#Éléments du Plan de Test : Arrêt d'urgence du robot, Commandes de gestion de la couleur des LEDs, Gestion du buzzer, Envoi du flux radar IR
Déplacement d'un robot[obstacle présent]
     #--------------Préconditions--------------#
    #Le robot ayant pour id 1 doit être allumé et en attente de connexion (SB_C démarré) et un obstacle doit être présent à 2m devant le robot
    Initialisation Ecran_Commande
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_1}
    Validation Manuelle Utilisateur     Vérifiez qu'un obstacle est présent à 20cm devant le robot
    Utilisateur demande à déplacer un robot    ${ID_ROBOT_1}    ${BTN_CMD_FORWARD}
    #SB_IHM indique la commande à Utilisateur
    SB_C exécute le déplacement cmd_deplacement     ${ID_ROBOT_1}    ${BTN_CMD_FORWARD}
    Sleep   3s      #Wait for the robot to stop
    SB_C exécute le déplacement cmd_arret       ${ID_ROBOT_1}
    #SB_IHM indique la commande à Utilisateur
    SB_C indique qu’un obstacle a été détecté à Utilisateur   ${ID_ROBOT_1}
    SB_IHM indique qu'un obstacle a été détecté à Utilisateur   ${ID_ROBOT_1}

'''---------------------COMMANDES DES PÉRIPHÉRIQUES---------------------'''
#Éléments du Plan de Test : Commandes de désactivation des périphériques
Commande des périphériques[désactivation]
    #--------------Préconditions--------------#
    #Le robot ayant pour id 1 doit être allumé et en attente de connexion (SB_C démarré)
    Initialisation Ecran_Commande
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_1}
    #Les périphériques sont activés par défaut sur le robot
    Vérifier que les périphériques sont activés
    #------------------Test------------------#
    Utilisateur demande à modifier Mode_Fonctionnement[led]
    Vérifier que le switch est en rouge     ${BTN_MODE_LED}
    Validation Manuelle Utilisateur     Vérifiez que les leds du robot sont désactivées
    Utilisateur demande à modifier Mode_Fonctionnement[camera]
    Vérifier que le switch est en rouge     ${BTN_MODE_CAMERA}
    Validation Manuelle Utilisateur     Vérifiez que le flux caméra n'est plus reçu
    Utilisateur demande à modifier Mode_Fonctionnement[radar]
    Vérifier que le switch est en rouge     ${BTN_MODE_RADAR}
    Validation Manuelle Utilisateur     Vérifiez que le radar du robot est désactivé
    Utilisateur demande à modifier Mode_Fonctionnement[buzzer]
    Vérifier que le switch est en rouge     ${BTN_MODE_BUZZER}
    Validation Manuelle Utilisateur     Vérifiez que le buzzer du robot est désactivé
    #Vérifier que les périphériques sont désactivés --> Tout check d'un coup ?

#Éléments du Plan de Test : Commandes d'activation des périphériques
Commande des périphériques[activation]
    #--------------Préconditions--------------#
    #Le robot ayant pour id 1 doit être allumé et en attente de connexion (SB_C démarré)
    Initialisation Ecran_Commande
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_1}
    #Il n'est pas possible de démarrer le robot avec les périphériques désactivés par défaut, une nouvelle compilation est nécessaire
    #On désactive donc les périphériques dans les préconditions
    Utilisateur demande à modifier Mode_Fonctionnement[led]
    Utilisateur demande à modifier Mode_Fonctionnement[camera]
    Utilisateur demande à modifier Mode_Fonctionnement[radar]
    Utilisateur demande à modifier Mode_Fonctionnement[buzzer]
    Vérifier que les périphériques sont désactivés
    #------------------Test------------------#
    Utilisateur demande à modifier Mode_Fonctionnement[led]
    Vérifier que le switch est en vert     ${BTN_MODE_LED}
    Validation Manuelle Utilisateur     Vérifiez que les leds du robot sont activées
    Utilisateur demande à modifier Mode_Fonctionnement[camera]
    Vérifier que le switch est en vert     ${BTN_MODE_CAMERA}
    Validation Manuelle Utilisateur     Vérifiez que le flux vidéo du robot est bien reçu
    Utilisateur demande à modifier Mode_Fonctionnement[radar]
    Vérifier que le switch est en vert     ${BTN_MODE_RADAR}
    Validation Manuelle Utilisateur     Vérifiez que le radar du robot est activé
    Utilisateur demande à modifier Mode_Fonctionnement[buzzer]
    Vérifier que le switch est en vert     ${BTN_MODE_BUZZER}
    Validation Manuelle Utilisateur      Vérifiez que le buzzer du robot est activé

'''---------------------JOURNALISATION D'ÉVÈNEMENTS---------------------'''
#Éléments du Plan de Test : Affichage des logs en console lors de l'exécution
Journalisation d'évènements[en console sur SB_C]
    #--------------Préconditions--------------#
    #Le robot ayant pour id 1 doit être allumé et en attente de connexion (SB_C démarré)
    #------------------Test------------------#
    Initialisation Ecran_Commande
    Validation Manuelle Utilisateur     Vérifiez que des logs sont présents en console SSH dans le bon format et que la date correspond à l'exécution

#Éléments du Plan de Test : Affichage des logs en console lors de l'exécution
Journalisation d'évènements[en console sur SB_IHM]
    #--------------Préconditions--------------#
    #Le robot ayant pour id 1 doit être allumé et en attente de connexion (SB_C démarré)
    #------------------Test------------------#
    Initialisation Ecran_Commande
    #Validation Manuelle Utilisateur     Vérifiez que ?

#Éléments du Plan de Test : Enregistrement des logs en mémoire lors de l'exécution, Alerte pour espace mémoire insuffisant des robots
Journalisation d'évènements[en mémoire sur SB_C]
    #--------------Préconditions--------------#
    #Le robot ayant pour id 1 doit être allumé et en attente de connexion (SB_C démarré)
    #------------------Test------------------#
    Initialisation Ecran_Commande
    Validation Manuelle Utilisateur     Vérifiez qu'un fichier de logs existe sur la RaspberryPi (robot), qu'il contient des logs dans le bon format, et que la date correspond à l'exécution

#Éléments du Plan de Test : Enregistrement des logs en mémoire lors de l'exécution
Journalisation d'évènements[en mémoire sur SB_IHM]
    #--------------Préconditions--------------#
    #Le robot ayant pour id 1 doit être allumé et en attente de connexion (SB_C démarré)
    #------------------Test------------------#
    Initialisation Ecran_Commande
    Validation Manuelle Utilisateur     Vérifiez qu'un fichier de logs existe sur la tablette, qu'il contient des logs dans le bon format, et que la date correspond à l'exécution





