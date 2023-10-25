"""
SB_IHM.resource
Author: Florentin LEPELTIER & Matéo RONDEAU
Date: 9 June, 2023

Ce fichier décrit les fonctionnalités d'automatisation des tests de validation en lien avec SB_IHM

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

Resource    tools_config.resource
Resource    defs.resource
Library     AppiumLibrary
Library     Dialogs

Resource    defs.resource

*** Variables ***

*** Keywords ***
'''------------------------------------- ECRAN_ACCUEIL -------------------------------------'''
SB_IHM affiche Ecran_Accueil
    wait activity    .view.activities.HomeScreenActivity    3

SB_IHM récupère Mode_Fonctionnement
    sleep    6s

Vérifier affichage message accueil
    [Arguments]     ${ID}    ${message}
    ${text_element}    Get Text    ${ID}
    Should Match    ${text_element}    ${message}

'''------------------------------------- ECRAN_COMMANDE -------------------------------------'''
SB_IHM affiche Ecran_Commande
    wait activity    .view.activities.CommandScreenActivity    3

#Sélection d'un robot
SB_IHM indique que le robot est sélectionné à Utilisateur
    [Arguments]     ${RELATIVE_LAYOUT_FLUX_VIDEO}
    Element Should Be Visible    ${RELATIVE_LAYOUT_FLUX_VIDEO}

SB_IHM indique que le robot n'est pas sélectionné à Utilisateur
    [Arguments]     ${RELATIVE_LAYOUT_FLUX_VIDEO}
    ${isVisible}=  Run Keyword And Return Status   Element Should Be Visible   ${RELATIVE_LAYOUT_FLUX_VIDEO}
    Should Be Equal  ${isVisible}    false

SB_IHM indique qu'aucun robot n'est sélectionné à Utilisateur
    [Arguments]     @{ROBOT_LIST}
    FOR    ${ID_ROBOT}    IN    @{ROBOT_LIST}
        ${RELATIVE_LAYOUT_FLUX_VIDEO_ID_ROBOT}    Catenate    'RELATIVE_LAYOUT_FLUX_VIDEO_'    ${ID_ROBOT}
        SB_IHM indique que le robot n'est pas sélectionné à Utilisateur     ${RELATIVE_LAYOUT_FLUX_VIDEO_ID_ROBOT}
    END

#Déplacement d'un robot
SB_IHM indique qu'un obstacle a été détecté à Utilisateur
    [Arguments]     ${ID_ROBOT}
    Validation Manuelle Utilisateur    Vérifiez que le radar est rouge sur l'IHM

Vérifier que les boutons de commandes et de gestion des périphériques sont masqués
    Validation Manuelle Utilisateur    Vérifiez que les les boutons de commandes et de gestion des périphériques sont masqués

Vérifier que les boutons de commandes et de gestion des périphériques sont affichés
    Validation Manuelle Utilisateur    Vérifiez que les les boutons de commandes et de gestion des périphériques sont affichés

SB_IHM indique la pression bouton commande à Utilisateur
    [Arguments]     ${BTN_CMD}
    #${background}=  Get Element Attribute  ${BTN_CMD}    background
    #Should Be Equal  ${background}    arrow_up_blue
    #TODO : Attribute 'background' is not valid for element

SB_IHM indique le relâchement bouton commande à Utilisateur
    [Arguments]     ${BTN_CMD}
    #${background}=  Get Element Attribute  ${BTN_CMD}    background
    #Should Be Equal  ${background}    arrow_up_blue
    #TODO : Attribute 'background' is not valid for element

#Activation/désactivation de périphériques
Vérifier que le switch est en rouge
    [Arguments]     ${ID_SWITCH}
    ${isChecked}=    Get Element Attribute   ${ID_SWITCH}   checked
    Should Be Equal  ${isChecked}    false

Vérifier que le switch est en vert
    [Arguments]     ${ID_SWITCH}
    ${isChecked}=    Get Element Attribute   ${ID_SWITCH}   checked
    Should Be Equal  ${isChecked}    true

Vérifier que les périphériques sont désactivés
    Vérifier que le switch est en rouge     ${BTN_MODE_LED}
    Validation Manuelle Utilisateur     Vérifiez que les leds du robot sont désactivées
    Vérifier que le switch est en rouge     ${BTN_MODE_CAMERA}
    Validation Manuelle Utilisateur     Vérifiez que le flux vidéo du robot n'est pas reçu
    Vérifier que le switch est en rouge     ${BTN_MODE_RADAR}
    Validation Manuelle Utilisateur     Vérifiez que le radar du robot est désactivé
    Vérifier que le switch est en rouge     ${BTN_MODE_BUZZER}
    Validation Manuelle Utilisateur     Vérifiez que le buzzer du robot est désactivé

Vérifier que les périphériques sont activés
    Vérifier que le switch est en vert     ${BTN_MODE_LED}
    Validation Manuelle Utilisateur     Vérifiez que les leds du robot sont activées
    Vérifier que le switch est en vert     ${BTN_MODE_CAMERA}
    Validation Manuelle Utilisateur     Vérifiez que le flux vidéo du robot est bien reçu
    Vérifier que le switch est en vert     ${BTN_MODE_RADAR}
    Validation Manuelle Utilisateur     Vérifiez que le radar du robot est activé
    Vérifier que le switch est en vert     ${BTN_MODE_BUZZER}
    Validation Manuelle Utilisateur     Vérifiez que le buzzer du robot est activé

#Author : Matéo RONDEAU
#Vidéo
Vérifier que la vidéo fonctionne
    [Arguments]    ${ID_ECRAN}
    Element Should Be Visible   ${ID_ECRAN}

'''------------------------------------- ECRAN_LOGS -------------------------------------'''
SB_IHM affiche Ecran_Logs
    wait activity    .view.activities.LogsScreenActivity    3

#Author : Matéo RONDEAU
SB_IHM indique que l'écran de logs est affiché
    [Arguments]     ${PAGE_NAME}
    Element Should Be Visible   ${PAGE_NAME}

#Author : Matéo RONDEAU
#Bouton pour afficher les logs
SB_IHM affiche à l'écran les logs du bouton sélectionné
    [Arguments]    ${BUTTON_LOGS}
    Execute Manual Step     Vérifiez que les logs s'affichent dans le bon format et que la date correspond à l'exécution.

