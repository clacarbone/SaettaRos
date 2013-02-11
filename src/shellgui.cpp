#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#define MAX_LEN 99

WINDOW *create_newwin(int height, int width, int starty, int startx, int vert, int horz);
void destroy_win(WINDOW *local_win);

char buffer[MAX_LEN+1] = {0};
int out_pipe[2];
int saved_stdout;

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

	const char mesg[]="Testing stdout redirection";
	WINDOW *my_win, *my_win2;
	int cycle = 0, pid, p[2], row, col;
	/*saved_stdout = dup(STDOUT_FILENO);  // save stdout for display later

	if( pipe(out_pipe) != 0 ) {           // make a pipe
	exit(1);
	}

	dup2(out_pipe[1], STDOUT_FILENO);     // redirect stdout to the pipe
	close(out_pipe[1]);*/

	/* open pipe */


	if(pipe(p) == -1)
	{
		perror("pipe call error");
		return(1);
	}
	
	switch (pid=fork())
	{
		case -1: perror("error: fork call");
			return(2);

		case 0:  /* if child then write down pipe */
			//close(p[0]);  /* first close the read end of the pipe */
			if(dup2(p[1], 1) == -1 ) /* stdout == write end of the pipe */
			{
				perror( "dup2 failed" );
				return(1);
			}
			s_catch_signals ();
			setvbuf(stdout, NULL, _IOLBF, 1000);
			while(!s_interrupted)
			{
				printf("Test %d\n",cycle);
				cycle++;
				usleep(10000);
			}
			printf("\n\tChild quitting cleanly\n\n");
			break;
		default:
			s_catch_signals ();
			std::string mystr;
			//close(p[1]);  /* first close the write end of the pipe */
			if(dup2(p[0], 0 ) == -1 ) /* stdin == read end of the pipe */
			{
				perror( "dup2 failed" );
				return(1);
			}
			initscr();				/* start the curses mode */
			getmaxyx(stdscr,row,col);		/* get the number of rows and columns */
			mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
							/* print the message at the center of the screen */

			refresh();
			my_win2 = create_newwin(10,col,row-10, 0, 0, 0);	
			wrefresh(my_win2);
			my_win = create_newwin(8,col-2,row-9, 1, ' ', ' ');
			wrefresh(my_win);
			wprintw(my_win,"This screen has %d rows and %d columns\n",row,col);
			wprintw(my_win,"Try resizing your window(if possible) and then run this program again\n");
			scrollok(my_win,TRUE);
			int myy,myx;
			std::stringstream ss;
			while(!s_interrupted)
			{
				while( std::getline(std::cin, mystr) )
				{
					ss.clear();
					ss << mystr << std::endl;
					mystr = ss.str();
					//std::cout<< mystr<<std::endl;
					waddstr(my_win,mystr.c_str());
					wrefresh(my_win);
					//getyx(my_win,myy,myx);
					//wmove(my_win,myy+1,0);
				}
				usleep(100000);

			}
			endwin();
			
			
			break;
	}



	//dup2(saved_stdout, STDOUT_FILENO);  /* reconnect stdout for testing */
	//setvbuf(stdout, NULL, _IOLBF, 0);
	endwin();
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
