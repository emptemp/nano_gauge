#define initMotor   (0)

#define EMA_ALPHA   (1)

const byte motorPin1 = 8;
const byte motorPin2 = 9;
const byte motorPin3 = 10;
const byte motorPin4 = 11;
const byte inputPin = 7;

const uint8_t fss = 160;
uint16_t delayTime = 4255;

uint8_t a;
uint8_t b;
uint8_t c;
uint8_t d;

bool  ccw=0;
float e=2.71;
uint16_t current=0;
uint16_t pos = 0;

volatile uint32_t current_time = 0;
volatile uint32_t previous_time = 0;
volatile uint32_t duration;

const uint16_t fbuflen = 50;
uint16_t fbuf[fbuflen];
uint8_t i = 0;

void setup() { 
  Serial.begin(115200);
  ACSR = B01011010;
  
#if initMotor
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(inputPin, INPUT);
  turn(150,1);
  //turn(30,0);
  delay(1000);
#endif
}

void turn(int16_t fullsteps, bool ccw)
{
  /// 1 schleifendurchgang entspricht 4 pi rad
  uint16_t fz = fss;
  
  /// muss degree/2 mal ausgeführt werden für degree
  for(int16_t n=0; n<fullsteps; n++)
  {
    //Serial.print("n: ");
    //Serial.print(n); 
    //Serial.print("\n");
    delayTime = 1000000/(3*fz);
    ///   jede schleife entspricht 2 degree (2° / 4 pi)
    for(int8_t k=5; k>=0; k--)
    {               
      a = 1 & (0x38 >> abs(5*ccw - k)); ///00111000 >> k 
      b = 1 & (0x0E >> abs(5*ccw - k)); ///00011100 >> k
      c = 1 & (0x23 >> abs(5*ccw - k)); ///00111000 >> k 
                                        ///ccw := counterclockwise
      digitalWrite(motorPin1, a);
      digitalWrite(motorPin2, b);
      digitalWrite(motorPin3, b);
      digitalWrite(motorPin4, c);
      delayMicroseconds(delayTime);
    }
    /// fz berechnen
    if(n < fullsteps/2)         { fz = fz + e*fz/pow(n+2,2);  }
    else if(n >= fullsteps/2)   { fz = fz - e*fz/pow(fullsteps-n+2,2);  }

    //Serial.print(fz);
    //Serial.print("\n");
  }
}

void position(int16_t future)
{
   future = future - current;
   current = current + future;
   if (future < 0)      { turn(abs(future), 1); }
   else if (future > 0) { turn(future, 0); }
   else                 {  }  
}

uint16_t sma()
{
  uint32_t average = 0;
  for(uint16_t k = 0; k < fbuflen; k++)
  {
    average += fbuf[k];
  }
  return average/fbuflen;
}

uint16_t ema()
{ 
    static uint16_t exponential_average=duration;
    
    exponential_average=(EMA_ALPHA*(uint32_t)duration + (100 - EMA_ALPHA)*(uint32_t)exponential_average)/100;
    return exponential_average;
}

void loop() 
{
  noInterrupts();
  Serial.print(duration);
  Serial.print(",");
  Serial.print(sma());
  Serial.print(",");
  Serial.print(ema());
  Serial.print("\n");
  interrupts();
}

ISR(ANALOG_COMP_vect)
{
   current_time = micros();
   duration = current_time - previous_time;
   previous_time = current_time;
   fbuf[i%fbuflen] = duration; i++;
}
