#ifndef errorresponseframe_hpp
#define errorreponseframe_hpp

#include "frame.hpp"
#include "configframe.hpp"

/**
 * ErrorResponseFrame class:
 * A subclass representing error response frame.
 * Attributes and their setters/getters for unique fields used in the frame.
 * Overrides virtual methods defined in the base class. 
 */
class ErrorResponseFrame: public Frame 
{
protected:
  uint16_t ERROR_RESPONSE_1;
  uint16_t ERROR_RESPONSE_2;  

public:
  // default constructor
  ErrorResponseFrame();
  // constructor with parameters
  ErrorResponseFrame(std::shared_ptr<CapabilityConfigFrame>& config_frame_value);

  // setters
  // default value for the SYNC field of ErrorResponseFrame, assuming version = 3: 11110011 (0xF3)
  void setSYNC(uint16_t sync_value = (0xAA << 8) | 0xF3) override {SYNC = sync_value;}
  void setErrorResponse1(uint16_t error_response_1_value) {ERROR_RESPONSE_1 = error_response_1_value;}
  void setErrorResponse2(uint16_t error_response_2_value) {ERROR_RESPONSE_2 = error_response_2_value;}

  // getters
  uint16_t getErrorResponse1() {return ERROR_RESPONSE_1;}
  uint16_t getErrorResponse2() {return ERROR_RESPONSE_2;}

  // FRAMESIZE for ErrorResponseFrame is fixed
  uint16_t computeFRAMESIZE() const override {return Frame::computeFRAMESIZE()+2+2;}

  // overridden method for writing a frame into buffer 
  uint16_t frameToBits(uint8_t* buffer) override;

  // overridden method for reading a frame from buffer 
  uint16_t bitsToFrame(uint8_t* buffer) override;
};

#endif