/* 
 * File:   Vision2.hpp
 * Author: Claudio Carbone
 *
 * Created on February 15, 2013, 6:06 PM
 */

#ifndef VISION2_HPP
#define	VISION2_HPP
#include <cv.h>
#include <cv.hpp>
#include <highgui.h>

//#define __GXX_EXPERIMENTAL_CXX0X__
#include <thread>
#include <mutex>
#include <map>
#include <BlobResult.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/CvBridge.h>
#include <gtk/gtk.h>

#define MAX_LOST                100000000
#define ROB_MAX                 15
#define MAX_LINE_LEN            4096

namespace saetta_vision_two
{

    template<class Type1>
    class Matrix
    {
    public:

        Matrix(unsigned int r, unsigned int c) : innVector(r*c)
        {
            _rows = r;
            _cols = c;
        }

        Type1& operator() (unsigned int row, unsigned int col)
        {
            return innVector.at(row * _cols + col);
        }

        Type1& operator[] (unsigned int index)
        {
            return innVector.at(index);
        }

        unsigned int rows()
        {
            return _rows;
        }

        unsigned int cols()
        {
            return _cols;
        }

        void addrow(unsigned int n)
        {
            for (unsigned int i = 0; i < n; i++)
                for (unsigned int j = 0; j < _cols; j++)
                    innVector.insert(innVector.end(), Type1());
            _rows+=1;
        }

        /*void addcolumn(unsigned int n)
        {
            std::vector<Type1>::iterator myiter;
            myiter = innVector.begin();
            while (myiter != innVector.end())
            {
                myiter += cols;
                innVector.insert(Type1());
                myiter += 1;
            }
        }*/

        virtual ~Matrix()
        {
        }


    private:
        std::vector<Type1> innVector;
        unsigned int _rows;
        unsigned int _cols;
    };

    typedef struct
    {
        CvPoint2D32f head;
        CvPoint2D32f tail;
        CvPoint2D32f center;
        CvPoint2D32f coord;

        bool moving;
        float orientation;
        bool active;
        bool lost;
        int id;
    } Robot_t;

    typedef struct
    {
        // Current List of Robots
        Robot_t robList[ROB_MAX];
        int robNum;
        int init;
    } RobotList_t;

    class VisionConfig
    {
    public:
        VisionConfig();
        virtual ~VisionConfig();
        void set_camera(int width, int height);
        void set_windows(int width, int height);

        template<typename T>
        void set_mapname(T mapname)
        {
            map_name = mapname;
        }
        void set_showwindows(bool show);

    private:
        
        bool open_windows;
        std::pair<int, int> window_size;
        std::pair<int, int> camera_size;
        std::string map_name;
        friend class Vision;

    };

    class Robot
    {
    public:
        static inline Robot_t createRobot(CvPoint2D32f h, CvPoint2D32f t, Matrix<std::pair<float, float >> &pixelMap);
        static inline void printRobot(Robot_t rob);
        static inline int findNearestRobot(int from, float (&distMat)[ROB_MAX][ROB_MAX]);
        static inline void deactivateRob(int id, RobotList_t *avRobList, float (&distMat)[ROB_MAX][ROB_MAX]);
        static inline void updateRobotList(RobotList_t *avRobList, RobotList_t potRobList, float (&distMat)[ROB_MAX][ROB_MAX]);
        static inline void updateRobotListAndrea(RobotList_t *avRobList, RobotList_t potRobList);
        static inline float computeDist(CvPoint2D32f p, CvPoint2D32f q);
        static inline void updateDistMatrix(int idAv, int idPot, float (&distMat)[ROB_MAX][ROB_MAX]);
        static inline void computeDistMatrix(RobotList_t *avRobList, RobotList_t potRobList, float (&distMat)[ROB_MAX][ROB_MAX]);
        static inline void associateRob2Measure(int idAv, int idPot, RobotList_t *avRobList, RobotList_t *potRobList, float (&distMat)[ROB_MAX][ROB_MAX]);
        static inline int findNearestMeasure(int from, float (&distMat)[ROB_MAX][ROB_MAX]);
        static inline CvPoint2D32f getGlobalCoord(int x, int y, Matrix<std::pair<float, float >> &pixelMap);
    private:

    };

    class Vision : public Robot
    {
    public:
        Vision(VisionConfig& config);
        virtual ~Vision();
        int Start();
        int Stop();
        RobotList_t getRobList();
        void imageCallback(const sensor_msgs::ImageConstPtr& msg);

    private:
        int InitPixelMap(const char * map_file);
        int checkDistMarker(float a, float b);
        IplImage* GetThresholdedImage(IplImage* imgHSV, CvScalar Low, CvScalar High);
        IplImage* blobDetection2(IplImage* imgThreshRed, IplImage* imgThreshGreen);

        void capture_loop();
        std::map<std::string,GtkWidget*> winHndlMap;
        CvCapture* capture;
        VisionConfig configuration;
        std::thread thread_camera_analyzer;
        IplImage* frame;
        RobotList_t avRobList;
        bool th_process;
        bool th_quit;
        CvScalar lowTrsh1, highTrsh1, lowTsrh2, highTsrh2;
        bool th_ok;
        float distMatrix[ROB_MAX][ROB_MAX];
        float minDistMatrix[ROB_MAX][ROB_MAX];
        Matrix<std::pair<float, float >> pixelMap;
        std::mutex mutexRobListAccess;
        //IplImage* imgTracking;

    };


}
#endif	/* VISION_HPP */

