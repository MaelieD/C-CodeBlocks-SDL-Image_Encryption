Projet projet (Code::Blocks)
Programme pour le chiffrement et le d�chiffrement d'images en RGB-color (24 bits ou canal alpha valant 255).

Utilisation simple avec des param�tres par d�faut:
	Dans /bin/Debug/ ou /bin/Release/, 
		cliquer sur lancement.py et C ou D pour chiffrer et d�chiffrer l'image.
	R�sultats dans /bin/Debug/stdout ou /bin/Release/stdout.

Param�tres � modifier en fonction des images :
	largeurImg, 
	longueurImg, 
	chemin de l'image dans IMG_Load().
Exemple : 256 256 images/lena100/image1.bmp

Touche C pour le chiffrement 
	(si un fichier key.txt existe dans le dossier, le programme va le lire pour utiliser la cl� donn�e, sinon il en cr�era une qui sera sauvegarder dans key.txt).
Touche D pour le d�chiffrement 
	(si un fichier key.txt, l'image donn�e sera d�chiffr�e avec, sinon le d�chiffrement sera inutile).
Touche S pour sauvegarder l'image.
	(si elle a �t� chiffr�e, elle sera sauvegard�e imageChiffree.bmp, si d�chiffr�e, elle sera imageDechiffree.bmp, dans le dossier images du projet).
Touche ECHAP pour quitter ou fermer la fen�tre.