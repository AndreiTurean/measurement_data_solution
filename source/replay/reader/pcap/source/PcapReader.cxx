#include "pch.h"
#include <pcap/PcapReader.hpp>
#include <Packet.h>
#include <PcapLiveDevice.h>

namespace replay
{
    namespace pcap
    {
        PcapReader::PcapReader(InterfaceAccess* interfaceAccess)
        {
            interfaceAccess_ = interfaceAccess;
            extension_ = ".pcap";
        }
        PcapReader::~PcapReader()
        {

        }

        //! ReaderIfc interface implementation
        uint64_t PcapReader::getCurrentPosition()
        {
            return 0;
        }
        uint64_t PcapReader::getEndPosition()
        {
            return -1;
        }
        void PcapReader::setPositon(uint64_t)
        {

        }
        DataPackagePtr PcapReader::readData()
        {
            DataPackagePtr pkg = nullptr;

            pcpp::RawPacket packet;
            if(reader_->getNextPacket(packet))
            {
                pkg = std::make_shared<DataPackage>();
                pkg->timestamp = packet.getPacketTimeStamp().tv_sec * 1000000 + packet.getPacketTimeStamp().tv_nsec;
                pkg->size = packet.getRawDataLen();
                pkg->sourceHandle = 0XEAA8;
                pkg->cycle = 0;
                pkg->id = (uint16_t)packet.getLinkLayerType();
                pkg->type = PackageType::ethernet;
                pkg->payload = new uint8_t[pkg->size];
                std::memcpy(pkg->payload, packet.getRawData(), packet.getRawDataLen());
            }

            return pkg;
        }

        bool PcapReader::openFile(const std::string& fileName)
        {
            reader_ = pcpp::IFileReaderDevice::getReader(fileName);

            if(reader_ == nullptr)
            {
                return false;
            }


            return reader_->open();
        }

        void PcapReader::closeFile()
        {
            reader_->close();
        }

        const std::string& PcapReader::getExtension()
        {
            return extension_;
        }

        //! InterfaceAccess interface implementation
        void* PcapReader::getInterface(const std::string&)
        {
            return nullptr;
        }

        //! GuiControlIfc interface implementation
        void PcapReader::show(ImGuiContext* ctx)
        {
            ImGui::SetCurrentContext(ctx);
        }
    }
}