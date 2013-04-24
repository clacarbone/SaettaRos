/* 
 * File:   Vision.cpp
 * Author: attilio
 * 
 * Created on February 15, 2013, 6:06 PM
 */

#include <gtk-2.0/gtk/gtkstyle.h>
#include <gtk-2.0/gtk/gtkwidget.h>

#include "Vision.hpp"

namespace saetta_vision
{

    /*Vision::Vision ( ) : thread_camera_analyzer ( &Vision::capture_loop, this )
    {
        capture = 0;
        frame = 0;
        avRobList.robNum = 0;
        avRobList.init = 1;
        //thread_camera_analyzer = new std::thread(Vision::Vision::capture_loop());
        th_process = false;
        th_quit = false;
        th_ok = true;
        //    capture = cvCaptureFromCAM(0);    
    }*/


    Vision::Vision( VisionConfig& config ) : winHndlMap( ), thread_camera_analyzer( &Vision::capture_loop, this ), pixelMap( config.camera_size.first*config.camera_size.second )
    {
        capture = 0;
        frame = 0;
        avRobList.robNum = 0;
        avRobList.init = 1;
        //thread_camera_analyzer = new std::thread(Vision::Vision::capture_loop());
        th_process = false;
        th_quit = false;
        th_ok = true;
        configuration = config;
    }

    Vision::~Vision( )
    {
        th_process = false;
        th_quit = true;
        thread_camera_analyzer.join();
        cvDestroyAllWindows();
    }

    int Vision::Start( )
    {
        capture = cvCreateCameraCapture(-1);
        if (!capture)
        {
            printf("Capture failure\n");
            return -1;
        }

        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 1280);
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 720);

        if (configuration.open_windows)
        {

            cvNamedWindow("Video", CV_WINDOW_NORMAL);
            GtkWidget* localhndl;
            localhndl = GTK_WIDGET(cvGetWindowHandle("Video"));
            winHndlMap["Video"] = localhndl;
#ifdef DEBUG_H   
            cvNamedWindow("Binary Red");
            localhndl = GTK_WIDGET(cvGetWindowHandle("Binary Red"));
            winHndlMap["Binary Red"] = localhndl;

            cvNamedWindow("Binary Green");
            localhndl = GTK_WIDGET(cvGetWindowHandle("Binary Green"));
            winHndlMap["Binary Green"] = localhndl;
#endif    
            cvNamedWindow("Output", CV_WINDOW_NORMAL);
            localhndl = GTK_WIDGET(cvGetWindowHandle("Output"));
            winHndlMap["Output"] = localhndl;

            cvResizeWindow("Video", configuration.window_size.first, configuration.window_size.second);
            cvResizeWindow("Output", configuration.window_size.first, configuration.window_size.second);
        }
	//head - red
        lowTrsh1 = cvScalar(0,250,50);
        highTrsh1 = cvScalar(0,256,200);
	//tail - green
        lowTsrh2 = cvScalar(55,229,51);
        highTsrh2 = cvScalar(79,256,192);



        // Load Map
        if(Vision::InitPixelMap(configuration.map_name.c_str()) != 0)
	    return -1;

        frame = cvQueryFrame(capture);
        frame = cvQueryFrame(capture);
        frame = cvQueryFrame(capture);

        th_process = true;
        return 0;

    }

    int Vision::Stop( void )
    {
        th_process = false;
        return 0;
    }

    RobotList_t Vision::getRobList( void )
    {
        mutexRobListAccess.lock();
        RobotList_t newlist = avRobList;
        mutexRobListAccess.unlock();
        return newlist;
    }

    bool Vision::getTryRobList(RobotList_t& roblist  )
    {
        if (mutexRobListAccess.try_lock())
        {
            roblist = avRobList;
            mutexRobListAccess.unlock();
            return true;
        }
        return false;
    }

    void Vision::capture_loop( )
    {

        th_process = false;
        th_quit = false;
        while (1)
        {
            if (th_process)
            {
                data_avail = 0;
                frame = cvQueryFrame(capture);
                if (frame == 0)
                {
                    std::cout << "Error in frame querying" << std::endl;
                    th_ok = false;
                    exit(0);
                }

                frame = cvCloneImage(frame);
                cvSmooth(frame, frame, CV_GAUSSIAN, 3, 3); //smooth the original image using Gaussian kernel



                IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
                cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV



                IplImage* imgThreshRed = Vision::GetThresholdedImage(imgHSV, lowTrsh1, highTrsh1);

                IplImage* imgThreshGreen = Vision::GetThresholdedImage(imgHSV, lowTsrh2, highTsrh2);

                cvSmooth(imgThreshRed, imgThreshRed, CV_GAUSSIAN, 3, 3); //smooth the binary image using Gaussian kernel

                cvSmooth(imgThreshGreen, imgThreshGreen, CV_GAUSSIAN, 3, 3); //smooth the binary image using Gaussian kernel


                IplImage* imgFinal = Vision::blobDetection2(imgThreshRed, imgThreshGreen);

                cvSmooth(imgFinal, imgFinal, CV_GAUSSIAN, 3, 3); //smooth the binary image using Gaussian kernel

                if (configuration.open_windows)
                {


                    cvShowImage("Video", frame);
#ifdef DEBUG_H
                    cvShowImage("Binary Red", imgThreshRed);
                    cvShowImage("Binary Green", imgThreshGreen);
#endif
                    cvShowImage("Output", imgFinal);
                }


                //Clean up used images
                cvReleaseImage(&imgHSV);
                cvReleaseImage(&imgThreshRed);
                cvReleaseImage(&imgThreshGreen);
                cvReleaseImage(&imgFinal);
                cvReleaseImage(&frame);


                //Wait 10mS
                int c = cvWaitKey(10);
                //If 'ESC' is pressed, break the loop
                if ((char) c == 27) break;

            }

            else
            {
                if (th_quit)
                    break;
                else
                    data_avail = 1;
                usleep(100000);
            }
        }
    }


    //This function threshold the HSV image and create a binary image

    IplImage* Vision::GetThresholdedImage( IplImage* imgHSV, CvScalar Low, CvScalar High )
    {
        IplImage* imgThresh = cvCreateImage(cvGetSize(imgHSV), IPL_DEPTH_8U, 1);
        //cv::inRange(imgHSV, Low, High, imgThresh);
        cvInRangeS(imgHSV, Low, High, imgThresh);
        return imgThresh;
    }

    // This function detects two different types of blobs

    IplImage* Vision::blobDetection2( IplImage* imgThreshRed, IplImage* imgThreshGreen )
    {
        // get blobs and filter them using its area
        int i, j;
        int areaBlob = 100;
        float distMark = 30;
        CBlobResult blobsRed, blobsGreen, whiteRedBlobs, whiteGreenBlobs;
        CBlob *currentBlob;
        double px, py;

        // Create Image
        IplImage* displayedImage = cvCreateImage(cvGetSize(imgThreshRed), IPL_DEPTH_8U, 3);

        // find all the RED related blobs in the image
        blobsRed = CBlobResult(imgThreshRed, NULL, 0);
        // find all the GREEN related blobs in the image
        blobsGreen = CBlobResult(imgThreshGreen, NULL, 0);

        // select the ones with mean gray-level equal to 255 (white) and put
        // them in the whiteBlobs variable
        blobsRed.Filter(whiteRedBlobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 1.0);
        blobsGreen.Filter(whiteGreenBlobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 1.0);

#ifdef DEBUG_PRINT    
        printf("White Blobs: %d\n", whiteBlobs.GetNumBlobs());
#endif

        // display filtered blobs
        cvMerge(imgThreshRed, imgThreshRed, imgThreshRed, NULL, displayedImage);

        // RED
        CvPoint2D32f redCenter[whiteRedBlobs.GetNumBlobs()];

        for (i = 0; i < whiteRedBlobs.GetNumBlobs(); i++)
        {
            currentBlob = whiteRedBlobs.GetBlob(i);
            px = (currentBlob->MaxX() + currentBlob->MinX()) / 2.0;
            py = (currentBlob->MaxY() + currentBlob->MinY()) / 2.0;
            redCenter[i] = cvPoint2D32f(px, py);

#ifdef DEBUG_PRINT    
            printf("%2.2f\t%2.2f\n", px, py);
#endif

            if (currentBlob->Area() > areaBlob)
            {
                // Add Cross to the image
                currentBlob->FillBlob(displayedImage, CV_RGB(255, 0, 0));
                cvCircle(displayedImage, cvPointFrom32f(redCenter[i]), 2, cvScalar(255, 0, 0), 10, 8, 0);
            }
        }

        // GREEN
        CvPoint2D32f greenCenter[whiteGreenBlobs.GetNumBlobs()];

        for (i = 0; i < whiteGreenBlobs.GetNumBlobs(); i++)
        {
            currentBlob = whiteGreenBlobs.GetBlob(i);
            px = (currentBlob->MaxX() + currentBlob->MinX()) / 2.0;
            py = (currentBlob->MaxY() + currentBlob->MinY()) / 2.0;
            greenCenter[i] = cvPoint2D32f(px, py);

#ifdef DEBUG_PRINT    
            printf("%2.2f\t%2.2f\n", px, py);
#endif

            if (currentBlob->Area() > areaBlob)
            {
                // Add Cross to the image
                currentBlob->FillBlob(displayedImage, CV_RGB(255, 0, 0));
                cvCircle(displayedImage, cvPointFrom32f(greenCenter[i]), 2, cvScalar(0, 255, 0), 10, 8, 0);
            }
        }

        // Populating the list of potential robots
        RobotList_t potRobList;
        potRobList.robNum = 0;

        for (i = 0; i < ROB_MAX; i++)
            potRobList.robList[i].active = 0;

        int redUsage[whiteRedBlobs.GetNumBlobs()];
        int greenUsage[whiteGreenBlobs.GetNumBlobs()];

        for (i = 0; i < whiteRedBlobs.GetNumBlobs(); i++)
            redUsage[i] = 0;

        for (j = 0; j < whiteGreenBlobs.GetNumBlobs(); j++)
            greenUsage[j] = 0;


        // Detect Robots
        float distCenter[whiteRedBlobs.GetNumBlobs()][whiteGreenBlobs.GetNumBlobs()];
        for (i = 0; i < whiteRedBlobs.GetNumBlobs(); i++)
        {
            currentBlob = whiteRedBlobs.GetBlob(i);
            if (currentBlob->Area() > areaBlob)
            {
                for (j = 0; j < whiteGreenBlobs.GetNumBlobs(); j++)
                {
                    currentBlob = whiteGreenBlobs.GetBlob(j);
                    if (currentBlob->Area() > areaBlob)
                    {
                        distCenter[i][j] = Robot::computeDist(redCenter[i], greenCenter[j]);
                        //printf("[%d] - [%d]: %2.2f\n", i, j, distCenter[i][j]);
                        // Print a connection line if this could be a robot
                        if (redUsage[i] == 0 && greenUsage[j] == 0 && checkDistMarker(distCenter[i][j], distMark))
                        {
                            cvLine(displayedImage, cvPointFrom32f(redCenter[i]), cvPointFrom32f(greenCenter[j]), cvScalar(0, 255, 255), 2, 8, 0);
                            // Check Robot
                            potRobList.robList[potRobList.robNum] = Robot::createRobot(redCenter[i], greenCenter[j], pixelMap);

                            potRobList.robNum++;
                            redUsage[i] = 1;
                            greenUsage[j] = 1;
                            //                        printRobot(potRobList.robList[potRobList.robNum - 1]);


                            CvBox2D tmp;
                            tmp.angle = potRobList.robList[potRobList.robNum - 1].orientation;
                            tmp.center = potRobList.robList[potRobList.robNum - 1].center;
                            tmp.size = cvSize2D32f(30, 50);
                            cvEllipseBox(displayedImage, tmp, cvScalar(255, 255, 0), 4, 3, 0);
                            //			printRobot(potRobList.robList[potRobList.robNum-1]);

                        }

                    }
                }
            }
        }


        // Matching The List of Potential Robots with previous List of Robots
        //    updateRobotList_tAndrea(&avRobList, potRobList);
   //     mutexRobListAccess.lock();
        Robot::updateRobotList(&avRobList, potRobList, distMatrix);
   //     mutexRobListAccess.unlock();
        /*
            // Print robots
            for (i = 0; i < ROB_MAX; i++) {
                if (avRobList.robList[i].active == 1) {
                    CvBox2D tmp;
                    tmp.angle = avRobList.robList[i].orientation;
                    tmp.center = avRobList.robList[i].center;
                    tmp.size = cvSize2D32f(50, 30);
                    cvEllipseBox(displayedImage, tmp, cvScalar(255, 255, 0), 4, 3, 0);
                    printRobot(avRobList.robList[i]);
                }
            }
         */



        /* Control Law */

        return displayedImage;

    }

    int Vision::InitPixelMap( const char * map_file )
    {

        FILE *map;

        map = fopen(map_file, "r");
        char line[MAX_LINE_LEN];
        int i;
        float indiceRiga;
        float indiceColonna;
        float x_coord;
        float y_coord;

        if (map == NULL)
        {
	    char mystr[1024];
	    getcwd(mystr, 1024);
	    printf("Current path: \"%s\"\n", mystr);
            printf("InitPixelMap: failed to open file\n");
            printf("%s\n", map_file);
            // getchar();
            return -1;
        }
        printf("%s\n", map_file); //GIO
        //Read data from file
        for (i = 0; i < configuration.camera_size.first * configuration.camera_size.second; i++)
        {

            //Read one line at a time
            char * rc = fgets(line, MAX_LINE_LEN, map);

            if (line == NULL)
            {
                printf("InitPixelMap: failed to read from file\n");
                fclose(map);
                //getchar();
                return -1;
            }

            else
            {
                sscanf(line, "%f\t%f\t%f\t%f", &indiceRiga, &indiceColonna, &x_coord, &y_coord);
                //pixelMap(indiceRiga, indiceColonna).first = x_coord;
                //pixelMap(indiceRiga, indiceColonna).second = y_coord;
                //                        printf("%2.2f %2.2f \n",pixelMap.x_coord[i],pixelMap.y_coord[i]);
                pixelMap[i].first= x_coord;
                pixelMap[i].second = y_coord;	            
		}
        }

        fclose(map);

        return 0;
    }

    int Vision::checkDistMarker( float a, float b )
    {
        float epsilon = 20;
        return (fabs(a - b) < epsilon);
    }

    VisionConfig::VisionConfig( ) : window_size( 0, 0 ), camera_size( 0, 0 )
    {
        open_windows = false;

    }

    VisionConfig::~VisionConfig( )
    {

    }

    void VisionConfig::set_camera( int width, int height )
    {
        camera_size.first = width;
        camera_size.second = height;

    }

    void VisionConfig::set_windows( int width, int height )
    {
        window_size.first = width;
        window_size.second = height;
    }

    void VisionConfig::set_showwindows( bool show )
    {
        open_windows = show;
    }

    Robot_t Robot::createRobot( CvPoint2D32f h, CvPoint2D32f t, std::vector<std::pair<float, float >> &pixelMap )
    {

        Robot_t rob;
        rob.head = h;
        rob.tail = t;
        rob.center = cvPoint2D32f((h.x + t.x) / 2.0, (h.y + t.y) / 2.0);



        rob.active = 1;
        rob.id = -1;

        CvPoint2D32f h_c = getGlobalCoord(h.x, h.y, pixelMap);
        CvPoint2D32f t_c = getGlobalCoord(t.x, t.y, pixelMap);

        rob.orientation = cvFastArctan((h_c.y - t_c.y), h_c.x - t_c.x);

        rob.coord = getGlobalCoord(rob.center.x, rob.center.y, pixelMap);


#ifdef DEBUG_H    
        printf("Head: %2.2f %2.2f\n", h.x, h.y);
        printf("Tail: %2.2f %2.2f\n", t.x, t.y);
        printf("Center: %2.2f %2.2f\n", rob.center.x, rob.center.y);
#endif

        return rob;

    }

    CvPoint2D32f Robot::getGlobalCoord( int x, int y, std::vector<std::pair<float, float >> &pixelMap )
    {

        CvPoint2D32f coord;
	int index = y * 1280 + x;
        coord.x = pixelMap[index].first;
        coord.y = pixelMap[index].second;
        return coord;

    }

    void Robot::printRobot( Robot_t rob )
    {

        printf("Rob[%d]:%2.2f %2.2f %2.2f; coords:%2.2f %2.2f \n", rob.id, rob.center.x, rob.center.y, rob.orientation, rob.coord.x, rob.coord.y);
    }

    int Robot::findNearestRobot( int from, float (&distMat)[ROB_MAX][ROB_MAX] )
    {
        float min_dist = INFINITY;
        int minId = -1;
        int i = 0;
        for (i = 0; i < ROB_MAX; i++)
            if (distMat[i][from] < min_dist)
            {
                min_dist = distMat[i][from];
                minId = i;
            }
        return minId;
    }

    void Robot::deactivateRob( int id, RobotList_t *avRobList, float (&distMat)[ROB_MAX][ROB_MAX] )
    {
        avRobList->robList[id].active = 0;
        if (avRobList->robList[id].lost < MAX_LOST)
            avRobList->robList[id].lost++;
        updateDistMatrix(id, -1, distMat);
    }

    void Robot::updateRobotList( RobotList_t *avRobList, RobotList_t potRobList, float (&distMat)[ROB_MAX][ROB_MAX] )
    {
        int i;
        // Allowed movement
        float distTh = 100; //mm

        // If any potential robot  has been detected... 
        if (potRobList.robNum > 0)
        {
            // If first step let's create the data structure
            if (avRobList->init == 1)
            {
                // Fist let's create the identified robots
                for (i = 0; i < potRobList.robNum; i++)
                {
                    //potRobList.robList[i].active = 1;
                    avRobList->robList[i] = potRobList.robList[i];
                    avRobList->robList[i].active = 1;
                    avRobList->robNum++;
                    avRobList->robList[i].id = i;
                    avRobList->init = 0;
                    avRobList->robList[i].lost = 0;
                    //                printf("Robot %d : (%f, %f)\n", i, avRobList->robList[i].center.x, avRobList->robList[i].center.y);
                }
                // Then let's fill the array till the maximum number of potential robots
                for (i = potRobList.robNum; i < ROB_MAX; i++)
                {
                    avRobList->robList[i].lost = MAX_LOST;
                }
            }// Real step
            else
            {
                // Let's fill in the matrix of distances for this step
                Robot::computeDistMatrix(avRobList, potRobList, distMat);

                // Try to associate to each robot id the next position
                avRobList->robNum = 0;
                int associated = 0;
                int deactivated = 0;
                int assigned[ROB_MAX]; // 0 if i-th is not assigned to any new location, 1 if it is assigned
                for (i = 0; i < ROB_MAX; i++)
                    assigned[i] = 0;


                while (associated + deactivated < ROB_MAX)
                {

                    for (i = 0; i < ROB_MAX; i++)
                    {
                        // If the i-th robot has not been assigned to any new location yet
                        if (assigned[i] == 0)
                        {
                            //                        float dist;
                            int idPot, idAv;
                            float minDist_pot_av, minDist_av_pot;

                            //                        idPot;
                            //                        idAv;
                            //                        dist = INFINITY;

                            // Compute the distance between the i-th robot id and the new locations
                            idPot = Robot::findNearestMeasure(i, distMat);

                            // The only case it would get -1 is if all distances are infinity
                            // When can this happen?
                            if (idPot>-1)
                                minDist_av_pot = distMat[i][idPot];
                            else
                                minDist_av_pot = INFINITY;

                            //printf("ROB %d SCEGLIE POT %d\n",i,idPot);
                            //se la mindistap è accettabile
                            if (minDist_av_pot < distTh + (distTh * avRobList->robList[i].lost) / 4)
                            {

                                //Compute the distance between the selected location and the closest robot
                                idAv = findNearestRobot(idPot, distMat);
                                // When does this happen?
                                if (idAv>-1)
                                    minDist_pot_av = distMat[idAv][idPot];
                                else
                                    minDist_pot_av = INFINITY;


                                //printf("POT %d SCEGLIE ROB %d dist %f\n",i,idPot,minDist_pot_av);
                                // If the match is accomplished then associate the i-th robot to the location
                                if (idAv == i)
                                { //se corrispondono
                                    Robot::associateRob2Measure(idAv, idPot, avRobList, &potRobList, distMat);

                                    assigned[i] = 1; //aggiorno rob associati
                                    associated++;
                                    //printf ( "Robot %d : (%5.2f, %5.2f, %5.2f), px(%5.2f,%5.2f)\n", i, avRobList->robList[i].coord.x, avRobList->robList[i].coord.y, avRobList->robList[i].orientation, avRobList->robList[i].center.x, avRobList->robList[i].center.y );
                                    if (minDist_av_pot > 5)
                                        avRobList->robList[i].moving = true;
                                    else
                                        avRobList->robList[i].moving = false;
                                    //printf("Robot %d : dist %f\n",idAv,minDist_av_pot);

                                }

                            }
                            else
                            {
                                // Deactivate the i-th robot
                                Robot::deactivateRob(i, avRobList, distMat);
                                //printf ( "Robot %d lost da %d step\n", i, avRobList->robList[i].lost );
                                deactivated++;
                                // This robot is deactivated (then is it marked as assigned)
                                assigned[i] = 1;
                            }
                        }
                    }
                    //               printf("ASS %d DEACT %d\n", associated, deactivated);
                }

            }
        }
    }

    /* This function update the robot List */
    void Robot::updateRobotListAndrea( RobotList_t *avRobList, RobotList_t potRobList )
    {
        int i, j;
        // Allowed movement
        float distTh = 100;

        if (potRobList.robNum > 0)
        {



            if (avRobList->init == 1)
            {
                for (i = 0; i < potRobList.robNum; i++)
                {
                    avRobList->robList[i] = potRobList.robList[i];
                    avRobList->robList[i].active = 1;
                    avRobList->robNum++;
                    avRobList->robList[i].id = i;
                    avRobList->init = 0;

                }

            }
            else
            {

                // Try to associate to each robot id the next position
                avRobList->robNum = 0;

                for (i = 0; i < ROB_MAX; i++)
                {
                    float dist;
                    int idx;

                    idx = -1;
                    dist = INFINITY;

                    // Compute the distance between the i-th robot id and the new locations
                    for (j = 0; j < potRobList.robNum; j++)
                    {
                        if ((computeDist(avRobList->robList[i].coord, potRobList.robList[j].coord) < distTh) && (potRobList.robList[j].active == 1))
                        {
                            if (computeDist(avRobList->robList[i].coord, potRobList.robList[j].coord) < dist)
                            {
                                if (idx != -1)
                                    potRobList.robList[idx].active = 1;

                                idx = j;
                                dist = computeDist(avRobList->robList[i].coord, potRobList.robList[j].coord);
                                printf("Dist: %f\n", dist);
                                // Deactive this robot for further associations
                                potRobList.robList[j].active = 0;
                            }
                        }
                    }

                    // Assign to the i-th robot the new location
                    if (idx > -1)
                    {
                        avRobList->robList[i] = potRobList.robList[idx];
                        avRobList->robList[i].active = 1;
                        avRobList->robList[i].id = i;
                        avRobList->robNum++;
                    }
                    else
                    {
                        // Deactivate the i-th robot
                        avRobList->robList[i].active = 0;
                    }

                }
            }
        }

        for (i = 0; i < ROB_MAX; i++)
        {
            if (avRobList->robList[i].active == 1)
                printRobot(avRobList->robList[i]);
        }


    }

    void Robot::updateDistMatrix( int idAv, int idPot, float (&distMat)[ROB_MAX][ROB_MAX] )
    {
        int i;
        for (i = 0; i < ROB_MAX; i++)
            distMat[idAv][i] = INFINITY;

        if (idPot != -1) //se idPot==-1 sto solo disattivando idAv altrimenti sto anche disattivando la misura 
            for (i = 0; i < ROB_MAX; i++)
                distMat[i][idPot] = INFINITY;
    }

    void Robot::associateRob2Measure( int idAv, int idPot, RobotList_t *avRobList, RobotList_t *potRobList, float (&distMat)[ROB_MAX][ROB_MAX] )
    {

        avRobList->robList[idAv] = potRobList->robList[idPot]; //aggiorno misura    
        avRobList->robList[idAv].active = 1; //attivo avRob
        avRobList->robList[idAv].lost = 0;
        avRobList->robNum++;
        potRobList->robList[idPot].active = 0; //disattivo potRob
        Robot::updateDistMatrix(idAv, idPot, distMat);


    }

    float Robot::computeDist( CvPoint2D32f p, CvPoint2D32f q )
    {
        return cv::sqrt((float) cv::pow(p.x - q.x, 2) + cv::pow(p.y - q.y, 2));
    }

    void Robot::computeDistMatrix( RobotList_t *avRobList, RobotList_t potRobList, float (&distMat)[ROB_MAX][ROB_MAX] )
    {
        int i, j;
        for (i = 0; i < ROB_MAX; i++)
            for (j = 0; j < ROB_MAX; j++)
                if (potRobList.robList[j].active == 1)
                    distMat[i][j] = Robot::computeDist(avRobList->robList[i].coord, potRobList.robList[j].coord);
                else
                    distMat[i][j] = INFINITY;
    }

    int Robot::findNearestMeasure( int from, float (&distMat)[ROB_MAX][ROB_MAX] )
    {
        float min_dist = INFINITY;
        int minId = -1;
        int i = 0;
        for (i = 0; i < ROB_MAX; i++)
            if (distMat[from][i] < min_dist)
            {
                min_dist = distMat[from][i];
                minId = i;
            }
        return minId;
    }

}
