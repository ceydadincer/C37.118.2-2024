#ifndef frame_hpp
#define frame_hpp

#include <cstdint>
#include <array>
#include <string>


class Frame
{
protected:  

  uint16_t SYNC;
  uint16_t FRAMESIZE;
  uint16_t STREAM_ID;
  uint32_t SOC;
  uint8_t LEAP_BYTE;
  uint32_t FRACSEC;
  uint16_t CHK;

public:

  Frame();
  Frame(uint16_t sync_value, uint16_t framesize_value, uint16_t stream_id_value, 
             uint32_t soc_value, uint8_t leap_byte_value, uint32_t fracsec_value, uint16_t chk_value);
  ~Frame();

  void setSYNC(uint16_t sync_value);
  void setFRAMESIZE(uint16_t framesize_value) {FRAMESIZE = framesize_value;}
  void setSTREAM_ID(uint16_t stream_id_value) {STREAM_ID = stream_id_value;}
  void setSOC(uint32_t soc_value) {SOC = soc_value;}
  void setLEAP_BYTE(uint8_t leap_byte_value) {LEAP_BYTE = leap_byte_value;}
  void setFRACSEC(uint32_t fracsec_value);
  void setCHK(uint16_t chk_value) {CHK = chk_value;}

  const uint16_t getSYNC() const {return SYNC;}
  const uint16_t getFRAMESIZE() const {return FRAMESIZE;}
  const uint16_t getSTREAM_ID() const {return STREAM_ID;}
  const uint32_t getSOC() const {return SOC;}
  const uint8_t getLEAP_BYTE() const {return LEAP_BYTE;}
  const uint32_t getFRACSEC() const {return FRACSEC;}
  const uint16_t getCHK() const {return CHK;};

  uint16_t computeCRC(uint8_t* data, uint8_t dataLength);

//  calculateFRAMESIZE()
//  void calculateTime();
  uint8_t writeBuffer(uint8_t* buffer, uint8_t value) const;
  uint8_t writeBuffer(uint8_t* buffer, uint16_t value) const;
  uint8_t writeBuffer(uint8_t* buffer, uint32_t value) const;
  uint8_t writeBuffer(uint8_t* buffer, uint64_t value) const;
  uint16_t writeBuffer(uint8_t* buffer, const std::string& value) const;
  uint8_t writeBuffer(uint8_t* buffer, const std::array<uint8_t, 16>& value) const;
  virtual uint16_t frameToBits(uint8_t* buffer);
  void readBuffer(uint8_t* buffer);

};

#endif
