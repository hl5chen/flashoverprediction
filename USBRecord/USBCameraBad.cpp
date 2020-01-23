// CameraStreamer.cpp
// Multi-Threading more than two USB Cameras
// connected to a NVIDIA Jetson Nano Developer Kit using OpenCV
// Drivers for the camera and OpenCV


#include "CameraStreamer.hpp"
#include <string>
 

 /*
CameraStreamer::CameraStreamer(vector<string> stream_source){
        camera_source = stream_source;
        camera_count = camera_source.size();
        isUSBCamera = false;
        
        startMultiCapture();
}
*/

CameraStreamer::CameraStreamer(vector<int> capture_index)
{
    camera_index = capture_index;
    camera_count = capture_index.size();
    isUSBCamera = true;
    
    startMultiCapture();
}
    
CameraStreamer::~CameraStreamer()
{
    stopMultiCapture();
}
    
void CameraStreamer::captureFrame(int index)
{
    VideoCapture *capture = camera_capture[index];

    int frame_width = capture.get(CV_CAP_PROP_FRAME_WIDTH/2); 
    int frame_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT/2); 
    VideoWriter video("outcpp" + to_string(index) + ".avi" ,CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height));

    while (true)
    {
        Mat frame;
        (*capture) >> frame; //Grab frame from camera capture
        frame_queue[index]->push(frame); //Put frame to the queue
        frame.release(); //relase frame resource
    }

}
    
void CameraStreamer::startMultiCapture()
{
    VideoCapture *capture;
    thread *t;
    concurrent_queue<Mat> *q;
    for (int i = 0; i < camera_count; i++)
    {
        //Make VideoCapture instance
        if (!isUSBCamera){
            /*
            string url = camera_source[i];
            capture = new VideoCapture(url);
            cout << "Camera Setup: " << url << endl;
            */
        }
        else{
            int idx = camera_index[i];
            capture = new VideoCapture(idx);
            cout << "Camera Setup: " << to_string(idx) << endl;
        }
    
        camera_capture.push_back(capture); //Put VideoCapture to the vector
        t = new thread(&CameraStreamer::captureFrame, this, i); //Make thread instance
        camera_thread.push_back(t); //Put thread to the vector
        q = new concurrent_queue<Mat>; //Make a queue instance
        frame_queue.push_back(q); //Put queue to the vector
    }
}
    
void CameraStreamer::stopMultiCapture()
{
    VideoCapture *cap;
    for (int i = 0; i < camera_count; i++) {
        cap = camera_capture[i];
        if (cap->isOpened()){
            //Relase VideoCapture resource
            cap->release();
            cout << "Capture " << i << " released" << endl;
        }
    }
}