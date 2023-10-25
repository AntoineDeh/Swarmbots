"""
tools_config.resource
Author: Florentin LEPELTIER (inspiré des travaux de Matthias Brun)
Date: 9 June, 2023

Ce fichier décrit les keywords utilisés pour le lancement et l'arrêt de l'application Swarmbots, sur cible.
# TODO : Non fonctionnel sur AVD (faire le test sur une Nexus 10 API 31)

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

Library    Process
Library    AppiumLibrary


*** Variables ***

# Localisation de appium (ex: /home/<login>/Appium/node_modules/.bin/appium)
#/Users/florentinlepeltier/node_modules/.bin/appium
#/Users/florentinlepeltier/Appium/node_modules/.bin/appium
${APPIUM}               /Users/florentinlepeltier/node_modules/.bin/appium

# Localisation du SDK Android (ex: /home/<login>/Android/Sdk/)
${ANDROID_HOME}         /Users/florentinlepeltier/Library/Android/sdk

${AVD_NAME}                 Nexus_One_API_23
${EMULATOR_NAME}            emulator-5554
${SWARMBOTS_PACKAGE}        project.src
${SWARMBOTS_ACTIVITY}       .view.activities.HomeScreenActivity
${SWARMBOTS_APK}            ${EXECDIR}/SWARMBOTS/bin/Swarmbots_v1.apk

*** Keywords ***

Start SWARMBOTS (on avd)
    Exec Appium
    StartApp SWARMBOTS (on avd)

Stop SWARMBOTS (on avd)
    StopApp SWARMBOTS (on avd)
    Terminate Appium

Start SWARMBOTS
    Exec Appium
    StartApp SWARMBOTS

Stop SWARMBOTS
    StopApp SWARMBOTS
    Terminate Appium

Exec Appium
    start process       ${APPIUM}
    ...                 -p    4723    -a    127.0.0.1    -pa    /wd/hub    --allow-insecure\=adb_shell
    ...                 env:ANDROID_HOME=${ANDROID_HOME}
    ...                 alias=Appium
    sleep    3s

Terminate Appium
    terminate process    Appium

StartApp SWARMBOTS (on avd)
    open application    http://127.0.0.1:4723/wd/hub    allowTestPackages=true    automationName=UIAutomator2    platformName=Android
    ...                 avd=${AVD_NAME}    avdArgs=-wipe-data
    ...                 app=${SWARMBOTS_APK}    appPackage=${SWARMBOTS_PACKAGE}    appActivity=${SWARMBOTS_ACTIVITY}
    ...                 alias=Swarmbots

StopApp SWARMBOTS (on avd)
    switch application    Swarmbots
    close application
    run process    adb    -s    ${EMULATOR_NAME}    emu    kill

StartApp SWARMBOTS
    open application    http://127.0.0.1:4723/wd/hub    allowTestPackages=true    automationName=UIAutomator2    platformName=Android
    ...                 app=${SWARMBOTS_APK}    appPackage=${SWARMBOTS_PACKAGE}    appActivity=${SWARMBOTS_ACTIVITY}
    ...                 alias=Swarmbots

StopApp SWARMBOTS
    switch application    Swarmbots
    close application



