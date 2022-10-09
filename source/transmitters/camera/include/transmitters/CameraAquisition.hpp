#pragma once
#include <cstdint>
#include <opencv2/opencv.hpp>

namespace transmitters
{
    class CameraAquisition
    {
        cv::VideoCapture captionApi_;
    public:
        CameraAquisition(int instanceNb = 0, int apiPreference = 0);

        cv::Mat getImage();
    };
}