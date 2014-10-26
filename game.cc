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
int blocListe[10][2];


void brique () {

	for(int s=0; s<10; s++) {
		for( int m=0; m<3; m++) {
			if(blocListe[s][0] != -1) {
			screen[ blocListe[s][1] ][ blocListe[s][0] +m ]= '#';
			}
		}
	}
	
}


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
double y = 5;
bool vertical;
bool horizontal;

void update_game(int key) {

	screen[(int)x][(int)y] = 1;


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
int blocNb=getBloc(x,y);
	if ( blocNb != -1 ) {
		blocListe[blocNb][0]=-1;
		blocListe[blocNb][1]=-1;
		brique();
	}	

	
}

void rebond (int n, int vertical, int horizontal ) {

	if(n==1) { 
		if( horizontal==1) 
			horizontal=0;
		else
			horizontal=1;
	}


	if(n==2) {
		if(vertical==1)
			vertical=0;
		else
			vertical=1;
	}	
	
}

	
	
int getBloc (int x, int y) {

int actuelX;
int actuelY;

		
	for (int n=0; n<10; n++) {
		
		actuelX=blocListe[n][0];
		actuelY=blocListe[n][1];
		
		for ( int t=0; t<3; t++) {
		 	if( actuelY==y && actuelX+t==x) {
				return n;
			}
		}
	
	}	

return -1;

}


/******************************************************************************/

int main() {
	int key;

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

blocListe[6][0]=5;
blocListe[6][1]=7;

blocListe[7][0]=9;
blocListe[7][1]=7;

blocListe[8][0]=13;
blocListe[8][1]=7;

blocListe[9][0]=17;
blocListe[9][1]=7;




	do {
		key = read_keybord();
		clear_screen();
		brique (); 
		update_game(key);
		brique();
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

