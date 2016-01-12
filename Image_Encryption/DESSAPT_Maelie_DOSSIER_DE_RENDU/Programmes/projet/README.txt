Projet projet (Code::Blocks)
Programme pour le chiffrement et le déchiffrement d'images en RGB-color (24 bits ou canal alpha valant 255).

Utilisation simple avec des paramètres par défaut:
	Dans /bin/Debug/ ou /bin/Release/, 
		cliquer sur lancement.py et C ou D pour chiffrer et déchiffrer l'image.
	Résultats dans /bin/Debug/stdout ou /bin/Release/stdout.

Paramètres à modifier en fonction des images :
	largeurImg, 
	longueurImg, 
	chemin de l'image dans IMG_Load().
Exemple : 256 256 images/lena100/image1.bmp

Touche C pour le chiffrement 
	(si un fichier key.txt existe dans le dossier, le programme va le lire pour utiliser la clé donnée, sinon il en créera une qui sera sauvegarder dans key.txt).
Touche D pour le déchiffrement 
	(si un fichier key.txt, l'image donnée sera déchiffrée avec, sinon le déchiffrement sera inutile).
Touche S pour sauvegarder l'image.
	(si elle a été chiffrée, elle sera sauvegardée imageChiffree.bmp, si déchiffrée, elle sera imageDechiffree.bmp, dans le dossier images du projet).
Touche ECHAP pour quitter ou fermer la fenêtre.