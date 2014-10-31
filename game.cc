/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <fcntl.h>
using namespace std;

int read_keybord();
int getBloc (int x, int y);

/******************************************************************************/
#define H 30 
#define L 60
int barreD= (L/2) -5;
int barreF= (L/2) +5;
double VitesseBalle=1;
int VitesseJeu=100;
int VitesseBarre=3;

char screen[H][L];
int blocListe[50][2];
int blocDelete[50];

void screen_display() {
   system("clear");
   for (int i=0; i<H; i++) {
    for (int j=0; j<L; j++) {

      if(screen[i][j]==1)
	printf("\033[36;01mO\033[00m");
      else
	cout << screen[i][j];
    }
    cout << endl;
  }
}

void clear_screen() {
  for (int i=0; i<H; i++) {
    for (int j=0; j<L; j++) {
      //Le screen
      if(i==0 || i== H-1 || j==0 || j==L-1) {
	screen[i][j] = '-';
	if(j==0 || j==L-1) 
	  screen[i][j] = '|';
      }else{
	screen[i][j] = ' ';
      }

      //La barre
      if (i==H-3 && j> barreD && j< barreF ) { 
	screen[i][j]='o';
      }
      
				
    }
  }
}

double x=2;
double y = 6;
bool vertical;
bool horizontal;

void update_game(int key) {

  screen[(int)x][(int)y] = 1;// la balle


  //Déplacement de la barre
  if(key=='s') {
    if(barreD>=1) {
      barreD=barreD-VitesseBarre;
      barreF=barreF-VitesseBarre;
    }	
  }

  if(key=='d') {
    if(barreF<L-1) {	
      barreD=barreD+VitesseBarre;
      barreF=barreF+VitesseBarre;
    }
  }

  //Le rebond de la barre
  if( y>barreD && y<barreF && x==H-3) {
    vertical=1;
  }

  //Limite du screen
  if ( vertical==0 ) {
    x = x + VitesseBalle;
    if(x==H)
      vertical=1;
  } 

  if (horizontal==0 ) {
    y = y + VitesseBalle;
      if(y==L)
      horizontal=1;
  } 

  if(horizontal==1) {
    y = y -VitesseBalle;
    if(y==0)	
      horizontal=0;
  }

  if (vertical==1) {
    x = x - VitesseBalle;
    if(x==0)
      vertical=0;
  }


  //Les briques
  int flag=0
    ;
  for(int n=0; n<50; n++) {
    for( int m=0; m<3; m++) {
      
      if(blocDelete[n]==0)
	screen[ blocListe[n][1] ][ blocListe[n][0] +m ]= '#';
      // else
      //  screen[ blocListe[n][1] ][ blocListe[n][0] +m ]= '|--';
    
    }
  }
      
  for(int n=0; n<10; n++) {
    if ((blocListe[n][1]==x) &&
	(blocListe[n][0] <=y)&& (blocListe[n][0]+3 > y)){
      blocDelete[n]=1;
	
      //Le rebonb
      if (blocDelete[n]==1) {
	
	if(vertical==0){
	  vertical=1;
	  flag=1;
	}
	
	if (vertical==1 && flag==0){
	  vertical=0;
	  flag =1;
	}
	
	flag=0;
	blocListe[ n ][0]= 0;	
	blocListe[ n ][1]= 0;
	
      }
      
  
    }
  }
  
    
  
}


void stage1() {

  int n=-1;
  int place=5;
  for ( int m=0; m<5; m++) { 
  for ( int n=0; n<10; n++) {
    blocListe[n+10*m][0]=5+4*n;
    blocListe[n+10*m][1]=5+m;
  }
  }

}


	


/******************************************************************************/

int main() {
  int key;
  
  /*
  blocListe[0][0]=5;
  blocListe[0][1]=5;

  blocListe[1][0]=9;
  blocListe[1][1]=5;

  blocListe[2][0]=13;
  blocListe[2][1]=5;

  blocListe[3][0]=17;
  blocListe[3][1]=5;

  blocListe[4][0]=21;
  blocListe[4][1]=5;

  blocListe[5][0]=25;
  blocListe[5][1]=5;

  blocListe[6][0]=29;
  blocListe[6][1]=5;

  blocListe[7][0]=33;
  blocListe[7][1]=5;

  blocListe[8][0]=37;
  blocListe[8][1]=5;

  blocListe[9][0]=41;
  blocListe[9][1]=5;
  */  

  for ( int n=0; n<50; n++) {
    blocDelete[n]=0;
  }

  stage1();

	do {
		key = read_keybord();
		clear_screen(); 
		update_game(key);
		screen_display();
		usleep(VitesseJeu * 1000);
	} while (key != 'y'); 
}

/******************************************************************************/

int kbhit(void) {
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF){
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

int read_keybord() {
	int key;
	if (kbhit())
		key = getc(stdin);
	else
		key = 0;
	return key;
}

/******************************************************************************/
/******************************************************************************/

