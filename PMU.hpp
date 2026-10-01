#ifndef pmu_hpp
#define pmu_hpp

#include <vector>
#include <memory>
#include <array>
#include <cstdint>
#include <iostream>

/**
 * PMU class: 
 * A class to store attributes and their setters/getters for PMU related fields used in the frames.
 * Channel names (CHNAM) and conversion factors (SCALE) are set together.
 * PMUMeasurements is defined separately and its attributes are used by DataFrame. They should be set after 
 * CHNAM and SCALE values are set (should be during CapabilityConfigFrame construction).
 */
class PMU
{
  // a struct to separate DataFrame fields from others
  struct PMUMeasurements
  {
    uint16_t STAT_FLAG;
    uint16_t TIMEQUALITY;

    std::vector<uint64_t> PHASOR_values;
    std::vector<uint32_t> FREQ_values;
    std::vector<uint32_t> DFREQ_values;
    std::vector<uint32_t> ANALOG_values;
    std::vector<uint16_t> DIGITAL_values;
  };

protected:
  uint8_t PMU_NAME_length;
  std::string PMU_NAME;
  uint16_t PMU_ID;
  // set to 3 by default
  uint16_t PMU_VERSION = 0x03;
  std::array<uint8_t, 16> G_PMU_ID;
  uint16_t FORMAT;

  // number of channels
  uint16_t PHNMR;
  uint16_t ANNMR;
  uint16_t FRNMR;
  uint16_t DFDTNMR;
  uint16_t DGNMR;

  // channel names length
  std::vector<uint8_t> PHCHNAM_lengths;
  std::vector<uint8_t> ANCHNAM_lengths;
  std::vector<uint8_t> FRCHNAM_lengths;
  std::vector<uint8_t> DFDTCHNAM_lengths;
  std::vector<uint8_t> DGCHNAM_lengths;

  // channel names for the CHNAM field, 1-256 bytes in total
  std::vector<std::string> PHCHNAM;
  std::vector<std::string> ANCHNAM;
  std::vector<std::string> FRCHNAM;
  std::vector<std::string> DFDTCHNAM;
  std::vector<std::string> DGCHNAM;

  // conversion factors for channels
  std::vector<std::array<uint8_t, 16>> PHSCALE;
  std::vector<uint64_t> FRSCALE;
  std::vector<uint64_t> DFDTSCALE;
  std::vector<uint64_t> ANSCALE;

  // mask word for digital status words
  std::vector<uint32_t> DIGUNIT;

  uint32_t PMU_LAT;
  uint32_t PMU_LON;
  uint32_t PMU_ELEV;
  uint16_t PMUFLAG;
  uint32_t WINDOW;
  uint32_t GRP_DLY;
  uint16_t PMU_DATA_RATE;
  uint16_t CFGCNT; 

  // a pointer to PMUMeasurements object for access to DataFrame fields
  std::unique_ptr<PMUMeasurements> Measurements; 
  
public:
  PMU();

  // setters (PMU specific)

  void setPMU_NAME(const std::string& pmu_name_value);
  void setPMU_ID(uint16_t pmu_id_value){PMU_ID = pmu_id_value;}
  void setPMU_VERSION(uint16_t pmu_version_value){PMU_VERSION = pmu_version_value;}
  void setG_PMU_ID(const std::array<uint8_t, 16>& g_pmu_id_value){G_PMU_ID = g_pmu_id_value;}
  void setFORMAT(uint16_t format_value){FORMAT = format_value;}
  
  // set channel name and scale values and the number of channels together
  void setChannelPhasor(const std::vector<std::string>& ph_chnam_values, const std::vector<std::array<uint8_t, 16> >& ph_scale_values);
  void setChannelAnalog(const std::vector<std::string>& an_chnam_values, const std::vector<uint64_t>& an_scale_values);
  void setChannelFrequency(const std::vector<std::string>& fr_chnam_values, const std::vector<uint64_t>& fr_scale_values);
  void setChannelDFDT(const std::vector<std::string>& dfdt_chnam_values, const std::vector<uint64_t>& dfdt_scale_values);
  void setChannelDigital(const std::vector<std::string>& dig_chnam_values, const std::vector<uint32_t>& dig_unit_values);

  void setPMU_LAT(uint32_t pmu_lat_value){PMU_LAT = pmu_lat_value;}
  void setPMU_LON(uint32_t pmu_lon_value){PMU_LON = pmu_lon_value;}
  void setPMU_ELEV(uint32_t pmu_elev_value){PMU_ELEV = pmu_elev_value;}
  void setPMUFLAG(uint16_t pmuflag_value){PMUFLAG = pmuflag_value;}
  void setWINDOW(uint32_t window_value){WINDOW = window_value;}
  void setGRP_DLY(uint32_t grp_dly_value){GRP_DLY = grp_dly_value;}
  void setPMU_DATA_RATE(uint16_t pmu_data_rate_value){PMU_DATA_RATE = pmu_data_rate_value;}
  void setCFGCNT(uint16_t cfg_cnt_value){CFGCNT = cfg_cnt_value;}


  // setters (PMUMeasurements specific)
  
  void setSTAT_FLAG(uint16_t stat_flag_value){Measurements->STAT_FLAG = stat_flag_value;}
  void setTIMEQUALITY(uint16_t timequality_value){Measurements->TIMEQUALITY = timequality_value;}
  void setPHASOR_values(const std::vector<uint64_t>& phasor_values);
  void setFREQ_values(const std::vector<uint32_t>& freq_values);
  void setDFREQ_values(const std::vector<uint32_t>& dfreq_values);
  void setANALOG_values(const std::vector<uint32_t>& analog_values);
  void setDIGITAL_values(const std::vector<uint16_t>& digital_values);


  // getters (PMU specific)

  uint8_t getPMU_NAME_length() const {return PMU_NAME_length;}
  const std::string& getPMU_NAME() const {return PMU_NAME;}
  uint16_t getPMU_ID() const {return PMU_ID;}
  uint16_t getPMU_VERSION() const {return PMU_VERSION;}
  const std::array<uint8_t, 16>& getG_PMU_ID() const {return G_PMU_ID;}
  uint16_t getFORMAT() const {return FORMAT;}

  uint16_t getPHNMR() const {return PHNMR;}
  uint16_t getANNMR() const {return ANNMR;}
  uint16_t getFRNMR() const {return FRNMR;}
  uint16_t getDFDTNMR() const {return DFDTNMR;}
  uint16_t getDGNMR() const {return DGNMR;}

  const std::vector<uint8_t>& getPHCHNAM_lengths() const {return PHCHNAM_lengths;}
  const std::vector<uint8_t>& getANCHNAM_lengths() const {return FRCHNAM_lengths;};
  const std::vector<uint8_t>& getFRCHNAM_lengths() const {return DFDTCHNAM_lengths;};
  const std::vector<uint8_t>& getDFDTCHNAM_lengths() const {return ANCHNAM_lengths;};
  const std::vector<uint8_t>& getDGCHNAM_lengths() const {return DGCHNAM_lengths;};

  const std::vector<std::string>& getPHCHNAM() const {return PHCHNAM;}
  const std::vector<std::string>& getFRCHNAM() const {return FRCHNAM;}
  const std::vector<std::string>& getDFDTCHNAM() const {return DFDTCHNAM;}
  const std::vector<std::string>& getANCHNAM() const {return ANCHNAM;}
  const std::vector<std::string>& getDGCHNAM() const {return DGCHNAM;}

  const std::vector<std::array<uint8_t, 16>>& getPHSCALE() const {return PHSCALE;}
  const std::vector<uint64_t>& getFRSCALE() const {return FRSCALE;};
  const std::vector<uint64_t>& getDFDTSCALE() const {return DFDTSCALE;};
  const std::vector<uint64_t>& getANSCALE() const {return ANSCALE;}
  const std::vector<uint32_t>& getDIGUNIT() const {return DIGUNIT;}

  uint32_t getPMU_LAT() const {return PMU_LAT;}
  uint32_t getPMU_LON() const {return PMU_LON;}
  uint32_t getPMU_ELEV() const {return PMU_ELEV;}
  uint16_t getPMUFLAG() const {return PMUFLAG;}
  uint32_t getWINDOW() const {return WINDOW;}
  uint32_t getGRP_DLY() const {return GRP_DLY;}
  uint16_t getPMU_DATA_RATE() const {return PMU_DATA_RATE;}
  uint16_t getCFGCNT() const {return CFGCNT;}

  // getters (PMUMeasurements specific)
  
  uint16_t getSTAT_FLAG() const {return Measurements->STAT_FLAG;}
  uint16_t getTIMEQUALITY() const {return Measurements->TIMEQUALITY;}
  const std::vector<uint64_t>& getPHASOR_values() const {return Measurements->PHASOR_values;}
  const std::vector<uint32_t>& getFREQ_values() const {return Measurements->FREQ_values;}
  const std::vector<uint32_t>& getDFREQ_values() const {return Measurements->DFREQ_values;}
  const std::vector<uint32_t>& getANALOG_values () const {return Measurements->ANALOG_values;}
  const std::vector<uint16_t>& getDIGITAL_values() const {return Measurements->DIGITAL_values;}
};

#endif