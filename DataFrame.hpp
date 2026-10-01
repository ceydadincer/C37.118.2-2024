#ifndef dataframe_hpp
#define dataframe_hpp

#include "frame.hpp"
#include "configframe.hpp"
#include <vector>
#include <memory>
#include <iostream>

class PMU;

/**
 * DataFrame class:
 * A subclass representing periodic data frame, also a base class for discrete data frame.
 * Only an attribute for storing PMUs and its setter/getter.
 * Overrides virtual methods defined in the base class. 
 */
class DataFrame: public Frame
{
protected:
  // a pointer to the stream's CapabilityConfigFrame for accessing PMU related attributes for measurements
  // can only be set during the creation of the frame
  std::shared_ptr<CapabilityConfigFrame> configFrame;

public:
  // constructor with parameter
  DataFrame(std::shared_ptr<CapabilityConfigFrame>& config_frame_value);

  // setter for SYNC
  // default value for the SYNC field of DataFrame, assuming version = 3: 10000011 (0x83)
  virtual void setSYNC(uint16_t sync_value = (0xAA << 8) | 0x83) override {SYNC = sync_value;}

  // FRAMESIZE computation
  virtual uint16_t computeFRAMESIZE() const override;

  // overridden method for writing a frame into buffer 
  virtual uint16_t frameToBits(uint8_t* buffer) override;

  // overridden method for reading a frame from buffer 
  virtual uint16_t bitsToFrame(uint8_t* buffer) override;

};

/**
 * DiscreteDataFrame class:
 * A subclass representing discrete data frame, inherits DataFrame base class.
 * No new attributes.
 * Overrides virtual methods defined in the base class. 
 */
class DiscreteDataFrame: public DataFrame
{
  DiscreteDataFrame(std::shared_ptr<CapabilityConfigFrame>& config_frame_value);

  // setter
  // default value for the SYNC field of DiscreteDataFrame, assuming version = 3: 10010011 (0x93)
  void setSYNC(uint16_t sync_value = (0xAA << 8) | 0x93) override {SYNC = sync_value;}

  // FRAMESIZE computation
  uint16_t computeFRAMESIZE() const override;

  // overridden method for writing a frame into buffer 
  uint16_t frameToBits(uint8_t* buffer) override;

  // overridden method for reading a frame from buffer 
  uint16_t bitsToFrame(uint8_t* buffer) override;
};

#endif