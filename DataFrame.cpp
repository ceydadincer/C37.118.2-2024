#include "dataframe.hpp"

DataFrame::DataFrame()
{
  // Bits 6-4 are set to 0 (cleared)
  Frame();
  SYNC &= ~0x70; 
}


DataFrame::DataFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value)
{
  Frame(sync_value, framesize_value, stream_id_value, soc_value, leap_byte_value, fracsec_value, chk_value);
}


DataFrame::~DataFrame() {}



