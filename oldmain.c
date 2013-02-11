#include "main.h"

void main_init() {

    int i;
    float starting_speed = 0.0;
    init_robot();
    flag = 0;
    robot_state = malloc(sizeof (float) *3);

    for (i = 0; i < LEN_PIC_BUFFER; i++) {
        set_vel_2_array(pic_buffer[i], starting_speed, starting_speed);
    }

    write(pic_fd, pic_message_reset_steps_acc, PACKET_TIMING_LENGTH + 1);
    tcflush(pic_fd, TCOFLUSH);
    sync();
    steps_anomaly = 0;

	init_wifi();
}

//=========================================================================================================

//=========================================================================================================

void termination_handler(int signum) {

    int i;


    pthread_mutex_destroy(&mutex);  
    pthread_cancel(thread_pic);

    // Clean the bufffer	
    tcflush(pic_fd, TCIFLUSH);
    tcflush(pic_fd, TCOFLUSH);	
 

   pthread_cancel(thread_main);
	pthread_cancel(thread_wifi);
	close_wifi();
   fprintf(fp_log, "\nNum packet data: correct  %d wrong  %d sent_wrong: %d \n", num_packet_data_ok, num_packet_data_wrong, num_packet_sent_wrong);
    fflush(fp_log);
    fclose(fp_log);
    printf("scritto\n");
    set_vel_2_array(pic_buffer[pic_last_vel_2_send], 0, 0);
    write(pic_fd, pic_buffer[pic_last_vel_2_write], PACKET_SPEED_LENGTH + 1);
    sync();
 
    exit(0);
}
//=========================================================================================================


void setup_termination() {
    if (signal(SIGINT, termination_handler) == SIG_IGN) signal(SIGINT, SIG_IGN);
    if (signal(SIGHUP, termination_handler) == SIG_IGN) signal(SIGHUP, SIG_IGN);
    if (signal(SIGTERM, termination_handler) == SIG_IGN) signal(SIGTERM, SIG_IGN);
}

//=========================================================================================================

void* tf_wifi_read(void* thread_arg){
setup_termination();
	int bytes_read, addr_len;
	int sin_size, connected;
	uint8 *sizes= ( uint8* ) malloc ( sizeof ( uint8 ) *2 );
	uint8 *buffer;
    	/* Receive message from client */
	unsigned int size;
	size = sizeof ( struct sockaddr_in );

	
	pay.v=0;
	pay.w=0; 
	addr_len = sizeof(struct sockaddr);
	
	sin_size = sizeof(struct sockaddr_in);
	
	
	while(1) {
	

		connected = accept(wifi_fd, (struct sockaddr *)&client_addr,&sin_size);
	        printf("\n I got a connection from (%s , %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		bytes_read=1;
		while (bytes_read>0){
			bytes_read = recv ( connected, ( void* ) sizes, sizeof ( uint8 ) *2, 0 );

		        if ( bytes_read <= 0 ){
		            printf ( "error: %s\n",strerror ( errno ) );
			    //exit(1); 
			}       
            		buffer=malloc ( sizes[1] );
            		bytes_read = recv ( connected, buffer, sizes[1], 0 );

            		if ( connected < 0 ){
		                printf ( "error: %s\n",strerror ( errno ) );
 				exit(1);               
		        }

                        switch ( sizes[0] ){
                		case SPEED:
				if (bytes_read==0) {
					((wifi_vel*)buffer)->v=0;
					((wifi_vel*)buffer)->w=0;																
					}
					else
{
						// Set Velocity	for the PIC			
						set_robot_speed(&(((wifi_vel*)buffer)->v),&(((wifi_vel*)buffer)->w));
printf("RECEIVED DATA: linear %9.7f   angular %9.7f\n",((wifi_vel*)buffer)->v,((wifi_vel*)buffer)->w);
}
					break;
		                case MSG:
					if(((wifi_msg*)buffer)->msg==MSG_STOP){
						//termination_handler(0);
						//raise(SIGINT);
pid_t process_id=getpid();
kill(process_id,SIGINT);	
					}
                    			break;
                	}
           	}
        }
        close ( connected );
	free ( sizes );
}

//=========================================================================================================
void* tf_main(void* thread_arg) {

    float angle;
    float v_ref, w_ref;
	int i;
    long int counter=0;

	
    setup_termination();

    angle = 0.0;
    gyro_integral = 0.0;
    gyro_sup_integral = 0.0;
    float a,b, v_return=0.0, w_return=0.0;
    a=4;
    b=0;
//    set_robot_speed(&a,&b);

 #define RAD2DEG(x) ((float) x*180/M_PI )

	struct timeval tvb, tva;

	int cycle;
	char debug_buf[24];
	log_fd=open("timing_main.txt",O_CREAT | O_WRONLY | O_TRUNC , S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);

	// The timing of the first cycle is not allined due to the 
	// fact that the two threads are not setup exactly at the
	// same time
	pthread_cond_wait(&cond, &mutex);
	pthread_mutex_unlock(&mutex);		

	while(1){
		counter++;
		pthread_cond_wait(&cond, &mutex);
		printf("Alive! [%d]\n",counter);
		pthread_mutex_unlock(&mutex);
		gettimeofday(&tvb,NULL);
	
		//
		get_robot_state(&robot_state);
		//printf("%f\t%f\t%f\n",state[0],state[1],RAD2DEG(state[2]));
		//printf("lu: %9.7f   \tlw: %9.7f\n",last_v_ref,last_w_ref);


		gettimeofday(&tva,NULL);



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
                sprintf(debug_buf,"%ld\n",cycle);
                write(log_fd, debug_buf,strlen(debug_buf));
#endif	
	}

}

//=========================================================================================================

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





//=========================================================================================================

int main(int argc, char* argv[]) {

    printf("INIT\n");

    main_init();


    fp_log = fopen(file_log, "w");
    setup_termination();

	pthread_mutex_init(&mutex, NULL);


    pthread_attr_t attr_main;
    pthread_attr_init(&attr_main);
	// Main thread
	pthread_create(&thread_main, &attr_main, &tf_main, NULL);
	pthread_create(&thread_pic, &attr_main, &tf_pic2netus, NULL);	

    pthread_create(&thread_wifi, &attr_main, &tf_wifi_read, NULL);	

	
    printf("FD Pic: %d\n", pic_fd);
    printf("FD Xbee Write: %d\n ", xbee_fd_w);
    printf("FD Xbee Read: %d\n ", xbee_fd_r);
    pthread_join(thread_main, NULL);
    return 0;
}
//=========================================================================================================



