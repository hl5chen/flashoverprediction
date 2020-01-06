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
    const uint CAM_NUM = 2;
    VideoCapture camCaptures[CAM_NUM];
    
    Mat camFrames[CAM_NUM]; //hold the resulting frames from each camera
    string labels[CAM_NUM]; //highgui window name
 
    //Initialization of VideoCaptures
    for (int i = 0; i < CAM_NUM; i++){
        labels[i] = "Camera " + to_string(i);
        camCaptures[i].open(i);
    }
 
    //continous loop until 'Esc' key is pressed
    while (waitKey(1) != 27){
        for (int i = 0; i < CAM_NUM; i++)
        {
            //capturing frame-by-frame from each capture
            camCaptures[i] >> camFrames[i];
            imshow(labels[i], camFrames[i]);
        }
    }
 
    //Releasing all VideoCapture resources
    for (int i = 0; i < CAM_NUM; i++)
    {
        camCaptures[i].release();
    }
}