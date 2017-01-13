/*

 REMOTE CONTROL FOR JVC USING FORD FOCUS STEERING WHEEL CONTROL

 PIC: 12F683
       _ _
 VDD -| º |- VSS
 SIG -|   |- INPUT (GPIO 0)
     -|   |-
     -|___|-
     
 WHERE INPUT IS THE STEERING WHEEL CONTROL USING THIS DIAGRAM:
    _______
   |       |--GND-- TO GND/VSS
   |CONTROL|
   |_______|--SIG----0-------INPUT 
                     |
                     Z
                     Z R= 330 omh
                     Z
                     |
                    VDD (+5V)
   
  AND WHERE SIGNAL IS THE JVC REMOTE CONTROL PORT (A 3.5mm JACK SOCKET),
  THIS IS THE PLUG
             _______
   _________/
  |__|__|__|
            \_______
   
   ^      ^
  SIG    GND
  
  
  VDD NEEDS TO BE +5V, WHICH CAN BE DONE WITH SOME VOLTAGE REGULATOR LIKE LM78L05
  
  REFERENCES: http://www.avforums.com/forums/portable-dvd-media-players-recorders/248455-jvc-stalk-adapter-diy.html
*/ 

#include ".\mando.h"

#define OUT PIN_A5 

#define BUTTON_VOL_UP      1
#define BUTTON_VOL_DOWN    2
#define BUTTON_SEEK_UP     3
#define BUTTON_SEEK_DOWN   4
#define BUTTON_SRC         5

#include ".\jvc.c"

unsigned int ad = 0;             // holds 8-bit a/d value 0 to 255

void init(void){
   setup_oscillator(OSC_8MHZ);   // set for 8MHz internal osc
   output_high(OUT);              // 
   set_tris_a(0x01);             // gpio.0 input, rest outputs  
   setup_adc_ports(sAN0 );       // a/d enabled on gpio.0
   setup_adc(ADC_CLOCK_DIV_16);  // a/d clock /16 @8MHz,Tad=2uS
   //setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   //setup_timer_1(T1_DISABLED);
   //setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC); // disable comparator module
   //setup_vref(FALSE);
}

int get_button(){
   set_adc_channel(0);            // set a/d channel to gpio.0
   while(!adc_done()) {   
                                    // wait until read is done
   }
   ad = read_adc();               // read a/d into ad variable
   if(20<=ad && ad<50){
      return BUTTON_VOL_DOWN;
   }else if(50<=ad && ad<90){
      return BUTTON_VOL_UP;
   }else if(90<=ad && ad<130){
      return BUTTON_SEEK_UP;
   }else if(130<=ad && ad<170){
      return BUTTON_SEEK_DOWN;
   }else if(170<=ad && ad<220){
      return BUTTON_SRC;
   }
   return -1;
}


int check_button(unsigned int val){
   // wait 0.1 secs then check if is the same button
   delay_ms(100);
   if(get_button()==val){
      return true;
   }else{
      return false;
   }
}



void main()
{
   init();
   enable_interrupts(GLOBAL);      
   enable_interrupts(INT_TIMER0);   // enable TIMER0
   
   while(TRUE)                      // continuous loop
   {
     switch(get_button()){
      case BUTTON_VOL_UP:
         if(check_button(BUTTON_VOL_UP)){
            push(vol_up);
         }
      break;
      case BUTTON_VOL_DOWN:
         if(check_button(BUTTON_VOL_DOWN)){
            push(vol_down);
         }
      break;
      case BUTTON_SEEK_UP:
         if(check_button(BUTTON_SEEK_UP)){
            //wait 0.2s and if the button is pressed send up
            delay_ms(200);
            if(get_button()==BUTTON_SEEK_UP){
               push(skip_up);               
            }else{
               //send next track
               push(skip_next);
            }
         }
      break;
      case BUTTON_SEEK_DOWN:
         if(check_button(BUTTON_SEEK_DOWN)){
            //wait 0.2s and if the button is pressed send down
            delay_ms(200);
            if(get_button()==BUTTON_SEEK_DOWN){
               push(skip_down);
            }else{
               //send previous track
               push(skip_prev);               
            }            
         }
      break;
      case BUTTON_SRC:
         if(check_button(BUTTON_SRC)){
            push(src);             
         }
      break;
      default:
         ad=0;
      break;
     }
   }
}
