/* 
 * File:   newmain.cpp
 * Author: erupter
 *
 * Created on December 3, 2012, 4:53 PM
 */

#include <cstdlib>
#include <ros/ros.h>
//#include "serverWIFI.h"
#include "Saetta_Base.h"
#define MAX_LEN 99
#define NUM_SENS 5
char buffer[MAX_LEN+1] = {0};
int out_pipe[2];
int saved_stdout;
using namespace std;
char *portname = "/dev/ttyO3";

int main_proc(int argc, char **argv);

//using namespace SaettaServerWifi;

/*------------------------------------------------------------------------------
 * listenerCallback()
 * Receive twist message.
 *----------------------------------------------------------------------------*/

/*void listenerCallback (const geometry_msgs::Twist::ConstPtr & msg)
{
    ROS_INFO("Received linear: [%f], angular: [%f]\n", msg->linear.x, msg->angular.z);
}*/


void listenerCallback (const turtlesim::Velocity::ConstPtr & msg)
{
    ROS_INFO("Received linear: [%f], angular: [%f]\n", msg->linear, msg->angular);
}

/*------------------------------------------------------------------------------
 * main()
 * Main function to set up ROS node.
 *----------------------------------------------------------------------------*/
int main(int argc, char** argv)
{
    WINDOW *my_win, *my_win2;
    int cycle = 0, pid, p[2], row, col;
    int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf ("error %d opening %s: %s", errno, portname, strerror (errno));
        return -1;
    }

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

int main_proc (int argc, char **argv)
{
    set_interface_attribs (fd, B115200, 0);
    close(fd);
    std::string mypath;
    std::stringstream ss;
    ss << argv[0];
    mypath = ss.str();
    int last = mypath.find_last_of ('/',mypath.length());
    mypath = mypath.substr (0,last);
    printf ("The current working directory is %s\n", mypath.c_str());
    // Setup termination handler
    chdir(mypath.c_str());
    int res = file_exists("acc.txt");
    //res ? printf("%s exists\n","acc.txt") : printf("File doesn't exist\n");
    //return 0;

    main_init();

    fp_log = fopen(file_log, "w");
    setup_termination();

    pthread_mutex_init(&mutex, NULL);

    pthread_t thread_output;
    pthread_attr_t attr_main;
    pthread_attr_init(&attr_main);
    pthread_create(&thread_pic, &attr_main, &tf_pic2netus, NULL);
    printf("FD Pic: %d\n", pic_fd);
    setup_termination();
    struct timeval tvb, tva;

    int cycle;
    char debug_buf[24];
    log_fd=open("timing_main.txt",O_CREAT | O_WRONLY | O_TRUNC , S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);

    // The timing of the first cycle is not allined due to the 
    // fact that the two threads are not setup exactly at the
    // same time
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);		



    /*WifiTx mywifi;
    char myaddr[]= "192.168.1.11";
    mywifi.init_wifi(myaddr);
    mywifi.send_wifi_vel(1.0,0.0);
    cout << "Sending speed command for 2 seconds.\n";
    sleep (2);
    cout << "Done.\n";
    mywifi.send_wifi_vel(0,0);*/
    Saetta_Base* localbase = new Saetta_Base();
    
    

    // Set up ROS.
    ros::init(argc, argv, "Saetta_Base");
    ros::NodeHandle n;

    // Declare variables that can be modified by launch file or command line.
    int rate = 20;
    const char *ip = "192.168.1.57";

    
//    localbase->InitWifi(ip);
    // Initialize node parameters from launch file or command line.
    // Use a private node handle so that multiple instances of the node can be run simultaneously
    // while using different parameters.
    ros::NodeHandle base_handle_("~");
    
    //base_handle_.param("rate", rate, int(40));
    ros::Subscriber sub = n.subscribe("/saetta/velocity", 10, &Saetta_Base::listenerCallback, localbase);

    // Create a new NodeExample object.
    //Saetta_Base *base_node = new Saetta_Base();

    // Create a subscriber.
    // Name the topic, message queue, callback function with class name, and object containing callback function.
    //ros::Subscriber sub_message = n.subscribe("example", 1000, &base_node.messageCallback, Saetta_Base);

    // Tell ROS how fast to run this node.
    ros::Rate r(rate);
    long counter = 0;
    while (n.ok())
    {
        ros::spinOnce();
        
	counter++;
	pthread_cond_wait(&cond, &mutex);
//	printf("Alive! [%ld]\n",counter);
	printf("Steps\tL:%04.2f\tR:%04.2f\n",robot_state[0],robot_state[1]);
	pthread_mutex_unlock(&mutex);
	gettimeofday(&tvb,NULL);

	//
	get_robot_state(&robot_state);
	//printf("%f\t%f\t%f\n",state[0],state[1],RAD2DEG(state[2]));
	//printf("lu: %9.7f   \tlw: %9.7f\n",last_v_ref,last_w_ref);


	gettimeofday(&tva,NULL);
	printf("IR Ranger:\n");
	int i;
	for (i=0; i<NUM_SENS; i++){
		printf("S%02d  %04u\n ", i, *(ir->range+i));
	}


	if (tva.tv_sec==tvb.tv_sec){
	    //printf("%ld\n", tva.tv_usec-tvb.tv_usec);
#ifdef LOG_FROM_MAIN_THREAD 
   	    cycle=tva.tv_usec-tvb.tv_usec;
#endif
	}
	else {
            int delta;
            delta = 1000000-tvb.tv_usec;
            //printf("%ld\n",tva.tv_usec+delta);
#ifdef LOG_FROM_MAIN_THREAD
	    cycle=tva.tv_usec+delta;
#endif
	}
#ifdef LOG_FROM_MAIN_THREAD
	sprintf(debug_buf,"%d\n",cycle);
	write(log_fd, debug_buf,strlen(debug_buf));
#endif	
        r.sleep();

    }

//    dup2(saved_stdout, STDOUT_FILENO);  /* reconnect stdout for testing */
    return (EXIT_SUCCESS);
}

void main_init() {
    int i;
    float starting_speed = 0.0;
    init_robot();
    flag = 0;
    robot_state = (float *) malloc(sizeof (float) *3);

    for (i = 0; i < LEN_PIC_BUFFER; i++) {
        set_vel_2_array(pic_buffer[i], starting_speed, starting_speed);
    }

    write(pic_fd, pic_message_reset_steps_acc, PACKET_TIMING_LENGTH + 1);
    tcflush(pic_fd, TCOFLUSH);
    sync();
    steps_anomaly = 0;

}

void setup_termination() {
    if (signal(SIGINT, termination_handler) == SIG_IGN) signal(SIGINT, SIG_IGN);
    if (signal(SIGHUP, termination_handler) == SIG_IGN) signal(SIGHUP, SIG_IGN);
    if (signal(SIGTERM, termination_handler) == SIG_IGN) signal(SIGTERM, SIG_IGN);
}

void termination_handler(int signum) {

    int i;


    pthread_mutex_destroy(&mutex);  
    pthread_cancel(thread_pic);

    // Clean the bufffer	
    tcflush(pic_fd, TCIFLUSH);
    tcflush(pic_fd, TCOFLUSH);	
 
   fprintf(fp_log, "\nNum packet data: correct  %d wrong  %d sent_wrong: %d \n", num_packet_data_ok, num_packet_data_wrong, num_packet_sent_wrong);
    fflush(fp_log);
    fclose(fp_log);
    printf("scritto\n");
    set_vel_2_array(pic_buffer[pic_last_vel_2_send], 0, 0);
    write(pic_fd, pic_buffer[pic_last_vel_2_write], PACKET_SPEED_LENGTH + 1);
    sync();
 
    exit(0);
}

void* tf_pic2netus(void *args) {
	
	
	int i;
	unsigned char buf[256];
	struct timeval tvb, tva;	
	int counter;
	int byte_read;
	int tot_byte_read;
	int an_ret;

#ifdef LOG_FROM_PIC_THREAD
	int cycle;
	unsigned char debug_buf[128];
	pic_log_fd=open("timing_pic.txt",O_CREAT | O_WRONLY | O_TRUNC , S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
#endif	


#ifdef LOG_FROM_PIC_THREAD	
	cycle=0;
#endif
	
	counter=0;

	tcflush(pic_fd, TCIFLUSH);
	tcflush(pic_fd, TCOFLUSH);

	int rr;
	// Hook cycle
	do {	
		rr=read(pic_fd, buf, 1);
		printf("Read %d\n",rr);
	}
	while(buf[0]!=0x0A);
	
	while(1) {
		memset(buf,'\0',128);
		counter++;
		byte_read=0;
		printf("Cycle: %d\n",counter);
             // Get the start
                do {
                        read(pic_fd, buf, 1);
                }
                while(buf[0]!='S');
                byte_read++;
 		gettimeofday(&tva,NULL);

 	        if (tva.tv_sec==tvb.tv_sec){
        	        printf("%ld  \t%d\n", tva.tv_usec-tvb.tv_usec, byte_read);
#ifdef LOG_FROM_PIC_THREAD
                	cycle=tva.tv_usec-tvb.tv_usec;
#endif
        	}
        	else {
                	int delta;
                	delta = 1000000-tvb.tv_usec;
                	printf("%ld  \t%d\n",tva.tv_usec+delta, byte_read);
#ifdef LOG_FROM_PIC_THREAD
                	cycle=tva.tv_usec+delta;
#endif
        	}

#ifdef LOG_FROM_PIC_THREAD
                sprintf(debug_buf,"%ld  \t%d\n",cycle,byte_read);
                write(pic_log_fd, debug_buf,strlen(debug_buf));
#endif


              
		// Get the whole pkg    
                do {
                        read(pic_fd,buf+byte_read,1);
                        byte_read++;
                }
                while(*(buf+byte_read-1)!='\n');
/*                for (i=0; i< byte_read; i++) {
                        printf("%02x\t",buf[i]);
                //      printf("%c\t",buf[i]);
                }       
                printf("\n");
*/                tot_byte_read=byte_read;
//		write(pic_fd, pic_message_timing, 4);
		analizza_pacchetto(buf,byte_read);
		byte_read=0;
                // Get the whole pkg    
                memset(buf,'\0',128);
                byte_read=0;
                do {
                        read(pic_fd,buf+byte_read,1);
                        byte_read++;
                }
                while(*(buf+byte_read-1)!='\n');
/*                for (i=0; i< byte_read; i++) {
                        printf("%02x\t",buf[i]);
                //      printf("%c\t",buf[i]);
                }
                printf("\n");
*/                tot_byte_read+=byte_read;
		an_ret=analizza_pacchetto(buf,byte_read);
		gettimeofday(&tvb,NULL);
	
		if (an_ret == LOAD_PACKET_ANALYZED )
			pthread_cond_signal(&cond);
		else
			printf("problem!\n");

	}
	
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



