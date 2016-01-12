Programmes du projet de cyptographie.
Auteur : Maélie DESSAPT

Attention, ces programmes ont été conçu sur l'IDE Code::Blocks, sous Windows 8.1, et n'ont pas pu être testés sur d'autres plateformes en raison de problèmes d'installation et de compatibilité des bibliothèques SDL et SDL_image.
Code::Blocks, SDL 1.2 et SDL_Image doivent être installés pour le bon fonctionnement de ces programmes.
Tutoriel utilisé : https://openclassrooms.com/courses/apprenez-a-programmer-en-c/installation-de-la-sdl

Fichiers d'erreurs (stderr) ou de sorties (stdout) disponibles après Build/Run dans les projets /bin/debug/ ou /bin/release/, en fonction du mode de la cible donnée à la compilation sous Code::Blocks.

1#
projet

Programme pour le chiffrement et le déchiffrement d'images en RGB-color (24 bits ou canal alpha valant 255).
Paramètres à modifier en fonction des images :
	largeurImg, 
	longueurImg, 
	chemin de l'image dans IMG_Load().
Touche C pour le chiffrement 
	(si un fichier key.txt existe dans le dossier, le programme va le lire pour utiliser la clé donnée, sinon il en créera une qui sera sauvegarder dans key.txt).
Touche D pour le déchiffrement 
	(si un fichier key.txt, l'image donnée sera déchiffrée avec, sinon le déchiffrement sera inutile).
Touche S pour sauvegarder l'image.
	(si elle a été chiffrée, elle sera sauvegardée imageChiffree.bmp, si déchiffrée, elle sera imageDechiffree.bmp, dans le dossier images du projet).
Touche ECHAP pour quitter ou fermer la fenêtre.


2#
projetGrayscale

Programme pour le chiffrement et le déchiffrement d'images en niveaux de gris (ne fonctionne que pour une image 8 bits).
Paramètres à modifier en fonction des images :
	largeurImg, 
	longueurImg, 
	chemin de l'image dans IMG_Load().
Touche C pour le chiffrement 
	(si un fichier key.txt existe dans le dossier, le programme va le lire pour utiliser la clé donnée, sinon il en créera une qui sera sauvegarder dans key.txt).
Touche D pour le déchiffrement 
	(si un fichier key.txt, l'image donnée sera déchiffrée avec, sinon le déchiffrement sera inutile).
Touche S pour sauvegarder l'image.
	(si elle a été chiffrée, elle sera sauvegardée imageChiffree.bmp, si déchiffrée, elle sera imageDechiffree.bmp, dans le dossier images du projet).
Touche ECHAP pour quitter ou fermer la fenêtre.


3#
generationImage

Programme pour la génération d'un certain nombre d'image avec un pixel différent de 1 bit, à partir d'une image initiale.
Paramètres à modifier en fonction des images à générer :
	largeurImg de l'image initiale, 
	longueurImg de l'image initiale, 
	chemin de l'image initiale dans IMG_Load().
Dans fonction saveNewImage(), choisir la valeur à décrémenter, R, G, ou B, du pixel à modifier pour une image en couleur. Pour une image en niveaux de gris, décrémenter les trois valeurs.
Créera 100 images, de image1.bmp à image100.bmp dans le dossier images.


4#
securityAnalysis

Programme pour le calcul de paramètres nécéssaires à l'analyse de sécurité.
Paramètres à modifier en fonction des images à prendre en compte pour le calcul des coefficients et indices :
	largeurImg des images, 
	longueurImg des images, 
	chemin des images dans IMG_Load().
Calcul des indices pour 2 images : calculStats(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
	Modifier le chemin des images dans IMG_Load selon le besoin.
Calcul des moyennes des coef corr, NPCR, UACI pour 100 images : calculStats100(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
	Modifier le chemin du dossier des images à prendre en compte, elles doivent être nommées image1.bmp à image2.bmp (ou modifier le code).
Calcul des Standard Deviations pour 100 images : calculSD100(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
	Modifier mCoef, mNPCR et mUACI avec les moyennes correspondantes à l'échantillon de 100 images.
