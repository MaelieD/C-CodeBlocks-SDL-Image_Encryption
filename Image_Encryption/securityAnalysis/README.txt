Projet securityAnalysis (Code::Blocks)

Utilisation simple avec des paramètres par défaut:
	Dans /bin/Debug/ ou /bin/Release/, 
		cliquer sur calculStats.py puis sur la touche 1.
		cliquer sur calculStats100.py puis sur la touche 2.
		cliquer sur calculSD100.py puis sur la touche 3.
	Résultats dans /bin/Debug/stdout ou /bin/Release/stdout.

Arguments à mettre pour appeller ces fonctions :

calculStats(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg, argv);
	1. longueur de l'image
	2. largeur de l'image
	3. chemin vers l'image 1
	4. chemin vers l'image 2

Exemple : 256 256 images/lena100C/image1.bmp images/lena100C/image2.bmp
Appuyer sur la touche 1 pour lancer cette fonction.


calculStats100(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg, argv);
	1. longueur des images (même longueur)
	2. largeur des images (même largeur)
	3. chemin vers le dossier contenant les 100 images, nommées de image1.bmp à image100.bmp

Exemple : 256 256 images/lena100C/
Appuyer sur la touche 2 pour lancer cette fonction.


calculSD100(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
	1. longueur des images (même longueur)
	2. largeur des images (même largeur)
	3. chemin vers le dossier contenant les 100 images, nommées de image1.bmp à image100.bmp
	4. moyenne des coefficients de corrélation obtenue grâce à calculStats100();
	5. moyenne des indices NPCR obtenue grâce à calculStats100();
	6. moyenne des indices UACI obtenue grâce à calculStats100();
Exemple : 256 256 images/lena100C/ 0.000289 98.613225 33.126162 
Appuyer sur la touche 3 pour lancer cette fonction.