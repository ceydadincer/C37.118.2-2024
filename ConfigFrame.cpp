#include "configframe.hpp"
#include "pmu.hpp"
#include <algorithm>

// CapabilityConfigFrame class

// default constructor adjusts the SYNC default value for CapabilityConfigFrame 
CapabilityConfigFrame::CapabilityConfigFrame(): Frame()
{
  // bits 6-4 for the SYNC field of CapabilityConfigFrame are 010
  SYNC |= 0x20; 
}

// set attributes PDC_NAME and PDC_NAME_length for the PDC_NAME fields
void CapabilityConfigFrame::setPDC_NAME(const std::string& pdc_name_value)
{
  PDC_NAME = pdc_name_value;
  PDC_NAME_length = pdc_name_value.size();
}

// set attributes PMUList and NUM_PMU using its size
void CapabilityConfigFrame::setPMUList(const std::vector<std::shared_ptr<PMU>>& pmu_list)
{
  PMUList = pmu_list;
  NUM_PMU = pmu_list.size();
}

// computes FRAMESIZE by adding the sizes of the CapabilityConfigFrame specific fields to those of common fields  
uint16_t CapabilityConfigFrame::computeFRAMESIZE() const
{
  // first total size of non-PMU-specific fields (1 for PDC_NAME_length bit)
  uint16_t counter = Frame::computeFRAMESIZE() + 2 + 4 + 1 + getPDC_NAME_length() + 2 + 2 + 2;

  for (const std::shared_ptr<PMU>& pmu: getPMUList())
  {
    // second total size of PMU-specific fields (fixed sized ones are added first)
    counter += 1 + pmu->getPMU_NAME_length() + 2 + 2 + 16 + 2 + 2 + 2 + 2 + 2 + 2 + 4 + 4 + 4 + 2 + 4 + 4 + 2 + 2;
    // iterate over the channel vector and add the CHNAM and SCALE size contribution of each channel
    // CHNAM of each channel is a string and therefore its size differs, we add them one by one
    for (uint8_t chnam_length : pmu->getPHCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
      counter += 16;
    }

    for (uint8_t chnam_length: pmu->getFRCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
      counter += 8;
    }

    for (uint8_t chnam_length : pmu->getDFDTCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
      counter += 8;
    }

    for (uint8_t chnam_length : pmu->getANCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
      counter += 8;
    }
    
    for (uint8_t chnam_length: pmu->getDGCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
      counter += 4;
    }
  }
  return counter;
}

// overridden method for writing a frame into buffer 
uint16_t CapabilityConfigFrame::frameToBits(uint8_t* buffer)
{
  // write common fields and advance the buffer pointer
  buffer += Frame::frameToBits(buffer);
  buffer += writeBuffer(buffer, getCONT_IDX());
  buffer += writeBuffer(buffer, getTIME_BASE());
  buffer += writeBuffer(buffer, getPDC_NAME_length());
  buffer += writeBuffer(buffer, getPDC_NAME(), getPDC_NAME_length());
  buffer += writeBuffer(buffer, getNUM_PMU());

  for (const std::shared_ptr<PMU>& pmu: getPMUList())
  {
    buffer += writeBuffer(buffer, pmu->getPMU_NAME_length());
    buffer += writeBuffer(buffer, pmu->getPMU_NAME(), pmu->getPMU_NAME_length());
    buffer += writeBuffer(buffer, pmu->getPMU_ID());
    buffer += writeBuffer(buffer, pmu->getPMU_VERSION());
    buffer += writeBuffer(buffer, pmu->getG_PMU_ID());
    buffer += writeBuffer(buffer, pmu->getFORMAT());
    buffer += writeBuffer(buffer, pmu->getPHNMR());
    buffer += writeBuffer(buffer, pmu->getANNMR());
    buffer += writeBuffer(buffer, pmu->getFRNMR());
    buffer += writeBuffer(buffer, pmu->getDFDTNMR());
    buffer += writeBuffer(buffer, pmu->getDGNMR());
    
    for (int i = 0; i < pmu->getPHNMR(); i++)
    {
      buffer += writeBuffer(buffer, pmu->getPHCHNAM_lengths()[i]);
      buffer += writeBuffer(buffer, pmu->getPHCHNAM()[i], pmu->getPHCHNAM_lengths()[i]);
    }

    for (int i = 0; i < pmu->getFRNMR(); i++)
    {
      buffer += writeBuffer(buffer, pmu->getFRCHNAM_lengths()[i]);
      buffer += writeBuffer(buffer, pmu->getFRCHNAM()[i], pmu->getFRCHNAM_lengths()[i]);
    }

    for (int i = 0; i < pmu->getDFDTNMR(); i++)
    {
      buffer += writeBuffer(buffer, pmu->getDFDTCHNAM_lengths()[i]);
      buffer += writeBuffer(buffer, pmu->getDFDTCHNAM()[i], pmu->getDFDTCHNAM_lengths()[i]);
    }

    for (int i = 0; i < pmu->getANNMR(); i++)
    {
      buffer += writeBuffer(buffer, pmu->getANCHNAM_lengths()[i]);
      buffer += writeBuffer(buffer, pmu->getANCHNAM()[i], pmu->getANCHNAM_lengths()[i]);
    }

    for (int i = 0; i < pmu->getDGNMR(); i++)
    {
      buffer += writeBuffer(buffer, pmu->getDGCHNAM_lengths()[i]);
      buffer += writeBuffer(buffer, pmu->getDGCHNAM()[i], pmu->getDGCHNAM_lengths()[i]);
    }

    for (int i = 0; i < pmu->getPHNMR(); i++)
    {
      buffer += writeBuffer(buffer, pmu->getPHSCALE()[i]);
    }

    for (int i = 0; i < pmu->getFRNMR(); i++)
    {
      buffer += writeBuffer(buffer, pmu->getFRSCALE()[i]);
    }

    for (int i = 0; i < pmu->getDFDTNMR(); i++)
    {
      buffer += writeBuffer(buffer, pmu->getDFDTSCALE()[i]);
    }

    for (int i = 0; i < pmu->getANNMR(); i++)
    {
      buffer += writeBuffer(buffer, pmu->getANSCALE()[i]);
    }

    for (int i = 0; i < pmu->getDGNMR(); i++)
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

  buffer += writeBuffer(buffer, getSTREAM_DATA_RATE());
  buffer += writeBuffer(buffer, getWAIT_TIME());
  // compute CRC and set CHK before writing it to buffer
  setCHK(computeCRC(buffer, getFRAMESIZE()-2));
  buffer += writeBuffer(buffer, getCHK());
  // return the size of the written bytes
  return getFRAMESIZE();
}

// overridden method for reading a frame from buffer 
uint16_t CapabilityConfigFrame::bitsToFrame(uint8_t* buffer) 
{
  // read common fields and advance the buffer pointer
  buffer += Frame::bitsToFrame(buffer); 
  
  // placeholder variables to read the buffer into
  uint8_t var1byte;
  uint16_t var2byte;
  uint32_t var4byte;
  uint64_t var8byte;
  std::string var_string;

  buffer += readBuffer(buffer, var2byte);
  setCONT_IDX(var2byte);
  buffer += readBuffer(buffer, var4byte);
  setTIME_BASE(var4byte);
  // reading of names is done in two steps
  buffer += readBuffer(buffer, var1byte);
  buffer += readBuffer(buffer, var_string, var1byte);
  setPDC_NAME(var_string);
  buffer += readBuffer(buffer, var2byte);
  setNUM_PMU(var2byte);  

  std::vector<std::shared_ptr<PMU>> pmu_list (getNUM_PMU());
  for (std::shared_ptr<PMU>& pmu: pmu_list)
  {
    pmu = std::make_shared<PMU>();
    buffer += readBuffer(buffer, var1byte); 
    buffer += readBuffer(buffer, var_string, var1byte);  
    pmu->setPMU_NAME(var_string);
    buffer += readBuffer(buffer, var2byte);
    pmu->setPMU_ID(var2byte);
    buffer += readBuffer(buffer, var2byte);
    pmu->setPMU_VERSION(var2byte);
    std::array<uint8_t, 16> g_pmu_id;
    buffer += readBuffer(buffer, g_pmu_id);
    pmu->setG_PMU_ID(g_pmu_id);
    buffer += readBuffer(buffer, var2byte);
    pmu->setFORMAT(var2byte);
    
    // need to be defined to be used later
    uint16_t phnmr;
    buffer += readBuffer(buffer, phnmr);
    uint16_t annmr;
    buffer += readBuffer(buffer, annmr);
    uint16_t frnmr;
    buffer += readBuffer(buffer, frnmr);
    uint16_t dfdtnmr;
    buffer += readBuffer(buffer, dfdtnmr);
    uint16_t dgnmr;
    buffer += readBuffer(buffer, dgnmr);

    // placeholder variable for chnam_length
    uint8_t length;
    std::vector<std::string> phchnam (phnmr);
    for (int i = 0; i < phnmr; i++)
    {
      buffer += readBuffer(buffer, length);
      buffer += readBuffer(buffer, phchnam[i], length);
    }
    
    std::vector<std::string> frchnam (frnmr);
    for (int i = 0; i < frnmr; i++)
    {
      buffer += readBuffer(buffer, length);
      buffer += readBuffer(buffer, frchnam[i], length);
    }

    std::vector<std::string> dfdtchnam (dfdtnmr);
    for (int i = 0; i < dfdtnmr; i++)
    {
      buffer += readBuffer(buffer, length);
      buffer += readBuffer(buffer, dfdtchnam[i], length);
    }

    std::vector<std::string> anchnam (annmr);
    for (int i = 0; i < annmr; i++)
    {
      buffer += readBuffer(buffer, length);
      buffer += readBuffer(buffer, anchnam[i], length);
    }
    
    std::vector<std::string> dgchnam (dgnmr);
    for (int i = 0; i < dgnmr; i++)
    {
      buffer += readBuffer(buffer, length);
      buffer += readBuffer(buffer, dgchnam[i], length);
    }

    std::vector<std::array<uint8_t, 16>> phscale (phnmr);
    for (int i = 0; i < phnmr; i++)
    {
      buffer += readBuffer(buffer, phscale[i]);
    }
    
    std::vector<uint64_t> frscale (frnmr);
    for (int i = 0; i < frnmr; i++)
    {
      buffer += readBuffer(buffer, frscale[i]);
    }

    std::vector<uint64_t> dfdtscale (dfdtnmr);
    for (int i = 0; i < dfdtnmr; i++)
    {
      buffer += readBuffer(buffer, dfdtscale[i]);
    }
    
    std::vector<uint64_t> anscale (annmr);
    for (int i = 0; i < annmr; i++)
    {
      buffer += readBuffer(buffer, anscale[i]);
    }
    
    std::vector<uint32_t> digunit (dgnmr);
    for (int i = 0; i < dgnmr; i++)
    {
      buffer += readBuffer(buffer, digunit[i]);
    }

    pmu->setChannelPhasor(phchnam, phscale);
    pmu->setChannelFrequency(frchnam, frscale);
    pmu->setChannelDFDT(dfdtchnam, dfdtscale);
    pmu->setChannelAnalog(anchnam, anscale);
    pmu->setChannelDigital(dgchnam, digunit);
  
    buffer += readBuffer(buffer, var4byte);
    pmu->setPMU_LAT(var4byte);
    buffer += readBuffer(buffer, var4byte);
    pmu->setPMU_LON(var4byte);
    buffer += readBuffer(buffer, var4byte);
    pmu->setPMU_ELEV(var4byte);  
    buffer += readBuffer(buffer, var2byte);
    pmu->setPMUFLAG(var2byte);
    buffer += readBuffer(buffer, var4byte);
    pmu->setWINDOW(var4byte);
    buffer += readBuffer(buffer, var4byte);
    pmu->setGRP_DLY(var4byte);
    buffer += readBuffer(buffer, var2byte);
    pmu->setPMU_DATA_RATE(var2byte);
    buffer += readBuffer(buffer, var2byte);
    pmu->setCFGCNT(var2byte);
  }
  // set the PMUList as attribute
  setPMUList(pmu_list);
  buffer += readBuffer(buffer, var2byte);
  setSTREAM_DATA_RATE(var2byte);
  buffer += readBuffer(buffer, var2byte);
  setWAIT_TIME(var2byte);  
  buffer += readBuffer(buffer, var2byte);
  setCHK(var2byte);
  // return the size of the read bytes
  return getFRAMESIZE(); 
}

// StreamConfigFrame class

// default constructor adjusts the SYNC default value for StreamConfigFrame 
StreamConfigFrame::StreamConfigFrame(): CapabilityConfigFrame()
{
  // bits 6-4 for the SYNC field of StreamConfigFrame are 011
  SYNC |= 0x30; 
}

// constructor adjusts the SYNC default value for StreamConfigFrame and sets some atttributes using CapabilityConfigFrame
StreamConfigFrame::StreamConfigFrame(std::shared_ptr<CapabilityConfigFrame> capability_config_frame): CapabilityConfigFrame()
{
  // bits 6-4 for the SYNC field of StreamConfigFrame are 011
  SYNC |= 0x30; 
  setSTREAM_ID(capability_config_frame->getSTREAM_ID());
  setCONT_IDX(capability_config_frame->getCONT_IDX());
  setTIME_BASE(capability_config_frame->getTIME_BASE());
  setPDC_NAME(capability_config_frame->getPDC_NAME());
  setNUM_PMU(capability_config_frame->getNUM_PMU());
  setSTREAM_DATA_RATE(capability_config_frame->getSTREAM_DATA_RATE());
  setWAIT_TIME(capability_config_frame->getWAIT_TIME());
  setPMUList(capability_config_frame->getPMUList());
}


