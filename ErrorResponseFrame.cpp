#include "errorresponseframe.hpp"

// default constructor adjusts the SYNC default value for ErrorResponseFrame
ErrorResponseFrame::ErrorResponseFrame(): Frame()
{
  // bits 6-4 for the SYNC field of ErrorResponseFrame are 111
  SYNC |= 0x70; 
}

// constructor adjusts the SYNC default value for ErrorResponseFrame amd sets STREAM_ID using CapabilityConfigFrame
ErrorResponseFrame::ErrorResponseFrame(std::shared_ptr<CapabilityConfigFrame>& capability_config_frame): Frame()
{
  // bits 6-4 for the SYNC field of ErrorResponseFrame are 111
  SYNC |= 0x70; 
  setSTREAM_ID(capability_config_frame->getSTREAM_ID());
}

// overridden method for writing a frame into buffer 
uint16_t ErrorResponseFrame::frameToBits(uint8_t* buffer)
{
  // write common fields and advance the buffer pointer
  buffer += Frame::frameToBits(buffer);
  buffer += writeBuffer(buffer, getErrorResponse1());
  buffer += writeBuffer(buffer, getErrorResponse2()); 
  // compute CRC and set CHK before writing it to buffer
  setCHK(computeCRC(buffer, getFRAMESIZE()-2));
  buffer += writeBuffer(buffer, getCHK());
  // return the size of the written bytes
  return getFRAMESIZE();
}

// overridden method for reading a frame from buffer 
uint16_t ErrorResponseFrame::bitsToFrame(uint8_t* buffer)
{
  // read common fields and advance the buffer pointer
  buffer += Frame::bitsToFrame(buffer);
  uint16_t errorResponse1;
  buffer += readBuffer(buffer, errorResponse1);
  setErrorResponse1(errorResponse1);
  uint16_t errorResponse2;
  buffer += readBuffer(buffer, errorResponse2);
  setErrorResponse2(errorResponse2);
  uint16_t chk;
  buffer += readBuffer(buffer, chk);
  setCHK(chk);
  // return the size of the read bytes
  return getFRAMESIZE(); 
}