#include "../modules/models.h"
#include "../dictionary.h"
#include "../dependencies/parallel_hashmap/phmap.h"



bool test_models_bytes_to_float_conversion_05()
{  
  bool passed = false;
  int stride = 4;
  unsigned char byte_arr[stride];
  byte_arr[0] = 0x00;
  byte_arr[1] = 0x00;
  byte_arr[2] = 0x00;
  byte_arr[3] = 0x3F;

  float value = models::_convert_bytes_to_float(byte_arr, stride);

  if(value == 0.5f)
  {
    passed = true;
  }
  return passed;
}

bool test_models_bytes_to_float_conversion_51()
{  
  bool passed = false;
  int stride = 4;
  unsigned char byte_arr[stride];
  byte_arr[0] = 0x33;
  byte_arr[1] = 0x33;
  byte_arr[2] = 0xA3;
  byte_arr[3] = 0x40;

  float value = models::_convert_bytes_to_float(byte_arr, stride);

  if(value == 5.1f)
  {
    passed = true;
  }
  return passed;
}


bool test_models_float_to_bytes_conversion()
{
  bool passed = true;
  models::_convert_float_to_bytes(1);

  // Hexadecimal (base 16) conversion -> counting from 0 to F. Prefix Hex constants with 0x
  // 0 1 2 3 4 5 6 7 8 9 A B C D E F
  // Multiply the last digit by 1, 
  // Multiply the second to last digit by 16, 
  // Multiply the third to the last digit by 16 x 16, 
  // Multiply the fourth to the last digit by 16 x 16 x 16, Multiply the fifth to the last digit by 16 x 16 x 16 x 16 and so on until all the digits are used.
  // The byte #0 is 0x00 -> 0
  // The byte #1 is 0x00 -> 0 
  // The byte #2 is 0x00 -> 0 
  // The byte #3 is 0x3F -> 63

  return passed;
}