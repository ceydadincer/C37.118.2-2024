#include "pmu.hpp"

// constructor initializes Measurements with a unique_ptr to PMUMeasurements object
PMU::PMU() 
{
  std::unique_ptr<PMUMeasurements> Measurements = std::make_unique<PMUMeasurements>();
}

// setters (PMU specific)

// set attributes PMU_NAME and PMU_NAME_length for the PMU_NAME fields
void PMU::setPMU_NAME(const std::string& pmu_name_value) 
{
  PMU_NAME = pmu_name_value;
  PMU_NAME_length = (static_cast<uint8_t>(pmu_name_value.size()));
}

// set channel name and scale values and the number of channels together

void PMU::setChannelPhasor(const std::vector<std::string>& ph_chnam_values, const std::vector<std::array<uint8_t, 16>>& ph_scale_values)
{
  // check whether the number of channel scales match channel names
  if (ph_chnam_values.size() != ph_scale_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }
  PHCHNAM = ph_chnam_values;
  PHSCALE = ph_scale_values;
  PHNMR = ph_chnam_values.size();

  // save each string length to the vector one by one 
  PHCHNAM_lengths.clear();
  PHCHNAM_lengths.reserve(PHCHNAM.size());
  for (const std::string& name : PHCHNAM)
  {
    // check whether the given names exceed 255 bytes
    if (name.size() > 255)
    {
        std::cout << "CHANNEL NAME CANNOT EXCEED 255 BYTES" << std::endl;
        return;
    }
    PHCHNAM_lengths.push_back(static_cast<uint8_t>(name.size()));
  }
}

void PMU::setChannelFrequency(const std::vector<std::string>& fr_chnam_values, const std::vector<uint64_t>& fr_scale_values)
{
  // check whether the number of channel scales match channel names
  if (fr_chnam_values.size() != fr_scale_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }
  FRCHNAM = fr_chnam_values;
  FRSCALE = fr_scale_values;
  FRNMR = fr_chnam_values.size();

  // save each string length to the vector one by one 
  FRCHNAM_lengths.clear();
  FRCHNAM_lengths.reserve(FRCHNAM.size());
  for (const std::string& name : FRCHNAM)
  {
    // check whether the given names exceed 255 bytes
    if (name.size() > 255)
    {
        std::cout << "CHANNEL NAME CANNOT EXCEED 255 BYTES" << std::endl;
        return;
    }
    FRCHNAM_lengths.push_back(static_cast<uint8_t>(name.size()));
  }
}

void PMU::setChannelDFDT(const std::vector<std::string>& dfdt_chnam_values, const std::vector<uint64_t>& dfdt_scale_values)
{
  // check whether the number of channel scales match channel names
  if (dfdt_chnam_values.size() != dfdt_scale_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }
  DFDTCHNAM = dfdt_chnam_values;
  DFDTSCALE = dfdt_scale_values;
  DFDTNMR = dfdt_chnam_values.size();

  // save each string length to the vector one by one 
  DFDTCHNAM_lengths.clear();
  DFDTCHNAM_lengths.reserve(DFDTCHNAM.size());
  for (const std::string& name : DFDTCHNAM)
  {
    // check whether the given names exceed 255 bytes
    if (name.size() > 255)
    {
        std::cout << "CHANNEL NAME CANNOT EXCEED 255 BYTES" << std::endl;
        return;
    }
    DFDTCHNAM_lengths.push_back(static_cast<uint8_t>(name.size()));
  }
}

void PMU::setChannelAnalog(const std::vector<std::string>& an_chnam_values, const std::vector<uint64_t>& an_scale_values)
{ 
  // check whether the number of channel scales match channel names
  if (an_chnam_values.size() != an_scale_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }
  ANCHNAM = an_chnam_values;
  ANSCALE = an_scale_values;
  ANNMR = an_chnam_values.size();

  // save each string length to the vector one by one 
  ANCHNAM_lengths.clear();
  ANCHNAM_lengths.reserve(ANCHNAM.size());
  for (const std::string& name : ANCHNAM)
  {
    // check whether the given names exceed 255 bytes
    if (name.size() > 255)
    {
        std::cout << "CHANNEL NAME CANNOT EXCEED 255 BYTES" << std::endl;
        return;
    }
    ANCHNAM_lengths.push_back(static_cast<uint8_t>(name.size()));
  }
}

void PMU::setChannelDigital(const std::vector<std::string>& dig_chnam_values, const std::vector<uint32_t>& dig_unit_values)
{
  // check whether the number of channel scales match channel names
  if (dig_chnam_values.size() != dig_unit_values.size())
  {
    std::cout << "VALUE LISTS FOR CHNAM AND SCALE HAVE TO BE THE SAME SIZE" << std::endl;
  }

  // save each string length to the vector one by one 
  DGCHNAM_lengths.clear();
  DGCHNAM_lengths.reserve(DGCHNAM.size());
  for (const std::string& name : DGCHNAM)
  {
    // check whether the given names exceed 255 bytes
    if (name.size() > 255)
    {
        std::cout << "CHANNEL NAME CANNOT EXCEED 255 BYTES" << std::endl;
        return;
    }
    DGCHNAM_lengths.push_back(static_cast<uint8_t>(name.size()));
  }
  DGCHNAM = dig_chnam_values;
  DIGUNIT = dig_unit_values;
  DGNMR = dig_chnam_values.size();
}


// setters (PMUMeasurements specific) (need to be called AFTER setting the channel names/scales)

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

