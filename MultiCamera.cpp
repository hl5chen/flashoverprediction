// MultiCamera.cpp
// Multi-Threading more than two USB Cameras
// connected to a NVIDIA Jetson Nano Developer Kit using OpenCV
// Drivers for the camera and OpenCV

#include "CameraStreamer.hpp"
#include "opencv2\highgui.hpp"


std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

 
void main()
{
 
    /*   //IP camera URLs
    vector<string> capture_source = {
        "rtsp://192.168.2.100/profile2/media.smp",
        "rtsp://192.168.0.100/profile2/media.smp"
    };
    */


   //CSI Camera Initialization
/*
    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ;
    int flip_method = 0 ;

    std::string pipeline = gstreamer_pipeline(capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if(!cap.isOpened()) {
        std::cout<<"Failed to open camera."<<std::endl;
        return (-1);
    }

    cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;

    std::cout << "Hit ESC to exit" << "\n" ;
    
    while(waitKey(20) != 27)
    {
    	if (!cap.read(img)) 
        {
            std::cout<<"Capture read error"<<std::endl;
            break;
	    }
	
        cv::imshow("CSI Camera",img);
    }

    cap.release();
    cv::destroyAllWindows() ;
    return 0;

*/
/*
   //USB Camera 

    vector<int> capture_index = { 0, 1 }; //USB Camera indices
    vector<string> label; //Highgui window titles

    for (int i = 0; i < capture_index.size(); i++)
    {
        string title = "CCTV " + to_string(i);
        label.push_back(title);
    }

    //Make an instance of CameraStreamer
    CameraStreamer cam(capture_index);

    while (waitKey(20) != 27)
    {
        //Retrieve frames from each camera capture thread
        for (int i = 0; i < capture_index.size(); i++)
        {
            Mat frame;
            //Pop frame from queue and check if the frame is valid
            if (cam.frame_queue[i]->try_pop(frame))
            {
                //Show frame on Highgui window
                imshow(label[i], frame);
            }
        }
    }
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////
    //CSI and USB 

    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ;
    int flip_method = 0 ;

    std::string pipeline = gstreamer_pipeline(capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);

    vector<int> capture_index = { 0, 1 }; //USB Camera indices
    vector<string> label; //Highgui window titles

    for (int i = 0; i < capture_index.size(); i++)
    {
        string title = "CCTV " + to_string(i);
        label.push_back(title);
    }

    //Make an instance of CameraStreamer
    CameraStreamer cam(0); //CameraStreamer cam(capture_index);
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER); //CSI 

    if(!cap.isOpened()) {
        std::cout<<"Failed to open camera."<<std::endl;
        return (-1);
    }

    cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;

    std::cout << "Hit ESC to exit" << "\n" ;
    
    while(waitKey(20) != 27)
    {
    	if (!cap.read(img)) 
        {
            std::cout<<"Capture read error"<<std::endl;
            break;
	    }
        cv::imshow("CSI Camera",img);

        for (int i = 0; i < capture_index.size(); i++)
        {
            Mat frame;
            //Pop frame from queue and check if the frame is valid
            if (cam.frame_queue[i]->try_pop(frame))
            {
                //Show frame on Highgui window
                imshow(label[i], frame);
            }
        }
    }

    cap.release();
    cv::destroyAllWindows() ;
    return 0;

}