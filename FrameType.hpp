// Macros for FrameType, not used in the program currently

// DATA = 000, HEADER = 001, CFG1 = 010, CFG2 = 011, CFG3 = 101, CMD = 100;
enum class FrameTypeVer2
{
  DATA = 0, 
  HEADER = 1,
  CFG1 = 2, 
  CFG2 = 3, 
  CFG3 = 5,  
  CMD = 4
};

// PERIODIC_DATA = 1000, DISCRETE_DATA = 1001, CAPABILITY = 1010, CONFIGURATION = 1011
// CMD = 1100, RENAME_CMD = 1101, CONFIG_CMD = 1110, ERROR_RESPONSE = 1111
enum class FrameTypeVer3
{
  PERIODIC_DATA = 8, 
  DISCRETE_DATA = 9,
  CAPABILITY = 10, 
  CONFIGURATION = 11, 
  CMD = 12,  
  RENAME_CMD = 13,
  CONFIG_CMD = 14,
  ERROR_RESPONSE = 15
};