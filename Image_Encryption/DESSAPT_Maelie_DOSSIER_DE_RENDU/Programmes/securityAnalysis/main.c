#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

Uint32 obtenirPixel(SDL_Surface*, int, int); // recuperer valeur d'un pixel
void recupTabRGB(SDL_Surface*, SDL_Surface*, int*, int*, int, int); // recuperation des tableaux RGB 1 et 2
double coefCorrelation(int*, int*, int, int); // calcul coefficient de correlation
double calculNPCR(int*, int*, int, int); // calcul de l'indice NPCR
double calculUACI(int*, int*, int, int); // calcul de l'indice UACI
void calculStats(SDL_Surface*, SDL_Surface*, int*, int*, int, int, char**); // calcul de tous les indices pour 2 images
void calculSD100(SDL_Surface*, SDL_Surface*, int*, int*, int, int, char**); // calcul des SD values pour échantillon de 100 images
void calculStats100(SDL_Surface*, SDL_Surface*, int*, int*, int, int, char**); // calcul des moyennes de coef, NPCR, UACI pour un echantillon de 100 images

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    int longueurImg = atoi(argv[1]);
    int largeurImg = atoi(argv[2]);
    SDL_Surface *screen = SDL_SetVideoMode(2*atoi(argv[1]), atoi(argv[2]), 16, SDL_HWSURFACE|SDL_DOUBLEBUF); // 512 256
    if ( !screen )
    {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }
    //printf("%d %s %s %s %s %s", argc, argv[0], argv[1], argv[2], argv[3], argv[4]);
    // centre the bitmap on screen
    SDL_Rect posImg1;
    posImg1.x = 0;
    posImg1.y = 0;
    SDL_Rect posImg2;
    posImg2.x = 256;
    posImg2.y = 0;

    // récupération des tableaux de RGB
    int N = 3*longueurImg*largeurImg;
    int* tabRGB1 = (int*)malloc(N*sizeof(int));
    int* tabRGB2 = (int*)malloc(N*sizeof(int));

    // load an image
    SDL_Surface *image1 = NULL, *image2 = NULL;

    // program main loop
    int continuer = 0;
    while (continuer == 0)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                continuer = 1;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: // exit if ESCAPE is pressed
                        continuer = 1;
                        break;
                    case SDLK_1:  // calcul des indices pour 2 images
                        if(argc != 5)
                        {
                            fprintf(stderr, "Trop ou pas assez d'arguments pour cette fonction : longueur, largeur, chemin image1, chemin image2\n");
                            break;
                        }
                        calculStats(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg, argv);
                        break;
                    case SDLK_2:  // calcul du coef corr, NPCR, UACI pour 100 images
                        if(argc != 4)
                        {
                            fprintf(stderr, "Trop ou pas assez d'arguments pour cette fonction : longueur, largeur, chemin 100 images\n");
                            break;
                        }
                        calculStats100(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg, argv);
                        break;
                    case SDLK_3:  // calcul des SD pour 100 images
                        if(argc != 7)
                        {
                            fprintf(stderr, "Trop ou pas assez d'arguments pour cette fonction : longueur, largeur, chemin 100 images, moyenne des coeff de corrélation, moyenne NPCR, moyenne UACI\n");
                            break;
                        }
                        calculSD100(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg, argv);
                        break;
                }
            }
            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw images
        SDL_BlitSurface(image1, 0, screen, &posImg1);
        SDL_BlitSurface(image2, 0, screen, &posImg2);

        // DRAWING ENDS HERE

        // finally, update the screen :)
        SDL_Flip(screen);
    } // end main loop

    free(tabRGB1);
    free(tabRGB2);
    // free loaded images
    SDL_FreeSurface(image1);
    SDL_FreeSurface(image2);

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}

/*******************************************************************/
Uint32 obtenirPixel(SDL_Surface *surface, int x, int y)
{
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    switch(nbOctetsParPixel)
    {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;
    }
}

/*******************************************************************/
void recupTabRGB(SDL_Surface* image1, SDL_Surface* image2, int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg)
{
    Uint32 pixel;
    Uint8 r,g,b;
    int i = 0, j = 0, k = 0;
    // parcours des pixels de l'image
    for(i=0;i < largeurImg;i++)
    {
        for(j=0;j < longueurImg;j++)
        {
            pixel = obtenirPixel(image1,i,j); // on recupere un pixel
            SDL_GetRGB(pixel, image1->format, &r,&g,&b); // on recupere ses composantes RGB
            tabRGB1[k]= r;
            tabRGB1[k+1] = g;
            tabRGB1[k+2] = b;
            pixel = obtenirPixel(image2,i,j); // on recupere un pixel
            SDL_GetRGB(pixel, image2->format, &r,&g,&b); // on recupere ses composantes RGB
            tabRGB2[k]= r;
            tabRGB2[k+1] = g;
            tabRGB2[k+2] = b;
            //fprintf(stderr, "RGB1 : %d %d %d : %d %d %d\n", r, g, b, tabRGB2[k], tabRGB2[k+1], tabRGB2[k+2]);
            //k++; //
            k+=3;
        }
    }
}

/*******************************************************************/
double coefCorrelation(int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg)
{
    //printf("Calcul du Coefficient de Correlation\n");
    double coef = 0, N = (double)longueurImg*(double)largeurImg, x = 0, y = 0, x2 = 0, y2 = 0, xy = 0;
    int i = 0;

    for(i = 0; i < N ;i++)
    {
        xy += tabRGB1[i]*tabRGB2[i];
        x += tabRGB1[i];
        y += tabRGB2[i];
        x2 += pow(tabRGB1[i],2);
        y2 += pow(tabRGB2[i],2);
    }
    coef = ((N*xy)-(x*y))/(sqrt(((N*x2)-pow(x,2))*((N*y2)-pow(y,2))));

    //printf("N = %f, Corr = %f\n", N, coef);
    return coef;
}

/*******************************************************************/
double calculNPCR(int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg)
{
    //printf("Calcul NPCR\n");
    int i = 0;
    double NPCR = 0, D = 0, N = longueurImg*largeurImg;

    for(i = 0; i < N ;i++)
    {
        if(tabRGB1[i] != tabRGB2[i])
        {
            D += 1.0f;
        }
    }

    NPCR = (D/N)*100.0f;

    //printf("D = %f, N = %f, NPCR = %f\n", D, N, NPCR);
    return NPCR;
}

/*******************************************************************/
double calculUACI(int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg)
{
    //printf("Calcul UACI\n");
    int i = 0;
    double UACI = 0, s = 0, N = longueurImg*largeurImg, val = 255;

    for(i = 0; i < N ;i++)
    {
        s += fabs(tabRGB1[i]-tabRGB2[i])/val;
    }

    UACI = (1.0f/N)*s*100.0f;

    //printf("N = %f, UACI = %f\n", N, UACI);
    return UACI;
}

/*******************************************************************/
void calculStats(SDL_Surface* image1, SDL_Surface* image2, int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg, char** argv)
{
    double coef = 0, NPCR = 0, UACI = 0;

    image1 = IMG_Load(argv[3]);
    image2 = IMG_Load(argv[4]); //"images/echantillon_3_lena/lenaCC2.bmp"
    if (!image1 || !image2)
    {
        fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if(image1->w != image2->w || image1->h != image2->h)
    {
        fprintf(stderr, "Images de tailles différentes: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    recupTabRGB(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
    // calcul du coefficient de correlation
    coef = coefCorrelation(tabRGB1, tabRGB2, longueurImg,largeurImg);
    // calcul de l'indice NPCR
    NPCR = calculNPCR(tabRGB1, tabRGB2, longueurImg, largeurImg);
    // calcul de l'indice UACI
    UACI = calculUACI(tabRGB1, tabRGB2, longueurImg, largeurImg);

    printf("coef corr = %f, NPCR = %f, UACI = %f\n", coef, NPCR, UACI);
}

/*******************************************************************/
void calculSD100(SDL_Surface* image1, SDL_Surface* image2, int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg, char** argv)
{
    double coef = 0, NPCR = 0, UACI = 0, N = 100*100, mCoef = atof(argv[4]), mNPCR = atof(argv[5]), mUACI = atof(argv[6]), sumCoef = 0, sumNPCR = 0, sumUACI = 0;
    char* nomImage = (char*)malloc(50*sizeof(char));
    char buffer[20];
    int i = 0, j = 0;

    for(i = 1; i <= 100 ;i++)
    {
        // CONCATENE NOM FICHIER
        strcat(nomImage,argv[3]);
        strcat(nomImage,"image");
        itoa(i,buffer,10);
        //printf("i = %d = %s\n", i, buffer);
        strcat(nomImage,buffer);
        strcat(nomImage,".bmp");
        image1 = IMG_Load(nomImage);
        if (!image1)
        {
            fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        for(j = 1; j <= 100 ;j++)
        {
            // CONCATENE NOM FICHIER
            memset(nomImage, 0, sizeof(nomImage));
            strcat(nomImage,argv[3]);
            strcat(nomImage,"image");
            itoa(j,buffer,10);
            strcat(nomImage,buffer);
            strcat(nomImage,".bmp");
            image2 = IMG_Load(nomImage);
            if (!image2)
            {
                fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
            }

            if(i != j)
            {
                recupTabRGB(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
                // calcul du SD du coefficient de correlation
                coef = coefCorrelation(tabRGB1, tabRGB2, longueurImg,largeurImg);
                coef = coef-mCoef;
                sumCoef += pow(coef,2);
                // calcul du SD de l'indice NPCR
                NPCR = calculNPCR(tabRGB1, tabRGB2, longueurImg, largeurImg);
                NPCR = NPCR-mNPCR;
                sumNPCR += pow(NPCR,2);
                // calcul du SD de l'indice UACI
                UACI = calculUACI(tabRGB1, tabRGB2, longueurImg, largeurImg);
                UACI = UACI-mUACI;
                sumUACI += pow(UACI,2);
            }
        }

        memset(nomImage, 0, sizeof(nomImage));

    }
    sumCoef = sqrt(sumCoef/N);
    sumNPCR = sqrt(sumNPCR/N);
    sumUACI = sqrt(sumUACI/N);

    printf("coefSD = %f, NPCRSD = %f, UACISD = %f\n", sumCoef, sumNPCR, sumUACI);
}

/*******************************************************************/
void calculStats100(SDL_Surface* image1, SDL_Surface* image2, int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg, char** argv)
{
    double coef = 0, NPCR = 0, UACI = 0, N = 100*100;
    char* nomImage = (char*)malloc(50*sizeof(char));
    char buffer[20];
    int i = 0, j = 0;

    for(i = 1; i <= 100 ;i++)
    {
        // CONCATENE NOM FICHIER
        strcat(nomImage,argv[3]);
        strcat(nomImage,"image"); //"images/lena100C/image"
        itoa(i,buffer,10);
        //printf("i = %d = %s\n", i, buffer);
        strcat(nomImage,buffer);
        strcat(nomImage,".bmp");
        image1 = IMG_Load(nomImage);
        if (!image1)
        {
            fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        for(j = 1; j <= 100 ;j++)
        {
            // CONCATENE NOM FICHIER
            memset(nomImage, 0, sizeof(nomImage));
            strcat(nomImage,argv[3]);
            strcat(nomImage, "image");
            itoa(j,buffer,10);
            strcat(nomImage,buffer);
            strcat(nomImage,".bmp");
            image2 = IMG_Load(nomImage);
            if (!image2)
            {
                fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
            }

            if(i != j)
            {
                //printf("j = %d = %s\n", j, buffer);
                recupTabRGB(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
                // calcul du coefficient moyen de correlation
                coef += coefCorrelation(tabRGB1, tabRGB2, longueurImg,largeurImg);
                // calcul de l'indice moyen NPCR
                NPCR += calculNPCR(tabRGB1, tabRGB2, longueurImg, largeurImg);
                // calcul de l'indice moyen UACI
                UACI += calculUACI(tabRGB1, tabRGB2, longueurImg, largeurImg);
            }
        }
        memset(nomImage, 0, sizeof(nomImage));
    }

    // Moyennes
    coef = coef/N;
    NPCR = NPCR/N;
    UACI = UACI/N;

    printf("coef corr = %f, NPCR = %f, UACI = %f\n", coef, NPCR, UACI);
}
