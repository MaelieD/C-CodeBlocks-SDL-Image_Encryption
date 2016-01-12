Projet generationImage (Code::Blocks)
Programme pour la génération d'un certain nombre d'image avec un pixel différent de 1 bit, à partir d'une image initiale.

Utilisation simple avec des paramètres par défaut:
	Dans /bin/Debug/ ou /bin/Release/, 
		cliquer sur lancement.py et ECHAP pour lancer la génération des 100 images.
	Résultats dans /bin/Debug/stdout ou /bin/Release/stdout.

Paramètres à modifier en fonction des images à générer :
	largeurImg de l'image initiale, 
	longueurImg de l'image initiale, 
	chemin de l'image initiale dans IMG_Load().
Exemple : 256 256 images/image1.bmp
Dans fonction saveNewImage(), choisir la valeur à décrémenter, R, G, ou B, du pixel à modifier pour une image en couleur. Pour une image en niveaux de gris, décrémenter les trois valeurs.
Créera 100 images, de image1.bmp à image100.bmp dans le dossier images.
