"""
SB_C.resource
Author: Florentin LEPELTIER
Date: 9 June, 2023

Ce fichier décrit les fonctionnalités d'automatisation des tests de validation en lien avec SB_C

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
'''------------------------------------- SB_C -------------------------------------'''
#ETATS DE CONNEXION
SB_C indique qu’il est en attente de connexion à Utilisateur
    [Arguments]     ${ID_ROBOT}
    Execute Manual Step    Vérifiez la couleur des LEDs du robot ${ID_ROBOT} : vert clignotant

SB_C indique qu’il est connecté à Utilisateur
    [Arguments]     ${ID_ROBOT}
    Execute Manual Step    Vérifiez la couleur des LEDs du robot ${ID_ROBOT} : vert fixe

SB_C indique qu’un obstacle a été détecté à Utilisateur
    [Arguments]     ${ID_ROBOT}
    Execute Manual Step    Vérifiez l'état d'urgence du robot ${ID_ROBOT} : LEDs rouge clignotant + buzzer

SB_C indique qu’il est sélectionné à Utilisateur
    [Arguments]     ${ID_ROBOT}
    Execute Manual Step    Vérifiez la couleur des LEDs du robot ${ID_ROBOT} : bleu fixe

#DEPLACEMENTS
SB_C exécute le déplacement cmd_deplacement
    [Arguments]     ${ID_ROBOT}     ${CMD_DEPLACEMENT}
    Execute Manual Step    Vérifiez que le robot ${ID_ROBOT} s'est déplacé : ${CMD_DEPLACEMENT}

SB_C exécute le déplacement cmd_arret
    [Arguments]     ${ID_ROBOT}
    Execute Manual Step    Vérifiez que le robot ${ID_ROBOT} s'est stoppé

#Author : Matéo RONDEAU
SB_C se déconnecte de la tablette
    [Arguments]     ${ID_ROBOT}
    Execute Manual Step    Déconnectez le robot ${ID_ROBOT} proprement en ligne de commande SSH. Un délai de 10s va maintenant démarrer.
