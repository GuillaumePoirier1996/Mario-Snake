#include "jeu.h"

int main(int argc,char *argv[])
{
    SDL_Surface *ecran=NULL;
    SDL_Surface *menu=NULL;

    SDL_Rect positionMenu;

    SDL_Init(SDL_INIT_VIDEO);


    int continuer=4;
    int mode;
    SDL_Event event;
    int x;
    int y;

SDL_WM_SetIcon(IMG_Load("image/icon.jpg"),NULL);
ecran=SDL_SetVideoMode(1366,768,32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
SDL_WM_SetCaption("MARIO SNAKE",NULL);

menu=IMG_Load("image/ecran_acc1.jpg");

    positionMenu.x=0;
    positionMenu.y=0; // positionne le menu


    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024);
    Mix_Music *musique;
    musique=Mix_LoadMUS("Sons/menu.mp3");
    Mix_PlayMusic(musique,-1);

    while(continuer)
    {
        SDL_WaitEvent(&event);

        switch(event.type)
        {

        case SDL_QUIT:
            {
            continuer=0;
            break;
            }

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:{
                continuer=0;
                break;
            }
            case SDLK_q:{
                mode=1;
                jouer(ecran, mode);
            }

            case SDLK_w:{
                mode=2;
                jouer(ecran, mode);
            }
            break;
            }   //
        }


        SDL_BlitSurface(menu,NULL,ecran,&positionMenu);
        SDL_Flip(ecran);
}
    SDL_CloseAudio();
    SDL_FreeSurface(menu);
    return EXIT_SUCCESS;

}
