#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>




#define TAILLE_BLOC 34
#define TAILLE_MAX_PSEUDO 10
enum direction{HAUT,BAS,GAUCHE,DROITE,CORPS};

enum{VIDE,MUR,SNAKE1,DECOR,PIECE,SNAKE2,PORTEGAUCHE,PORTEDROITE};

 typedef struct{
    int pl;
    int pc;
    int pll;
    int pcc;
    int manger;
    int compteurDePomme;
    int positionTete;
} mario_struct;

 typedef struct{
    int **plateau;
    int lignes;
    int colonnes;
    int positionLIGNEpomme;
    int positionLIGNEpommePrecedente;
    int positionCOLONNEpommePrecedente;
    int positionCOLONNEpomme;
    int positionPORTEgaucheLigne;
    int positionPORTEgaucheColonne;
    int positionPORTEdroiteLigne;
    int positionPORTEdroiteColonne;
    int continuer;
} map_struct;




void afficher_map(map_struct *carte,SDL_Surface *mur,SDL_Surface *pomme,SDL_Surface *decor,SDL_Surface *ecran,SDL_Rect position);
void placer_snake(mario_struct *snake,map_struct *carte);
void choisir_map();
void fin_de_partie();
void jouer (SDL_Surface* ecran,int mode);
void deplacerSnake(map_struct *carte,SDL_Rect *pos,int direction,int sNAKE);
void clean_map(map_struct *carte);
void enregister_Score(int compteurpiece,char* pseudo);
void placer_piece(map_struct *carte);
void lire_plateau(char *fichier,map_struct *carte);






#endif // JEU_H_INCLUDED
