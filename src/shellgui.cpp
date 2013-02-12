/* 
 * File:   newmain.cpp
 * Author: erupter
 *
 * Created on December 3, 2012, 4:53 PM
 */
#define PROC_SEPARATION
#include <cstdlib>
#include <random>
#include <unistd.h>
#include <ncurses.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <functional>
#include <mutex>
#include <thread>
#include <list>
#include <iostream>
#include <utility>
#include <chrono>
#include <functional>
#include <atomic>


#define MAX_LEN 99
#define NUM_SENS 5
char buffer[MAX_LEN+1] = {0};
int out_pipe[2];
int saved_stdout;
using namespace std;
char *portname = "/dev/ttyO3";
const char mesg[]="Testing stdout redirection";
int main_proc(int argc, char **argv);
WINDOW *create_newwin(int height, int width, int starty, int startx, int vert, int horz);
void destroy_win(WINDOW *local_win);
void curses_windows_setup();
void pipe_eater(void);
WINDOW *win_shell, *win_shell_borders, *win_ir_borders, *win_ir;
std::list<std::string> local_messages;
std::mutex msg_mutex;

struct shared_mem {        /* Defines "structure" of shared memory */
    unsigned int IR[5];
    float odom[2];
    int uptodate;
};




/*------------------------------------------------------------------------------
 * main()
 * Main function to set up ROS node.
 *----------------------------------------------------------------------------*/
int main(int argc, char** argv)
{


    int cycle = 0, pid, p[2], row, col;

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
			setvbuf(stdout, NULL, _IOLBF, 1000);
			main_proc(argc,argv);
			printf("\n\tChild quitting cleanly\n\n");
			break;
		default:
			//s_catch_signals ();

			std::string mystr;
			//close(p[1]);  /* first close the write end of the pipe */
			if(dup2(p[0], 0 ) == -1 ) /* stdin == read end of the pipe */
			{
				perror( "dup2 failed" );
				return(1);
			}
			/* Create shared memory object and set its size */
			std::thread t(pipe_eater);
			struct shared_mem *rptr;
			int shared_mem_fd;
			shared_mem_fd = shm_open("/saetta_shared_mem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
			if (shared_mem_fd == -1)
			    /* Handle error */;


			if (ftruncate(shared_mem_fd, sizeof(struct shared_mem)) == -1)
			    /* Handle error */;


			/* Map shared memory object */


			rptr = (struct shared_mem*) mmap(NULL, sizeof(struct shared_mem),
			       PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
			if (rptr == MAP_FAILED)
			    /* Handle error */;


			/* Now we can refer to mapped region using fields of rptr;
			   for example, rptr->len */

			initscr();				/* start the curses mode */
			getmaxyx(stdscr,row,col);		/* get the number of rows and columns */
			//mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
							/* print the message at the center of the screen */

			curses_windows_setup();
			//refresh();
			
			//wprintw(win_shell,"This screen has %d rows and %d columns\n",row,col);
			//wprintw(win_shell,"Try resizing your window(if possible) and then run this program again\n");
			scrollok(win_shell,TRUE);
			int myy,myx;
			std::stringstream ss;
			int i=0;
			const char fill_char[] = "|";
			while(1)
			{
				//wmove(win_ir,0,0);
				for (int i=0; i<5; i++)
				{
					mvwprintw(win_ir,i,0,"S%1d: %3u",i,rptr->IR[i]);
					int maxwidth = col-9-2, width;
					float fwidth;
					fwidth = (float)rptr->IR[i] / 255.0;
					width = maxwidth-(int)(float(maxwidth)*fwidth);
					for (int j=9; j<col-1; j++)
					{
						if(j<col-1-width)
							mvwprintw(win_ir,i,j," ");
						else
							mvwprintw(win_ir,i,j,fill_char);
					}
					wrefresh(win_ir);						
					/*ss.clear();
					ss << "S" << i << ": " << rptr->IR[i] << std::endl;
					mystr= ss.str();
					waddstr(win_ir,mystr.c_str());
					wmove(win_ir,i,0);*/
				}
				wrefresh(win_ir);
				msg_mutex.lock();
				while(local_messages.size()>0)
				{

					//waddstr(win_shell,local_messages.front().c_str());

					wprintw(win_shell, "%s\n",local_messages.front().c_str());
					local_messages.pop_front();
				}
				msg_mutex.unlock();
				//wprintw(win_shell,"%4d Messages in list: %ld\n",i,local_messages.size());
				wrefresh(win_shell);
				++i;
				usleep(1000000);

			}
			t.join();
			endwin();
			
			
			break;
	}



	//dup2(saved_stdout, STDOUT_FILENO);  /* reconnect stdout for testing */
	//setvbuf(stdout, NULL, _IOLBF, 0);
	endwin();
	printf("\n\tProgram quitting cleanly\n\n");
	return (EXIT_SUCCESS);
}

void pipe_eater (void)
{
	std::string mystr;
	std::stringstream ss;
	while(1){
	while( std::getline(std::cin, mystr) )
	{
		msg_mutex.lock();
		local_messages.push_back(mystr);
		msg_mutex.unlock();

	}}
}

int main_proc (int argc, char **argv)
{
	int cycle=0;
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned int> distribution(0,255);
    auto dice = std::bind ( distribution, generator );
	/* Create shared memory object and set its size */

	struct shared_mem *rptr;
	int shared_mem_fd;
	shared_mem_fd = shm_open("/saetta_shared_mem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (shared_mem_fd == -1)
	    /* Handle error */;


	if (ftruncate(shared_mem_fd, sizeof(struct shared_mem)) == -1)
	    /* Handle error */;


	/* Map shared memory object */


	rptr = (struct shared_mem*)mmap(NULL, sizeof(struct shared_mem),
	       PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
	if (rptr == MAP_FAILED)
	    /* Handle error */;

	while(1)
	{
		for(int i=0; i<5; i++)
			rptr->IR[i]=dice();
		printf("Cycle %d\n",cycle);
		++cycle;
		//usleep(5000000);
		sleep(5);

	}
	/* Now we can refer to mapped region using fields of rptr;
   	for example, rptr->len */

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


int file_exists(const char * filename)
{
    FILE * file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void curses_windows_setup()
{

	#define IR_X_POS 0
	#define IR_Y_POS 0
	#define IR_X_LENGTH 
	#define IR_Y_LENGTH 7
	#define SHELL_X_POS 0
	#define SHELL_Y_POS
	#define SHELL_X_LENGTH
	#define SHELL_Y_LENGTH 12
	#define WIN_SPACE 2
	int row,col;
	getmaxyx(stdscr,row,col);

	win_ir_borders = create_newwin( IR_Y_LENGTH, col, IR_Y_POS, IR_X_POS, 0, 0);
	wrefresh(win_ir_borders);
	win_ir = create_newwin( IR_Y_LENGTH-2, col-2, IR_Y_POS+1, IR_X_POS+1, ' ', ' ');
	wrefresh(win_ir);

	int shell_y_length = SHELL_Y_LENGTH, shell_y_pos = IR_Y_POS+IR_Y_LENGTH+WIN_SPACE;
	if (SHELL_Y_LENGTH > row-IR_Y_LENGTH-WIN_SPACE)
		shell_y_length = row-IR_Y_LENGTH-WIN_SPACE;
	else
		shell_y_length = SHELL_Y_LENGTH;
	
	win_shell_borders = create_newwin(shell_y_length,col,shell_y_pos, SHELL_X_POS, 0, 0);	
	wrefresh(win_shell_borders);
	win_shell = create_newwin(shell_y_length-2,col-2,shell_y_pos+1, SHELL_X_POS+1, ' ', ' ');
	wrefresh(win_shell);	

	mvwprintw(win_ir_borders,0,col/2-1,"IRs");
	wrefresh(win_ir_borders);

}

