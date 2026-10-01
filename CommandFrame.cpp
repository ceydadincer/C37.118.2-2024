#include "commandframe.hpp"
#include "pmu.hpp"

// CommandFrame class

// default constructor adjusts the SYNC default value for CommandFrame 
CommandFrame::CommandFrame(): Frame()
{
  // bits 6-4 for the SYNC field of CommandFrame are 100
  SYNC |= 0x40; 
}

// constructor with parameters calls the setters with given parameters
CommandFrame::CommandFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
                           uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t cmd_value, 
                           std::vector<uint8_t>& extframe_value, uint16_t chk_value):
                           Frame(sync_value, framesize_value, stream_id_value, soc_value, leap_byte_value, 
                                 fracsec_value, chk_value)
{
  setCMD(cmd_value);
  setEXTFRAME(extframe_value);
}

// overridden method for writing a frame into buffer 
uint16_t CommandFrame::frameToBits(uint8_t* buffer)
{
  // write common fields and advance the buffer pointer  
  buffer += Frame::frameToBits(buffer);
  buffer += writeBuffer(buffer, getCMD());
  for (uint8_t byte: getEXTFRAME())
  {
    buffer += writeBuffer(buffer, byte);
  }
  // compute CRC and set CHK before writing it to buffer
  setCHK(computeCRC(buffer, getFRAMESIZE()-2));
  buffer += writeBuffer(buffer, getCHK());
  // return the size of the written bytes
  return getFRAMESIZE();
}

// overridden method for reading a frame from buffer 
uint16_t CommandFrame::bitsToFrame(uint8_t* buffer)
{
  // read common fields and advance the buffer pointer
  buffer += Frame::bitsToFrame(buffer);
  uint16_t cmd;
  buffer += readBuffer(buffer, cmd);
  setCMD(cmd);
  uint16_t field_size = getFRAMESIZE() - Frame::computeFRAMESIZE() - 2;
  std::vector<uint8_t> extframe(field_size);
  for (int i = 0; i < field_size; i++)
  {
    buffer += readBuffer(buffer, extframe[i]);
  }
  setEXTFRAME(extframe);
  uint16_t chk;
  buffer += readBuffer(buffer, chk);
  setCHK(chk);
  // return the size of the read bytes
  return getFRAMESIZE(); 
}

// ConfigRenameCommandFrame class

/* 
// constructor adjusts the SYNC default value for ConfigRenameCommandFrame 
ConfigRenameCommandFrame::ConfigRenameCommandFrame(): Frame()
{
  // bits 6-4 for the SYNC field of ConfigRenameCommandFrame are 101
  SYNC |= 0x50; 
}

// set attributes PDC_NAME and PDC_NAME_length for the PDC_NAME fields
void ConfigRenameCommandFrame::setPDC_NAME(const std::string& pdc_name_value)
{
  PDC_NAME = pdc_name_value;
  PDC_NAME_length = pdc_name_value.size();
}

// set attributes PMUList and NUM_PMU using its size
void ConfigRenameCommandFrame::setPMUList(const std::vector<std::shared_ptr<PMU>>& pmu_list)
{
  PMUList = pmu_list;
  NUM_PMU = pmu_list.size();
}

// computes FRAMESIZE by adding the sizes of the ConfigRenameCommandFrame specific fields to those of common fields  
uint16_t ConfigRenameCommandFrame::computeFRAMESIZE() const
{
  // first total size of non-PMU-specific fields (1 for PDC_NAME_length bit)
  uint16_t counter = Frame::getFRAMESIZE() + 2 + 1 + getPDC_NAME_length() + 2;

  for (const std::shared_ptr<PMU>& pmu: getPMUList())
  {
    // second total size of PMU-specific fields (fixed sized ones are added first)
    counter += 1 + pmu->getPMU_NAME_length() + 2 + 16 + 2 + 2 + 2 + 2 + 2 + 4 + 4 + 4;
    // iterate over the channel vector and add the CHNAM and SCALE size contribution of each channel
    // CHNAM of each channel is a string and therefore its size differs, we add them one by one
    for (uint8_t chnam_length : pmu->getPHCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
    }
    for (uint8_t chnam_length: pmu->getFRCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
    }
    for (uint8_t chnam_length : pmu->getDFDTCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
    }
    for (uint8_t chnam_length : pmu->getANCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
    }
    for (uint8_t chnam_length: pmu->getDGCHNAM_lengths()) 
    {
      counter += 1 + chnam_length;
    }
  }
  return counter;
}

// overridden method for writing a frame into buffer 
uint16_t ConfigRenameCommandFrame::frameToBits(uint8_t* buffer)
{
  // write common fields and advance the buffer pointer
  buffer += Frame::frameToBits(buffer);
  buffer += writeBuffer(buffer, getCONT_IDX());
  buffer += writeBuffer(buffer, getPDC_NAME_length());
  buffer += writeBuffer(buffer, getPDC_NAME(), getPDC_NAME_length());
  buffer += writeBuffer(buffer, getNUM_PMU());

  for (const std::shared_ptr<PMU>& pmu: getPMUList())
  {
    buffer += writeBuffer(buffer, pmu->getPMU_NAME_length());
    buffer += writeBuffer(buffer, pmu->getPMU_NAME(), pmu->getPMU_NAME_length());
    buffer += writeBuffer(buffer, pmu->getPMU_ID());
    buffer += writeBuffer(buffer, pmu->getG_PMU_ID());
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
    buffer += writeBuffer(buffer, pmu->getPMU_LAT());
    buffer += writeBuffer(buffer, pmu->getPMU_LON());
    buffer += writeBuffer(buffer, pmu->getPMU_ELEV());
  }
  // compute CRC and set CHK before writing it to buffer 
  setCHK(computeCRC(buffer, getFRAMESIZE()-2));
  buffer += writeBuffer(buffer, getCHK());
  // return the size of the written bytes
  return getFRAMESIZE();
}
*/

// ConfigStreamCommandFrame class
/* 
// constructor adjusts the SYNC default value for ConfigStreamCommandFrame 
ConfigStreamCommandFrame::ConfigStreamCommandFrame(): Frame()
{
  // bits 6-4 for the SYNC field of ConfigStreamCommandFrame are 110
  SYNC |= 0x60; 
}

// set attributes PDC_NAME and PDC_NAME_length for the PDC_NAME fields
void ConfigStreamCommandFrame::setPDC_NAME(const std::string& pdc_name_value)
{
  PDC_NAME = pdc_name_value;
  PDC_NAME_length = pdc_name_value.size();
}

// set attributes PMUList and NUM_PMU using its size
void ConfigStreamCommandFrame::setPMUList(const std::vector<std::shared_ptr<PMU>>& pmu_list)
{
  PMUList = pmu_list;
  NUM_PMU = pmu_list.size();
}

// computes FRAMESIZE by adding the sizes of the ConfigStreamCommandFrame specific fields to those of common fields  
uint16_t ConfigStreamCommandFrame::computeFRAMESIZE() const
{
  // first total size of non-PMU-specific fields (1 for PDC_NAME_length bit)
  uint16_t counter = Frame::getFRAMESIZE() + 2 + 4 + 1 + getPDC_NAME_length() + 2 + 2 + 2;

  for (const std::shared_ptr<PMU>& pmu: getPMUList())
  {
    // second total size of PMU-specific fields (fixed sized ones are added first)
    counter += 1 + pmu->getPMU_NAME_length() + 2 + + 16 + 2 + 2 + 2 + 2 + 2 + 2 + 2 + 4 + 4 + 2 + 2;
    // iterate over the channel vector and add the CHNAM and SCALE size contribution of each channel
    for (uint8_t chnam_length : pmu->getPHCHNAM_lengths()) 
    {
      counter += chnam_length;
      counter += 16;
    }

    for (uint8_t chnam_length: pmu->getFRCHNAM_lengths()) 
    {
      counter += chnam_length;
      counter += 8;
    }

    for (uint8_t chnam_length : pmu->getDFDTCHNAM_lengths()) 
    {
      counter += chnam_length;
      counter += 8;
    }

    for (uint8_t chnam_length : pmu->getANCHNAM_lengths()) 
    {
      counter += chnam_length;
      counter += 8;
    }
    
    for (uint8_t chnam_length: pmu->getDGCHNAM_lengths()) 
    {
      counter += chnam_length;
      counter += 4;
    }
  }
  return counter;
}

// overridden method for writing a frame into buffer 
uint16_t ConfigStreamCommandFrame::frameToBits(uint8_t* buffer)
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
 */