#include "configframe.hpp"
#include "PMU.hpp"
#include <algorithm>

ConfigFrame::ConfigFrame()
{
  // Bits 6-4 are updated as 011 
  Frame();
  SYNC &= ~0x70;
  SYNC |= 0x30; 
  uint16_t NUM_PMU = 0;
  uint16_t CONT_IDX = 0;
  PMUList = {};
}


ConfigFrame::ConfigFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint16_t cont_idx_value, 
    uint32_t time_base_value, uint16_t stream_data_rate_value):
    Frame(sync_value, framesize_value, stream_id_value, soc_value, leap_byte_value, fracsec_value, chk_value)
{

}

ConfigFrame::~ConfigFrame() {}




void ConfigFrame::setPMUList(const std::vector<std::shared_ptr<PMU>>& pmu_list)
{
  for (std::shared_ptr<PMU> pmu: pmu_list)
  {
    addPMU(pmu);
  }
}


void ConfigFrame::addPMU(const std::shared_ptr<PMU> pmu)
{
  if(pmu == nullptr) 
  {
    std::cout << "PMU UNAVAILABLE" << std::endl;
    return;
  }

  auto it = std::find(PMUList.begin(), PMUList.end(), pmu);
  if (it != PMUList.end()) 
  {
    std::cout << "PMU ALREADY IN THE SYSTEM" << std::endl;
    return;
  }

  PMUList.push_back(pmu);
  NUM_PMU++;
}


const std::shared_ptr<PMU> ConfigFrame::getPMU(int idx) const
{
  if(idx < 0 || idx >= NUM_PMU)
  {
    std::cout << "INDEX OUT OF RANGE" << std::endl;
  }
  return PMUList[idx];
}


uint16_t ConfigFrame::frameToBits(uint8_t* buffer)
{
  buffer += Frame::frameToBits(buffer);
  buffer += writeBuffer(buffer, getCONT_IDX());
  buffer += writeBuffer(buffer, getTIME_BASE());
  buffer += writeBuffer(buffer, getPDC_NAME());
  buffer += writeBuffer(buffer, getNUM_PMU());

  for (std::shared_ptr<PMU> pmu: getPMUList())
  {
    buffer += writeBuffer(buffer, pmu->getPMU_NAME());
    buffer += writeBuffer(buffer, pmu->getPMU_ID());
    buffer += writeBuffer(buffer, pmu->getPMU_VERSION());
    buffer += writeBuffer(buffer, pmu->getG_PMU_ID());
    buffer += writeBuffer(buffer, pmu->getFORMAT());
    buffer += writeBuffer(buffer, pmu->getPHNMR());
    buffer += writeBuffer(buffer, pmu->getANNMR());
    buffer += writeBuffer(buffer, pmu->getFRNMR());
    buffer += writeBuffer(buffer, pmu->getDFDTNMR());
    buffer += writeBuffer(buffer, pmu->getDGNMR());
    
    for(int i = 0; i < pmu->getPHNMR(); i++)
    {
			buffer += writeBuffer(buffer, pmu->getPHCHNAM()[i]);
		}

    for(int i = 0; i < pmu->getFRNMR(); i++)
    {
			buffer += writeBuffer(buffer, pmu->getFRCHNAM()[i]);
		}

    for(int i = 0; i < pmu->getDFDTNMR(); i++)
    {
			buffer += writeBuffer(buffer, pmu->getDFDTCHNAM()[i]);
		}


    for(int i = 0; i < pmu->getANNMR(); i++)
    {
			buffer += writeBuffer(buffer, pmu->getANSCALE()[i]);
		}

    for(int i = 0; i < pmu->getPHNMR(); i++)
    {
			buffer += writeBuffer(buffer, pmu->getPHSCALE()[i]);
		}

    for(int i = 0; i < pmu->getFRNMR(); i++)
    {
			buffer += writeBuffer(buffer, pmu->getFRSCALE()[i]);
		}

    for(int i = 0; i < pmu->getDFDTNMR(); i++)
    {
			buffer += writeBuffer(buffer, pmu->getDFDTSCALE()[i]);
		}

    for(int i = 0; i < pmu->getANNMR(); i++)
    {
			buffer += writeBuffer(buffer, pmu->getANSCALE()[i]);
		}

    for(int i = 0; i < pmu->getDGNMR(); i++)
    {
			buffer += writeBuffer(buffer, pmu->getDIGUNIT()[i]);
		}

    buffer += writeBuffer(buffer, pmu->getPMU_LAT());
    buffer += writeBuffer(buffer, pmu->getPMU_LON());
    buffer += writeBuffer(buffer, pmu->getPMU_ELEV());
    buffer += writeBuffer(buffer, pmu->getPMUFLAG());
    buffer += writeBuffer(buffer, pmu->getWINDOW());
    buffer += writeBuffer(buffer, pmu->getGRP_DLY());
    buffer += writeBuffer(buffer, pmu->getPMU_DATA_RATE());
    buffer += writeBuffer(buffer, pmu->getCFGCNT());
  } 
  buffer += getSTREAM_DATA_RATE();
  buffer += getWAIT_TIME();
  buffer += getCHK();

}


CapabilityFrame::CapabilityFrame()
{
  ConfigFrame();
  SYNC &= ~0x70;
  SYNC |= 0x20; 
}


CapabilityFrame::CapabilityFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint16_t cont_idx_value, 
    uint32_t time_base_value, uint16_t stream_data_rate_value): 
    ConfigFrame(sync_value, framesize_value, stream_id_value, soc_value, leap_byte_value, fracsec_value, chk_value, 
    cont_idx_value, time_base_value, stream_data_rate_value)
{

}


