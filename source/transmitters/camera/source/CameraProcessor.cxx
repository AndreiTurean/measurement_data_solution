#include <transmitters/CameraProcessor.hpp>

namespace transmitters
{
    CameraProcessor::CameraProcessor()
    {
        this->caAPI_ = CameraAquisition();
    }
    DataPackageCPtr CameraProcessor::getPackage()
    {
        VideoHeader* hdr = getRawPackage();
        DataPackagePtr pkg = std::make_shared<DataPackage>();
        pkg->cycle_ = 0;
        pkg->size = hdr->sizeX * hdr->sizeY;
        pkg->timestamp = 0;
        pkg->type = PackageType::camera;
        pkg->payload = hdr->data;
        return pkg;
    }

    VideoHeader* CameraProcessor::getRawPackage()
    {
        cv::Mat rawImage = caAPI_.getImage();

        VideoHeader* hdr = new VideoHeader;
        hdr->sizeX = rawImage.size().height;
        hdr->sizeY = rawImage.size().width;
        hdr->data = rawImage.data;
        return hdr;
    }
}