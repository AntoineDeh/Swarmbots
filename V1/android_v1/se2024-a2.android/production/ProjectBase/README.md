SwarmBots
ProSE équipe A2.
Mahery FONG (mahery.fong@reseau.eseo.fr)
Version 0.0.1

# Génération de la documentation Doxygen

La génération de la documentation Doxygen côté SB_IHM se fait par le logiciel Doxywizard (sur Windows).

Pour l'installation, veuillez télécharger la version binaire pour Windows sur la page : https://www.doxygen.nl/download.html à l'onglet "Sources and Binaries" -> 
"A binary distribution for Windows. All 64bit versions of Windows are supported." le fichier : doxygen-1.9.7-setup.exe. 

Génération de la documentation : 

1. Projet : 
- Choisissez la racine du projet comme répertoire du code source. 
- Cocher la case : Scan recursively. 
- Choisissez le répertoire de destination pour la génération de documents.
Puis cliquez sur Next.

2. Mode : 
- Choisissez le mode d'extraction : Documents entities only 
- L'optimisation du language : Optimize for Java or C# output
Puis cliquez sur Next.

3. Output : 
- Choisissez la génération HTML + plain HTML + with search function
Puis cliquez sur Next.

4. Diagrams : 
-  Choisissez la génération des diagrammes : Use built-in class diagram generator. 
Puis cliquez sur Next.

Enfin cliquer sur Show HTML output. 

Consultation de la documentation : 

Vous pourrez consulter cette documentation dans le répertoire de destination de la génération de documents que vous avez choisi, puis dans "html" et cliquez sur le "index.html".

Sauvegarde de la configuration (Doxyfile) : 

Vous pouvez sauvegarder les configurations de doxygen (dans un fichier Doxyfile) en cliquant sur File -> Save et choisir le répertoire désiré. 

A la réouverture du logiciel Doxywizard, il vous faudra cliquer sur File -> Open et sélectionner le Doxyfile. Les paramètres seront automatiquement repris et vous pourez 
