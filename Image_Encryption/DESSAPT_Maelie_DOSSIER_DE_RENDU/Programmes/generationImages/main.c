#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

Uint32 obtenirPixel(SDL_Surface*, int, int);
void definirPixel(SDL_Surface*, int, int, Uint32);
void saveNewImage(SDL_Surface*, SDL_PixelFormat*);

int main ( int argc, char** argv )
{
    int longueurImg = atoi(argv[1]);
    int largeurImg = atoi(argv[2]);
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError() );
        exit(EXIT_FAILURE);
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load an image
    SDL_Surface* image = NULL;
    image = IMG_Load(argv[3]); // image initial à partir de laquelle on va générer les images modifiées de 1 bit // "images/image1.bmp"
    SDL_PixelFormat* fmt = image->format;
    if (!image)
    {
        fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // centre the bitmap on screen
    SDL_Rect dstrect;
    dstrect.x = (screen->w - image->w) / 2;
    dstrect.y = (screen->h - image->h) / 2;

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
                        saveNewImage(image, fmt);
                        //done = true;//image = IMG_Load("images/lenaA.tif");
                    break;
                }
            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw bitmap
        SDL_BlitSurface(image, 0, screen, &dstrect);

        // DRAWING ENDS HERE

        // finally, update the screen :)
        SDL_Flip(screen);
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(image);

    // all is well ;)
    printf("Exited cleanly\n");
    return EXIT_SUCCESS;
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
void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    switch(nbOctetsParPixel)
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
void saveNewImage(SDL_Surface* image, SDL_PixelFormat* fmt)
{
    char* nomNewImage = (char*)malloc(50*sizeof(char));
    if(!nomNewImage)
    {
        fprintf(stderr, "Erreur dans l'allocation de nomNewImage\n");
        exit(EXIT_FAILURE);
    }
    Uint32 pixel;
    Uint8 r,g,b;
    int i = 0, j = 2;
    char buffer[20];
    // parcours des pixels de l'image
    for(i=1;i < 100;i++) // max i en fonction du nombre d'image à générer
    {
        // AJOUT DIFFERENCE DE 1 BIT
        pixel = obtenirPixel(image,0,0); // on récupère le pixel à modifier
        SDL_GetRGB(pixel, fmt, &r,&g,&b); // on récupère ses composantes RGB
        r--;
        //g--;
        //b--;// faire rgb-- pour grayscale

        // RETOUR IMAGE
        pixel = SDL_MapRGBA(fmt, r, g, b, 255); // reformer le pixel
        definirPixel(image,0,0,pixel); // remplacer le pixel initial par le nouveau

        // CONCATENE NOM FICHIER
        strcat(nomNewImage,"images/image");
        itoa(j,buffer,10);
        j++;
        strcat(nomNewImage,buffer);
        strcat(nomNewImage,".bmp");

        // SAUVEGARDE NOUVELLE IMAGE
        SDL_SaveBMP(image,nomNewImage);
        printf("Image %s sauvegardee.\n", nomNewImage);
        memset(nomNewImage, 0, sizeof(nomNewImage));
    }
    free(nomNewImage);
}
