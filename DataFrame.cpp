#include "dataframe.hpp"
#include "pmu.hpp"

// DataFrame class

// constructor adjusts the SYNC default value for DataFrame (not needed for this frame) and set its CapabilityConfigFrame
DataFrame::DataFrame(std::shared_ptr<CapabilityConfigFrame>& config_frame_value):Frame()
{
  // bits 6-4 for the SYNC field of DataFrame are 000 (don't need to be changed)
  configFrame = config_frame_value;
  setSTREAM_ID(configFrame->getSTREAM_ID());
}

// computes FRAMESIZE by adding the sizes of the DataFrame specific fields to those of common fields  
uint16_t DataFrame::computeFRAMESIZE() const
{
  // first total size of non-PMU-specific fields
  uint16_t counter = Frame::computeFRAMESIZE();

  uint16_t field_size;
  for (const std::shared_ptr<PMU>& pmu: configFrame->getPMUList()) 
  {
    // second total size of PMU-specific fields
    counter += 2+2;

    // size of the fields for measurements depend on the format
    // Bit 1: PHASOR format
    if (pmu->getFORMAT() & 0x02){field_size = 8;}
    else {field_size = 4;}
    counter += field_size*pmu->getPHNMR(); 

    // Bit 2: ANALOG format
    if (pmu->getFORMAT() & 0x04){field_size = 4;}
    else {field_size = 2;}
    counter += field_size*pmu->getANNMR();

    // Bit 3: FREQ and DFREQ format
    if (pmu->getFORMAT() & 0x08){field_size = 4;}
    else {field_size = 2;}
    counter += field_size*pmu->getFRNMR();
    counter += field_size*pmu->getDFDTNMR();

    counter += 2*pmu->getDGNMR();
  }
  return counter;
}

// overridden method for writing a frame into buffer 
uint16_t DataFrame::frameToBits(uint8_t* buffer)
{
  // write common fields and advance the buffer pointer
  buffer += Frame::frameToBits(buffer);
  
  for (const std::shared_ptr<PMU>& pmu: configFrame->getPMUList())
  {
    buffer += writeBuffer(buffer, pmu->getSTAT_FLAG());
    buffer += writeBuffer(buffer, pmu->getTIMEQUALITY());

    for (uint64_t phasor : pmu->getPHASOR_values()) 
    {
      // Bit 1: PHASOR format: If 0, convert the data type to uint32_t before passing to writeBuffer
      if (!(pmu->getFORMAT() & 0x02)) {phasor = static_cast<uint32_t>(phasor);}
      buffer += writeBuffer(buffer, phasor);
    }
    
    for (uint32_t freq : pmu->getFREQ_values()) 
    {
      // Bit 3: FREQ and DFREQ format: If 0, convert the data type to uint16_t before passing to writeBuffer
      if (!(pmu->getFORMAT() & 0x08)) {freq = static_cast<uint16_t>(freq);}
      buffer += writeBuffer(buffer, freq);
    }

    for (uint32_t dfreq : pmu->getDFREQ_values()) 
    {    
      // Bit 3: FREQ and DFREQ format: If 0, convert the data type to uint16_t before passing to writeBuffer
      if (!(pmu->getFORMAT() & 0x08)) {dfreq = static_cast<uint16_t>(dfreq);}
      buffer += writeBuffer(buffer, dfreq);
    }

    for (uint32_t analog : pmu->getANALOG_values()) 
    {    
      // Bit 2: ANALOG format: If 0, convert the data type to uint16_t before passing to writeBuffer
      if (!(pmu->getFORMAT() & 0x08)) {analog = static_cast<uint16_t>(analog);}
      buffer += writeBuffer(buffer, analog);
    }

    for (uint16_t digital : pmu->getDIGITAL_values()) 
    {
        buffer += writeBuffer(buffer, digital);
    }
  }
  // compute CRC and set CHK before writing it to buffer
  setCHK(computeCRC(buffer, getFRAMESIZE()-2));
  buffer += writeBuffer(buffer, getCHK());
  // return the size of the written bytes
  return getFRAMESIZE();
}

// overridden method for reading a frame from buffer
// Note: this method is unfortunately very messy and redundant, it can be improved by implementing a logic to add channel measurements one by one
// and not set it as a vector and/or changing readBuffer logic. 
uint16_t DataFrame::bitsToFrame(uint8_t* buffer)
{
  // read common fields and advance the buffer pointer
  buffer += Frame::bitsToFrame(buffer);

  //helper local variables
  uint16_t value2byte;
  uint32_t value4byte;
  uint64_t value8byte;
  std::vector<uint16_t> values2byte;
  std::vector<uint32_t> values4byte;
  std::vector<uint64_t> values8byte;

  for (const std::shared_ptr<PMU>& pmu: configFrame->getPMUList())
  {
    values8byte.clear();
    for (int i = 0; i < pmu->getPHNMR(); i++) 
    {
      if (!(pmu->getFORMAT() & 0x02)) 
      {
        // Bit 1: PHASOR format: If 0, the data is read into a variable of 4 bits then casted to 8 bits before adding it to the vector
        buffer += readBuffer(buffer, value4byte);
        values8byte.push_back(static_cast<uint64_t>(value4byte));
      }
      else
      {
        // Bit 1: PHASOR format: If 1, the data is directly read and saved
        buffer += readBuffer(buffer, value8byte);
        values8byte.push_back(value8byte);
      }
    }
    pmu->setPHASOR_values(values8byte);

    values4byte.clear();
    for (int i = 0; i < pmu->getFRNMR(); i++) 
    {
      if (!(pmu->getFORMAT() & 0x08)) 
      {
        // Bit 2: FREQ/DFREQ format: If 0, the data is read into a variable of 2 bits then casted to 4 bits before adding it to the vector
        buffer += readBuffer(buffer, value2byte);
        values4byte.push_back(static_cast<uint32_t>(value2byte));
      }
      else
      {
        // Bit 2: FREQ/DFREQ format: If 1, the data is directly read and saved
        buffer += readBuffer(buffer, value4byte);
        values4byte.push_back(value4byte);
      }
    }
    pmu->setFREQ_values(values4byte);
    
    values4byte.clear();
    for (int i = 0; i < pmu->getDFDTNMR(); i++) 
    {
      if (!(pmu->getFORMAT() & 0x08)) 
      {
        // Bit 2: FREQ/DFREQ format: If 0, the data is read into a variable of 2 bits then casted to 4 bits before adding it to the vector
        buffer += readBuffer(buffer, value2byte);
        values4byte.push_back(static_cast<uint32_t>(value2byte));
      }
      else
      {
        // Bit 2: FREQ/DFREQ format: If 1, the data is directly read and saved
        buffer += readBuffer(buffer, value4byte);
        values4byte.push_back(value4byte);
      }
    }
    pmu->setDFREQ_values(values4byte);

    values4byte.clear();
    for (int i = 0; i < pmu->getANNMR(); i++) 
    {
      if (!(pmu->getFORMAT() & 0x04)) 
      {
        // Bit 3: ANALOG format: If 0, the data is read into a variable of 2 bits then casted to 4 bits before adding it to the vector
        buffer += readBuffer(buffer, value2byte);
        values4byte.push_back(static_cast<uint32_t>(value2byte));
      }
      else
      {
        // Bit 3: ANALOG format: If 1, the data is directly read and saved
        buffer += readBuffer(buffer, value4byte);
        values4byte.push_back(value4byte);
      }
    }
    pmu->setANALOG_values(values4byte);
    
    values2byte.clear();
    for (int i = 0; i < pmu->getDGNMR(); i++) 
    {
      buffer += readBuffer(buffer, value2byte);
      values2byte.push_back(value2byte);
    }
    pmu->setDIGITAL_values(values2byte);
  }
  uint16_t chk;
  buffer += readBuffer(buffer, chk);
  setCHK(chk);
  // return the size of the read bytes
  return getFRAMESIZE(); 
}

// DiscreteDataFrame class

// constructor adjusts the SYNC default value for DiscreteDataFrame and call the base constructor
DiscreteDataFrame::DiscreteDataFrame(std::shared_ptr<CapabilityConfigFrame>& config_frame_value): DataFrame(config_frame_value)
{
  // bits 6-4 for the SYNC field of DiscreteDataFrame are 001
  SYNC |= 0x10; 
}

// computes FRAMESIZE by adding the sizes of the DiscreteDataFrame specific fields to those of common fields  
uint16_t DiscreteDataFrame::computeFRAMESIZE() const
{
  // first total size of non-PMU-specific fields
  uint16_t counter = Frame::computeFRAMESIZE();
  
  // second total size of PMU-specific fields
  for (const std::shared_ptr<PMU>& pmu: configFrame->getPMUList()) 
  {
    counter += 2*pmu->getDGNMR();
  }

  return counter;
}

// overridden method for writing a frame into buffer 
uint16_t DiscreteDataFrame::frameToBits(uint8_t* buffer)
{
  // write common fields and advance the buffer pointer
  buffer += Frame::frameToBits(buffer);

  for (const std::shared_ptr<PMU>& pmu: configFrame->getPMUList())
  {
    for (uint16_t digital : pmu->getDIGITAL_values()) 
    {
      buffer += writeBuffer(buffer, digital);
    }
  }
  // compute CRC and set CHK before writing it to buffer
  setCHK(computeCRC(buffer, getFRAMESIZE()-2));
  buffer += writeBuffer(buffer, getCHK());
  // return the size of the written bytes
  return getFRAMESIZE();
}

// overridden method for reading a frame from buffer 
// for data frames, it's assumed that the PMUList of the frame has already been set as attribute
uint16_t DiscreteDataFrame::bitsToFrame(uint8_t* buffer)
{
  // read common fields and advance the buffer pointer
  buffer += Frame::bitsToFrame(buffer);
  uint16_t digital_value;
  std::vector<uint16_t> digital_values;
  for (const std::shared_ptr<PMU>& pmu: configFrame->getPMUList())
  {
    for (int i = 0; i < pmu->getDGNMR(); i++) 
    {
      buffer += readBuffer(buffer, digital_value);
      digital_values.push_back(digital_value);
    }
    pmu->setDIGITAL_values(digital_values);
  }
  uint16_t chk;
  buffer += readBuffer(buffer, chk);
  setCHK(chk);
  // return the size of the read bytes
  return getFRAMESIZE(); 
}