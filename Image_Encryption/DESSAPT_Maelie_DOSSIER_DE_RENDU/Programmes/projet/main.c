#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h> /* Inclusion du header de SDL_image (adapter le dossier au besoin) */

void initFenetreSDL(); // Start SDL et init de la fenetre
void pause(int, double, int*, SDL_Surface*, SDL_Surface*, int, int, int*, SDL_PixelFormat*,char*); // garder la fenetre ouverte jusqu'a fermeture via l'event "click de souris sur la croix rouge"
Uint32 obtenirPixel(SDL_Surface*, int, int, SDL_PixelFormat*); // recuperer un pixel de format 32 bits
int* recupTabBitsRGB(SDL_Surface*, int, int, int, int*, SDL_PixelFormat*); // recuperer les bits de l'image RGB
double obtenirNbRound(int, int*);// algo de calculs des round
int* constitutionKey(int, int, int*); // constitution pseudo aleatoire de la cle stockee dans un tableau
int* chiffrement(int*, int, double, int*); // algo de chiffrement
int* dechiffrement(int*, int, double, int*); // algo de dechiffrement
void definirPixel(SDL_Surface*, int, int, Uint32, SDL_PixelFormat*); // changer un pixel de l'image
void retourImage(SDL_Surface*, SDL_Surface*, int*, int, int, int, SDL_PixelFormat*); // Passer du tab de bits à l'image
int* verifFichierKey(int*, double*); // verifier qu'il n'y a pas de clé dans un fichier key.txt, si c'est le cas, on la récupère

int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL, *image = NULL;
    SDL_Rect positionImg;
    positionImg.x = 0;
    positionImg.y = 0;
    int largeurImg = atoi(argv[1]); // 800 - 512 - 256 - 100
    int longueurImg = atoi(argv[2]); // 600 - 512 - 256 - 125
    int L = 3*8*largeurImg*longueurImg;
    int bpp = 24;

    int* tabBits = NULL; // Tableau de bits (ordonnés et RGB)
    double R = 0; // nombre de tours à faire pour un chiffrement efficace
    int* K = NULL; // cle de chiffrement
    // Allocation de memoire dynamique pour utiliser le tas plutot que la pile qui est trop petite
    K = (int*)malloc(L * sizeof(int));

    /*printf("%s  %s\n", argv[0],argv[1]);
    printf("%d\n", argc);
    printf("%s\n",argv[1]);
    printf("%s\n", argv[2]);*/

    initFenetreSDL();

    // Creer la surface de la fenetre
    ecran = SDL_SetVideoMode(largeurImg, longueurImg, bpp, SDL_HWSURFACE);
    if(ecran == NULL)
    {
        fprintf(stderr, "Impossible de charger le mode video : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    image = IMG_Load(argv[3]); //"images/lena100/image1.bmp"
    if(!image)
    {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_PixelFormat* fmt = image->format;
    SDL_BlitSurface(image, NULL, ecran, &positionImg);
    SDL_Flip(ecran);
/******/
    // Tableau de bits de l'image
    tabBits = recupTabBitsRGB(image, largeurImg, longueurImg, L, tabBits,fmt);
/******/
    pause(L, R, tabBits, ecran, image, largeurImg, longueurImg, K,fmt, argv[2]);

    // Liberer le tableau de bits
    free(tabBits);
    free(K);

    // Liberer la surface
    SDL_FreeSurface(image);
    // Stop SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}

/*******************************************************************/
void initFenetreSDL()
{
    // Start SDL
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Afficher l'icone de la fenetre
    SDL_WM_SetIcon(IMG_Load("images/sdl_icone.bmp"), NULL);

    SDL_WM_SetCaption("Image a chiffrer/dechiffrer", NULL);
}

/*******************************************************************/
void pause(int L, double R, int* tabBits, SDL_Surface* ecran, SDL_Surface* image, int largeurImg, int longueurImg, int* K, SDL_PixelFormat* fmt, char* nomImage)
{
    int continuer = 1, i = 0;
    SDL_Event event;
    char* nomNewImage = "images/newImage.bmp";

    while (continuer)
    {
        SDL_WaitEvent(&event); // SDL_PollEvent : permet de verifier si on clique sur croix rouge puis continue le programme
        switch(event.type)
        {
            case SDL_QUIT: // SDL_QUIT : cliquer sur la croix rouge
                continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                    case SDLK_c: // presser la touche C : algo de chiffrement
                        K = verifFichierKey(K, &R); // si on veut chiffree une image a partir d'une cle en particulier
                        if(R == 0)
                        {
                            printf("Constitution nouvelle clé\n");
                            // Nombre de tours de chiffrement à faire
                            R = obtenirNbRound(L, tabBits);
                            // Constitution de la cle
                            K = constitutionKey(L, R, K);
                        }
                        // Chiffrement
                        tabBits = chiffrement(tabBits, L, R, K);
                        // Afficher la nouvelle image
                        retourImage(ecran, image, tabBits, L, largeurImg, longueurImg, fmt);
                        nomNewImage = "images/imageChiffree.bmp"; //nomImage
                        //printf("R : %f\n", R);
                        //for()
                        break;
                    case SDLK_d: // presser la touche D : algo de dechiffrement
                        K = verifFichierKey(K, &R);
                        tabBits = dechiffrement(tabBits, L, R, K);
                        retourImage(ecran, image, tabBits, L, largeurImg, longueurImg, fmt);
                        nomNewImage = "images/imageDechiffree.bmp";
                        break;
                    case SDLK_s: // sauvegarder l'image avec la touche S
                        SDL_SaveBMP(image,nomNewImage);
                        printf("Image sauvegardee.\n");
                        break;
                }
                break;
            // mettre les autres cas où on choisis de chiffrer ou dechiffrer !!!
            // c'est dans ces cas qu'il faudra appeler les fonctions de chiffrement/dechiffrement
        }
    }
}

/*******************************************************************/
Uint32 obtenirPixel(SDL_Surface *surface, int x, int y, SDL_PixelFormat* fmt)
{
    int nbOctetsParPixel = fmt->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    switch(nbOctetsParPixel)
    {
        case 1: // cas de 1 byte = 8 bits
            return *p;
        case 2: // cas de 2 bytes = 16 bits
            return *(Uint16 *)p;
        case 3: // cas de 3 bytes = 24 bits
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4: // cas de 4 bytes = 32 bits
            return *(Uint32 *)p;
        default:
            return 0;
    }
}

/*******************************************************************/
int* recupTabBitsRGB(SDL_Surface* image, int largeurImg, int longueurImg, int L, int* tabBits, SDL_PixelFormat* fmt)
{
    // Allocation de memoire dynamique pour utiliser le tas plutot que la pile qui est trop petite
    tabBits = (int*)malloc(L * sizeof(int));
    if(tabBits == NULL) // Verification que le malloc s'est fait correctement
    {
        fprintf(stderr, "Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // 3 tabs statiques pour ordonner les bits
    int tabR[8], tabG[8], tabB[8];

    Uint32 pixel;
    Uint8 r,g,b;
    int i = 0, j = 0, k = 0, indR = 0, indG = 8, indB = 16;
    // parcours des pixels de l'image
    for(i=0;i < largeurImg;i++)
    {
        for(j=0;j < longueurImg;j++)
        {
            pixel = obtenirPixel(image,i,j,fmt); // on recupere un pixel
            SDL_GetRGB(pixel, fmt, &r,&g,&b); // on recupere ses composantes RGB
            //printf("RGB1 : %d %d %d\n", r, g, b);
            /****/
            for (k = 1; k <= 8; k++) // pour un entier de 8 bits
            {
                tabR[8-k] = r%2;
                tabG[8-k] = g%2;
                tabB[8-k] = b%2;
                r = r/2;
                g = g/2;
                b = b/2;
            }
            for(k = 0; k < 8 ; k++)
            {
                tabBits[indR + k] = tabR[k];
                tabBits[indG + k] = tabG[k];
                tabBits[indB + k] = tabB[k];
            }

            /*for (k=0;k<24 ;k++)
            {
                printf("%d ", tabBits[indR + k]);
            }*/
            indR = indR + 24;
            indG = indG + 24;
            indB = indB + 24;
            //printf("\n");
            /****/

        }
    }

    return tabBits;
}

/*******************************************************************/
double obtenirNbRound(int L, int* tabBits)
{
    int i = 0;
    double R = 0, R1 = 0, R2 = 0, R3 = 0, E1 = 0.001f, E2 = 0.005f, T0 = ((L - 1.0f)/L); // size = 6291456 pour 512*512
    double P0 = 0, P1 = 0; // pour R2

    R1 = floor(128/(log10(L)/log(2))) + 1.0f; // resultat attendu R1 = 15

    R3 = floor(log2(log(E2)/log(T0))) + 1.0f; // resultat attendu R3 = 23

    // Calculs des proba de 0 et 1 dans le tableau de bits
    for(i=0; i < L ;i++)
    {
        if(tabBits[i] == 0)
            P0++;
        else
            P1++;
    }
    P0 = (P0/L); // *100 pour des proba en pourcentage

    double PR2 = P0, dif = fabs(0.50f-PR2);

    while(dif > E1) // resultat attendu R2 = 1 ?
    {
        PR2 = (pow(PR2,2) + pow((1 - PR2),2));
        dif = fabs(0.50f - PR2);
        R2 = R2 + 1.0f;
        //printf("%f, %f \n", PR2, R2);
    }
    // On cherche le plus grand nombre parmis les trois afin d'avoir un chiffrement efficace
    if(R1 >= R2 && R1 >= R3)
        R = R1;
    else if(R2 >= R1 && R2 >= R3)
        R= R2;
    else
        R = R3;

    //printf("R1 = %f, R2 = %f, R3 = %f, R = %f, P0 = %f, P1 = %f, PR2 = %f, T0 = %f, E2 = %f \n", R1, R2, R3, R, P0, P1, PR2, T0, E2);

    return R;
}

/*******************************************************************/
int* constitutionKey(int L, int R, int* K)
{
    int i = 0;
    if(K == NULL) // Verification que le malloc s'est fait correctement
    {
        fprintf(stderr, "Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
        for(i = 0; i < R ;i++)
        {
            K[i] = rand()%L;
            //printf("%d ", K[i]);
        }
        //printf("\n");
    FILE *f = fopen("key.txt", "w+");
    //fprintf(stderr, "%f ", R);
    for(i = 0; i < R ;i++)
        fprintf (f, "%d,", K[i]);
    fclose(f);

    return K;
}

/*******************************************************************/
int* chiffrement(int* tabBits, int L, double R, int* K)
{
    int r = 1, F = L-2, i = 0, j = 0, S = 0, X = 0,  k = 0;
    int tmp1 = 0, tmp2 = 0, tmp3 = 0;
    int64_t XG = 0;

    while(r <= R)
    {
        i = 0;
        S = L-1;
        X = K[r-1];
        XG = X * X;

        while(i < F)
        {
            X = fmod(fmod(fmod(pow(X,2) ,S)*X,S) + XG,S);
            //printf("\nS = %d, X = %d\n", S, X);
           // X = (((X*X%S)*X)+XG)%S;
            j = i + 1 + X;
            //printf("i : %d, j : %d, X = %d\n", i, j, X);
            tmp1 = tabBits[i];
            tmp2 = tabBits[j];
            tmp3 = (tmp1 + tmp2)%2;
            tabBits[i] = tmp3;
            tabBits[j] = tmp1;
            i++;
            S--;
            /*int indR = 0;
            for (k=0;k<24 ;k++)
            {
                printf("%d ", tabBits[indR + k]);
            }
            indR = indR + 24;*/
        }
        r++;
    }
    printf("chiffrement\n");

    return tabBits;
}

/*******************************************************************/
int* dechiffrement(int* tabBits, int L, double R, int* K)
{
    int r = R;
    int F = L-2, i = 0, j = 0, S = 0, X = 0, XG = 0;
    int tmp1 = 0, tmp2 = 0, tmp3 = 0;
    int* vector = NULL;
    vector = (int*)malloc(L*sizeof(int));
    if(vector == NULL)
    {
        fprintf(stderr, "Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    while(r > 0)
    {
        i = 0;
        S = L-1;
        X = K[r-1];
        XG = X*X;

        while(i < F)
        {
            X = fmod(fmod(fmod(pow(X,2) ,S)*X,S) + XG,S);
            j = i + 1 + X;
            vector[i] = j;
            i = i+1;
            S = S-1;
        }
        j = F-1;
        while(j >= 0)
        {
            i = vector[j];
            tmp1 = tabBits[j];
            tmp2 = tabBits[i];
            tmp3 = (tmp1 + tmp2)%2;
            tabBits[j] = tmp2;
            tabBits[i] = tmp3;
            j = j - 1;
        }
        r = r - 1;
    }
    printf("dechiffrement\n");
    return tabBits;
}

/*******************************************************************/
void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel, SDL_PixelFormat* fmt)
{
    int nbOctetsParPixel = fmt->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    switch(nbOctetsParPixel) // même chose que pour obtenirPixel
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

/*******************************************************************/
void retourImage(SDL_Surface* ecran, SDL_Surface* image, int* tabBits, int L, int largeurImg, int longueurImg, SDL_PixelFormat* fmt)
{
    SDL_Rect positionImg;
    positionImg.x = 0;
    positionImg.y = 0;
    Uint32* pixels = NULL;
    // Allocation de memoire dynamique pour utiliser le tas plutot que la pile qui est trop petite
    pixels = (Uint32*)malloc((largeurImg*longueurImg) * sizeof(int));
    if(pixels == NULL) // Verification que le malloc s'est fait correctement
    {
        fprintf(stderr, "Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    Uint32 pixel;
    Uint8 r,g,b;
    int i = 0, j = 0, k = 0, l = 0;
    int motbinaire;
    int decimal = 0, remise = 0;

    for(i = 0; i < L ; i = i+8)
    {
        motbinaire = tabBits[i+7]*1+tabBits[i+6]*10+tabBits[i+5]*100+tabBits[i+4]*1000+tabBits[i+3]*10000+tabBits[i+2]*100000+tabBits[i+1]*1000000+tabBits[i]*10000000;
        decimal = 0;
        remise = 0;
        j = 0;
        while(motbinaire != 0)
        {
            remise = motbinaire%10;
            motbinaire /= 10;
            decimal += remise*pow(2,j);
            ++j;
        }
        if(k == 0)
        {
            r = decimal;
            k++;
        }
        else if(k == 1)
        {
            g = decimal;
            k++;
        }
        else if(k == 2)
        {
            b = decimal;
            k = 0;
            pixel = SDL_MapRGBA(fmt, r, g, b, 255); // reformer un pixel
            pixels[l] = pixel;
            l++;
        }
        else
        {
            fprintf(stderr, "Erreur : conversion binaire à decimal");
            exit(EXIT_FAILURE);
        }
    }

    l = 0;
    for(i = 0; i < largeurImg ;i++)
    {
        for(j = 0; j < longueurImg ;j++)
        {
            definirPixel(image,i,j,pixels[l], fmt); // remplacer le pixel initial par le nouveau
            l++;
        }
    }

    if(image == NULL)
    {
        fprintf(stderr,"Echec modification Image : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_BlitSurface(image, NULL, ecran, &positionImg);
    SDL_Flip(ecran);

}

/*******************************************************************/
int* verifFichierKey(int* K, double* R)
{
    int i = 0;
    char str[256];
    char* token;
    FILE *file = fopen("key.txt", "r");

    if (file == NULL)
    {
        fprintf(stderr, "Le fichier key.txt est vide.\n");
        fclose(file);
    }
    else
    {
        fprintf(stderr, "Le fichier key.txt est plein. On peut le lire pour remplir K.\n");
        fscanf(file, "%s", str);
        //printf("%s\n", str);
        token = strtok(str,",");
        //printf("token : %s\n", token);
        while(token != NULL)
        {
            K[i] = atoi(token);
            token = strtok(NULL, ",");
            //printf("k = %d\n", K[i]);
            i++;
        }
        fclose(file); // On ferme le fichier qui a été ouvert
    }
    *R = i;

    return K;
}



