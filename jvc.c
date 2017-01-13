/*
 * Code needed to push buttons to the headunit from steering wheel port
 *
 * Before include this file you need to define a port as OUT
 */
 
 
 
int timer_count = 1;             // timer needs to overflow 1 time
 
//definition of codes, they are variables and not #define because are used later
//with some bit manipulation functions. could be improved by changin into #defines
int addr       = 0x47;
int vol_up     = 0x04;
int vol_down   = 0x05;
int skip_next  = 0x12;
int skip_prev  = 0x13;
int skip_up    = 0x14;
int skip_down  = 0x15;
//int mute       = 0x0E;
//int eq         = 0x0D;
int src        = 0x08;
 
// I won't explain how the algorithm works, just refer to 
// http://www.avforums.com/forums/portable-dvd-media-players-recorders/248455-jvc-stalk-adapter-diy.html
// to understand it.

void agc_bit(){
   output_low(OUT);
   delay_us(8500);
   output_high(OUT);
   delay_us(4000);
}
void start_bit(){
   output_low(OUT);
   delay_us(600);
   output_high(OUT);
   delay_us(1550);
}
void stop_bit(){
   output_low(OUT);
   delay_us(600);
   output_high(OUT);
   delay_us(1550);
   output_low(OUT);
   delay_us(600);
   output_high(OUT);
}
void logic0(){
   output_low(OUT);
   delay_us(600);
   output_high(OUT);
   delay_us(550);
}
void logic1(){
   output_low(OUT);
   delay_us(600);
   output_high(OUT);
   delay_us(1550);
}

void push(int button){
   int repeat;
   // send agc and start bits
   agc_bit();
   for (repeat=0;repeat<3;repeat++){
      setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
      timer_count = 1;
      set_rtcc(0);
      //send start
      start_bit();
      // send the addres 
      int r2;
      for (r2 = 0; r2<7; r2++){
         if(bit_test(addr,r2)){
            logic1();
         }else{
            logic0();
         }
      }
      // send the code
      for (r2 = 0; r2<7; r2++){
         if(bit_test(button,r2)){
            logic1();
         }else{
            logic0();
         }
      }
      // send stop bit
      stop_bit();
      // wait for a timer0 interrupt
      while(timer_count!=0){
      }
      delay_us(13460);
   }
   
}

#INT_TIMER0
void timer0(){
   timer_count--;
   clear_interrupt(int_timer0);
}
