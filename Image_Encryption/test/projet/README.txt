DIFFICULTES - IMAGE ENCRYPTION

1#
Installation de la biblioth�que SDL sur mon xUbuntu compliqu�e. 
Multiples probl�mes : fichiers abim�s, v1.2, v2, etc.
Temps perdu : 3 jours
Solution : coup de chance.

2#
Installation de SDL_image sur mon xUbuntu impossible.
Multiples probl�mes sur xUbuntu.
Temps perdu : 2 jours
Solution : installer CodeBlocks sous Windows et installer SDL et SDL_image dessus.

3#
Ev�nements des touches ECHAP, C, D simple.

4#
R�cup�rer les pixels de l'image.
Toutes les recherches ont men� � la m�me fonction �crites de diff�rentes fa�ons.
Solution : fonction obtenirPixel (tuto internet sdz)
Sources : 
https://openclassrooms.com/forum/sujet/transformer-une-image-en-matrice-34264
https://openclassrooms.com/courses/modifier-une-image-pixel-par-pixel

5#
Convertir et stocker les pixels en bits (selon RGB)
Plusieurs probl�mes � d�crire.
Temps perdu : 2 jours
Solution : 3 tableaux tmp de 8 bits pour stocker correctement les bits pour R, G et B; un malloc tabBits pour stocker dynamiquement tous les bits de l'image sur le tas plut�t que la pile qui est trop petite; gestion 3 par 3, RGB, tous les 24 bits.

6#
Calculs de R1, R2, R3. R2 et R3 ne se calcule/s'affiche pas correctement. 
Probl�me de logarithme, -1.#INF, etc.
Temps perdu : 1 jour
Solution : fixer les valeurs selon des calculs fait main.

7#
Constitution de la cl�. Probl�me de pseudo-al�atoire.
Toujours la m�me suite de valeurs (seeds) pour la cl�.
rand()%L pour obtenir des valeurs pseudo al�atoire entre 0 et L-1
Solution : srand(time(NULL)) pour l'al�atoire en fonction de la date (seconde).

8#
Calcul de j dans l'algorithme de chiffrement. Valeur n�gative donc probl�me d'index dans le tableau de bits.
Solution : valeur absolue de j = fabs(i + 1 + X);

9#
Affichage de l'image � partir de tableau de bits.
Conversion decimal->binaire et binaire->decimal. Reconstitution des pixels.
Solution : fonction definirPixel (tuto internet sdz)
Source : https://openclassrooms.com/courses/modifier-une-image-pixel-par-pixel