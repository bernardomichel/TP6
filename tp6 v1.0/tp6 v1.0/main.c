#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "curses.h"

#define NUMEVENTS 8

typedef unsigned int uint;

int main(int argc, char *argv[]) 
{
	WINDOW * winTest=NULL;                     //Variable en dónde se guarda la terminal (Window) en donde voy a trabajar.
	int i;				                       //Variable para iterar
	int xWinTestPos ,yWinTestPos;              //Variables en donde voy a guardar la posición del cursor.
	int xEvRec, yEvRec;
	int xEvLast, yEvLast; //Variables en donde hago backup de la posición del cursor donde voy a acualizar la pantalla.
	int xAction, yAction;
	unsigned long long pressedKeyCounter = 0l; //Variable en donde voy a acumilar la cantidad de teclas presionadas antes de la 'Q'

	char *nameevents[NUMEVENTS]={"WRQ","RRQ","DATA","ACK","LASTDATA","TIMEOUT","ERROR"};
	char charevents[NUMEVENTS]={'w','r','d','a','l','t','e'};
	//Inicializo la terminal y verifico que se haya inicializado correctamente:
	winTest = initscr();
	if(winTest != NULL)
	{
		//Inicializo los colores.
		start_color();

		//Declaro los "color pairs" siempre primero foreground y después background.
		init_pair(1,COLOR_WHITE, COLOR_BLACK);
		
		//Esta opción hace que siempre que se llame a una función que escribe se refresque la pantalla.
		immedok(winTest,TRUE);

		//Elijo el primer set de colores (letras azules con fondo negro) y escribo un texto de prueba.
		color_set(1,NULL);
		mvprintw(1,8,"Programa de simulación de cliente TFTP implementado con FSM\n");
		mvprintw(3,3,"Cuando el usuario presiona las teclas de Eventos entiende que se generó un");
		mvprintw(4,1,"nuevo evento y responde ante ese evento realizando una acción y cambiando el\n estado.");
				
		//Creo el encabezado de la tabla que se muestra en pantalla.
		mvprintw(7,6,"EVENTOS");	mvprintw(7,30, "|");	mvprintw(7,50,	"STATUS DE LA FSM:");
		
		
		mvprintw(12,40, "Evento recibido: ");
		getyx(winTest,yEvRec,xEvRec);
		
		mvprintw(15,40, "Ultimo evento recibido: ");
		getyx(winTest,yEvLast,xEvLast);

		mvprintw(18,40, "Acción ejecutada: ");
		getyx(winTest,yAction,xAction);

		move(24,0);			
		printw("Presionar 'Q' para terminar la simulación...\n");

		//Muestro en pantalla los eventos y sus teclas correspondientes.
		for(i=0;i<NUMEVENTS;i++){
			mvprintw(7+(2*i+1),30,"|");
			mvprintw(11+(2*+i-1),6,"%c = %s",charevents[i],nameevents[i]);
			mvprintw(11+(2*(i-1)),30,"|");
		}
			

		//Con las dos opciones de abajo elijo que el getch()sea no bloqueante (nodelay TRUE) y que no
		//muestr los caracteres cuando el usuario los escribe (noecho).
		nodelay(winTest, TRUE);
		noecho();

		//Ahora se itera para crear un "V" de números en la terminal.
		/*for(i=0;i<20;i++)
		{
			color_set(1,NULL);
			
			if(i<10)
				move(i+2,getmaxx(winTest)/4+i);
			else
				move(21-i,getmaxx(winTest)/4+i);
			
			printw("%d\n",i);
		}
			*/
		//Me muevo debajo de la V y geteo caracteres hasta que se apriete la Q.
		//Notar que como getch() es no bloqueante se llama todo el tiempo en el while
		//si el usuario no presionó ninguna tecla getch() devuelve ERR en lugar de la tecla apretada.
		
		//Backup de la posición actual del cursor antes de iterar...
		getyx(winTest,yWinTestPos,xWinTestPos);
		
		//Itero esperando que se apriete una 'Q' para salir.
		do
		{
			i = getch();
			if(i != ERR)
			{
				pressedKeyCounter++;
				//mvprintw(yWinTestPos,xWinTestPos,"%c pressed. %llu Keys Pressed\n",(char)i, pressedKeyCounter);
			}
				
		}
		while(tolower(i) != 'q');
		
		//Llamo para termiar PDCurses.
		endwin();
		}
		
	return 0;
}
