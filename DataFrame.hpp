/**
 * TODO: Separate periodic from discrete either with an attribute FrameType or hierarchy and the constructor
 */


#ifndef dataframe_hpp
#define dataframe_hpp

#include "frame.hpp"

class DataSource;

class DataFrame: public Frame
{
protected:
  DataSource* source;

public:
  DataFrame();
  DataFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value);
  ~DataFrame();

  /*
  void encode(uint8_t buffer);
  void decode(uint8_t buffer);
  */
};

class DiscreteDataFrame: public DataFrame
{
  DiscreteDataFrame()
  {
    SYNC &= ~0x70;
    SYNC |= 0x10; 
  }
};
#endif