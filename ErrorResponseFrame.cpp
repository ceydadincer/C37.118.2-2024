#include "ErrorResponseFrame.hpp"

ErrorResponseFrame::ErrorResponseFrame()
{
  Frame();
  SYNC &= ~0x70;
  SYNC |= 0x70; 
}


ErrorResponseFrame::ErrorResponseFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint16_t error_response_1_value, 
    uint16_t error_response_2_value): Frame(sync_value, framesize_value, stream_id_value, soc_value, leap_byte_value,
    fracsec_value, chk_value)
{
  setErrorResponse1(error_response_1_value);
  setErrorResponse2(error_response_2_value);
  SYNC &= ~0x70;
  SYNC |= 0x70; 
  
}


ErrorResponseFrame::~ErrorResponseFrame()
{

}


void ErrorResponseFrame::setErrorResponse1(uint16_t error_response_1_value)
{
  ERROR_RESPONSE_1 = error_response_1_value;
}


void ErrorResponseFrame::setErrorResponse2(uint16_t error_response_2_value)
{
  ERROR_RESPONSE_2 = error_response_2_value;
}


uint16_t ErrorResponseFrame::getErrorResponse1()
{
  return ERROR_RESPONSE_1;
}


uint16_t ErrorResponseFrame::getErrorResponse2()
{
  return ERROR_RESPONSE_2;
}
