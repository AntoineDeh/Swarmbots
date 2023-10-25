"""
complete_CU_scenarios_mateo.resource
Author: Matéo RONDEAU
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

#Ce fichier est temporaire et sera fusionné avec le CU_scenarios ultérieurement.

*** Settings ***
Resource        /../tools_config.resource
Resource        /../defs.resource
Resource        /../User_interactions.resource
Resource        /../SB_C.resource
Resource        /../SB_IHM.resource
Resource        /../CU_scenarios.resource

*** Keywords ***
'''-----------------------------------------SCENARIOS DE VALIDATION-----------------------------------------'''

'''---------------------TRANSMISSION DES FLUX CAMERA---------------------'''
#Envoi et réception des flux vidéo
Transmission d'un flux caméra
    #--------------Préconditions--------------#
    #SB_IHM et SB_C du robot ${id_robot} sont connectés.
    SB_IHM affiche Ecran_Commande
    #------------------Test------------------#
    #Utilisateur demande à sélectionner un robot afin de faire afficher les switchs à l'écran
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_1}
    SB_C indique qu’il est sélectionné à Utilisateur    ${ID_ROBOT_1}
    #Vérification que le Mode_Fonctionnement de la caméra est actif
    Vérifier que le switch est en vert  ${BTN_MODE_CAMERA}
    #Vérifier que le fragment affichant la vidéo est bien à l'écran
    Vérifier que la vidéo fonctionne    ${BTN_FLUX_VIDEO_ROBOT_1}
    Validation Manuelle Utilisateur    Vérifier que la vidéo fonctionne

Transmission de plusieurs flux caméra
    #--------------Préconditions--------------#
    #SB_IHM et SB_C du robot ${id_robot} sont connectés.
    SB_IHM affiche Ecran_Commande
    #------------------Test------------------#
    #Utilisateur demande à sélectionner le premier robot afin de faire afficher ses switchs à l'écran.
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_1}
    SB_C indique qu’il est sélectionné à Utilisateur    ${ID_ROBOT_1}
    #Vérification que le Mode_Fonctionnement de la caméra est actif
    Vérifier que le switch est en vert  ${BTN_MODE_CAMERA}
    #Vérifier que le fragment affichant la vidéo est bien à l'écran
    Vérifier que la vidéo fonctionne    ${BTN_FLUX_VIDEO_ROBOT_1}
    Validation Manuelle Utilisateur     Vérifier que la vidéo s'affiche bien sur l'écran de gauche
    sleep   3s
    #Utilisateur demande à sélectionner le second robot afin de faire afficher ses switchs à l'écran.
    Utilisateur demande à sélectionner un robot     ${BTN_FLUX_VIDEO_ROBOT_2}
    SB_C indique qu’il est sélectionné à Utilisateur    ${ID_ROBOT_2}
    #Vérification que le Mode_Fonctionnement de la caméra est actif
    Vérifier que le switch est en vert  ${BTN_MODE_CAMERA}
    #Vérifier que le fragment affichant la vidéo est bien à l'écran
    Vérifier que la vidéo fonctionne    ${BTN_FLUX_VIDEO_ROBOT_2}
    Validation Manuelle Utilisateur     Vérifier que la vidéo s'affiche bien sur l'écran de droite

'''---------------------CONSULTATION DES LOGS---------------------'''
#Arriver sur la page des logs
Consultation des logs
    #--------------Préconditions--------------#
    #Affichage de l'écran commande.
    SB_IHM affiche Ecran_Commande
    #------------------Test------------------#
    #Affichage de l'écran logs.
    Utilisateur demande à afficher Ecran_Logs
    sleep   2s
    SB_IHM indique que l'écran de logs est affiché    ${NAME_PAGE_LOGS}

#selection d'une source de logs
Consultation des logs[selection d'une source de logs]
    #--------------Préconditions--------------#
    #Affichage de la page des logs
    SB_IHM affiche Ecran_Logs
    #------------------Test------------------#
    #Appui sur le bouton logs IHM.
    Utilisateur demande à afficher les logs de l'IHM
    #Affichage des logs de l'IHM à l'écran
    SB_IHM affiche à l'écran les logs du bouton sélectionné     ${BTN_LOGS_ROBOT_IHM}

#Transfert et stockage des logs des robots
#Effacement des logs côté SB_C
Consultation des logs[transfert et stockage des logs des robots]
    #--------------Préconditions--------------#
    #Affichage de la page des logs
    SB_IHM affiche Ecran_Logs
    #------------------Test------------------#
    Utilisateur demande à afficher les logs du premier robot
    #Affichage des logs du robot1 sur l'écran.
    SB_IHM affiche à l'écran les logs du bouton sélectionné     ${BTN_LOGS_ROBOT_1}

Consultation des logs[effacement des logs SB_C]
    #--------------Préconditions--------------#
    #Affichage de la page des logs
    SB_IHM affiche Ecran_Logs
    Consultation des logs[transfert et stockage des logs des robots]
    #------------------Test------------------#
    Validation Manuelle Utilisateur     Vérifier que les logs sur SB_C ont bien été supprimés

'''-----------------------------------------ROBUSTESSE DE CONNEXION-----------------------------------------'''

#Checks de connexions successifs
Robustesse de connexion
    #--------------Préconditions--------------#
    #Vérifier que le robot a été allumé préalablement.
    SB_C indique qu’il est en attente de connexion à Utilisateur    ${ID_ROBOT_1}
    #------------------Test------------------#
    SB_IHM affiche Ecran_Commande
    SB_C se déconnecte de la tablette    ${ID_ROBOT_1}
    sleep   60s
    Validation Manuelle Utilisateur     Vérifier que sur la tablette le robot qui était connecté est maintenant déconnecté.


'''-----------------------------------------EXPORT DES LOGS-----------------------------------------'''
#Export des logs par l’OS Android
Export des logs
    #--------------Préconditions--------------#
    #Affichage de la page des logs
    SB_IHM affiche Ecran_Logs
    #------------------Test------------------#
    #Sélection de logs.
    #appui sur le bouton export.
    #Export les logs sélectionnés dans un dossier dans l'android.
