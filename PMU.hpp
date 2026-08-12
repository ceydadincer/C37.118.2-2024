/** 
* TODO: Implement cpp, figure out how the channels' characteristics and measurements should be saved separately
*/

#ifndef pmu_hpp
#define pmu_hpp

#include <vector>
#include <memory>
#include <array>
#include <cstdint>
#include <iostream>

class PMU
{
  struct PMUMeasurements
  {
    uint16_t STAT_FLAG;
    uint16_t TIMEQUALITY;

    std::vector<uint64_t> PHASOR_values = {};
    std::vector<uint32_t> FREQ_values = {};
    std::vector<uint32_t> DFREQ_values = {};
    std::vector<uint32_t> ANALOG_values = {};
    std::vector<uint16_t> DIGITAL_values = {};
  };

protected:
  // Fields unique to the PMU
  
  std::string PMU_NAME = "";
  uint16_t PMU_ID;
  uint16_t PMU_VERSION = 3;
  std::array<uint8_t, 16> G_PMU_ID;

  uint32_t PMU_LAT;
  uint32_t PMU_LON;
  uint32_t PMU_ELEV;

  uint32_t WINDOW;
  uint32_t GRP_DLY;
  uint16_t CFGCNT = 0; 

  // Fields unique to the stream
  
  uint16_t FORMAT;

  uint16_t PMUFLAG;

  uint16_t PMU_DATA_RATE;

  // number of channels
  uint16_t PHNMR = 0;
  uint16_t ANNMR = 0;
  uint16_t FRNMR = 0;
  uint16_t DFDTNMR = 0;
  uint16_t DGNMR = 0;
  // channel names for the CHNAM field, 1-256 bytes in total
  std::vector<std::string> PHCHNAM = {};
  std::vector<std::string> ANCHNAM = {};
  std::vector<std::string> FRCHNAM = {};
  std::vector<std::string> DFDTCHNAM = {};
  std::vector<std::string> DIGCHNAM = {};
  // conversion factors for channels
  std::vector<std::array<uint8_t, 16>> PHSCALE = {};
  std::vector<uint64_t> FRSCALE = {};
  std::vector<uint64_t> DFDTSCALE = {};
  std::vector<uint64_t> ANSCALE = {};
  // mask word for digital status words
  std::vector<uint32_t> DIGUNIT = {};

  // Fields unique to the frame
  std::unique_ptr<PMUMeasurements> Measurements; 
  
public:
  // Constructor
  PMU();
  /*
  !! If were to be used, the order in which the attributes are defined should match to the constructor
  PMU(
    std::string& pmu_name_value,
    uint16_t pmu_id_value,
    uint16_t pmu_version_value,
    std::array<uint8_t, 16> g_pmu_id_value,
    uint16_t format_value,
    uint16_t phnmr_value,
    uint16_t annmr_value,
    uint16_t frnmr_value,
    uint16_t dfdtnmr_value,
    uint16_t dgnmr_value,
    std::vector<std::string>& chnam_phasor_values,
    std::vector<std::string>& chnam_analog_values,
    std::vector<std::string>& chnam_freq_values,
    std::vector<std::string>& chnam_dfdt_values,
    std::vector<std::string>& chnam_digital_values,
    std::vector<std::array<uint8_t, 16>>& ph_scale_values,
    std::vector<uint64_t>& fr_scale_values,
    std::vector<uint64_t>& dfdt_scale_values,
    std::vector<uint64_t>& an_scale_values,
    std::vector<uint32_t>& dig_unit_values,
    uint32_t pmu_lat_value,
    uint32_t pmu_lon_value,
    uint32_t pmu_elev_value,
    uint16_t pmuflag_value,
    uint32_t window_value,
    uint32_t grp_dly_value,
    uint16_t pmu_data_rate_value,
    uint16_t cfg_cnt_value
  );
  */

  ~PMU();

  void setPMU_NAME(const std::string& pmu_name_value);
  void setPMU_ID(uint16_t pmu_id_value){PMU_ID = pmu_id_value;}
  void setPMU_VERSION(uint16_t pmu_version_value){PMU_VERSION = pmu_version_value;}
  void setG_PMU_ID(const std::array<uint8_t, 16>& g_pmu_id_value){G_PMU_ID = g_pmu_id_value;}
  void setPMU_LAT(uint32_t pmu_lat_value){PMU_LAT = pmu_lat_value;}
  void setPMU_LON(uint32_t pmu_lon_value){PMU_LON = pmu_lon_value;}
  void setPMU_ELEV(uint32_t pmu_elev_value){PMU_ELEV = pmu_elev_value;}
  void setWINDOW(uint32_t window_value){WINDOW = window_value;}
  void setGRP_DLY(uint32_t grp_dly_value){GRP_DLY = grp_dly_value;}
  void setCFGCNT(uint16_t cfg_cnt_value){CFGCNT = cfg_cnt_value;}

  void setFORMAT(uint16_t format_value){FORMAT = format_value;}
  void setPMUFLAG(uint16_t pmuflag_value){PMUFLAG = pmuflag_value;}
  // may be PMUs have a list of max data rates and this function can check whether it is in the list
  void setPMU_DATA_RATE(uint16_t pmu_data_rate_value){PMU_DATA_RATE = pmu_data_rate_value;}

  void setPHNMR(uint16_t phnmr_value){PHNMR = phnmr_value;}
  void setANNMR(uint16_t annmr_value){ANNMR = annmr_value;}
  void setFRNMR(uint16_t frnmr_value){FRNMR = frnmr_value;}
  void setDFDTNMR(uint16_t dfdtnmr_value){DFDTNMR = dfdtnmr_value;}
  void setDGNMR(uint16_t dgnmr_value){DGNMR = dgnmr_value;}

  void setPHCHNAM(const std::vector<std::string>& ph_chnam_values){PHCHNAM = ph_chnam_values;}
  void setANCHNAM(const std::vector<std::string>& an_chnam_values){ANCHNAM = an_chnam_values;}
  void setFRCHNAM(const std::vector<std::string>& freq_chnam_values){FRCHNAM = freq_chnam_values;}
  void setDFDTCHNAM(const std::vector<std::string>& dfdt_chnam_values){DFDTCHNAM = dfdt_chnam_values;}
  void setDIGCHNAM(const std::vector<std::string>& dig_chnam_values){DIGCHNAM = dig_chnam_values;}

  void setPHSCALE(const std::vector<std::array<uint8_t, 16>>& ph_scale_values){PHSCALE = ph_scale_values;}
  void setFRSCALE(const std::vector<uint64_t>& fr_scale_values){FRSCALE = fr_scale_values;}
  void setDFDTSCALE(const std::vector<uint64_t>& dfdt_scale_values){DFDTSCALE = dfdt_scale_values;}
  void setANSCALE(const std::vector<uint64_t>& an_scale_values){ANSCALE = an_scale_values;}
  void setDIGUNIT(const std::vector<uint32_t>& dig_unit_values){DIGUNIT = dig_unit_values;}

  void setChannelPhasor(const std::vector<std::string>& ph_chnam_values, const std::vector<std::array<uint8_t, 16> >& ph_scale_values);
  void setChannelAnalog(const std::vector<std::string>& an_chnam_values, const std::vector<uint64_t>& an_scale_values);
  void setChannelFrequency(const std::vector<std::string>& fr_chnam_values, const std::vector<uint64_t>& fr_scale_values);
  void setChannelDFDT(const std::vector<std::string>& dfdt_chnam_values, const std::vector<uint64_t>& dfdt_scale_values);
  void setChannelDigital(const std::vector<std::string>& dig_chnam_values, const std::vector<uint32_t>& dig_unit_values);

  void addChannelPhasor(const std::string& ph_chnam_value, const std::array<uint8_t, 16>& ph_scale_value);
  void addChannelAnalog(const std::string& an_chnam_value, uint64_t an_scale_value);
  void addChannelFrequency(const std::string& fr_chnam_value, uint64_t fr_scale_value);
  void addChannelDFDT(const std::string& dfdt_chnam_value, uint64_t dfdt_scale_value);
  void addChannelDigital(const std::string& dig_chnam_value, uint32_t dig_unit_value);

  void setSTAT_FLAG(uint16_t stat_flag_value){Measurements->STAT_FLAG = stat_flag_value;}
  void setTIMEQUALITY(uint16_t timequality_value){Measurements->TIMEQUALITY = timequality_value;}
  void setPHASOR_values(const std::vector<uint64_t>& phasor_values);
  void setFREQ_values(const std::vector<uint32_t>& freq_values);
  void setDFREQ_values(const std::vector<uint32_t>& dfreq_values);
  void setANALOG_values(const std::vector<uint32_t>& analog_values);
  void setDIGITAL_values(const std::vector<uint16_t>& digital_values);

  void addPhasorValue(uint64_t phasor_value, int idx);
  void addFrequencyValue(uint32_t freq_value, int idx);
  void addDfreqValue(uint32_t dfreq_value, int idx);
  void addAnalogValue(uint32_t analog_value, int idx);  
  void addDigitalValue(uint16_t digital_value, int idx);

  void setMeasurements(uint16_t stat_flag_value, uint16_t timequality_value, const std::vector<uint64_t>& phasor_values,
  const std::vector<uint32_t>& freq_values, const std::vector<uint32_t>& dfreq_values, const std::vector<uint32_t>& analog_values, 
  const std::vector<uint16_t>& digital_values);

  const std::string& getPMU_NAME() const {return PMU_NAME;}
  uint16_t getPMU_ID() const {return PMU_ID;}
  uint16_t getPMU_VERSION() const {return PMU_VERSION;}
  const std::array<uint8_t, 16>& getG_PMU_ID() const {return G_PMU_ID;}
  uint32_t getPMU_LAT() const {return PMU_LAT;}
  uint32_t getPMU_LON() const {return PMU_LON;}
  uint32_t getPMU_ELEV() const {return PMU_ELEV;}
  uint32_t getWINDOW() const {return WINDOW;}
  uint32_t getGRP_DLY() const {return GRP_DLY;}
  uint16_t getCFGCNT() const {return CFGCNT;}
  
  uint16_t getFORMAT() const {return FORMAT;}
  uint16_t getPMUFLAG() const {return PMUFLAG;}
  uint16_t getPMU_DATA_RATE() const {return PMU_DATA_RATE;}

  uint16_t getPHNMR() const {return PHNMR;}
  uint16_t getANNMR() const {return ANNMR;}
  uint16_t getFRNMR() const {return FRNMR;}
  uint16_t getDFDTNMR() const {return DFDTNMR;}
  uint16_t getDGNMR() const {return DGNMR;}

  const std::vector<std::string>& getPHCHNAM() const {return PHCHNAM;}
  const std::vector<std::string>& getANCHNAM() const {return ANCHNAM;};
  const std::vector<std::string>& getFRCHNAM() const {return FRCHNAM;};
  const std::vector<std::string>& getDFDTCHNAM() const {return DFDTCHNAM;};
  const std::vector<std::string>& getDGCHNAM() const {return DIGCHNAM;};

  const std::vector<std::array<uint8_t, 16>>& getPHSCALE() const {return PHSCALE;}
  const std::vector<uint64_t>& getFRSCALE() const {return FRSCALE;};
  const std::vector<uint64_t>& getDFDTSCALE() const {return DFDTSCALE;};
  const std::vector<uint64_t>& getANSCALE() const {return ANSCALE;}
  const std::vector<uint32_t>& getDIGUNIT() const {return DIGUNIT;}

};

#endif