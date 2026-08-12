#ifndef configframe_hpp
#define configframe_hpp

#include "frame.hpp"
#include <vector>
#include <memory>

class PMU;

class ConfigFrame: public Frame
{
protected:
  uint16_t CONT_IDX = 0;
  uint32_t TIME_BASE;
  std::string PDC_NAME = "";
  uint16_t NUM_PMU = 0;
  uint16_t WAIT_TIME = 0;
  uint16_t STREAM_DATA_RATE;
  std::vector<std::shared_ptr<PMU>> PMUList;

public:
  ConfigFrame();
  ConfigFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint16_t cont_idx_value, 
    uint32_t time_base_value, uint16_t stream_data_rate_value);
  ~ConfigFrame();

  void setPDC_NAME(const std::string& pdc_name_value){PDC_NAME = pdc_name_value;}
  void setWAIT_TIME(uint16_t wait_time_value){WAIT_TIME = wait_time_value;}
  void setNUM_PMU(uint16_t num_pmu_value){NUM_PMU = num_pmu_value;}

  void setCONT_IDX(uint16_t cont_idx_value){CONT_IDX = cont_idx_value;}
  void setTIME_BASE(uint16_t time_base_value){TIME_BASE = time_base_value;}
  void setSTREAM_DATA_RATE(uint16_t stream_data_rate){STREAM_DATA_RATE = stream_data_rate;}

  void setPMUList(const std::vector<std::shared_ptr<PMU>>& pmu_list);
  void addPMU(const std::shared_ptr<PMU> pmu);

  const std::string& getPDC_NAME() const {return PDC_NAME;}
  uint16_t getWAIT_TIME() const {return WAIT_TIME;}
  uint16_t getNUM_PMU() const {return NUM_PMU;}

  uint16_t getCONT_IDX() const {return CONT_IDX;}
  uint16_t getTIME_BASE() const {return TIME_BASE;}
  uint16_t getSTREAM_DATA_RATE() const {return STREAM_DATA_RATE;}

  const std::vector<std::shared_ptr<PMU>> getPMUList() const {return PMUList;}
  const std::shared_ptr<PMU> getPMU(int idx) const;

  uint16_t frameToBits(uint8_t* buffer);
};


class CapabilityFrame: public ConfigFrame
{
public:
  CapabilityFrame();
  CapabilityFrame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, uint32_t soc_value, 
    uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value, uint16_t cont_idx_value, 
    uint32_t time_base_value, uint16_t stream_data_rate_value);
};

#endif