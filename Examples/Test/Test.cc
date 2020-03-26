
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <opencv2/core/core.hpp>

#include "System.h"

#include "Frame.h"

#include "Tracking.h"

using namespace std;

int main(int argc, char **argv)
{
    double ts1 = 1584792783.768187;
    double ts2 = 1584792968.749244;

    string baseframe = "./1584792783.768187.png";
    string destframe = "./1584792968.749244.png";

    // Main loop
    cv::Mat im1;
    cv::Mat im2;

    im1 = cv::imread(baseframe, CV_LOAD_IMAGE_UNCHANGED);
    im2 = cv::imread(destframe, CV_LOAD_IMAGE_UNCHANGED);

    // cv::imshow("1",im1);
    // cv::imshow("2",im2);

    ORB_SLAM2::System SLAM(argv[1], argv[2], ORB_SLAM2::System::MONOCULAR, true);

    SLAM.CompareTwoFrame(im1, ts1, im2, ts2);
    SLAM.Shutdown();
    //mpTracker->GrabImageMonocular(im, timestamp);
    return 0;
}