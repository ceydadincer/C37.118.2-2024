#include "commandframe.hpp"

CommandFrame::CommandFrame()
{
  // Bits 6-4 are updated as 100  
  Frame();
  SYNC &= ~0x70;
  SYNC |= 0x40; 
}


CommandFrame::CommandFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
  uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint16_t cmd_value, std::vector<uint8_t>& extframe_value):
  Frame(sync_value, framesize_value, stream_id_value, soc_value, leap_byte_value, fracsec_value, chk_value)
{
  setCMD(cmd_value);
  setEXTRFRAME(extframe_value);
}


CommandFrame::~CommandFrame() {}


void CommandFrame::setCMD(uint16_t cmd_value)
{

}


void CommandFrame::setEXTRFRAME(std::vector<uint8_t>& extframe_value)
{

}


uint16_t CommandFrame::getCmd()
{

}


std::vector<uint8_t> CommandFrame::getEXTRFRAME()
{

}



ConfigRenameCommandFrame::ConfigRenameCommandFrame(): Frame()
{
  SYNC &= ~0x70;
  SYNC |= 0x50; 
}


ConfigRenameCommandFrame::ConfigRenameCommandFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint64_t cont_idx_value): 
    Frame(sync_value, framesize_value, stream_id_value, soc_value, leap_byte_value, fracsec_value, chk_value)
{
      SYNC &= ~0x70;
      SYNC |= 0x50; 
}



ConfigStreamCommandFrame::ConfigStreamCommandFrame(): Frame()
{
  SYNC &= ~0x70;
  SYNC |= 0x60; 
}


ConfigStreamCommandFrame::ConfigStreamCommandFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint64_t cont_idx_value): 
    Frame(sync_value, framesize_value, stream_id_value, soc_value, leap_byte_value, fracsec_value, chk_value)
{
  SYNC &= ~0x70;
  SYNC |= 0x60; 
}


