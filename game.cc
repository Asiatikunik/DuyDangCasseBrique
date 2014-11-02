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
int VitesseJeu=300;
int VitesseBarre=3;
bool fin=0;
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
			if(fin==0) {
				if (i==H-3 && j> barreD && j< barreF ) { 
					screen[i][j]='o';
				}
			}  /*else{

				screen[15][26]='G';
				screen[15][27]='a';
				screen[15][28]='m';
				screen[15][29]='e';
				screen[15][31]='O';
				screen[15][32]='v';
				screen[15][33]='e';
				screen[15][34]='r';
			}*/
		}
	}

}
double x= H-4;
double y= 30 ;
bool vertical;
bool horizontal;
bool pret=0;

void update_game(int key) {

	
	screen[(int)x][(int)y] = 1;// la balle
	


	if(key==' ') 
		pret=1;


	//Déplacement de la barre
	if(key=='s') {
		if(barreD>=0) {
			barreD=barreD-VitesseBarre;
			barreF=barreF-VitesseBarre;
		}	
	}

	if(key=='d') {
		if(barreF<L) {	
			barreD=barreD+VitesseBarre;
			barreF=barreF+VitesseBarre;
		}
		if (barreF==L-1) { //Pour la Barre ne Depasse pas le tableau
			barreD=barreD+2;
			barreF=barreF+2;	
		}
	}

	if(pret==0){
		y= (barreD+barreF)/2;
	}




	//Le rebond de la barre
	if( y>barreD && y<barreF && x==H-3) {
		vertical=1;
	}

	//Limite du screen
	if ( vertical==0 ) {
		if(pret==1) {
		x = x + VitesseBalle;
		if(x==H)
			vertical=1;
	}}

	if (horizontal==0 ) {
		if(pret==1) {
		y = y + VitesseBalle;
		if(y==L)
			horizontal=1;
	}}



	if(x==H)
	//horizontal=1;
	fin=1;
	

	if(horizontal==1) {
		 if(pret==1) {
		y = y -VitesseBalle;
		if(y==0)	
			horizontal=0;
	}}

	if (vertical==1) {
		 if(pret==1) {
		x = x - VitesseBalle;
		if(x==0)
			vertical=0;
	}}


	//Les briques
	int flag=0;
	for(int n=0; n<50; n++) {
		for( int m=0; m<3; m++) {

			if(blocDelete[n]==0)
				screen[ blocListe[n][1] ][ blocListe[n][0] +m ]= '#';
			// else
			//   screen[ blocListe[n][1] ][ blocListe[n][0] +m ]= '|';

		}
	}

	for(int n=0; n<50; n++) {
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


				int a=getBloc(x,y);
				if ( horizontal==0 && blocListe[a][0]-1==y && blocListe[a][1]-1==x && blocListe[a][1]+1==x && flag ==0) {
					horizontal=1;
					flag=1;
				}	



				if ( horizontal==1 && blocListe[a][0]+1==y && blocListe[a][1]-1==x && blocListe[a][1]+1==x && flag==0) {
					horizontal=1;
				}



				flag=0;
				blocListe[ n ][0]= -1;	
				blocListe[ n ][1]= -1;

			}
		}
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



void stage1() {

	for ( int m=0; m<5; m++) {
		for ( int n=0; n<10; n++) {
			blocListe[n+10*m][0]=6+5*n;
			blocListe[n+10*m][1]=5+m;
		}
	}

}





/******************************************************************************/

int main() {
	int key;

	for ( int n=0; n<65; n++) {
		blocDelete[n]=0;
	}
	stage1();

	//Fin du jeu si tout les blocs sont détruits
	for( int n=0; n<50; n++) {
		if(blocDelete[n]==0 || fin==1) {
			break;
		}else{
			fin=1;	
		}


	}



	do {
		key = read_keybord();
		clear_screen(); 
		update_game(key);
		screen_display();
		usleep(VitesseJeu * 1000);

		if(/*x==H*/ fin==1 ) {
			x=-1;
			y=-1;

			screen[15][26]='G';
			screen[15][27]='a';
			screen[15][28]='m';
			screen[15][29]='e';
			screen[15][31]='O';
			screen[15][32]='v';
			screen[15][33]='e';
			screen[15][34]='r';

			screen_display();
			key='y';

		}

	} while (key != 'y' );

	/*
	   if (fin==1) {
	   screen_display();
//	x=1;
//	y=1;

}	*/
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

