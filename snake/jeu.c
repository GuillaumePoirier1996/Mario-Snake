#include "jeu.h"



SDL_Rect position[100000], PositionSnake[100000], Posp[100000] ;
SDL_Rect position2[100000], PositionSnake2[100000], Posp2[100000]; //positions pour le deuxième serpent
SDL_Rect positionScore,positionTimer,positionScore2;


void jouer (SDL_Surface *ecran,int mode)
{

    Mix_Music *musique1;
    musique1=Mix_LoadMUS("Sons/interface.mp3");
    Mix_PlayMusic(musique1,-1);

    TTF_Init();
    srand(time(NULL));

    mario_struct SNAKE;
    mario_struct SNAKe2;
    map_struct carte;

    SDL_Surface  *Timer = NULL;
    TTF_Font *police = NULL;
    police = TTF_OpenFont("texte/angelina.ttf", 50); //charge la police pour le timer
    SDL_Color couleurNoire = {0, 0, 0}, couleurBlanche = {118,137,255};
    char temps[20000] = ""; //La taille du temps
    double compteurTemps=0;
    positionTimer.x =0;
    positionTimer.y =0;

    //pour les scores
    char score[20]="";
    SDL_Surface *Score=NULL;

    char score2[20]="";
    SDL_Surface *Score2=NULL;

    int difficulte=150;//rapidité de déplacement augmentera petit à petit

    int direction;
    int direction2;
    int coup=0;


    SDL_Surface *snake[5]= {NULL};
    SDL_Surface *snakeActuel[1000]= {NULL};

    SDL_Surface *snake2[5]= {NULL};
    SDL_Surface *snakeActuel2[1000]= {NULL};

    SDL_Surface *mur=NULL;
    SDL_Surface *decor=NULL;
    SDL_Surface *piece=NULL;

    //charge le graphisme du serpent
    snake[BAS]=IMG_Load("image/tete_bas.jpg");
    snake[HAUT]=IMG_Load("image/tete_haut.jpg");
    snake[GAUCHE]=IMG_Load("image/tete_gauche.png");
    snake[DROITE]=IMG_Load("image/tete_droite.jpg");
    snake[CORPS]=IMG_Load("image/corps.png");


    mur= IMG_Load("image/mur.png");
    decor=IMG_Load("image/decor.png");
    piece=IMG_Load("image/piece.png");


    //lecture du fichier texte pour charger la map
    lire_plateau("maps/plateau3.txt",&carte);

    //fenetre adaptable à la taille de la map mais mise en suite en plein ecran
    ecran= SDL_SetVideoMode(carte.colonnes*TAILLE_BLOC,carte.lignes*TAILLE_BLOC,32,SDL_HWSURFACE | SDL_DOUBLEBUF |SDL_FULLSCREEN);

    placer_snake(&SNAKE,&carte);// on place le serpent sur la map

    /* initialisation du serpent*/
    snakeActuel[0]=snake[SNAKE.positionTete];
    snakeActuel[1]=snake[CORPS];
    PositionSnake[0].x=SNAKE.pc;
    PositionSnake[0].y=SNAKE.pl;
    PositionSnake[1].x=SNAKE.pcc;
    PositionSnake[1].y=SNAKE.pll;
    Posp[0].x=SNAKE.pcc;            //donne la possition "précédente
    Posp[0].y=SNAKE.pll;

    if(mode==2) //si mode 2 joueur placer un deuxième serpert le morceau de code est le même
    {
    snake2[BAS]=IMG_Load("image/tete_bas1.jpg");
    snake2[HAUT]=IMG_Load("image/tete_haut1.jpg");
    snake2[GAUCHE]=IMG_Load("image/tete_gauche1.png");
    snake2[DROITE]=IMG_Load("image/tete_droite1.jpg");
    snake2[CORPS]=IMG_Load("image/corps.png");

        placer_snake(&SNAKe2,&carte);
        snakeActuel2[0]=snake2[SNAKe2.positionTete];
        snakeActuel2[1]=snake2[CORPS];
        PositionSnake2[0].x=SNAKe2.pc;
        PositionSnake2[0].y=SNAKe2.pl;
        PositionSnake2[1].x=SNAKe2.pcc;
        PositionSnake2[1].y=SNAKe2.pll;
        Posp2[0].x=SNAKe2.pcc;
        Posp2[0].y=SNAKe2.pll;

    }



    int continuer=1;//pour la boucle infinie
    int lejeuacommence=0; //sinon bug pour l'affichage de la queue et met en pause le jeu
    int i=0,j=0; //cosntantes pour les boucle
    int manger=3;   // permet de savoir si le serpent a mangé
    int cpiece=2;
    int cpiece2=2;
    int serpent1active=0;
    int serpent2active=0;
    int positionLIGNEpommePrecedente=5; //met une position piece précedente ne genant pas le decor
    int positionCOLONNEpommePrecedente=5;



    //pour la boucle permetant le deplacement continue et l'affichage du temps
    int tempsPrecedent = 0; // permet l'avacné sans resté appyyé du serpent
    int tempsActuel = 0;
    int seconde; //permet d'affiché le temps en seconde




    /* Initialisation du temps et du texte */
    tempsActuel = SDL_GetTicks();
    seconde=compteurTemps/1000;
    sprintf(temps, "Time:"" %d", seconde);
    Timer = TTF_RenderText_Blended(police, temps, couleurNoire);

    if(mode==1)
    {
        sprintf(score,"SCORE:"" %d",cpiece-2);
        Score=TTF_RenderText_Shaded(police,score,couleurNoire,couleurBlanche);
        positionTimer.x=ecran->w/2-Timer->w/2;
        positionTimer.y=0;
        positionScore.x=ecran->w/2-Score->w/2;
        positionScore.y=ecran->h-Score->h;

    }

    if(mode==2)
    {
        sprintf(score,"Player 1:"" %d",cpiece-2); // on enleve la tete et la queue à cpiece
        Score=TTF_RenderText_Shaded(police,score,couleurNoire,couleurBlanche);
        positionTimer.x=ecran->w/2-Timer->w/2;//permet de centrer l'affichage
        positionTimer.y=0;
        positionScore.x=ecran->w/4-Score->w/2;
        positionScore.y=ecran->h-Score->h;
        positionScore2.x=ecran->w/4*3-Score->w/2;
        positionScore2.y=ecran->h-Score->h;
        sprintf(score2,"Player 2:"" %d",cpiece2-2);
        Score2=TTF_RenderText_Shaded(police,score2,couleurNoire,couleurBlanche);
    }


    SDL_Event event;

    while(continuer)// qui permet de rester dans jouer et ne pas revenir dans la boucle menu
    {

        if(mode==1)
        {
            SDL_PollEvent(&event);
            switch(event.type)
            {
            case SDL_QUIT:
                continuer=0;
                break;
            case SDL_KEYDOWN: //permet de definir des actions lors d'appui sur des touches du clavier
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    continuer=0;
                    break;

                case SDLK_UP:
                    snakeActuel[0]=snake[HAUT];
                    direction=HAUT;
                    lejeuacommence=1;
                    break;

                case SDLK_DOWN:
                    snakeActuel[0]=snake[BAS];
                    direction=BAS;
                    lejeuacommence=1;
                    break;

                case SDLK_LEFT:
                    snakeActuel[0]=snake[GAUCHE];
                    direction=GAUCHE;
                    lejeuacommence=1;
                    break;

                case SDLK_RIGHT:
                    snakeActuel[0]=snake[DROITE];
                    direction=DROITE;
                    lejeuacommence=1;
                    break;

                case SDLK_p: //met le jeu en pause
                    lejeuacommence=0;
                    break;

                case SDLK_m: //met le jeu en pause
                    if(Mix_PausedMusic() == 1)//Si la musique est en pause
                    {
                        Mix_ResumeMusic(); //Reprendre la musique
                    }
                    else
                    {
                        Mix_PauseMusic(); //Mettre en pause la musique
                    }
                    break;
                }

                break;
            }
            tempsActuel = SDL_GetTicks();

            if (tempsActuel - tempsPrecedent > difficulte)
            {
                if(lejeuacommence==1)
                {
                    for(i=0; i<cpiece; i++) //mettre à jour les position précedente de tout le serpent uniquement quand le jeu tourne
                    {
                        Posp[i].x=PositionSnake[i].x;
                        Posp[i].y=PositionSnake[i].y;
                    }

                    deplacerSnake(&carte,&PositionSnake[0],direction,SNAKE1);
                    SDL_FreeSurface(ecran);
                    tempsPrecedent=tempsActuel;

                    compteurTemps += difficulte;/* On rajoute 100 ms au compteur */
                    seconde=compteurTemps/1000;
                    sprintf(temps,"Time:"" %d",seconde); /* On écrit dans la chaîne "temps" le nouveau temps */
                    SDL_FreeSurface(Timer); /* On supprime la surface précédente */
                    Timer = TTF_RenderText_Blended(police, temps, couleurNoire); /* On écrit la chaîne temps dans la SDL_Surface */
                }
            }

            else
            {
                SDL_Delay(difficulte - (tempsActuel - tempsPrecedent));

            }


            SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,255,255,255));

            if(manger==3)  // quand on mange la pomme une pomme reapparait dans la map
            {
                placer_piece(&carte);
                carte.plateau[positionLIGNEpommePrecedente][positionCOLONNEpommePrecedente]=0;
                manger=0;
            }

            if(PositionSnake[0].x==carte.positionCOLONNEpomme && PositionSnake[0].y==carte.positionLIGNEpomme)  //focntion qui sert à manger
            {
                cpiece++;

                sprintf(score,"SCORE: ""%d",cpiece-2);
                SDL_FreeSurface(Score);
                Score=TTF_RenderText_Shaded(police,score,couleurNoire,couleurBlanche);

                manger=3;
                positionLIGNEpommePrecedente=carte.positionLIGNEpomme;
                positionCOLONNEpommePrecedente=carte.positionCOLONNEpomme;
                snakeActuel[1]=snake[CORPS];//charge le corps dans la bonne direction en position 1 (c'est de la qu'on "grossi"

                for(i=2; i<cpiece; i++)
                {
                    snakeActuel[i]=snake[CORPS];
                }

            }


            if(coup!=0)  // en effet au début lorsqu'aucun coup n'est jouer le serpent est dirigé grace à position snake
            {

                for(i=1; i<cpiece; i++)
                {
                    snakeActuel[i]=snake[CORPS];
                }
            }


            //BLCO depalcement serpent
            position[0].x=PositionSnake[0].x*TAILLE_BLOC;   //Déplace la tête
            position[0].y=PositionSnake[0].y*TAILLE_BLOC;

            for (i=1; i<=cpiece; i++)   //permet le déplacement du corps!!!
            {
                position[i].x=Posp[i-1].x*TAILLE_BLOC;
                position[i].y=Posp[i-1].y*TAILLE_BLOC;
                PositionSnake[i].x=Posp[i-1].x;
                PositionSnake[i].y=Posp[i-1].y;

            }
             // fin bloc deplacement serpent



            clean_map(&carte);        //clean la map

            for (i=0; i<cpiece; i++)   //place le serpent sur la map
            {
                carte.plateau[PositionSnake[i].y][PositionSnake[i].x]=2;
            }

            carte.plateau[carte.positionLIGNEpomme][carte.positionCOLONNEpomme]=PIECE; // place la piece sur la map

            afficher_map(&carte,mur,piece,decor,ecran,position[0]);

            for (i=cpiece-1; i >=0; i--) //on decremente pour pouvoir toujours voir la tête lorsqu'on passe sur le corps meme si c'est iutle vu qu'on est sencé mourir dnas ce cas la
            {
                SDL_BlitSurface(snakeActuel[i],NULL,ecran,&position[i]);
            }

            positionTimer.x=ecran->w/2-Timer->w/2;
            positionScore.x=ecran->w/2-Score->w/2;


            SDL_BlitSurface(snakeActuel[cpiece-1],NULL,ecran,&position[cpiece-1]);
            SDL_BlitSurface(Timer, NULL, ecran, &positionTimer);
            SDL_BlitSurface(Score,NULL,ecran,&positionScore);
            SDL_Flip(ecran);//rafraichi l'ecran


            continuer=carte.continuer;

        }
        if(mode==2)
        {

            SDL_PollEvent(&event);
            switch(event.type)
            {
            case SDL_QUIT:
                continuer=0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    continuer=0;
                    break;

                case SDLK_UP:
                    snakeActuel[0]=snake[HAUT];
                    direction=HAUT;
                    serpent1active=1;
                    lejeuacommence=1;
                    break;

                case SDLK_DOWN:
                    snakeActuel[0]=snake[BAS];
                    direction=BAS;
                    serpent1active=1;
                    lejeuacommence=1;
                    break;

                case SDLK_LEFT:
                    snakeActuel[0]=snake[GAUCHE];
                    direction=GAUCHE;
                    serpent1active=1;
                    lejeuacommence=1;
                    break;

                case SDLK_RIGHT:
                    snakeActuel[0]=snake[DROITE];
                    direction=DROITE;
                    serpent1active=1;
                    lejeuacommence=1;
                    break;

                case SDLK_w:
                    snakeActuel2[0]=snake2[HAUT];
                    direction2=HAUT;
                    serpent2active=1;
                    lejeuacommence=1;
                    break;

                case SDLK_s:
                    snakeActuel2[0]=snake2[BAS];
                    direction2=BAS;
                    serpent2active=1;
                    lejeuacommence=1;
                    break;

                case SDLK_a:
                    snakeActuel2[0]=snake2[GAUCHE];
                    direction2=GAUCHE;
                    serpent2active=1;
                    lejeuacommence=1;
                    break;

                case SDLK_d:
                    snakeActuel2[0]=snake2[DROITE];
                    direction2=DROITE;
                    serpent2active=1;
                    lejeuacommence=1;
                    break;

                case SDLK_p:
                    lejeuacommence=0;
                    break;
                }

                break;
            }
            tempsActuel = SDL_GetTicks();

            if (tempsActuel - tempsPrecedent > difficulte)
            {
                if(lejeuacommence==1)
                {
                    if(serpent1active==1)
                    {
                        for(i=0; i<cpiece; i++)
                        {
                            Posp[i].x=PositionSnake[i].x;
                            Posp[i].y=PositionSnake[i].y;
                        }
                    }

                    deplacerSnake(&carte,&PositionSnake[0],direction,SNAKE1);
                    SDL_FreeSurface(ecran);


                    if(serpent2active==1)
                    {
                        for(i=0; i<cpiece2; i++)
                        {
                            Posp2[i].x=PositionSnake2[i].x;
                            Posp2[i].y=PositionSnake2[i].y;
                        }
                    }

                    deplacerSnake(&carte,&PositionSnake2[0],direction2,SNAKE2);
                    SDL_FreeSurface(ecran);


                    tempsPrecedent=tempsActuel;


                    coup++;


                    compteurTemps += difficulte;
                    seconde=compteurTemps/1000;
                    sprintf(temps,"Time:"" %d",seconde);
                    SDL_FreeSurface(Timer);
                    Timer = TTF_RenderText_Blended(police, temps, couleurNoire);
                }
            }

            else
            {
                SDL_Delay(difficulte - (tempsActuel - tempsPrecedent));

            }

            if(manger==3)
            {
                placer_piece(&carte);
                carte.plateau[positionLIGNEpommePrecedente][positionCOLONNEpommePrecedente]=0;
                manger=0;
            }


            if(PositionSnake[0].x==carte.positionCOLONNEpomme && PositionSnake[0].y==carte.positionLIGNEpomme)
            {
                cpiece++;

                sprintf(score,"Player 1: ""%d",cpiece-2);
                SDL_FreeSurface(Score);
                Score=TTF_RenderText_Shaded(police,score,couleurNoire,couleurBlanche);

                manger=3;
                positionLIGNEpommePrecedente=carte.positionLIGNEpomme;
                positionCOLONNEpommePrecedente=carte.positionCOLONNEpomme;
                snakeActuel[1]=snake[CORPS];



            }
            if(coup!=0 && serpent1active==1)
            {

                for(i=1; i<cpiece; i++)
                {
                    snakeActuel[i]=snake[CORPS];
                }
            }


            if(PositionSnake2[0].x==carte.positionCOLONNEpomme && PositionSnake2[0].y==carte.positionLIGNEpomme)
            {
                cpiece2++;

                sprintf(score2,"Player 2: ""%d",cpiece2-2);
                SDL_FreeSurface(Score2);
                Score2=TTF_RenderText_Shaded(police,score2,couleurNoire,couleurBlanche);

                manger=3;
                positionLIGNEpommePrecedente=carte.positionLIGNEpomme;
                positionCOLONNEpommePrecedente=carte.positionCOLONNEpomme;
                snakeActuel2[1]=snake2[CORPS];



            }
            if(coup!=0 && serpent2active==1)
            {

                for(i=1; i<cpiece2; i++)
                {
                    snakeActuel2[i]=snake2[CORPS];
                }
            }



            position[0].x=PositionSnake[0].x*TAILLE_BLOC;
            position[0].y=PositionSnake[0].y*TAILLE_BLOC;



            for (i=1; i<cpiece; i++)
            {
                position[i].x=Posp[i-1].x*TAILLE_BLOC;
                position[i].y=Posp[i-1].y*TAILLE_BLOC;
                PositionSnake[i].x=Posp[i-1].x;
                PositionSnake[i].y=Posp[i-1].y;
            }


            position2[0].x=PositionSnake2[0].x*TAILLE_BLOC;
            position2[0].y=PositionSnake2[0].y*TAILLE_BLOC;



            for (i=1; i<=cpiece2; i++)
            {
                position2[i].x=Posp2[i-1].x*TAILLE_BLOC;
                position2[i].y=Posp2[i-1].y*TAILLE_BLOC;
                PositionSnake2[i].x=Posp2[i-1].x;
                PositionSnake2[i].y=Posp2[i-1].y;
            }

            SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,255,255,255));

            clean_map(&carte);

            for (i=0; i<cpiece; i++)
            {
                carte.plateau[PositionSnake[i].y][PositionSnake[i].x]=SNAKE1;

            }
            for (i=0; i<cpiece2; i++)
            {
                carte.plateau[PositionSnake2[i].y][PositionSnake2[i].x]=SNAKE2;
            }

            afficher_map(&carte,mur,piece,decor,ecran,position[1]);

            for (i=cpiece-1; i >=0; i--)
            {
                SDL_BlitSurface(snakeActuel[i],NULL,ecran,&position[i]);

            }

            for (i=cpiece2-1; i >=0; i--)
            {
                SDL_BlitSurface(snakeActuel2[i],NULL,ecran,&position2[i]);
            }


            //rafraichi l'ecran

            positionTimer.x=ecran->w/2-Timer->w/2;
            positionTimer.y=0;
            positionScore.x=ecran->w/4-Score->w/2;
            positionScore.y=ecran->h-Score->h;
            positionScore2.x=ecran->w/4*3-Score->w/2;
            positionScore2.y=ecran->h-Score->h;
            SDL_BlitSurface(Timer, NULL, ecran, &positionTimer);
            SDL_BlitSurface(Score,NULL,ecran,&positionScore);
            SDL_BlitSurface(Score2,NULL,ecran,&positionScore2);

            SDL_Flip(ecran);
            continuer=carte.continuer;

        }
    }

    ecran= SDL_SetVideoMode(1366,768,32,SDL_HWSURFACE | SDL_DOUBLEBUF |SDL_FULLSCREEN);
    SDL_FreeSurface(mur);
    SDL_FreeSurface(piece);
    SDL_FreeSurface(decor);
    SDL_FreeSurface(ecran);
    for(int i=0; i<5; i++)
    {
            SDL_FreeSurface(snake[i]);
            SDL_FreeSurface(snake2[i]);
    }
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_CloseAudio();
    SDL_FreeSurface(Timer);
    SDL_FreeSurface(Score);

    return EXIT_SUCCESS;

}
