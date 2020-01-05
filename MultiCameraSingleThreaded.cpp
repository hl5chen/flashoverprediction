// MultiCameraSingleThreaded.cpp
// Runnning more than one USB Camera on Single Thread
// connected to a NVIDIA Jetson Nano Developer Kit using OpenCV
// Drivers for the camera and OpenCV

#include <opencv2\videoio.hpp>
#include <opencv2\highgui.hpp>
 
using namespace cv;
using namespace std;
 
void main()
{
    //The number of connected USB camera(s)
    const uint CAM_NUM = 3;
 
    //This will hold the VideoCapture objects
    VideoCapture camCaptures[CAM_NUM];
 
    //This will hold the resulting frames from each camera
    Mat camFrames[CAM_NUM];
 
    //This will be used for highgui window name
    string labels[CAM_NUM];
 
    //Initialization of VideoCaptures
    for (int i = 0; i < CAM_NUM; i++)
    {
        //Init label for highgui window name
        labels[i] = "Camera " + to_string(i);
 
        //Opening camera capture stream
        camCaptures[i].open(i);
    }
 
    //continous loop until 'Esc' key is pressed
    while (waitKey(1) != 27){
        for (int i = 0; i < CAM_NUM; i++)
        {
            //capturing frame-by-frame from each capture
            camCaptures[i] >> camFrames[i];
 
            //showing the resulting frame using highgui
            imshow(labels[i], camFrames[i]);
        }
    }
 
    //Releasing all VideoCapture resources
    for (int i = 0; i < CAM_NUM; i++)
    {
        camCaptures[i].release();
    }
}