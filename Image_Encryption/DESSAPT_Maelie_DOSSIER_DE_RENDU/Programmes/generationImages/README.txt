Projet generationImage (Code::Blocks)
Programme pour la g�n�ration d'un certain nombre d'image avec un pixel diff�rent de 1 bit, � partir d'une image initiale.

Utilisation simple avec des param�tres par d�faut:
	Dans /bin/Debug/ ou /bin/Release/, 
		cliquer sur lancement.py et ECHAP pour lancer la g�n�ration des 100 images.
	R�sultats dans /bin/Debug/stdout ou /bin/Release/stdout.

Param�tres � modifier en fonction des images � g�n�rer :
	largeurImg de l'image initiale, 
	longueurImg de l'image initiale, 
	chemin de l'image initiale dans IMG_Load().
Exemple : 256 256 images/image1.bmp
Dans fonction saveNewImage(), choisir la valeur � d�cr�menter, R, G, ou B, du pixel � modifier pour une image en couleur. Pour une image en niveaux de gris, d�cr�menter les trois valeurs.
Cr�era 100 images, de image1.bmp � image100.bmp dans le dossier images.
