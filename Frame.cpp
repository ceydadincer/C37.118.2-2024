#include "frame.hpp"
#include <iostream>
#include <cstring>

// default constructor adjusts the SYNC default value for all frames, leaving the frame specific bits 0
Frame::Frame() 
{
  /*
  * SYNC field:
  * The first byte (15-8) of SYNC is initialized as the value defined in the standard (0xAA)
  * Bit 7 is set to 1 by default
  * Bits 6-4 are temporarily 0 to be defined in the constructors of the sub-classes
  * Bits 3-0 are set to 0011 by default for Version 3
  */
  SYNC = (0xAA << 8) | 0x83;
}

// constructor with parameters calls the setters with given parameters
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
uint16_t Frame::computeCRC(uint8_t* buffer, uint8_t bufferLength) const
{
  uint16_t crc = 0xFFFF;  
  uint16_t temp;
  uint16_t quick;
  uint8_t i;
  for (i = 0; i < bufferLength; i++)
  {
    temp = (crc >> 8) ^ buffer[i];
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

// helper functions to write different data types to buffer
// each returns size so that the buffer can be incremented with the return value

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

uint8_t Frame::writeBuffer(uint8_t* buffer, const std::string& value, uint8_t value_length) const
{
  if (value_length > 0) 
  {
    std::memcpy(buffer, value.data(), value_length);
  }
  return value_length;
}

uint8_t Frame::writeBuffer(uint8_t* buffer, const std::array<uint8_t, 16>& value) const
{
  std::memcpy(buffer, value.data(), 16);
  return 16; 
}

// helper functions to read from buffer

uint8_t Frame::readBuffer(const uint8_t* buffer, uint8_t& value) const
{
  value = buffer[0];
  return 1;
}

uint8_t Frame::readBuffer(const uint8_t* buffer, uint16_t& value) const
{
  value = (static_cast<uint16_t>(buffer[0]) << 8) | 
           static_cast<uint16_t>(buffer[1]);
  return 2;
}

uint8_t Frame::readBuffer(const uint8_t* buffer, uint32_t& value) const
{
  value = (static_cast<uint32_t>(buffer[0]) << 24) |
          (static_cast<uint32_t>(buffer[1]) << 16) |
          (static_cast<uint32_t>(buffer[2]) << 8)  |
           static_cast<uint32_t>(buffer[3]);

  return 4;
}

uint8_t Frame::readBuffer(const uint8_t* buffer, uint64_t& value) const
{
  value = (static_cast<uint64_t>(buffer[0]) << 56) |
          (static_cast<uint64_t>(buffer[1]) << 48) |
          (static_cast<uint64_t>(buffer[2]) << 40) |
          (static_cast<uint64_t>(buffer[3]) << 32) |
          (static_cast<uint64_t>(buffer[4]) << 24) |
          (static_cast<uint64_t>(buffer[5]) << 16) |
          (static_cast<uint64_t>(buffer[6]) << 8)  |
           static_cast<uint64_t>(buffer[7]);

  return 8;
}

uint8_t Frame::readBuffer(const uint8_t* buffer, std::string& value, uint8_t value_length) const
{
  value.resize(value_length);
  std::memcpy(value.data(), buffer, value_length);

  return value_length;
}

uint8_t Frame::readBuffer(const uint8_t* buffer, std::array<uint8_t, 16>& value) const
{
  std::memcpy(value.data(), buffer, 16);
  return 16;
}

// base method for writing a frame into buffer, overridden and used by all frame classes 
// only writes the first common fields used, doesn't write CHK field which is at the end
uint16_t Frame::frameToBits(uint8_t* buffer)
{
  // compute and set FRAMESIZE
  setFRAMESIZE(computeFRAMESIZE());
  buffer += writeBuffer(buffer, getSYNC());
  buffer += writeBuffer(buffer, getFRAMESIZE());
  buffer += writeBuffer(buffer, getSTREAM_ID());
  buffer += writeBuffer(buffer, getSOC());
  buffer += writeBuffer(buffer, getLEAP_BYTE());
  uint16_t fracsec1 = (uint16_t) ((getFRACSEC() >> 8) & 0xFFFF);
  uint8_t fracsec2 = (uint8_t) (getFRACSEC() & 0xFF);
  buffer += writeBuffer(buffer, (fracsec1));
  buffer += writeBuffer(buffer, (fracsec2));
  // return the size of the written bytes
  return 14;
}

// base method for reading a frame into buffer, overridden and used by all frame classes 
// only reads the first common fields used, doesn't read CHK field which is at the end
uint16_t Frame::bitsToFrame(uint8_t* buffer)
{
  uint16_t sync;
  buffer += readBuffer(buffer, sync);
  setSYNC(sync);
  uint16_t framesize;
  buffer += readBuffer(buffer, framesize);
  setFRAMESIZE(framesize);  
  uint16_t stream_id;
  buffer += readBuffer(buffer, stream_id);
  setSTREAM_ID(stream_id);
  uint32_t soc;
  buffer += readBuffer(buffer, soc);
  setSOC(soc);
  uint8_t leap_byte;
  buffer += readBuffer(buffer, leap_byte);
  setLEAP_BYTE(leap_byte);
  uint16_t fracsec1;
  buffer += readBuffer(buffer, fracsec1);
  uint8_t fracsec2;
  buffer += readBuffer(buffer, fracsec2);
  uint32_t fracsec = (static_cast<uint32_t>(fracsec1) << 8) |
                     static_cast<uint32_t>(fracsec2);
  setFRACSEC(fracsec);
  // return the size of the read bytes
  return 14;
}