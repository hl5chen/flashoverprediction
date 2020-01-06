// CameraStreamer.hpp
// Multi-Threading more than two USB Cameras
// connected to a NVIDIA Jetson Nano Developer Kit using OpenCV
// Drivers for the camera and OpenCV

#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <concurrent_queue.h>
#include "opencv2\videoio.hpp"
#include <opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;
using namespace concurrency;
 
class CameraStreamer{
public:

    //vector<string> camera_source; //holds camera stream urls
    vector<int> camera_index; //holds usb camera indices
    vector<VideoCapture*> camera_capture; //holds OpenCV VideoCapture pointers
    vector<concurrent_queue<Mat>*> frame_queue; //holds queue(s) which hold images from each camera
    vector<thread*> camera_thread; //holds thread(s) which run the camera capture process
    
    //Constructor for IP Camera capture
    //CameraStreamer(vector<string> source);
    //Constructor for USB Camera capture
    CameraStreamer(vector<int> index);

    //Destructor for releasing resource(s)
    ~CameraStreamer();
    

private:

    bool isUSBCamera;
    int camera_count;
    //camera capturing process(es)
    void startMultiCapture();
    void stopMultiCapture();
    //main camera capturing process which will be done by the thread(s)
    void captureFrame(int index);
};