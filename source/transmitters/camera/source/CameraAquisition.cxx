#include <transmitters/CameraAquisition.hpp>

namespace transmitters
{
    CameraAquisition::CameraAquisition(int, int)
    {
        captionApi_ = cv::VideoCapture(0);
    } 

    cv::Mat CameraAquisition::getImage()
    {
        cv::Mat image;
        if(captionApi_.isOpened())
        {
            
            captionApi_ >> image;
        }

        return image;
    }
}