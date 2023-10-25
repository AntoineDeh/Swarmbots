SwarmBots
ProSE équipe A2.
Joshua MONTREUIL (joshua.montreuil@reseau.eseo.fr)
Version 0.0.1

# Installation Pré-requis

    Ce guide comporte deux parties, la première explique ce qu'il faut faire pour configurer le makefile pour une utilisation sur Raspberry Pi. La deuxième explique la configuration pour utiliser le makefile sur un pc de dev. 
    
    Si vous souhaitez pouvoir configurer le makefile dans les deux cas, appliquez les instructions des deux parties.
    
    -> Installez cmake
    -> Installez doxygen
    
    Etre en root toute l'installation

    WARNING : TOUS LES DOSSIERS A CREER SONT A CREER EN DEHORS DU REPERTOIRE DU PROJET ET EN DEHORS DU REPERTOIRE GIT DE L'EQUIPE.

## Configuration Makefile (utilisation sur Raspberry PI)

### Compilation croisée simple : (Pré-requis : un compte GitHub)

    Créez un dossier, et dans ce même dossier, exécutez la commande:

        $ git clone https://github.com/raspberrypi/tools.git

    Puis changez le chemin d'accès à ce répertoire dans le Makefile principal (à la ligne 11).

    Ensuite, changez le chemin vers le compilateur croisé dans le Makefile principal (à la ligne 16), suivez EXEMPLE (ligne 14).

### Compilation croisée bibliothèques tierces : (Pré-requis : une raspberryPi3B+)

    2 façons de faire :

	- Dans le même répertoire que précédemment créez un dossier "rootfs_bplus" et éxecutez la commande :
	
	  $ rsync -rl --delete-after --safe-links --copy-unsafe-links pi@<IP_de_la_PI>:/{lib,usr} <chemin_du_dossier_rootfs_bplus>

	-> <IP_de_la_PI> : 127.0.0.1 car sur réseau local lors du dev sinon vérifier sur la pi.
	-> <chemin_du_dossier_rootfs_bplus> : chemin du dossier créé précédemment.

    Puis changez le chemin d'accès à ce dossier sur le Makefile principal (à la ligne 22), suivez EXEMPLE1 (ligne 19).

	- Dans le répertoire de votre choix, créez un dossier "rootfs_bplus" et éxecutez la commande :

	  $ rsync -rl --delete-after --safe-links --copy-unsafe-links pi@<IP_de_la_PI>:/{lib,usr} <chemin_du_dossier_rootfs_bplus>

	-> <IP_de_la_PI> : 127.0.0.1 car sur réseau local lors du dev sinon vérifier sur la pi.
	-> <chemin_du_dossier_rootfs_bplus> : chemin du dossier créé précédemment.

    Puis changez le chemin d'accès à ce dossier sur le Makefile principal (à la ligne 22), suivez EXEMPLE2 (ligne 20).

### Compilation et exécution de tests avec le framework CMocka pour Raspberry Pi

       Remarque : si vous venez de faire l'installation pour le pc de dev, supprimer tous les fichier dans le répertoire "build" et allez à la ligne 105. Sinon continuez à la ligne suivante.

       Dans un répertoire que vous choisissez, créez un dossier permettant de stocker la librairie du framework.

       Dans un répertoire que vous choisissez, exécutez la commande :
       
         $ wget https://cmocka.org/files/1.1/cmocka-1.1.5.tar.xz

       puis :

            $ tar xf cmocka-1.1.5.tar.xz

       entrez dans le répertoire "cmocka-1.1.5"

       Dans ce répertoire, ouvrez le fichier "DefineOptions.cmake" et mettez l'option de la ligne 1 à "ON". (enregistrez le fichier).

       Dans le même répertoire, créez un dossier "build".

       Allez ensuite dans le répertoire "cmake",

       Créez un fichier nommé "Toolchain-cross-raspberry.cmake"

       Dans ce fichier, ecrivez :

            
            SET(CMAKE_SYSTEM_NAME Linux)
            SET(CMAKE_SYSTEM_VERSION 1)

            # Specify the cross compiler
            SET(CMAKE_C_COMPILER <repertoire de la partie "Compilation croisée simple">/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc)
            SET(CMAKE_CXX_COMPILER <repertoire de la partie "Compilation croisée simple">/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++)

            # Where is the target environment
            SET(CMAKE_FIND_ROOT_PATH <repertoire vers le dossier rootfs_bplus>/rootfs_bplus)
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
            SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")

            # Search for programs only in the build host directories
            SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

            # Search for libraries and headers only in the target directories
            SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
            SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

       -> <repertoire de la partie "Compilation croisée simple"> correspond au répertoire où vous avez clone les outils de cross-compilation précédement. EXEMPLE : "/home/joshua/Documents/ProSe".
        
       -> <repertoire vers le dossier rootfs_bplus> correspond au répertoire du fichier où vous avez fait la commande "rsync". EXEMPLE : /home/joshua/Documents/ProSe/tools/".
        
       (n'oubliez pas d'enregistrer le fichier.)

       Ce fichier permet de faire de la cross-compilation vers une Raspberry Pi avec cmake et générer une bonne librairie cmocka pour la cible.

       Retournez dans le répertoire "cmocka-1.1.5", allez dans le répertoire build et entrez la commande suivante :

            $ cmake -DCMAKE_INSTALL_PREFIX=<choisir un emplacement> -DCMAKE_TOOLCHAIN_FILE=<emplacement de Toolchain-cross-raspberry.cmake> ..

            -> (oubliez pas les ".." à la fin)
            -> <choisir un emplacement> : emplacement du dossier pour stocker la librairie (créé quelques lignes plus haut dans le README). EXEMPLE : /home/toto/logiciels/cmocka/cmocka-raspberry/
            -> <emplacement de Toolchain-cross-raspberry.cmake> : emplacement du fichier créé précédemment. EXEMPLE : /home/joshua/Documents/Outils_Logiciels/src_lib/cmocka-1.1.5/cmake/Toolchain-cross-raspberry.cmake
        
        puis dans ce même dossier build: 

            $ make

        puis ce même dossier build :

            $ make install

        Dans le Makefile principal du projet :

            Changez le chemin d'accès vers la librairie que vous venez d'installer (à la ligne 29), suivez EXEMPLE (ligne 28).

## Configuration Makefile (utilisation sur pc de développement)

### Compilation et exécution de tests avec le framework CMocka pour PC Dev

        Remarque : si vous venez de faire l'installation pour la cible Raspberry PI, supprimez tous les fichier dans le répertoire "build" et allez à la ligne 145. Sinon continuez à la ligne suivante. 

        Dans un répertoire que vous choisissez, créez un dossier permettant de stocker la librairies du framework.

        Dans un repertoire que vous choisissez, exécutez la commande :

            $ wget https://cmocka.org/files/1.1/cmocka-1.1.5.tar.xz

        puis :

            $ tar xf cmocka-1.1.5.tar.xz

        entrez dans le répertoire "cmocka-1.1.5"

        Dans ce répertoire ouvrez le fichier "DefineOptions.cmake" et mettez l'option de la ligne 1 à "ON". (enregistrez le fichier).

        Dans ce même répertoire, créez un dossier "build" (ou pas si existant), entrez dans ce dossier et exécutez la commande :

            $ cmake -DCMAKE_INSTALL_PREFIX=<choisir un emplacement> ..

            -> (oubliez pas les ".." à la fin)
            -> <choisir un emplacement> : emplacement du dossier pour stocker la librairie (créé quelques lignes plus haut dans le README). EXEMPLE : /home/toto/logiciels/cmocka/cmocka-x86_64/
        
        puis dans ce même dossier build: 

            $ make

        puis ce même dossier build :

            $ make install

        Dans le Makefile principal du projet :

            Changez le chemin d'accès vers la librairie que vous venez d'installer (à la ligne 34), suivez EXEMPLE (ligne 33).

# Lancement de la compilation

    De la même façon que pour la configuration du makefile, cette explication est en deux partie, pour la Raspberry Pi et pour le pc de dev.

    Remarque : Avant la compilation pour une nouvelle cible, lancez la commande :

        $ make clean

## Lancement de la compilation pour la cible raspberry Pi

    Dans le répertoire du projet où se situe le Makefile principal, lancez la commande :

        $ make TARGET=raspberry all

## Lancement de la compilation pour le pc de dev

    Dans le répertoire du projet où se situe le Makefile principal, lancez la commande :

        $ make all

# Exécution du Programme principal

    De la même façon que pour le lancement de la compilation, cette explication est en deux parties, pour la Raspberry Pi et pour le pc de dev.

## Exécution du Programme principal pour la cible raspberry Pi

    Ici, on suppose qu'une connexion ssh entre le pc de développement et la Raspberry Pi est possible.

    Dans un premier temps, il faut copier l'exécutable sur la Raspberry Pi.

    Exécutez la commande :

        $ scp <nom_exécutable> pi@<IP de la Pi>:<répertoire de copie>

        -> <nom-exécutable> : swarm_bots_raspberry.elf. Si vous avez changé le Makefile, ce sera le nom que vous avez choisi.

        -> <IP de la Pi> ; IP de la Pi.

        -> <répertoire de copie> : répertoire où vous souhaitez mettre l'exécutable.

    Ensuite dans le <répertoire de copie> (sur la Raspberry Pi) :

        $ ./<nom-exécutable>

## Exécution du Programme principal pour le pc de dev

    Placez-vous dans le répertoire bin/ et exécutez :

        $ ./<nom-exécutable>

        -> <nom-exécutable> : swarm_bots. Si vous avez changé le Makefile, ce nom sera celui que vous avez choisi.

# Exécution du programme de test

    De la même façon que pour le lancement de la compilation, cette explication est en deux parties, pour la Raspberry Pi et pour le pc de dev.

## Exécution du programme de test pour la cible raspberry Pi

    Ici on propose deux version :
    
    Version 1 :envoie automatique des fichiers exécutables vers la cible Raspberry Pi :
    Une connexion ssh doit être possible entre le pc de dev et la raspberry.
    
    Vous devez également changer l'adresse de la cible ainsi que le mdp dans le Makefile.
    
    Dans le répertoire du Makefile principal :
    
    	$ make upload
    	
    	Pour télécharger le programme principal sur la cible.
    	
    	$ make upload_test
    	
    	Pour télécharger le programme principal sur la cible.
    
    
    
    
    
    
    Ici, on suppose qu'une connexion ssh entre le pc de développement et la Raspberry Pi est possible.

    Version 2 :Dans un premier temps, il faut copier l'exécutable sur la Raspberry Pi.

    Exécutez la commande :

        $ scp <nom_exécutable> pi@<IP de la Pi>:<répertoire de copie>

        -> <nom-exécutable> : swarm_bots_test_raspberry.elf. Si vous avez changé le Makefile ce sera le nom que vous avez choisi.

        -> <IP de la Pi> ; IP de la Pi.

        -> <répertoire de copie> : répertoire où vous souhaitez mettre l'exécutable.

    Ensuite dans le <repertoire de copie> (sur la Raspberry Pi) :

        $ ./<nom-exécutable> [-text] [-subunit] [-tap] [-xml]

## Exécution du programme de test pour le pc de dev

    Placez vous dans le répertoire bin/ et exécutez :

        $ ./<nom-exécutable> [-text] [-subunit] [-tap] [-xml]

        -> <nom-exécutable> : swarm_bots_test. Si vous avez changé le Makefile, ce nom sera celui que vous avez choisi.

# Données de couvertures de test

    WARNING : Ces données ne sont disponibles que pour le pc de dev, pas pour la Raspberry Pi.

## GCOV sans Eclipse (Rapport GCOV avec gcovr)

    Remarque : pour une bonne utilisation de cet outil, il est toujours préférable d'exécuter dans le repertoire du Makefile principal :

        $ make test_report_clean

    Dans un premier temps, exécutez le programme de test du pc de développement (cf section "Exécution du programme de test pour le pc de dev").

    Puis, dans le répertoire du Makefile principal :

        $ make test_report

    Ensuite, ouvrez le fichier "index.html" dans le répertoire /report du projet.

## GCOV avec Eclipse

    Remarque : Pour supprimer les indicateurs de couvertures dans l'éditeur :

        $ make clean

    Dans un premier temps, exécutez le programme de test du pc de développement (cf section "Exécution du programme de test pour le pc de dev").

    Puis sur le programme de test <nom-exécutable> dans bin/ >  (clic droit) > Profiling Tools > Profile Code Coverage.

    Ensuite vous pouvez, sur la vue gcov, Sort coberage per folder, double clic sur un fichier de src.

# Compilation de la documentation Doxygen

    Dans le répertoire où se situe le makefile principal, exécutez la commande :

        $ make documentation

    Cette commande génerera la documentation au format doxygen à partir de vos commentaires dans ce même format et à partir des options du fichier "Doxyfile".

    Vous pourrez consulter cette documentation dans le répertoire "doc" du projet, puis dans "html" et cliquez sur le "index.html".
