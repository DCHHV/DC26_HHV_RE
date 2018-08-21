/*
 * DEF CON 26 HHV Challenge
 *   Challenge sketch
 *   @mediumrehr
 *   Aug 2018
 * 
 * Make sure to load the Setup sketch first!
 * 
 * When programming this sketch onto the challenge board,
 * make sure the bootloader clock speed is the same as the
 * clock speed set for uploading this sketch!
 */
 
 #include "obfuscator.h"
void setup(){s();Spl(s0);pm(0,0);pm(5,1);pm(7,1);pm(10,1);dw(5,0);dw(7,0);dw(10,0);if(cE()){dw(5,1);Spl(s1);Spl(s2);}Spl(s3);Spl(s4);}
void loop(){uint16_t key=ar(0);
  if(key<900){lks=key;d(50);
    if((key==lks)&(!kp)){
      kp=1;uint8_t value=0;
      if((key>740)&(key<800)){value=1;Spl((char)0x41);}
      else if((key>480)&(key<550)){value=2;Spl((char)0x42);}
      else if((key>=0)&(key<50)){value=3;Spl((char)0x43);}
      else if((key>650)&(key<730)){value=4;Spl((char)0x44);}
      up[ke]=value;cP(value,ke);ke++;
      if(ke>=8){ke=0;wp=0;for(i=0;i<4;i++){up[i]=0;}}
    }}else{kp=0;}
  d(100);}
void cP(uint8_t key, uint8_t count){
  if(!wp){if(key==pw[count]){d(20);}else{wp=1;}}
  dw(10,1);d(20);dw(10,0);
  if(count>=(8-1)){
    if(wp){dw(10,1);Spl(s7);d(1000);dw(10,0);}
    else{dw(7,1);Spl(s5);Spl(s6);}}}
