"""
SB_IHM.resource
Author: Florentin LEPELTIER
Date: 9 June, 2023

Ce fichier décrit les fonctionnalités d'automatisation des tests de validation en lien avec l'acteur Utilisateur

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
Library    AppiumLibrary
Library    Dialogs

Resource    defs.resource

*** Variables ***

*** Keywords ***
'''------------------------------------- GENERAL -------------------------------------'''
Utilisateur démarre SB_C
    [Arguments]     ${ID_ROBOT}
    Validation Manuelle Utilisateur    Veuillez démarrer le robot ${ID_ROBOT}

Utilisateur démarre SB_IHM
    log     Utilisateur démarre l'application Swarmbots sur la tablette

'''------------------------------------- ECRAN ACCUEIL -------------------------------------'''
Utilisateur demande à afficher Ecran_Commande
    Click Element    ${BTN_ECRAN_CMD_USER}

'''------------------------------------- ECRAN COMMANDE -------------------------------------'''
Utilisateur demande à déplacer un robot
    [Arguments]    ${ID_ROBOT}  ${BTN_CMD}
    Long Press    ${BTN_CMD}    3

Utilisateur demande à stopper un robot
    log     Utilisateur relâche le bouton précédemment pressé (automatique)

Utilisateur demande à afficher Ecran_Logs
    Click Element    ${BTN_ECRAN_LOGS}

Utilisateur demande à sélectionner un robot
    [Arguments]      ${BTN_ROBOT}
    Click Element    ${BTN_ROBOT}

Utilisateur demande à modifier Mode_Fonctionnement[camera]
   Click Element    ${BTN_MODE_CAMERA}

Utilisateur demande à modifier Mode_Fonctionnement[buzzer]
   Click Element    ${BTN_MODE_BUZZER}

Utilisateur demande à modifier Mode_Fonctionnement[radar]
   Click Element    ${BTN_MODE_RADAR}

Utilisateur demande à modifier Mode_Fonctionnement[led]
   Click Element    ${BTN_MODE_LED}

'''------------------------------------- ECRAN LOGS -------------------------------------'''
Utilisateur demande à afficher les logs de l'IHM
    Click Element   ${BTN_LOGS_ROBOT_IHM}

Utilisateur demande à afficher les logs du premier robot
    Click Element   ${BTN_LOGS_ROBOT_1}

Utilisateur demande à afficher les logs du second robot
    Click Element   ${BTN_LOGS_ROBOT_2}