/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <fcntl.h>
using namespace std;

/* test commentaire */

/* retourne 0 si aucune touche n'est appuyée, 
 * sinon retourne le code ASCII de la touche */
int read_keybord();

/******************************************************************************/
#define H 25 
#define L 50

char screen[H][L];

void screen_display() {
	system("clear");
	for (int i=0; i<H; i++) {
		for (int j=0; j<L; j++) {
			
			if(screen[i][j]==1)
				printf("\033[36;01mX\033[00m");
			else
			cout << screen[i][j];
		}
		cout << endl;
	}
}

void clear_screen() {
	for (int i=0; i<H; i++) {
		for (int j=0; j<L; j++) {
			if(i==0 || i== H-1 || j==0 || j==L-1) {
				screen[i][j] = '*';
			}else{
			screen[i][j] = ' ';
			}
		}
	}
}

/******************************************************************************/
double x = 2;
double y = 5;
bool vertical;
bool horizontal;

void update_game(int key) {
	double vert;
	double hori;

	screen[(int)x][(int)y] = 1;

if (key=='s') 
	vertical=0;

if (key == 'z') 
	vertical=1;

if(key== 'q')
	horizontal=1;

if(key== 'd')
	horizontal=0;

if ( vertical==0 ) {
	x = x + 1;
	if(x==25)
		vertical=1;
	}

if (horizontal==0 ) {
	y = y + 1;
	if(y==50)
		horizontal=1;
	}

if (vertical==1) {
	x = x - 1;
	if(x==0	)
		vertical=0;
	}

if (horizontal==1 ) {
	y = y - 1;
	if(y==0) 
		horizontal=0;
}


}

/******************************************************************************/

int main() {
	int key;
	do {
		key = read_keybord();
		clear_screen();
		update_game(key);
		screen_display();
		usleep(100 * 1000);
	} while (key != 'y');
}

/******************************************************************************/

int kbhit(void)
{
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

	if(ch != EOF)
	{
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

