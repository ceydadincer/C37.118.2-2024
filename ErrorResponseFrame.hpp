#ifndef errorresponseframe_hpp
#define errorreponseframe_hpp

#include "Frame.hpp"

class ErrorResponseFrame: public Frame 
{
protected:
  uint16_t ERROR_RESPONSE_1;
  uint16_t ERROR_RESPONSE_2;  

public:
  ErrorResponseFrame();
  ErrorResponseFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint16_t error_response_1_value, 
    uint16_t error_response_2_value);
  ~ErrorResponseFrame();

  void setErrorResponse1(uint16_t error_response_1_value);
  void setErrorResponse2(uint16_t error_response_2_value);

  uint16_t getErrorResponse1();
  uint16_t getErrorResponse2();
};

#endif