#include "frame.hpp"
#include <iostream>
// #include <chrono>
// #include <ctime>
#include <cstring>

Frame::Frame() 
{
  // The first byte (15-8) of SYNC is initialized as the value defined in the standard (0xAA)
  // Bit 7 is set to 1 by default
  // Bits 6-4 are temporarily 0 to be defined in the constructors of the sub-classes
  // Bits 3-0 are set to 0011 by default for Version 3
  SYNC = (0xAA << 8) || 10000011;
}


Frame::Frame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, 
    uint32_t soc_value, uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value)
{
  setSYNC(sync_value);
  setFRAMESIZE(framesize_value); 
  setSTREAM_ID(stream_id_value);
  setSOC(soc_value);
  setLEAP_BYTE(leap_byte_value);
  setFRACSEC(fracsec_value);
  setCHK(chk_value);
}


Frame::~Frame() {}


void Frame::setSYNC(uint16_t sync_value)
{
  SYNC = sync_value;
}


//FRACSEC is allowed 24 bits only 
void Frame::setFRACSEC(uint32_t fracsec_value)
{
  if (fracsec_value > 0xFFFFFF)
  {
    std::cout << "VALUE FOR FRACSEC OUT OF RANGE" << std::endl;
    return;
  } 
  FRACSEC = fracsec_value;
}


// directly taken from the 2011 standard
uint16_t Frame::computeCRC(uint8_t* data, uint8_t dataLength)
{
  uint16_t crc = 0xFFFF;  
  uint16_t temp;
  uint16_t quick;
  uint8_t i;
  for (i = 0; i < dataLength; i++)
  {
    temp = (crc >> 8) ^ data[i];
    crc <<= 8;
    quick = temp ^ (temp >> 4);
    crc ^= quick;
    quick <<= 5;
    crc ^= quick;
    quick <<= 7;
    crc ^= quick;
  }
  return crc;
}


/*
void Frame::calculateTime()
{
  auto current = std::chrono::system_clock::now();
  std::time_t current_time = std::chrono::system_clock::to_time_t(current);
}
*/

// returns size so that the buffer can be incremented with the return value
uint8_t Frame::writeBuffer(uint8_t* buffer, uint8_t value) const
{
  buffer[0] = value & 0xFF;
  return 1;
}


uint8_t Frame::writeBuffer(uint8_t* buffer, uint16_t value) const
{
  buffer[0] = (value >> 8) & 0xFF;
  buffer[1] = value & 0xFF;
  return 2;
}


uint8_t Frame::writeBuffer(uint8_t* buffer, uint32_t value) const
{
  buffer[0] = (value >> 24) & 0xFF;
  buffer[1] = (value >> 16) & 0xFF;
  buffer[2] = (value >> 8) & 0xFF;
  buffer[3] = value & 0xFF;
  return 4;
}


uint8_t Frame::writeBuffer(uint8_t* buffer, uint64_t value) const
{
  buffer[0] = (value >> 56) & 0xFF;
  buffer[1] = (value >> 48) & 0xFF;
  buffer[2] = (value >> 40) & 0xFF;
  buffer[3] = (value >> 32) & 0xFF;
  buffer[4] = (value >> 24) & 0xFF;
  buffer[5] = (value >> 16) & 0xFF;
  buffer[6] = (value >> 8) & 0xFF;
  buffer[7] = value & 0xFF;
  return 8;
}


uint16_t Frame::writeBuffer(uint8_t* buffer, const std::string& value) const
{
  uint16_t size = static_cast<uint16_t>(value.length());
  if (size > 0) 
  {
      std::memcpy(buffer, value.c_str(), size);
  }
  return size;
}


uint8_t Frame::writeBuffer(uint8_t* buffer, const std::array<uint8_t, 16>& value) const
{
    // Copy all 16 bytes from the array directly into the target serialization buffer
    std::memcpy(buffer, value.data(), 16);
    
    // Return 16 so your calling packet loop knows exactly how far to advance the buffer pointer
    return 16; 
}


uint16_t Frame::frameToBits(uint8_t* buffer)
{
  buffer += writeBuffer(buffer, getSYNC());
  buffer += writeBuffer(buffer, getFRAMESIZE());
  buffer += writeBuffer(buffer, getSTREAM_ID());
  buffer += writeBuffer(buffer, getSOC());
  buffer += writeBuffer(buffer, getLEAP_BYTE());
  uint16_t fracsec1 =  (uint16_t) ((getFRACSEC() >> 8) & 0xFFFF);
  uint8_t fracsec2 = (uint8_t) (getFRACSEC() & 0xFF);
  buffer += writeBuffer(buffer, (fracsec1));
  buffer += writeBuffer(buffer, (fracsec2));
  return 14;
}