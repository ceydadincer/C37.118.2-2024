#include "frame.hpp"
#include "dataframe.hpp"
#include "configframe.hpp"
#include "commandframe.hpp"
#include "pmu.hpp"
#include <iostream>
#include <vector>

int main()
{
  // first create multiple PMUs and set their attributes
  std::shared_ptr<PMU> pmu1 = std::make_shared<PMU>();
  pmu1->setPMU_NAME("pmu1");
  pmu1->setPMU_ID(123);
  pmu1->setG_PMU_ID({123, 233});
  pmu1->setFORMAT(12);
  pmu1->setChannelPhasor({"ph1", "ph2"}, {{123, 23}, {254, 1}});
  pmu1->setChannelFrequency({"fr"}, {1});
  pmu1->setChannelDFDT({"dfdt1"}, {2});
  pmu1->setChannelAnalog({"an1"}, {3});
  pmu1->setChannelDigital({"dg1"}, {4});

  // then create frames and set their attributes
  CapabilityConfigFrame* cfg1 = new CapabilityConfigFrame();

  // then use these frames to write to buffer
  // ...
}
