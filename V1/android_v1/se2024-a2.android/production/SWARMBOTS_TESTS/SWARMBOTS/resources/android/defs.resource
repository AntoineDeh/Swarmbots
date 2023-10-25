"""
SB_IHM.resource
Author: Florentin LEPELTIER
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

"""
Notes
RELATIVE_LAYOUT_FLUX_VIDEO_ID_ROBOT :
    Please, respect this format as it is used in some loops to dynamically check for selected robots
    see : SB_IHM indique qu'aucun robot n'est sélectionné à Utilisateur (SB_IHM.resource)
"""
*** Settings ***
Resource    tools_config.resource
Library     Dialogs

*** Variables ***
#-----------------------STRINGS-----------------------
${MESSAGE_ACCUEIL}                  Connexion en cours...
${MESSAGE_ACCUEIL_ERREUR}           Veuillez renseigner au moins 1 robot dans le fichier de configuration
${MESSAGE_ACCUEIL_SUCCES}           TOUCHER L'ÉCRAN POUR COMMENCER
#-----------------------ROBOTS-----------------------
${ID_ROBOT_1}                       1
${ID_ROBOT_2}                       2
@{ROBOT_LIST}    Create List    ${ID_ROBOT_1}    ${ID_ROBOT_2}
#-----------------------ECRAN ACCUEIL-----------------------
${BTN_ECRAN_CMD_USER}               id=${SWARMBOTS_PACKAGE}:id/toucher_ici
${TEXT_VIEW_ECRAN_ACCUEIL}          id=${SWARMBOTS_PACKAGE}:id/connexion_e
#-----------------------ECRAN COMMANDE-----------------------
${BTN_ECRAN_LOGS}                   id=${SWARMBOTS_PACKAGE}:id/frame_68
${BTN_ECRAN_ACCUEIL}                id=${SWARMBOTS_PACKAGE}:id/frame_67
${BTN_FLUX_VIDEO_ROBOT_1}           id=${SWARMBOTS_PACKAGE}:id/fragmentContainerView
${RELATIVE_LAYOUT_FLUX_VIDEO_1}     id=${SWARMBOTS_PACKAGE}:id/cache_frame5
${BTN_FLUX_VIDEO_ROBOT_2}           id=${SWARMBOTS_PACKAGE}:id/fragmentContainerView5
${RELATIVE_LAYOUT_FLUX_VIDEO_2}     id=${SWARMBOTS_PACKAGE}:id/cache_frame6
${RES_SELECTED_ROBOT}               id=${SWARMBOTS_PACKAGE}:@drawable/frame_5_cache
#Commandes
${BTN_CMD_FORWARD}                  id=${SWARMBOTS_PACKAGE}:id/btnForward
${BTN_CMD_LEFT}                     id=${SWARMBOTS_PACKAGE}:id/btnLeft
${BTN_CMD_RIGHT}                    id=${SWARMBOTS_PACKAGE}:id/btnRight
${BTN_CMD_BACKWARD}                 id=${SWARMBOTS_PACKAGE}:id/btnBackward
${BTN_CMD}                          id=${SWARMBOTS_PACKAGE}:id/commandBtns
#Mode fonctionnement
${BTN_MODE_RADAR}                   id=${SWARMBOTS_PACKAGE}:id/switch_radar
${BTN_MODE_BUZZER}                  id=${SWARMBOTS_PACKAGE}:id/switch_buzzer
${BTN_MODE_CAMERA}                  id=${SWARMBOTS_PACKAGE}:id/switch_camera
${BTN_MODE_LED}                     id=${SWARMBOTS_PACKAGE}:id/switch_led
${SWITCH_FRAGMENT}                  id=${SWARMBOTS_PACKAGE}:id/fragmentContainerView2

#-----------------------ECRAN LOGS-----------------------
#Boutons
${BTN_ECRAN_CMD}                    id=${SWARMBOTS_PACKAGE}:id/frame_67
${BTN_LOGS_EXPORT}                  id=${SWARMBOTS_PACKAGE}:id/frame_68
#Boutons logs
${BTN_LOGS_ROBOT_IHM}              id=${SWARMBOTS_PACKAGE}:id/button
${BTN_LOGS_ROBOT_1}                id=${SWARMBOTS_PACKAGE}:id/button2
${BTN_LOGS_ROBOT_2}                id=${SWARMBOTS_PACKAGE}:id/button3
#Nom de la page
${NAME_PAGE_LOGS}                  id=${SWARMBOTS_PACKAGE}:id/logs

*** Keywords ***
#-----------------------GENERIC KEYWORDS-------------------------
Validation Manuelle Utilisateur
    [Arguments]     ${message}
    Execute Manual Step    ${message}
