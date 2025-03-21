#include "jeu.h"

void lire_plateau(char *fichier,map_struct *carte)
{
    FILE *f;
    char str[100];
    int i,j;
    char ch;
    carte->lignes = 0;
    f = fopen(fichier,"r");
    if (f == NULL)
    {
        printf("Je ne peux pas ouvrir le fichier %s\n",fichier);
        exit(-1);
    }
    while (fgets(str,sizeof(str),f) != NULL)
    {
        str[strlen(str)-1] = '\0';
        carte->colonnes = strlen(str);
        carte->lignes++;
    }
    fclose(f);
    carte->plateau = (int **)malloc(carte->lignes*sizeof(int *));
    for (i=0; i < carte->lignes; i++)
    {
        carte->plateau[i] = (int *)malloc(carte->colonnes*sizeof(int));
    }
    f = fopen(fichier,"r");
    i = 0;
    j = 0;
    while ((ch = getc(f)) != EOF)
    {
        if (ch != '\n')
        {
            if (ch == '#')
            {
                carte->plateau[i][j] = MUR;
            }
            if (ch == ' ')
            {
                carte->plateau[i][j] = VIDE;
            }
            if (ch == '-')
            {
                carte->plateau[i][j] = DECOR;
            }


            if (ch == 'g')
            {
                carte->plateau[i][j] =PORTEGAUCHE;
                carte->positionPORTEgaucheLigne=i;
                carte->positionPORTEgaucheColonne=j;
            }
            if (ch == 'd')
            {
                carte->plateau[i][j] =PORTEDROITE;
                carte->positionPORTEdroiteLigne=i;
                carte->positionPORTEdroiteColonne=j;
            }

            j++;
            if (j == carte->colonnes)
            {
                j = 0;
                i++;
            }
        }
    }
    carte->continuer=1;
}

void afficher_map(map_struct *carte,SDL_Surface *mur,SDL_Surface *pomme,SDL_Surface *decor,SDL_Surface *ecran,SDL_Rect position)
{
    int i,j;
    for (i=0; i < carte->lignes; i++)
    {
        for (j=0; j < carte->colonnes; j++)
        {
            position.x=j*TAILLE_BLOC;
            position.y=i*TAILLE_BLOC;
            if (carte->plateau[i][j] ==MUR)
            {
                SDL_BlitSurface(mur,NULL,ecran,&position);
            }
            if(carte->plateau[i][j]==DECOR)
            {
                SDL_BlitSurface(decor,NULL,ecran,&position);
            }
            if(carte->plateau[i][j]==PIECE)
            {
                SDL_BlitSurface(pomme,NULL,ecran,&position);
            }

        }
    }
}

void deplacerSnake(map_struct *carte,SDL_Rect *pos,int direction,int sNAKE)
{

    switch(direction)
    {
    case HAUT:
        if(carte->plateau[pos->y-1][pos->x]==MUR || carte->plateau[pos->y-1][pos->x]==sNAKE)
        {
            carte->continuer=0;
            fin_de_partie();
            break;
        }
        pos->y--;
        break;
    case BAS:
        if(carte->plateau[pos->y+1][pos->x]==MUR || carte->plateau[pos->y+1][pos->x]==sNAKE)
        {
            carte->continuer=0;
            fin_de_partie();
            break;
        }
        pos->y++;
        break;
    case GAUCHE:
        if(carte->plateau[pos->y][pos->x-1]==MUR || carte->plateau[pos->y][pos->x-1]==sNAKE)
        {
            carte->continuer=0;
            fin_de_partie();
            break;
        }

        if(pos[0].x==carte->positionPORTEgaucheColonne && pos[0].y==carte->positionPORTEgaucheLigne )
        {
            pos[0].x=carte->positionPORTEdroiteColonne;
            break;
        }
        pos->x--;
        break;
    case DROITE:
        if(carte->plateau[pos->y][pos->x+1]==MUR || carte->plateau[pos->y][pos->x+1]==sNAKE)
        {
            carte->continuer=0;
            fin_de_partie();
            break;
        }
        if(pos[0].x==carte->positionPORTEdroiteColonne && pos[0].y==carte->positionPORTEgaucheLigne)
        {
            pos[0].x=carte->positionPORTEgaucheColonne;
            break;
        }
        pos->x++;
        break;

    }

}

void fin_de_partie(){
    SDL_Surface *ecran=NULL;
    SDL_Surface *menu=NULL;
    SDL_Rect positionMenu;

    Mix_Music *musiquefin;
    musiquefin=Mix_LoadMUS("Sons/fin.mp3");
    Mix_PlayMusic(musiquefin,1);

    int continuer=4;
    int mode;
    SDL_Event event;
    int x;
    int y;

    menu=IMG_Load("image/fin.jpg");
    ecran=SDL_SetVideoMode(menu->w,menu->h,32, SDL_HWSURFACE | SDL_DOUBLEBUF |SDL_FULLSCREEN);

    positionMenu.x=0;
    positionMenu.y=0;

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
                jouer(ecran,1);
            }

            case SDLK_w:{
                jouer(ecran,2);
            }
            break;
            }
        }


        SDL_BlitSurface(menu,NULL,ecran,&positionMenu);
        SDL_Flip(ecran);
}

    SDL_Quit();
    return EXIT_SUCCESS;
}

void placer_snake(mario_struct *snake,map_struct *carte)
{

    int lignesnake;
    int colonesnake;
    snake->positionTete=rand()%4; //determine haut bas gauche droite
    switch(snake->positionTete)
    {
    case HAUT:
        do
        {
            colonesnake=rand()%(carte->colonnes);
            lignesnake=rand()%(carte->lignes);
        }
        while(lignesnake==carte->lignes || carte->plateau[lignesnake][colonesnake]!=VIDE || carte->plateau[lignesnake+1][colonesnake]!=VIDE );
        snake->pc=colonesnake;
        snake->pcc=colonesnake;
        snake->pll=lignesnake+1;
        snake->pl=lignesnake;
        carte->plateau[lignesnake+1][colonesnake]=2;
        carte->plateau[lignesnake][colonesnake]=2;
        break;

    case BAS:
        do
        {
            colonesnake=rand()%((carte->colonnes));
            lignesnake=rand()%((carte->lignes));
        }
        while(lignesnake==0 ||carte->plateau[lignesnake][colonesnake]!=VIDE || carte->plateau[lignesnake-1][colonesnake]!=VIDE);
        snake->pl=lignesnake;
        snake->pll=lignesnake-1;
        snake->pcc=colonesnake;
        snake->pc=colonesnake;
        carte->plateau[lignesnake-1][colonesnake]=2;
        carte->plateau[lignesnake][colonesnake]=2;
        break;

    case GAUCHE:
        do
        {
            colonesnake=rand()%((carte->colonnes));
            lignesnake=rand()%((carte->lignes));
        }
        while(colonesnake==0 ||carte->plateau[lignesnake][colonesnake]!=VIDE || carte->plateau[lignesnake][colonesnake+1]!=VIDE);
        snake->pl=lignesnake;
        snake->pll=lignesnake;
        snake->pc=colonesnake;
        snake->pcc=colonesnake+1;
        carte->plateau[lignesnake][colonesnake+1]=2;
        carte->plateau[lignesnake][colonesnake]=2;
        break;
    case DROITE:
        do
        {
            colonesnake=rand()%((carte->colonnes));
            lignesnake=rand()%((carte->lignes));
        }
        while(colonesnake==carte->colonnes ||carte->plateau[lignesnake][colonesnake]!=VIDE || carte->plateau[lignesnake][colonesnake-1]!=VIDE);
        snake->pl=lignesnake;
        snake->pll=lignesnake;
        snake->pc=colonesnake;
        snake->pcc=colonesnake-1;
        carte->plateau[lignesnake][colonesnake-1]=2;
        carte->plateau[lignesnake][colonesnake]=2;
        break;
    }

}

void placer_piece(map_struct *carte)
{
    int lignePomme;
    int colonePomme;
    do
    {
    colonePomme=rand()%(carte->colonnes);
    lignePomme=rand()%(carte->lignes);
    }
    while(carte->plateau[lignePomme][colonePomme]!=VIDE);

    carte->plateau[lignePomme][colonePomme]=PIECE;
    carte->positionLIGNEpomme=lignePomme;
    carte->positionCOLONNEpomme=colonePomme;

}

void clean_map(map_struct *carte)
{
    int i,j;
    for( i=0; i<carte->lignes; i++)
    {
        for(j=0; j<carte->colonnes; j++)
        {
            if(carte->plateau[i][j]==SNAKE1 || carte->plateau[i][j]==SNAKE2)
            {
                carte->plateau[i][j]=0;
            }
        }
    }
}

