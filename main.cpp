#include "frame.hpp"
#include "dataframe.hpp"
#include "configframe.hpp"
#include "commandframe.hpp"
#include "PMU.hpp"
#include <iostream>
#include <vector>

#include <bitset>
int main()
{
  /*
  Frame f = Frame((170<<8) | 3, 456, 789, 101, 111, 124, 66);
  uint8_t buffer[1000];
  f.frameToBits(buffer);
  uint16_t value1 = (static_cast<uint16_t>(buffer[0]) << 8) | buffer[1];
  std::cout << value1 << std::endl;
  uint16_t value2 = (static_cast<uint16_t>(buffer[2]) << 8) | buffer[3];
  std::cout << value2 << std::endl;
  uint16_t value3 = (static_cast<uint16_t>(buffer[4]) << 8) | buffer[5];
  std::cout << value3 << std::endl;
  uint32_t value4 =
    (static_cast<uint32_t>(buffer[6]) << 24) |
    (static_cast<uint32_t>(buffer[7]) << 16) |
    (static_cast<uint32_t>(buffer[8]) << 8)  |
     static_cast<uint32_t>(buffer[9]);
  std::cout << value4 << std::endl;
  uint16_t value5 = (static_cast<uint16_t>(buffer[10]));
  std::cout << value5 << std::endl;
    uint32_t value6 =
    (static_cast<uint32_t>(buffer[11]) << 16) |
    (static_cast<uint32_t>(buffer[12]) << 8) |
    (static_cast<uint32_t>(buffer[13]) << 0)  |
     static_cast<uint32_t>(buffer[14]);
  std::cout << value6 << std::endl;
  return 0;
  */

  std::shared_ptr<PMU> pmu1 = std::make_shared<PMU>();
  ConfigFrame* cfg1 = new ConfigFrame();
  pmu1->setPMU_NAME("pmu1");
  pmu1->setPMU_ID(123);
  pmu1->setG_PMU_ID({123, 233});
  pmu1->setFORMAT(12);
  pmu1->setPHNMR(2);
  pmu1->setANNMR(1);
  pmu1->setFRNMR(1);
  pmu1->setDFDTNMR(1);
  pmu1->setDGNMR(1);
  pmu1->setChannelPhasor({"ph1", "ph2"}, {{123, 23}, {254, 1}});
  pmu1->setChannelFrequency({"fr"}, {1});
  pmu1->setChannelDFDT({"dfdt1"}, {2});
  pmu1->setChannelAnalog({"an1"}, {3});
  pmu1->setChannelDigital({"dg1"}, {4});
  
}
