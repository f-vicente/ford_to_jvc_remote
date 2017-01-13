#include <12F683.h>
#device adc=8

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES INTRC_IO                 //Internal RC Osc
#FUSES NOCPD                    //No EE protection
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOMCLR                   //Master Clear pin disabled
#FUSES PUT                      //Power Up Timer
#FUSES BROWNOUT                 //Reset when brownout detected
//#FUSES IESO                     //Internal External Switch Over mode enabled
//#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES RESERVED                 //Used to set the reserved FUSE bits

#use delay(clock=8000000)

