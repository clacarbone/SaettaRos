#include <ncurses.h>
#include <unistd.h>
#include <string.h> 
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
WINDOW *create_newwin(int height, int width, int starty, int startx, int vert, int horz);
void destroy_win(WINDOW *local_win);
void *th_generator(void * args);
long counter;
#define MAX_LEN 99

char buffer[MAX_LEN+1] = {0};
int out_pipe[2];
int saved_stdout;

int myfunct(int argc, char *argv[])
{	
	WINDOW *my_win, *my_win2;
	char mesg[]="Just a very very very long string that doesn't mean anything.";		/* message to be appeared on the screen */
	int row,col;				/* to store the number of rows and *
					 * the number of colums of the screen */
	initscr();				/* start the curses mode */
	getmaxyx(stdscr,row,col);		/* get the number of rows and columns */
	mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
		                	/* print the message at the center of the screen */

	refresh();
	my_win2 = create_newwin(5,72,row-5, 0, 0, 0);	
	wrefresh(my_win2);
	my_win = create_newwin(3,70,row-4, 1, ' ', ' ');
	wrefresh(my_win);
	wprintw(my_win,"This screen has %d rows and %d columns\n",row,col);
	wprintw(my_win,"Try resizing your window(if possible) and then run this program again\n");
	int cycle=0;
	while (1) {
		wprintw(my_win,"Cycle %d\r",cycle);
		wrefresh(my_win);
		usleep(100000);
	}
	wrefresh(my_win);
	move(100,150);
	refresh();
	getch();
	endwin();

	return 0;
}

volatile sig_atomic_t s_interrupted = 0;
volatile sig_atomic_t alarm_expired = 0;


static void s_signal_handler (int signal_value)
{
    if (signal_value == SIGTERM || signal_value == SIGSTOP || signal_value == SIGINT)
        s_interrupted = 1;
    if (signal_value == SIGALRM)
        //signal(SIGALRM,alarm_wakeup);
        alarm_expired = 1;
}

static void s_catch_signals (void)
{
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset (&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
    sigaction (SIGALRM, &action, NULL);
}

int main (int arg, char **argv) {


	int work=1;
	void * param = &work;
	WINDOW *my_win, *my_win2;
	int startx, starty, width, height, origx, origy;
	int ch, row, col;
	long cycle=0;

	saved_stdout = dup(STDOUT_FILENO);  /* save stdout for display later */

	if( pipe(out_pipe) != 0 ) {          /* make a pipe */
	exit(1);
	}

	dup2(out_pipe[1], STDOUT_FILENO);   /* redirect stdout to the pipe */
	close(out_pipe[1]);

	pthread_t th_gen;
	pthread_attr_t attr_main;
	pthread_attr_init(&attr_main);
	pthread_create(&th_gen, &attr_main, &th_generator, param);
	s_catch_signals ();
	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	getmaxyx(stdscr,row,col);
	height = 50;
	width = 50;
	//starty = (LINES - height) / 2;	/* Calculating for a center placement */
	//startx = (COLS - width) / 2;	/* of the window		*/
	starty=2;
	startx=2;
	origx=startx;
	origy=starty;
	printw("Press 'q' to exit");
	refresh();
	my_win2 = create_newwin(row, col, 0, 0, 0, 0);	
	wrefresh(my_win2);
	my_win = create_newwin(row-3, col-2, 1, 1, ' ', ' ');
	wrefresh(my_win);
	wprintw(my_win,"The size of this window is %d:%d and it's hooked at X/Y %d/%d\n\n",width,height,startx,starty);
	wrefresh(my_win);
	scrollok(my_win,TRUE);
	while(!s_interrupted)
	{
		char mystr[200];
		sprintf(mystr,"~ Cazzo te se pigli! ");
		waddstr(my_win,mystr);
		wrefresh(my_win);
		int myx,myy;
		getyx(my_win,myy,myx);
		++cycle;
		mvwprintw(my_win2, row-2, 1,"Cycle %6ld",counter);
		wrefresh(my_win2);
		refresh();
		wmove(my_win,myy,myx);
		usleep(100);
	}

	pthread_join(th_gen,NULL);	
	endwin();			/* End curses mode		  */
	dup2(saved_stdout, STDOUT_FILENO);  /* reconnect stdout for testing */
	printf("\n\tProgram quitting cleanly\n\n");
	return (EXIT_SUCCESS);
}

WINDOW *create_newwin(int height, int width, int starty, int startx, int vert, int horz)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, vert , horz);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}

void *th_generator(void *args)
{
	int work = (int)&args;
	counter=0;
	while(!s_interrupted)
	{
		counter++;
		usleep(10000);
	}
}
