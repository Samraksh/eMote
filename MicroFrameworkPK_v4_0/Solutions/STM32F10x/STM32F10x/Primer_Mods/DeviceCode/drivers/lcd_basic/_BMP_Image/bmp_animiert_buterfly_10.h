/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"
/* -------- Create Global variables ---------------------------------------------------*/

// filename    = C:\Users\Meiner\Documents\STM32Primer2\_Primer2BMP\_BMP_Image\animiert_buterfly_10.bmp
const u16 BMP_animiert_buterfly_10[32][40] = {
   0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDFFF , 0xDFFF , 0xDFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x8A52 , 0xA852 ,    0xAA52 , 0x8A52 , 0x8A52 , 0xFFFF , 0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xEF7B , 0xF07B , 0xEB7B , 0x0000 , 0x0300 , 0x0000 ,    0x0000 , 0x0000 , 0xF07B , 0xFFFF , 0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0x7DEF , 0xFFFF , 0xFFFF , 0x0000 , 0x0000 , 0x0400 , 0x4408 , 0x9209 , 0x494A , 0x4008 ,    0x4508 , 0x0000 , 0xF7BD , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDFFF , 0xDFFF , 0xFFFF , 0xFFFF ,    0x18C6 , 0x75AD , 0x75AD , 0x2000 , 0x2100 , 0x3F00 , 0x1200 , 0xF003 , 0xF7BD , 0x0100 , 0x1000 ,    0x2100 , 0x0000 , 0x7DEF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x18C6 , 0x18C6 , 0x79CE , 0x79CE , 0x59CE ,    0x6008 , 0x6108 , 0x0000 , 0x0E00 , 0x1F00 , 0x1E00 , 0x7100 , 0xA110 , 0x0E00 , 0x1000 , 0x0E00 ,    0x0000 , 0x8210 , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0x9AD6 , 0x59CE , 0x3DE7 , 0x9AD6 , 0x9BD6 , 0x6629 , 0x6629 , 0x8231 , 0x1800 ,    0x0800 , 0x0A00 , 0x1C00 , 0x1F00 , 0x1F00 , 0x0000 , 0x0900 , 0x1100 , 0x0F00 , 0x1000 , 0x0000 ,    0x0000 , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0x1CE7 , 0xB6B5 , 0x7ACE , 0xC95A , 0x0A63 , 0x0863 , 0x0000 , 0x0000 , 0x0F00 , 0x3900 , 0x3A00 ,    0x1900 , 0x1F00 , 0x1F00 , 0x1500 , 0x0D00 , 0x9202 , 0x0F00 , 0x0F00 , 0x1100 , 0x2000 , 0x0000 ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x7DEF , 0xB6B5 ,    0x9AD6 , 0x708C , 0x0000 , 0x0000 , 0x0500 , 0x2300 , 0x2300 , 0x3F00 , 0x1200 , 0x1F00 , 0x1F00 ,    0x1F00 , 0x1F00 , 0x1400 , 0x1F00 , 0x6F03 , 0x1000 , 0x1100 , 0x0C00 , 0x0000 , 0x0842 , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDFFF , 0xDFFF , 0x18C6 , 0x38C6 , 0x96B5 ,    0x0000 , 0x1200 , 0x1200 , 0x1F00 , 0x1200 , 0x1200 , 0x0E00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1F00 ,    0x7E00 , 0x1E00 , 0x1E00 , 0x1000 , 0x1000 , 0x0E00 , 0x0000 , 0x8210 , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x38C6 , 0x38C6 , 0x59CE , 0x59CE , 0x6108 , 0x0F00 ,    0x1F00 , 0x1E00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x0F00 , 0x1100 , 0x1F00 , 0x1F00 , 0x1F00 , 0xF003 ,    0x0300 , 0x1100 , 0x0F00 , 0x0F00 , 0x2100 , 0x0000 , 0x7DEF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0x9AD6 , 0xD7BD , 0xD7BD , 0x8631 , 0x8431 , 0x0800 , 0x0200 , 0x1F00 ,    0x1F00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x0E00 , 0x2E00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x5C01 , 0x0000 ,    0x0E00 , 0x1100 , 0x5208 , 0x0000 , 0xF7BD , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0x1CE7 , 0xB6B5 , 0x18C6 , 0x18C6 , 0x0000 , 0x0400 , 0x3A00 , 0x0D00 , 0x1F00 , 0x1500 ,    0x1700 , 0x1F00 , 0x1F00 , 0x1800 , 0x1000 , 0x1800 , 0x7502 , 0x1F00 , 0x1900 , 0x1400 , 0x1200 ,    0x2800 , 0x0400 , 0xF07B , 0xFFFF , 0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xB6B5 , 0x59CE , 0xF7BD , 0x18C6 , 0x0000 , 0x3300 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1200 , 0x0E00 ,    0x1F00 , 0x1F00 , 0x1F00 , 0x5001 , 0x0C00 , 0x7303 , 0x1B00 , 0x2E00 , 0x1B00 , 0x0C00 , 0x0000 ,    0x0642 , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x38C6 ,    0xD7BD , 0x18C6 , 0x18C6 , 0x6108 , 0x0E00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1000 , 0x1E00 ,    0x7E00 , 0x1F00 , 0xF003 , 0x1100 , 0x1F00 , 0x0F00 , 0x0F00 , 0x0000 , 0x0000 , 0x8210 , 0xFFFF ,    0xFFFF , 0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDFFF , 0xEF7B ,    0x38C6 , 0x18C6 , 0x96B5 , 0x0100 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1000 , 0x1000 , 0xF103 ,    0x1F00 , 0x7000 , 0x1F00 , 0x1100 , 0x1000 , 0x2100 , 0x2000 , 0x0000 , 0x7DEF , 0xFFFF , 0xFFFF ,    0x18C6 , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDBDE , 0x1084 ,    0xD7BD , 0x38C6 , 0x0000 , 0x2900 , 0x1F00 , 0x1600 , 0x1500 , 0x6F03 , 0x1B00 , 0x4E01 , 0x1F00 ,    0x0F00 , 0x1400 , 0x1000 , 0x0400 , 0x0000 , 0x0000 , 0xD7BD , 0xFFFF , 0x38C6 , 0x59CE , 0x5DEF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xD7BD , 0x1CE7 ,    0xDBDE , 0xCB5A , 0x0000 , 0x0F00 , 0x0600 , 0x0600 , 0x7102 , 0x1B00 , 0x1200 , 0x1900 , 0x1100 ,    0x0F00 , 0x0800 , 0x0000 , 0xF39C , 0xF39C , 0xFFFF , 0x9AD6 , 0xDBDE , 0xDBDE , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0x3CE7 , 0x8731 , 0x0000 , 0x0000 , 0x0000 , 0x0C00 , 0x0900 , 0x2C00 , 0x0A00 , 0x0C00 , 0x0C00 ,    0x0000 , 0x0000 , 0xDBDE , 0xDBDE , 0x3CE7 , 0x59CE , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0x59CE , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 ,    0x0000 , 0x0000 , 0x0000 , 0xD7BD , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x18C6 ,    0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 ,    0x0000 , 0x0000 , 0xD7BD , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x3CE7 , 0x9AD6 , 0x0100 ,    0x0000 , 0x0000 , 0x0C00 , 0x0C00 , 0x2900 , 0x0C00 , 0x0C00 , 0x0900 , 0x0B00 , 0x0000 , 0x0000 ,    0xDBDE , 0x3CE7 , 0x59CE , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x1CE7 , 0x1CE7 , 0xBEF7 , 0xCB5A , 0x0C63 , 0x0000 , 0x0F00 ,    0x0500 , 0x7102 , 0x1200 , 0x1B00 , 0x1200 , 0x1100 , 0x1B00 , 0x1200 , 0x0800 , 0x0000 , 0xF39C ,    0xFFFF , 0x9AD6 , 0xDBDE , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0x7DEF , 0xD7BD , 0xD7BD , 0x3084 , 0x0000 , 0x0000 , 0x0800 , 0x1F00 , 0x1700 ,    0x6F03 , 0x0E00 , 0x5D01 , 0x0E00 , 0x0F00 , 0x1D00 , 0x0E00 , 0x1200 , 0x0000 , 0x0000 , 0xD7BD ,    0xFFFF , 0x38C6 , 0x5DEF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xDFFF , 0xF7BD , 0x18C6 , 0x18C6 , 0x0000 , 0x0100 , 0x0200 , 0x1F00 , 0x1F00 , 0x1F00 , 0x0F00 ,    0x1100 , 0xEE03 , 0x1100 , 0x7000 , 0x0E00 , 0x1100 , 0x0F00 , 0x0100 , 0x2000 , 0x0000 , 0x7DEF ,    0xFFFF , 0x18C6 , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x38C6 ,    0xF7BD , 0x18C6 , 0x18C6 , 0x0000 , 0x0E00 , 0x1D00 , 0x1F00 , 0x1F00 , 0x1000 , 0x0F00 , 0x1E00 ,    0x6E00 , 0x1E00 , 0xF003 , 0x0F00 , 0x1F00 , 0x0F00 , 0x0F00 , 0x0000 , 0x0000 , 0x8210 , 0xFFFF ,    0xBEF7 , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xB6B5 , 0x18C6 ,    0x18C6 , 0x18C6 , 0x4108 , 0x3300 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1A00 , 0x1B00 , 0x1F00 , 0x1B00 ,    0x1F00 , 0x5001 , 0x0E00 , 0x7303 , 0x1B00 , 0x2E00 , 0x0900 , 0x0000 , 0x0000 , 0x0642 , 0xFFFF ,    0xBEF7 , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x1CE7 , 0xD7BD , 0x18C6 ,    0x18C6 , 0x0000 , 0x0400 , 0x3900 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1700 ,    0x1000 , 0x1800 , 0x7802 , 0x1F00 , 0x1900 , 0x1E00 , 0x0800 , 0x2B00 , 0x0400 , 0xF07B , 0xFFFF ,    0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x9AD6 , 0xD7BD , 0xD7BD ,    0x8631 , 0x8431 , 0x0800 , 0x1D00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x1F00 , 0x0E00 , 0x3000 ,    0x1F00 , 0x1C00 , 0x1F00 , 0x5C01 , 0x1600 , 0x1200 , 0x0C00 , 0x5208 , 0x0000 , 0xF7BD , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x38C6 , 0x38C6 , 0x59CE ,    0x59CE , 0x6108 , 0x0D00 , 0x1F00 , 0x1F00 , 0x1D00 , 0x1F00 , 0x1F00 , 0x1000 , 0x1000 , 0x1F00 ,    0x1000 , 0x1F00 , 0xF003 , 0x0000 , 0x0F00 , 0x0000 , 0x0F00 , 0x2100 , 0x0000 , 0x7DEF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDFFF , 0xDFFF , 0x18C6 , 0x38C6 ,    0x96B5 , 0x0000 , 0x1200 , 0x1200 , 0x0200 , 0x1200 , 0x1200 , 0x1000 , 0x0F00 , 0x1F00 , 0x1E00 ,    0x1F00 , 0x7E00 , 0x1C00 , 0x0E00 , 0x0E00 , 0x1000 , 0x0E00 , 0x0000 , 0x8210 , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x7DEF , 0xB6B5 , 0x9AD6 ,    0x708C , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x0000 , 0x2500 , 0x1100 , 0x1F00 , 0x1F00 , 0x1F00 ,    0x1F00 , 0x1F00 , 0x1D00 , 0x7103 , 0x1000 , 0x1100 , 0x0C00 , 0x0000 , 0x0842 , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x1CE7 , 0xB6B5 , 0x7ACE ,    0xC95A , 0x0A63 , 0x0D63 , 0x0A63 , 0x0A63 , 0x0000 , 0x2800 , 0x3A00 , 0x1900 , 0x1F00 , 0x5F00 ,    0x1F00 , 0x1900 , 0x6F02 , 0x3000 , 0x0F00 , 0x1100 , 0x2000 , 0x0000 , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x9AD6 , 0x59CE , 0x3DE7 ,    0x9AD6 , 0x9AD6 , 0x9AD6 , 0x9AD6 , 0x8631 , 0x0000 , 0x0800 , 0x0A00 , 0x1C00 , 0x1F00 , 0x1F00 ,    0x0D00 , 0x0F00 , 0x0F00 , 0x0F00 , 0x1000 , 0x0000 , 0x0000 , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0x18C6 ,    0x18C6 , 0x18C6 , 0x18C6 , 0x59CE , 0x6108 , 0x6108 , 0x0000 , 0x0E00 , 0x9F10 , 0x1F00 , 0x7000 ,    0x1000 , 0x9110 , 0x1000 , 0x0E00 , 0x0000 , 0x8210 , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDFFF , 0xDFFF ,    0xDFFF , 0xDFFF , 0x18C6 , 0x75AD , 0x75AD , 0x0000 , 0x2100 , 0x1AC6 , 0x1F00 , 0xEF03 , 0x1000 ,    0xF8BD , 0x1000 , 0x2100 , 0x0000 , 0x7DEF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0x7DEF , 0xFFFF , 0xFFFF , 0xF7BD , 0x0000 , 0x2221 , 0x4908 , 0x9209 , 0x4508 , 0x474A ,    0x4508 , 0x0000 , 0xF7BD , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xF07B , 0x0D63 , 0x0000 , 0x0300 , 0x0000 , 0x0000 , 0x0000 ,    0xF07B , 0xFFFF , 0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xDFFF , 0xFFFF , 0xFFFF , 0x8B52 , 0xA852 , 0x8A52 , 0xAA52 , 0x8A52 , 0xFFFF ,    0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xDFFF , 0xDFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF , 0xDFFF , 0xFFFF ,    0xFFFF , 0xFFFF , 0xFFFF , 0xFFFF 
} ;
