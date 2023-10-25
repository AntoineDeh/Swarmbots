"""
SB_test_cases_debug.resource
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

#Ce fichier est utilisé pour lancer l'exécution de tests de manière libre, à des fins de prototypage, de debug ou d'améliorations des cas de tests contractualisés.

*** Settings ***
Resource    resources/android/CU_scenarios.resource

#Suite Setup         Start Swarmbots
#Suite Teardown      Stop Swarmbots

*** Test Cases ***

Piloter un essaim de robots[un robot]
    #---INIT BEFORE STARTING APP---
    Utilisateur démarre SB_C    ${ID_ROBOT_1}
    SB_C indique qu’il est en attente de connexion à Utilisateur    ${ID_ROBOT_1}
    #-------------------------------
    Start Swarmbots
    _[P0]_Connexion d'un robot
    #---DISPLAY COMMAND SCREEN---
    Utilisateur demande à afficher Ecran_Commande
    SB_IHM affiche Ecran_Commande
    #----------------------------
    _[P4]_Robustesse de connexion

