#ifndef commandframe_hpp
#define commandframe_hpp

#include "frame.hpp"

#include <iostream>
#include <vector>

class DataSource;

class CommandFrame: public Frame
{
protected:
  uint16_t CMD;
  std::vector<uint8_t> EXTFRAME;
  
public:
  CommandFrame();
  CommandFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint16_t cmd_value, std::vector<uint8_t>& extframe_value);
  ~CommandFrame();

  void setCMD(uint16_t cmd_value);
  void setEXTRFRAME(std::vector<uint8_t>& extframe_value);

  uint16_t getCmd();
  std::vector<uint8_t> getEXTRFRAME();
};

// OldDataRequestFrame
// StreamIDAvaiableFrame


class ConfigRenameCommandFrame: public Frame
{
protected:
  uint16_t CONT_IDX;
  DataSource* source;

public:
  ConfigRenameCommandFrame();
  ConfigRenameCommandFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint64_t cont_idx_value);
};

class ConfigStreamCommandFrame: public Frame
{
protected:
  uint16_t CONT_IDX;
  DataSource* source;

public:
  ConfigStreamCommandFrame();
  ConfigStreamCommandFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint64_t cont_idx_value);
};




#endif