#include <SDL2/SDL.h>
#include <unistd.h>
#include "function.h"

#define WINDOW_WIDTH 700 //Defini longeur de la fenetre de jeu
#define WINDOW_HEIGHT 920 //Defini largeur de la fenetre de jeu
#define FPS 60
#define nbColumnBrick 3 //Defini le nombre de colonnes dans mon tableau de briques
#define nbLineBrick 3 //Defini la nombre lignes dans mon tableau de briques
    
// Variables de ma balle
int radiusBall;
int xBall;
int yBall;
int xspeedBall;
int yspeedBall;

// Variables de ma barre
int xRod;
int yRod;
int widhtRod;
int heightRod;
int xspeedRod;
int yspeedRod;

// Variables de mes briques et tableau de
int xBrick;
int yBrick;
int widhtBrick;
int heightBrick;
int tabLifeBrick[nbColumnBrick][nbLineBrick];
int lifeBrick = 2;

// Variables de vie du joueurs
int lifePlayer = 2;

// Variables des sprites
int posX;
int posY;
char imgBMPSrc;

/*----------------------------------------------------      Mes fonction        -------------------------------------------------*/
//Dessine une balle
void drawBall(){
    drawCircle(xBall,yBall,radiusBall);
    //sprite(xBall, yBall, "sprite/ball.bmp");
	xBall += xspeedBall;
	yBall += yspeedBall;			
}

// Dessine ma barre, change en fonction de la vie du joueur
void drawRod(){
    if(lifePlayer == 2){
        changeColor(255,0,0);                 
       	drawRect(xRod,yRod,widhtRod,heightRod); 
    }

    else{
        widhtRod= 100;
       	changeColor(255,0,0);                 
        drawRect(xRod,yRod,widhtRod,heightRod); 
    }
    
}

//Gére les rebond de ma balle
void bounceBall(){

    //si le joueur n'as plus de vie la barre va tomber et faire terminer le jeu 
    if((yBall>(WINDOW_HEIGHT-radiusBall))){
        lifePlayer--;
    }
	
	//Gére le rebond sur la largeur de mon écran
	if((xBall>(WINDOW_WIDTH-radiusBall))|| (xBall<radiusBall)){
		xspeedBall= -xspeedBall;	
	}
	
    //Gére les rebons sur la longeur de mon écran
	if((yBall>(WINDOW_HEIGHT-radiusBall))|| (yBall<radiusBall)){
		yspeedBall= -yspeedBall;
	}
   
    //Gére les rebons de ma barre
	if(yBall>(yRod-radiusBall) && (xBall<xRod+widhtRod) && (xBall>xRod)){
		yspeedBall= -yspeedBall;
        
	}

}

// Si le joueur n'as plus de vie, la barre tombe. Puis arrivé en bas tu tableau le jeu ce termine.
void defeat(){
    if (lifePlayer == 0){
            yRod += yspeedRod;
    }

    if(yRod >WINDOW_HEIGHT){
        printf("DEFAITE \nEssaye encore !");
        freeAndTerminate();
    }
}

// limite la barre à rester dans le tableau
void restrictRod(){
    if((xRod)<0){
        xRod=0;
    }

    else if((xRod+widhtRod)>WINDOW_WIDTH){
        xRod=WINDOW_WIDTH-widhtRod;
    }
}

// Dessine les briques
void drawBrick(){
    sprite(xBrick, yBrick, "sprite/brick.bmp");
    //drawRect(xBrick,yBrick,widhtBrick,heightBrick);
}

// Dessine les briques cassées
void drawBrokenBrick(){
    sprite(xBrick, yBrick, "sprite/brickBroken.bmp");
    //drawRect(xBrick,yBrick,widhtBrick,heightBrick);
}

// Image de mon fond d'écran
void drawScreen(){
    sprite(0, 0, "sprite/superMario.bmp");
}
/*----------------------------------------------        initgame        -----------------------------------------------------*/

void init_game(){
    //mettre le code d'initialisation ici 

    //Valeur position, taille et rayon et vitesse de ma balle
    radiusBall = 13;
    xBall = 200;    //WINDOW_WIDTH/2
    yBall =600;     //WINDOW_HEIGHT - 6*radiusBall
    xspeedBall =5;
    yspeedBall =-5;
    
    //Valeur position, taille et vitesse barre
    xRod = WINDOW_WIDTH/4;
    yRod = 761;
    widhtRod = 150;
    heightRod = 25;  //15
    xspeedRod = 15;
    yspeedRod = 5;

    //Valeur position et taille d'une brique
    xBrick = 32; //32
    yBrick = 20; //30
    widhtBrick = 100;      //100;
    heightBrick = 50;      //50;


    // Valeur tableau de vie de mes briques
    for(int i=0; i<nbColumnBrick; i++){      
        for(int j=0; j<nbLineBrick; j++){
            tabLifeBrick[i][j]= lifeBrick;
        }
    } 
}

/*----------------------------------------------        drawGame        -----------------------------------------------------*/
void drawGame(){
    /* Ici je dessine mon jeu
     * exemple position x, y modifiés dans KeyPressed() et utilisés pour
     * pouvoir deplacer la figure à chaque boucle de gameLoop()
     */
    clear();

    // Condition de victoire
    // verifie le tableau de vie de mes briques
    int brokenBrick = 0;
    for(int k=0; k<nbColumnBrick; k++){
        for(int l=0; l<nbLineBrick; l++){
            if(tabLifeBrick[k][l] == 0){
                brokenBrick += 1;
            }
        }
    
    }
    // temps que brokenBrick n'est pas égale à "nbColumnBrick * nbLineBrick" le jeu continue d'être dessiné
    if( brokenBrick != nbColumnBrick * nbLineBrick){
        drawScreen();
        drawBall();
        drawRod();
        bounceBall();
        restrictRod();
        defeat();
        //sprite(20, 20, "sprite/ball.bmp");                      
    }

    // Sinon victoire
    else{
        printf("VICTOIRE \nMerci d'avoir joué !");
        freeAndTerminate();
    }

    // Taille de l'espace entre mes briques
    int spaceBrick = 15;
    
    // Dèclare l'écart entre l'écran de jeu et la brique (en calculent la longeur de fenetre moins l'espace entre ma 1ere brique jusqu'a ma derniere brique diviser par 2 car j'ais les deux valeurs des mes deux écart additionner ensembles )
    int gapBrick = (WINDOW_WIDTH-((nbColumnBrick*widhtBrick)+((nbColumnBrick-1)*spaceBrick)))/2; 


/*------------------//Mon generateur de briques//------------------------------*/

    int i, j; 

    // Gère le nombre de colonnes
    for(int i=0; i<nbColumnBrick; i++){

        // Gère le nombre de lignes
        for(int j=0; j<nbLineBrick; j++){
                xBrick=gapBrick+i*(widhtBrick+spaceBrick);
                yBrick=gapBrick+j*(heightBrick+spaceBrick);
                
                    if (tabLifeBrick[i][j] == 2){
                        
                        // ChangeColor(255,69,0);
                        drawBrick();                                       
                    }

                    else if (tabLifeBrick[i][j] == 1){
                        
                        // ChangeColor(255,140,0);
                        drawBrokenBrick();                                       
                    }
             
            // Rebond sur mes briques 
            if(tabLifeBrick[i][j] >= 1){             	
             	
                // Rebond du dessous
                if((yBall<(yBrick+heightBrick+radiusBall)) && (xBall<(xBrick+widhtBrick)) && (xBall>xBrick) && (yBall>(yBrick+heightBrick)) && (yspeedBall<0)){
                                   
                    yspeedBall =-yspeedBall;
                    tabLifeBrick[i][j] = tabLifeBrick[i][j]-1;
                }
                
                // Rebond du dessus
                else if((yBall>(yBrick-radiusBall)) && (xBall<(xBrick+widhtBrick)) && (xBall>xBrick) && (yBall<(yBrick)) && (yspeedBall>0)){
                    
                    yspeedBall =-yspeedBall;
                    tabLifeBrick[i][j] = tabLifeBrick[i][j]-1;
                }

                // Rebond de gauche
                if((xBall>(xBrick-radiusBall)) && (yBall>(yBrick-radiusBall)) && (yBall<yBrick+heightBrick) && (xBall<(xBrick)) && (xspeedBall>0)){
                                  
                    xspeedBall =-xspeedBall;
                    tabLifeBrick[i][j] = tabLifeBrick[i][j]-1;
                }

                // Rebond de droite
                else if ((xBall < (xBrick + widhtBrick + radiusBall)) && (yBall>(yBrick+radiusBall)) && (yBall<yBrick+heightBrick) && (xBall>(xBrick+widhtBrick)) && (xspeedBall<0)){
                 
                    xspeedBall =-xspeedBall;
                    tabLifeBrick[i][j] = tabLifeBrick[i][j]-1;
                }      

            }  
            //printf("%d ", tabLifeBrick[j][i]);   
        }   
        //printf("\n");     
    }  
    //printf("\n");

    actualize();
    usleep(1000000 / FPS); // 60 images par seconde | 1000000 = 1 seconde
}

void KeyPressed(SDL_Keycode touche){
    /** @brief event.key.keysym.sym renvoi la touche appuyé
     *
     */
    switch (touche) {

        // Voir doc SDL_Keycode pour plus de touches https://wiki.libsdl.org/SDL_Keycode
        // ici exemple ou appuyer sur les touches Q | D modifie x (position du carré dans drawGame())
        case SDLK_q:
            // Touche q appuyé
            printf("Q\n");
            xRod-=xspeedRod;
            break;
        case SDLK_d:
            // Touche d appuyé
            printf("D\n");
            xRod+=xspeedRod;
            break;
        case SDLK_ESCAPE:
            freeAndTerminate();
            break;
        default:
            break;
    }
}

void joyButtonPressed(){
}

void gameLoop() {
    int programLaunched = 1;
    while (programLaunched == 1) {
        // Boucle pour garder le programme ouvert
        // lorsque programLaunched est different de 1
        // on sort de la boucle, donc de la fonction
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // boucle pour la gestion d'évenement
            switch (event.type) {
                /** event.type renvoi le type d'evenement qui se passe
                 * (si on appuie sur une touche, clique de souris...)
                 * en fonction du type d'évènement on à alors
                 * différentes information accessibles
                 * voir doc pour plus d'event https://wiki.libsdl.org/SDL_EventType
                 */
                case SDL_QUIT:
                    // quand on clique sur fermer la fénêtre en haut à droite
                    programLaunched = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    /* clique de la souris
                     * event.motion.y | event.motion.x pour les positions de la souris
                     */
                    printf("position de la souris x : %d , y : %d\n", event.motion.x, event.motion.y);
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                case SDL_JOYBUTTONDOWN:
                    break;
                default:
                    break;
            }
        }
        /* coder ici pour que le code s'execute après chaque évenement
         * exemple dessiner un carré avec position int x, int y qu'on peut
         * deplacer lorsqu'on appuie sur une touche
         */
        drawGame();
    }
}

/*----------------------------------------------        main        ---------------------------------------------------*/
int main(){
    /** @description 3 fonctions dans le main qui permettent de créer l'application et la maintenir ouverte :
     *  init(...) : initialiser la SDL/ fenêtre
     *  gameLoop() : boucle de jeu dans laquelle l'application reste ouverte
     *  freeAndTerminate() : quitte le programme proprement
     */
    init(WINDOW_WIDTH, WINDOW_HEIGHT);
    init_game();
    gameLoop();
    printf("Fin du programme\n");
    freeAndTerminate();
}
