#ifndef configframe_hpp
#define configframe_hpp

#include "frame.hpp"
#include <vector>
#include <memory>

class PMU;

/**
 * CapabilityConfigFrame class:
 * A subclass representing capability configuration frame (CFG1), also a base class for stream configuration frame (CFG3).
 * Attributes and their setters/getters for unique fields used in the frame.
 * Overrides virtual methods defined in the base class. 
 */
class CapabilityConfigFrame: public Frame
{
protected:
  uint16_t CONT_IDX;
  uint32_t TIME_BASE;
  // the first byte of PDC_NAME field
  uint8_t PDC_NAME_length;
  std::string PDC_NAME;
  uint16_t NUM_PMU;
  uint16_t STREAM_DATA_RATE;
  uint16_t WAIT_TIME;
  // a container of PMUs for accessing PMU related attributes  
  std::vector<std::shared_ptr<PMU>> PMUList;

public:
  CapabilityConfigFrame();

  // setters
  // default value for the SYNC field of CapabilityConfigFrame, assuming version = 3: 10100011 (0xA3)
  virtual void setSYNC(uint16_t sync_value = (0xAA << 8) | 0xA3) override {SYNC = sync_value;}
  void setCONT_IDX(uint16_t cont_idx_value){CONT_IDX = cont_idx_value;}
  void setTIME_BASE(uint16_t time_base_value){TIME_BASE = time_base_value;}
  void setPDC_NAME(const std::string& pdc_name_value);
  void setNUM_PMU(uint16_t num_pmu_value){NUM_PMU = num_pmu_value;}
  void setSTREAM_DATA_RATE(uint16_t stream_data_rate){STREAM_DATA_RATE = stream_data_rate;}
  void setWAIT_TIME(uint16_t wait_time_value){WAIT_TIME = wait_time_value;}
  void setPMUList(const std::vector<std::shared_ptr<PMU>>& pmu_list);

  // getters
  uint16_t getCONT_IDX() const {return CONT_IDX;}
  uint16_t getTIME_BASE() const {return TIME_BASE;}
  uint8_t getPDC_NAME_length() const {return PDC_NAME_length;}
  const std::string& getPDC_NAME() const {return PDC_NAME;}
  uint16_t getNUM_PMU() const {return NUM_PMU;}
  uint16_t getSTREAM_DATA_RATE() const {return STREAM_DATA_RATE;}
  uint16_t getWAIT_TIME() const {return WAIT_TIME;}
  const std::vector<std::shared_ptr<PMU>>& getPMUList() const {return PMUList;}

  // FRAMESIZE computation
  uint16_t computeFRAMESIZE() const override;

  // overridden method for writing a frame into buffer 
  uint16_t frameToBits(uint8_t* buffer) override;

  // overridden method for reading a frame from buffer 
  uint16_t bitsToFrame(uint8_t* buffer) override;
};

/**
 * StreamConfigFrame class:
 * A subclass representing stream configuration frame (CFG3), inherits CapabilityConfigFrame base class.
 * No new attributes.
 * Only overrides the logic for setting SYNC, everything else is identical to CapabilityConfigFrame. 
 */
class StreamConfigFrame: public CapabilityConfigFrame
{
public:
  // default constructor
  StreamConfigFrame();
  // constructor with parameters
  StreamConfigFrame(std::shared_ptr<CapabilityConfigFrame>);
  
  // setter
  // default value for the SYNC field of StreamConfigFrame, assuming version = 3: 10110011 (0xB3)
  void setSYNC(uint16_t sync_value = (0xAA << 8) | 0xB3) override {SYNC = sync_value;}
};

#endif