#ifndef frame_hpp
#define frame_hpp

#include <cstdint>
#include <array>
#include <string>

/**
 * Frame class:
 * A base class that is inherited by each frame class.
 * Attributes and their setters/getters for common fields used in the frames.
 * Methods for certain computations needed across the frame classes.
 * Base methods for writing/reading a frame to be overridden and used by all frame classes .
 * Methods with logic to write to or read from buffer, these functions can be made into a template.
 */
class Frame
{
protected:  
  // common fields
  uint16_t SYNC;
  uint16_t FRAMESIZE;
  uint16_t STREAM_ID;
  uint32_t SOC;
  uint8_t LEAP_BYTE;
  uint32_t FRACSEC;
  uint16_t CHK;

public:
  // default constructor
  Frame();
  // constructor with parameters
  Frame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, 
             uint32_t soc_value, uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value);

  // setters for the field attributes
  virtual void setSYNC(uint16_t sync_value) {SYNC = sync_value;};
  void setFRAMESIZE(uint16_t framesize_value) {FRAMESIZE = framesize_value;}
  void setSTREAM_ID(uint16_t stream_id_value) {STREAM_ID = stream_id_value;}
  void setSOC(uint32_t soc_value) {SOC = soc_value;}
  void setLEAP_BYTE(uint8_t leap_byte_value) {LEAP_BYTE = leap_byte_value;}
  void setFRACSEC(uint32_t fracsec_value);
  void setCHK(uint16_t chk_value) {CHK = chk_value;}

  // getters for the field attributes
  const uint16_t getSYNC() const {return SYNC;}
  const uint16_t getFRAMESIZE() const {return FRAMESIZE;}
  const uint16_t getSTREAM_ID() const {return STREAM_ID;}
  const uint32_t getSOC() const {return SOC;}
  const uint8_t getLEAP_BYTE() const {return LEAP_BYTE;}
  const uint32_t getFRACSEC() const {return FRACSEC;}
  const uint16_t getCHK() const {return CHK;};

  // helpful computation functions
  // returns total size of common fields
  virtual uint16_t computeFRAMESIZE() const {return 2+2+2+4+1+3+2;}
  uint16_t computeCRC(uint8_t* buffer, uint8_t bufferLength) const;
//  void computeTime();

  // helper functions to write to buffer
  uint8_t writeBuffer(uint8_t* buffer, uint8_t value) const;
  uint8_t writeBuffer(uint8_t* buffer, uint16_t value) const;
  uint8_t writeBuffer(uint8_t* buffer, uint32_t value) const;
  uint8_t writeBuffer(uint8_t* buffer, uint64_t value) const;
  uint8_t writeBuffer(uint8_t* buffer, const std::string& value, uint8_t value_length) const;
  uint8_t writeBuffer(uint8_t* buffer, const std::array<uint8_t, 16>& value) const;

  // helper functions to read from buffer
  uint8_t readBuffer(const uint8_t* buffer, uint8_t& value) const;
  uint8_t readBuffer(const uint8_t* buffer, uint16_t& value) const;
  uint8_t readBuffer(const uint8_t* buffer, uint32_t& value) const;
  uint8_t readBuffer(const uint8_t* buffer, uint64_t& value) const;
  uint8_t readBuffer(const uint8_t* buffer, std::string& value, uint8_t value_length) const;
  uint8_t readBuffer(const uint8_t* buffer, std::array<uint8_t, 16>& value) const;

  // base method for writing a frame into buffer, overridden and used by all frame classes 
  virtual uint16_t frameToBits(uint8_t* buffer);

  // base method for reading a frame into buffer, overridden and used by all frame classes 
  virtual uint16_t bitsToFrame(uint8_t* buffer);
};

#endif
