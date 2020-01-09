// simple_camera.cpp
// MIT License
// Copyright (c) 2019 JetsonHacks
// See LICENSE for OpenCV license and additional information
// Using a CSI camera (such as the Raspberry Pi Version 2) connected to a 
// NVIDIA Jetson Nano Developer Kit using OpenCV
// Drivers for the camera and OpenCV are included in the base image

#include <iostream>
#include <opencv2/opencv.hpp>
// #include <opencv2/videoio.hpp>
// #include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


class TickMeter
{
    public:
        TickMeter();
        void start();
        void stop();

        int64 getTimeTicks() const;
        double getTimeMicro() const;
        double getTimeMilli() const;
        double getTimeSec()   const;
        int64 getCounter() const;

        void reset();
    private:
        int64 counter;
        int64 sumTime;
        int64 startTime;
};

TickMeter::TickMeter()
{
    reset();
}
int64 TickMeter::getTimeTicks() const
{
    return sumTime;
}
double TickMeter::getTimeMicro() const
{
    return  getTimeMilli()*1e3;
}
double TickMeter::getTimeMilli() const
{
    return getTimeSec()*1e3;
}
double TickMeter::getTimeSec() const
{
    return (double)getTimeTicks()/cv::getTickFrequency();
}
int64 TickMeter::getCounter() const
{
    return counter;
}
void TickMeter::reset()
{
    startTime = 0;
    sumTime = 0;
    counter = 0;
}

void TickMeter::start()
{
    startTime = cv::getTickCount();
}
void TickMeter::stop()
{
    int64 time = cv::getTickCount();
    if ( startTime == 0 )
        return;
    ++counter;
    sumTime += ( time - startTime );
    startTime = 0;
}

std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

int main()
{
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

    std::cout << "Using pipeline: \n\t" << pipeline << "\n";
 
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if(!cap.isOpened()) {
        std::cout<<"Failed to open camera."<<std::endl;
        return (-1);
    }

    cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;
    int c;
    TickMeter tm;

    std::cout << "Hit ESC to exit" << "\n" ;
    
    while(true)
    {
        tm.stop();
        tm.start();
    	if (!cap.read(img)) {
            std::cout<<"Capture read error"<<std::endl;
            break;
	    }
	
        if(tm.getCounter() > 0)
        {
            cv::putText(frame, cv::format("Average FPS=%d",cvRound( tm.getCounter()/tm.getTimeSec())), cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));
        }

        cv::imshow("CSI Camera",img);
        int keycode = cv::waitKey(30) & 0xff ; 

        if (keycode == 27) break ;
    }

    cap.release();
    cv::destroyAllWindows() ;
    return 0;

    ////////////////////////////////////////////////////////////////////////////////////////
/*
    cv::Mat frame;
    int c;
    TickMeter tm;

    do
    {
        tm.stop();
        tm.start();
        capture.read(frame);

        if( frame.empty() )
        {
            break;
        }

        if(tm.getCounter() > 0)
        {
            cv::putText(frame, cv::format("Average FPS=%d",cvRound( tm.getCounter()/tm.getTimeSec())), cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));
        }

        cv::imshow("Video Capture Test", frame);

        c = cv::waitKey(1);
    }
    while(c != 27);
*/

}


