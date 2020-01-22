void initSound() {
  pinMode(speaker, OUTPUT); 
  ASSR &= ~(_BV(EXCLK) | _BV(AS2)); 
  TCCR2A |= _BV(WGM21) | _BV(WGM20);
  TCCR2B &= ~_BV(WGM22);
  // Do non-inverting PWM on pin OC2A (p.155)
  // On the Arduino this is pin 11.
  TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
  TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
  // No prescaler (p.158)
  TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  // Set initial pulse width to the first sample.
  OCR2A = 0;
  // Set up Timer 1 to send a sample every interrupt.
  noInterrupts();
  // Set CTC mode (Clear Timer on Compare Match) (p.133)
  // Have to set OCR1A *after*, otherwise it gets reset to 0!
  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
  // No prescaler (p.134)
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  // Set the compare register (OCR1A).
  // OCR1A is a 16-bit register, so we have to do this with
  // interrupts disabled to be safe.
  OCR1A = F_CPU / SAMPLE_RATE;    // 16e6 / 8000 = 2000
  // Enable interrupt when TCNT1 == OCR1A (p.136)
  TIMSK1 |= _BV(OCIE1A);
  interrupts(); 
}
void soundx(){
   // blank so you can check picked up noise
  }
  
  void sound0(){
    outValue = t+(t&t^t>>(b*2-c))-t*((t>>a)&(t%c?2:(a-c))&t>>b);    
    OCR2A =  outValue;
    t++;
  }

   void sound1(){
    outValue = ((t & ((t >> a))) + (t | ((t >> b)))) & (t >> (c + 1)) | (t >> a) & (t * (t >> b));
    OCR2A =  outValue;
    t++;
  }
    void sound2(){
    if (t > 65536) t = -65536;
      outValue = (t >> c | a | t >> (t >> 16)) * b + ((t >> (b + 1)) & (a + 1));
    OCR2A =  outValue;
    t++;
  }
    void sound3(){
    //outValue = t>>6^t&37|t+(t^t>>11)-t*((t%a?2:6)&t>>11)^t<<1&(t&b?t>>4:t>>10);
    outValue = t >> c ^ t & 37 | t + (t ^ t >> a) - t * ((t >> a ? 2 : 6)&t >> b)^t << 1 & (t & b ? t >> 4 : t >> 10);
    OCR2A =  outValue;
    t++;
  }
    void sound4(){
    // outValue = t>>6^t&37|t+(t^t>>11)-t*((t%a?2:6)&t>>11)^t<<1&(t&b?t>>4:t>>10);
    outValue = b * t >> a ^ t & (37 - c) | t + ((t ^ t >> 11)) - t * ((t >> 6 ? 2 : a)&t >> (c + b))^t << 1 & (t & 6 ? t >> 4 : t >> c);
    OCR2A =  outValue;
    t++;
  }
    void sound5(){
    // t>>6^t&37|t+(t^t>>11)-t*((t%a?2:6)&t>>11)^t<<1&(t&b?t>>4:t>>10);
    //outValue = t+(t&t^t>>6)-t*((t>>9)&(t%16?2:6)&t>>9)
    outValue = c * t >> 2 ^ t & (30 - b) | t + ((t ^ t >> b)) - t * ((t >> 6 ? a : c)&t >> (a))^t << 1 & (t & b ? t >> 4 : t >> c);
    OCR2A =  outValue;
    t++;
  }
    void sound6(){
    //outValue = ((t>>a&t)-(t>>a)+(t>>a&t))+(t*((t>>b)&b));
    outValue = ((t >> a & t) - (t >> a) + (t >> a & t)) + (t * ((t >> c)&b));
    OCR2A =  outValue;
    t++;
  }
    void sound7(){
    //SE CUELGA A 16KHz
    // outValue = ((t % 42 + b) * (a >> t) | (128 & b) - (t >> a)) % (t >> b) ^ (t & (t >> c));
    //outValue =  t>>b&t?t>>a:-t>>c ;
    outValue =  t >> b & t ? t >> a : -t >> c ;
    OCR2A =  outValue;
    t++;
  }
    void sound8(){
    //16KHz only work by 65536 loops
    if (t > 65536) t = -65536;
    outValue = (t >> a | c | t >> (t >> 16)) * b + ((t >> (b + 1)));
    OCR2A =  outValue;
    t++;
  }
    void sound9(){
    // outValue = ((t*(t>>a|t>>(a+1))&b&t>>8))^(t&t>>13|t>>6);
    outValue = ((t * (t >> a | t >> (a & c))&b & t >> 8)) ^ (t & t >> c | t >> 6);
    OCR2A =  outValue;
    t++;
  }
    void sound10(){
    //outValue = ((t>>32)*7|(t>>a)*8|(t>>b)*7)&(t>>7);
    outValue = ((t >> c) * 7 | (t >> a) * 8 | (t >> b) * 7) & (t >> 7);
    OCR2A =  outValue;
    t++;
  }
    void sound11(){
     // % is a too heavy operatin for atmel 328. Should not be used in any equation. Change it
     // outValue = ((t >> a % (128-b<<t))) * b * t >>( c*t<<4) * t >> 18 ;
     //DEFFO
     outValue = ((t >> a % (128 - b << (t >> (9 - c))))) * b * t >> ( c * t << 4) * t >> 18 ;
     // outValue = ((t >> a % (128-b<<(t>>(9-c))))) * b * t >>( c*t<<4) * t >> 18+(t >> c ? 2 : a)&t * (t >> b) ;
     // outValue = ((t >> 6 ? 2 : a)&t * (t >> c) | ( b) - (t >> a)) % (t >> b) + (4 | (t >> c));
     // outValue = ((t >> b ? c : a)&t * (a) | ( 8) - (t >> 1)) % (t >> b) + (4 | (t >> c));
    OCR2A =  outValue;
    t++;
  }
    void sound12(){
    // moola long
    outValue = (t * 12 & t >> a | t * b & t >> c | t * b & c / (b << 2)) - 2;
    //outValue = (t * a & t >> b | t * c & t >> 7 | t * 3 & t / 1024) - 1;
    OCR2A =  outValue;
    t++;
  }
    void sound13(){
    //moola viznu
    //outValue = (t * 5 & t >> 7) | (t * 3 & t >> 10);
    outValue = ((t * (t >> a) & (b * t >> 7) & (8 * t >> c)));
    OCR2A =  outValue;
    t++;
  }
    void sound14(){
    //Both cool. What to choose?
    //outValue=t>>c^t&1|t+(t^t>>21)-t*((t>>4?b:a)&t>>12)^t<<1&(a&12?t>>4:t>>10);
    outValue = t >> c ^ t & 1 | t + (t ^ t >> 21) - t * ((t >> 4 ? b : a)&t >> (12 - (a >> 1)))^t << 1 & (a & 12 ? t >> 4 : t >> 10);
    OCR2A =  outValue;
    t++;
  }
    void sound15(){
    //outValue = (t*((t>>a|t<<c)&29&t>>b));
    //((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t))
    //outValue=((t&(a<<a))?((t*(t^t%b)|(t>>c))>>1):(t>>3)|((t&(b<<b))?t<<2:t));
    outValue = ((t & (4 << c)) ? ((t * (t ^ t % b) | (t >> b)) >> 1) : (t >> a) | ((t & (c << b)) ? t << 1 : t));
    OCR2A =  outValue;
    t++;
  }

  void (*jumpTable[])(void) = {   
  sound0, sound1, sound2, sound3, sound4, sound5, sound6, sound7, sound8,
  sound9, sound10, sound11, sound12, sound13, sound14, sound15
};

ISR(TIMER1_COMPA_vect) {
  jumpTable[programNumber](); // go to sound routine
  }
