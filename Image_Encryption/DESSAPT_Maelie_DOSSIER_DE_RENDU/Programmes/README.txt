Programmes du projet de cyptographie.
Auteur : Ma�lie DESSAPT

Attention, ces programmes ont �t� con�u sur l'IDE Code::Blocks, sous Windows 8.1, et n'ont pas pu �tre test�s sur d'autres plateformes en raison de probl�mes d'installation et de compatibilit� des biblioth�ques SDL et SDL_image.
Code::Blocks, SDL 1.2 et SDL_Image doivent �tre install�s pour le bon fonctionnement de ces programmes.
Tutoriel utilis� : https://openclassrooms.com/courses/apprenez-a-programmer-en-c/installation-de-la-sdl

Fichiers d'erreurs (stderr) ou de sorties (stdout) disponibles apr�s Build/Run dans les projets /bin/debug/ ou /bin/release/, en fonction du mode de la cible donn�e � la compilation sous Code::Blocks.

1#
projet

Programme pour le chiffrement et le d�chiffrement d'images en RGB-color (24 bits ou canal alpha valant 255).
Param�tres � modifier en fonction des images :
	largeurImg, 
	longueurImg, 
	chemin de l'image dans IMG_Load().
Touche C pour le chiffrement 
	(si un fichier key.txt existe dans le dossier, le programme va le lire pour utiliser la cl� donn�e, sinon il en cr�era une qui sera sauvegarder dans key.txt).
Touche D pour le d�chiffrement 
	(si un fichier key.txt, l'image donn�e sera d�chiffr�e avec, sinon le d�chiffrement sera inutile).
Touche S pour sauvegarder l'image.
	(si elle a �t� chiffr�e, elle sera sauvegard�e imageChiffree.bmp, si d�chiffr�e, elle sera imageDechiffree.bmp, dans le dossier images du projet).
Touche ECHAP pour quitter ou fermer la fen�tre.


2#
projetGrayscale

Programme pour le chiffrement et le d�chiffrement d'images en niveaux de gris (ne fonctionne que pour une image 8 bits).
Param�tres � modifier en fonction des images :
	largeurImg, 
	longueurImg, 
	chemin de l'image dans IMG_Load().
Touche C pour le chiffrement 
	(si un fichier key.txt existe dans le dossier, le programme va le lire pour utiliser la cl� donn�e, sinon il en cr�era une qui sera sauvegarder dans key.txt).
Touche D pour le d�chiffrement 
	(si un fichier key.txt, l'image donn�e sera d�chiffr�e avec, sinon le d�chiffrement sera inutile).
Touche S pour sauvegarder l'image.
	(si elle a �t� chiffr�e, elle sera sauvegard�e imageChiffree.bmp, si d�chiffr�e, elle sera imageDechiffree.bmp, dans le dossier images du projet).
Touche ECHAP pour quitter ou fermer la fen�tre.


3#
generationImage

Programme pour la g�n�ration d'un certain nombre d'image avec un pixel diff�rent de 1 bit, � partir d'une image initiale.
Param�tres � modifier en fonction des images � g�n�rer :
	largeurImg de l'image initiale, 
	longueurImg de l'image initiale, 
	chemin de l'image initiale dans IMG_Load().
Dans fonction saveNewImage(), choisir la valeur � d�cr�menter, R, G, ou B, du pixel � modifier pour une image en couleur. Pour une image en niveaux de gris, d�cr�menter les trois valeurs.
Cr�era 100 images, de image1.bmp � image100.bmp dans le dossier images.


4#
securityAnalysis

Programme pour le calcul de param�tres n�c�ssaires � l'analyse de s�curit�.
Param�tres � modifier en fonction des images � prendre en compte pour le calcul des coefficients et indices :
	largeurImg des images, 
	longueurImg des images, 
	chemin des images dans IMG_Load().
Calcul des indices pour 2 images : calculStats(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
	Modifier le chemin des images dans IMG_Load selon le besoin.
Calcul des moyennes des coef corr, NPCR, UACI pour 100 images : calculStats100(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
	Modifier le chemin du dossier des images � prendre en compte, elles doivent �tre nomm�es image1.bmp � image2.bmp (ou modifier le code).
Calcul des Standard Deviations pour 100 images : calculSD100(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
	Modifier mCoef, mNPCR et mUACI avec les moyennes correspondantes � l'�chantillon de 100 images.
