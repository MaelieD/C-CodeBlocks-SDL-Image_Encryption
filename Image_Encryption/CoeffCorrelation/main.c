#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

Uint32 obtenirPixel(SDL_Surface*, int, int); // recuperer valeur d'un pixel
void recupTabRGB(SDL_Surface*, SDL_Surface*, int*, int*, int, int); // recuperation des tableaux RGB 1 et 2
void coefCorrelation(int*, int*, int, int); // calcul coefficient de correlation
void calculNPCR(int*, int*, int, int); // calcul de l'indice NPCR
void calculUACI(int*, int*, int, int); // calcul de l'indice UACI

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    int longueurImg = 256;
    int largeurImg = 256;
    SDL_Surface *screen = SDL_SetVideoMode(512, 256, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // load an image
    SDL_Surface *image1 = NULL, *image2 = NULL;
    image1 = IMG_Load("images/imageChiffreeLenaA.bmp");
    image2 = IMG_Load("images/imageChiffreeLenaC.bmp");
    if (!image1)
    {
        fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    // centre the bitmap on screen
    SDL_Rect posImg1;
    posImg1.x = 0;
    posImg1.y = 0;
    SDL_Rect posImg2;
    posImg2.x = 256;
    posImg2.y = 0;

    // récupération des tableaux de RGB
    int* tabRGB1 = (int*)malloc((3*longueurImg*largeurImg)*sizeof(int));
    int* tabRGB2 = (int*)malloc((3*longueurImg*largeurImg)*sizeof(int));
    recupTabRGB(image1, image2, tabRGB1, tabRGB2, longueurImg, largeurImg);
    // calcul du coefficient de correlation
    coefCorrelation(tabRGB1, tabRGB2, longueurImg,largeurImg);
    // calcul de l'indice NPCR
    calculNPCR(tabRGB1, tabRGB2, longueurImg, largeurImg);
    // calcul de l'indice UACI
    calculUACI(tabRGB1, tabRGB2, longueurImg, largeurImg);

    // program main loop
    bool done = false;
    while (!done)
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
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
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
            k+=3;
        }
    }
}

/*******************************************************************/
void coefCorrelation(int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg)
{
    printf("Calcul du Coefficient de Correlation\n");
    double coef = 0, N = 3*longueurImg*largeurImg, x = 0, y = 0, x2 = 0, y2 = 0, xy = 0;
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
    printf("%f\n", coef);
}

/*******************************************************************/
void calculNPCR(int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg)
{
    printf("Calcul NPCR\n");
    int i = 0;
    double NPCR = 0, D = 0, N = 3*longueurImg*largeurImg;

    for(i = 0; i < N ;i++)
    {
        if(tabRGB1[i] != tabRGB2[i])
        {
            D += 1.0f;
        }
    }

    NPCR = (D/N)*100.0f;

    printf("D = %f, N = %f, NPCR = %f\n", D, N, NPCR);
}

/*******************************************************************/
void calculUACI(int* tabRGB1, int* tabRGB2, int longueurImg, int largeurImg)
{
    printf("Calcul UACI\n");
    int i = 0;
    double UACI = 0, s = 0, N = 3*longueurImg*largeurImg, val = 255;

    for(i = 0; i < N ;i++)
    {
        s += fabs(tabRGB1[i]-tabRGB2[i])/val;
    }

    UACI = (1.0f/N)*s*100.0f;

    printf("N = %f, UACI = %f\n", N, UACI);
}
