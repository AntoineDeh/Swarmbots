"""
CU_scenarios.resource
Author: Florentin LEPELTIER & Matéo RONDEAU
Date: 9 June, 2023

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
Resource        tools_config.resource
Resource        defs.resource
Resource        User_interactions.resource
Resource        SB_C.resource
Resource        SB_IHM.resource

*** Keywords ***
'''-----------------------------------------SCENARIOS DE VALIDATION-----------------------------------------'''

'''---------------------CONNEXION DES ROBOTS---------------------'''
#Éléments du Plan de Test : Commandes de gestion de la couleur des LEDs
_[P0]_Connexion d'un robot
    Utilisateur démarre SB_IHM
    SB_IHM affiche Ecran_Accueil
    #Vérifier affichage message accueil    ${TEXT_VIEW_ECRAN_ACCUEIL}  ${MESSAGE_ACCUEIL}
    Sleep   6s    #Timer défini en Android
    SB_C indique qu’il est connecté à Utilisateur  ${ID_ROBOT_1}
    Vérifier affichage message accueil     ${BTN_ECRAN_CMD_USER}    ${MESSAGE_ACCUEIL_SUCCES}
    Validation Manuelle Utilisateur    Vérifiez que le robot ${ID_ROBOT_1} est connecté dans la liste des robots (pastille verte)
    Validation Manuelle Utilisateur    Vérifiez que le robot ${ID_ROBOT_2} est déconnecté dans la liste des robots (pastille rouge)

_[P0]_Connexion de plusieurs robots
    Utilisateur démarre SB_IHM
    SB_IHM affiche Ecran_Accueil
    Vérifier affichage message accueil    ${TEXT_VIEW_ECRAN_ACCUEIL}  ${MESSAGE_ACCUEIL}
    Sleep   8s    #Timer Android
    SB_C indique qu’il est connecté à Utilisateur  ${ID_ROBOT_1}
    SB_C indique qu’il est connecté à Utilisateur  ${ID_ROBOT_2}
    Vérifier affichage message accueil     ${BTN_ECRAN_CMD_USER}    ${MESSAGE_ACCUEIL_SUCCES}
    Validation Manuelle Utilisateur    Vérifiez que le robot ${ID_ROBOT_1} est connecté dans la liste des robots (pastille verte)
    Validation Manuelle Utilisateur    Vérifiez que le robot ${ID_ROBOT_2} est connecté dans la liste des robots (pastille verte)

'''---------------------TRANSMISSION DES FLUX CAMERA---------------------'''
#Author : Matéo RONDEAU
#Éléments du Plan de Test : Envoi et réception des flux vidéo
_[P1]_Transmission d'un flux caméra
    Vérifier que la vidéo fonctionne    ${BTN_FLUX_VIDEO_ROBOT_1}
    Validation Manuelle Utilisateur    Vérifiez que la vidéo de robot ${ID_ROBOT_1} est reçue

_[P1]_Transmission de plusieurs flux caméra
    Vérifier que la vidéo fonctionne    ${BTN_FLUX_VIDEO_ROBOT_1}
    Validation Manuelle Utilisateur    Vérifiez que la vidéo de robot ${ID_ROBOT_1} fonctionne
    Vérifier que la vidéo fonctionne    ${BTN_FLUX_VIDEO_ROBOT_2}
    Validation Manuelle Utilisateur    Vérifiez que la vidéo de robot ${ID_ROBOT_2} fonctionne

'''---------------------SELECTION D'UN ROBOT---------------------'''
#Éléments du Plan de Test : Commandes de gestion de la couleur des LEDs, Retour de sélection sur l'IHM
_[P1]_Sélection d'un robot
    #--------------Préconditions--------------#
    SB_IHM indique qu'aucun robot n'est sélectionné à Utilisateur
    Vérifier que les boutons de commandes et de gestion des périphériques sont masqués
    #------------------Test------------------#
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_1}
    SB_IHM indique que le robot est sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_1}
    Validation Manuelle Utilisateur    Vérifiez que le robot ${ID_ROBOT_2} n'est pas sélectionné
    #SB_IHM indique que le robot n'est pas sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_2} --> False != false
    Vérifier que les boutons de commandes et de gestion des périphériques sont affichés
    SB_C indique qu’il est sélectionné à Utilisateur        ${ID_ROBOT_1}

_[P1]_Sélection d'un robot[robot déjà sélectionné]
    #--------------Préconditions--------------#
    SB_IHM indique que le robot est sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_1}
    #------------------Test------------------#
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_2}
    SB_IHM indique que le robot est sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_2}
    Validation Manuelle Utilisateur    Vérifiez que le robot ${ID_ROBOT_1} n'est pas sélectionné
    #SB_IHM indique que le robot n'est pas sélectionné à Utilisateur    ${RELATIVE_LAYOUT_FLUX_VIDEO_1} --> False != false
    Vérifier que les boutons de commandes et de gestion des périphériques sont affichés
    SB_C indique qu’il est connecté à Utilisateur       ${ID_ROBOT_1}
    SB_C indique qu’il est sélectionné à Utilisateur    ${ID_ROBOT_2}

'''---------------------DEPLACEMENT DES ROBOTS---------------------'''

Effectuer un déplacement
    [Arguments]     ${ID_ROBOT}     ${BTN_CMD}
    Utilisateur demande à déplacer un robot    ${ID_ROBOT}    ${BTN_CMD}
    #SB_IHM indique la pression bouton commande à Utilisateur    ${BTN_CMD} --> Manuel
    SB_C exécute le déplacement cmd_deplacement     ${ID_ROBOT}    ${BTN_CMD}
    Utilisateur demande à stopper un robot
    #SB_IHM indique le relâchement bouton commande à Utilisateur    ${BTN_CMD} --> Manuel
    SB_C exécute le déplacement cmd_arret   ${ID_ROBOT}

#Éléments du Plan de Test : Commandes de déplacement, Retour de commande IHM
_[P1]_Déplacement d'un robot
    #--------------Préconditions--------------#
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
_[P1]_Déplacement d'un robot[obstacle présent][buzzer et radar on]
    #--------------Préconditions--------------#
    Validation Manuelle Utilisateur     Vérifiez qu'un obstacle est présent à 20cm devant le robot
    #------------------Test------------------#
    Utilisateur demande à déplacer un robot    ${ID_ROBOT_1}    ${BTN_CMD_FORWARD}
    #SB_IHM indique la pression bouton commande à Utilisateur    ${BTN_CMD} --> Manuel
    SB_C exécute le déplacement cmd_deplacement     ${ID_ROBOT_1}    ${BTN_CMD_FORWARD}
    SB_C exécute le déplacement cmd_arret       ${ID_ROBOT_1}
    #SB_IHM indique le relâchement bouton commande à Utilisateur    ${BTN_CMD} --> Manuel
    SB_C indique qu’un obstacle a été détecté à Utilisateur   ${ID_ROBOT_1}
    SB_IHM indique qu'un obstacle a été détecté à Utilisateur   ${ID_ROBOT_1}

#Éléments du Plan de Test : Arrêt d'urgence du robot, Commandes de gestion de la couleur des LEDs, Gestion du buzzer, Envoi du flux radar IR
_[P1]_Déplacement d'un robot[obstacle présent][buzzer et radar off]
    #--------------Préconditions--------------#
    Validation Manuelle Utilisateur     Vérifiez qu'un obstacle est présent à 20cm devant le robot
    #------------------Test------------------#
    Utilisateur demande à déplacer un robot    ${ID_ROBOT_1}    ${BTN_CMD_FORWARD}
    #SB_IHM indique la pression bouton commande à Utilisateur    ${BTN_CMD}
    SB_C exécute le déplacement cmd_deplacement     ${ID_ROBOT_1}    ${BTN_CMD_FORWARD}
    SB_C exécute le déplacement cmd_arret       ${ID_ROBOT_1}
    #SB_IHM indique le relâchement bouton commande à Utilisateur    ${BTN_CMD}
    Validation Manuelle Utilisateur    Vérifiez que le buzzer et les LEDs ne se sont pas allumé(e)s malgré la présence de l'obstacle


'''---------------------COMMANDES DES PÉRIPHÉRIQUES---------------------'''
#Éléments du Plan de Test : Commandes de désactivation des périphériques
_[P2]_Commande des périphériques[désactivation]
    #--------------Préconditions--------------#
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

#Éléments du Plan de Test : Commandes d'activation des périphériques
_[P2]_Commande des périphériques[activation]
    #--------------Préconditions--------------#
    #Vérifier que les périphériques sont désactivés : déjà fait via _[P2]_Commande des périphériques[désactivation]
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
_[P1]_Journalisation d'évènements[en console sur SB_C]
    Validation Manuelle Utilisateur     Vérifiez que des logs sont présents en console SSH dans le bon format et que la date correspond à l'exécution

#Éléments du Plan de Test : Affichage des logs en console lors de l'exécution
_[P1]_Journalisation d'évènements[en console sur SB_IHM]
    Validation Manuelle Utilisateur     Vérifiez que des logs sont présents sur l'IHM en console, dans le bon format et que la date correspond à l'exécution

#Éléments du Plan de Test : Enregistrement des logs en mémoire lors de l'exécution, Alerte pour espace mémoire insuffisant des robots
_[P3]_Journalisation d'évènements[en mémoire sur SB_C]
    Validation Manuelle Utilisateur     Vérifiez qu'un fichier de logs existe sur la RaspberryPi (robot), qu'il contient des logs dans le bon format, et que la date correspond à l'exécution

#Éléments du Plan de Test : Enregistrement des logs en mémoire lors de l'exécution
_[P3]_Journalisation d'évènements[en mémoire sur SB_IHM]
    Validation Manuelle Utilisateur     Vérifiez qu'un fichier de logs existe sur la tablette, qu'il contient des logs dans le bon format, et que la date correspond à l'exécution

'''-----------------------------------------ROBUSTESSE DE CONNEXION-----------------------------------------'''
#Author : Matéo RONDEAU
#Checks de connexions successifs
_[P4]_Robustesse de connexion
    SB_IHM affiche Ecran_Commande
    SB_C se déconnecte de la tablette    ${ID_ROBOT_1}
    sleep   10s
    Validation Manuelle Utilisateur     Vérifiez que sur la tablette le robot qui était connecté est maintenant déconnecté.

'''---------------------CONSULTATION DES LOGS---------------------'''
#Author : Matéo RONDEAU
#Éléments du Plan de Test : selection d'une source de logs
_[P4]_Consultation des logs[selection d'une source de logs]
    Utilisateur demande à afficher les logs de l'IHM
    SB_IHM affiche à l'écran les logs du bouton sélectionné     ${BTN_LOGS_ROBOT_IHM}

#Author : Matéo RONDEAU
#Éléments du Plan de Test : Transfert et stockage des logs des robots, Effacement des logs côté SB_C
_[P5]_Consultation des logs[transfert et stockage des logs des robots]
    Utilisateur demande à afficher les logs du premier robot
    SB_IHM affiche à l'écran les logs du bouton sélectionné     ${BTN_LOGS_ROBOT_1}

#Author : Matéo RONDEAU
_[P5]_Consultation des logs[effacement des logs SB_C]
    Validation Manuelle Utilisateur     Vérifiez que les logs ont bien été supprimés sur SB_C

'''-----------------------------------------EXPORT DES LOGS-----------------------------------------'''
#Éléments du Plan de Test : export des logs par l’OS Android
_[P6]_Export des logs
    #Sélection de logs.
    #appui sur le bouton export.
    #Export les logs sélectionnés dans un dossier dans l'android.



