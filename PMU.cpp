#include "PMU.hpp"

PMU::PMU()
{
  Measurements = std::make_unique<PMUMeasurements>(); 
}

PMU::~PMU() {}

void PMU::setPMU_NAME(const std::string& pmu_name_value)
{
  if (pmu_name_value.size() > 256)
  {
    std::cout << "PMU_NAME CAN'T BE MORE THAN 256 BYTES" << std::endl;
  }
  PMU_NAME = pmu_name_value;
}


void PMU::setChannelPhasor(const std::vector<std::string>& ph_chnam_values, const std::vector<std::array<uint8_t, 16>>& ph_scale_values)
{
  if (ph_chnam_values.size() != ph_scale_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }
  setPHCHNAM(ph_chnam_values);
  setPHSCALE(ph_scale_values);
  PHNMR = ph_chnam_values.size();
}


void PMU::setChannelAnalog(const std::vector<std::string>& an_chnam_values, const std::vector<uint64_t>& an_scale_values)
{ 
  if (an_chnam_values.size() != an_scale_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }
  setANCHNAM(an_chnam_values);
  setANSCALE(an_scale_values);
  ANNMR = an_chnam_values.size();
}


void PMU::setChannelFrequency(const std::vector<std::string>& fr_chnam_values, const std::vector<uint64_t>& fr_scale_values)
{
  if (fr_chnam_values.size() != fr_scale_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }
  setFRCHNAM(fr_chnam_values);
  setFRSCALE(fr_scale_values);
  FRNMR = fr_chnam_values.size();
}


void PMU::setChannelDFDT(const std::vector<std::string>& dfdt_chnam_values, const std::vector<uint64_t>& dfdt_scale_values)
{
  if (dfdt_chnam_values.size() != dfdt_scale_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }
  setDFDTCHNAM(dfdt_chnam_values);
  setDFDTSCALE(dfdt_scale_values);
  DFDTNMR = dfdt_chnam_values.size();
}


void PMU::setChannelDigital(const std::vector<std::string>& dig_chnam_values, const std::vector<uint32_t>& dig_unit_values)
{
  if (dig_chnam_values.size() != dig_unit_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }
  setDIGCHNAM(dig_chnam_values);
  setDIGUNIT(dig_unit_values);
  DGNMR = dig_chnam_values.size();
}



 void PMU::addChannelPhasor(const std::string& ph_chnam_value, const std::array<uint8_t, 16>& ph_scale_value)
{
  PHCHNAM.push_back(ph_chnam_value);
  PHSCALE.push_back(ph_scale_value);
  PHNMR++;
}


void PMU::addChannelAnalog(const std::string& an_chnam_value, uint64_t an_scale_value)
{
  ANCHNAM.push_back(an_chnam_value);
  ANSCALE.push_back(an_scale_value);
  ANNMR++;
}


void PMU::addChannelFrequency(const std::string& fr_chnam_value, uint64_t fr_scale_value)
{
  FRCHNAM.push_back(fr_chnam_value);
  FRSCALE.push_back(fr_scale_value);
  FRNMR++;
}


void PMU::addChannelDFDT(const std::string& dfdt_chnam_value, uint64_t dfdt_scale_value)
{
  DFDTCHNAM.push_back(dfdt_chnam_value);
  DFDTSCALE.push_back(dfdt_scale_value);
  DFDTNMR++;
}
 

void PMU::addChannelDigital(const std::string& dig_chnam_value, uint32_t dig_unit_value)
{
  DIGCHNAM.push_back(dig_chnam_value);
  DIGUNIT.push_back(dig_unit_value);
  DGNMR++;
}


void PMU::setMeasurements(uint16_t stat_flag_value, uint16_t timequality_value, const std::vector<uint64_t>& phasor_values,
  const std::vector<uint32_t>& freq_values, const std::vector<uint32_t>& dfreq_values, const std::vector<uint32_t>& analog_values, 
  const std::vector<uint16_t>& digital_values)
{
    setSTAT_FLAG(stat_flag_value);
    setTIMEQUALITY(timequality_value);
    setPHASOR_values(phasor_values);
    setFREQ_values(freq_values);
    setDFREQ_values(dfreq_values);
    setANALOG_values(analog_values);
    setDIGITAL_values(digital_values);
}

// setter functions for measurements need to be called AFTER setting the channel names/scales
void PMU::setPHASOR_values(const std::vector<uint64_t>& phasor_values)
{
  if (PHNMR != phasor_values.size())
  {
    std::cout << "INCORRECT SIZE FOR MEASUREMENTS" << std::endl;
  }
  Measurements->PHASOR_values = phasor_values;
}


void PMU::setFREQ_values(const std::vector<uint32_t>& freq_values)
{ 
  if (FRNMR != freq_values.size())
  {
    std::cout << "INCORRECT SIZE FOR MEASUREMENTS" << std::endl;
  }
  Measurements->FREQ_values = freq_values;
}


void PMU::setDFREQ_values(const std::vector<uint32_t>& dfreq_values)
{
  if (DFDTNMR != dfreq_values.size())
  {
    std::cout << "INCORRECT SIZE FOR MEASUREMENTS" << std::endl;
  }
  Measurements->DFREQ_values = dfreq_values;
}


void PMU::setANALOG_values(const std::vector<uint32_t>& analog_values)
{  
  if (ANNMR != analog_values.size())
  {
    std::cout << "INCORRECT SIZE FOR MEASUREMENTS" << std::endl;
  }
  Measurements->ANALOG_values = analog_values;
}


void PMU::setDIGITAL_values(const std::vector<uint16_t>& digital_values)
{
  if (DGNMR != digital_values.size())
  {
    std::cout << "INCORRECT SIZE FOR MEASUREMENTS" << std::endl;
  }
  Measurements->DIGITAL_values = digital_values;
}


void PMU::addPhasorValue(const uint64_t phasor_value, int idx)
{
  if(idx < 0 || idx >= PHNMR)
  {
    std::cout << "INDEX OUT OF RANGE" << std::endl;
  }
  Measurements->PHASOR_values[idx] = phasor_value;
}


void PMU::addFrequencyValue(const uint32_t freq_value, int idx)
{  
  if(idx < 0 || idx >= FRNMR)
  {
    std::cout << "INDEX OUT OF RANGE" << std::endl;
  }
  Measurements->FREQ_values[idx] = freq_value;
}


void PMU::addDfreqValue(const uint32_t dfreq_value, int idx)
{
  if(idx < 0 || idx >= DFDTNMR)
  {
    std::cout << "INDEX OUT OF RANGE" << std::endl;
  }
  Measurements->DFREQ_values[idx] = dfreq_value;
}


void PMU::addAnalogValue(const uint32_t analog_value, int idx)
{
  if(idx < 0 || idx >= ANNMR)
  {
    std::cout << "INDEX OUT OF RANGE" << std::endl;
  }
  Measurements->ANALOG_values[idx] = analog_value;
} 


void PMU::addDigitalValue(const uint16_t digital_value, int idx)
{
  if(idx < 0 || idx >= DGNMR)
  {
    std::cout << "INDEX OUT OF RANGE" << std::endl;
  }
  Measurements->DIGITAL_values[idx] = digital_value;
}