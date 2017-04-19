#include <Filters.h>

const byte motorPin1 = 8;
const byte motorPin2 = 9;
const byte motorPin3 = 10;
const byte motorPin4 = 11;
const byte inputPin = 7;
int fss = 240;
int delayTime = 4255;

unsigned int a;
unsigned int b;
unsigned int c;
unsigned int d;

bool  ccw=0;
float e=2.71;
int   current=0;
int   pos = 0;
long  val = 0;

volatile unsigned long current_time = 0;
long previous_time = 0;
volatile unsigned long duration;


void setup() { 
  Serial.begin(19200);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(inputPin, INPUT);
  ACSR = B01011011;
  //attachInterrupt(digitalPinToInterrupt(inputPin), count, FALLING);
  
  turn(960,1);
  turn(30,0);
}

void turn(int partsteps, bool ccw)
{
  /// 1 schleifendurchgang entspricht 2pi rad
  /// partsteps durch 6 ergibt die doppelten fullsteps
  /// rest wird in partsteps ausgeführt
  int fullsteps2 = partsteps / 6;
  int rest = partsteps % 6;
  int halt = 0;

  /// fz ist fss
  int fz = fss;
  
  ///  printf("%d %d\n", fullsteps, rest);
  /// muss degree/2 mal ausgeführt werden für degree
  for(int n=0; n<=fullsteps2; n++)
  {
    delayTime = 1000000/(3*fz);
    ///   jede schleife entspricht 2 fullsteps (2°)
    ///    restlichen partsteps für alle partsteps
    if(n == fullsteps2) { halt = 6 - rest; } 
    for(int k=5; k>=halt; k--)
    {               
      a = 1 & (56 >> abs(5*ccw - k)); ///00111000 >> k 
      b = 1 & (14 >> abs(5*ccw - k)); ///00011100 >> k
      c = 1 & (35 >> abs(5*ccw - k)); ///00111000 >> k 
                                      ///ccw := counterclockwise
      digitalWrite(motorPin1, a);
      digitalWrite(motorPin2, b);
      digitalWrite(motorPin3, b);
      digitalWrite(motorPin4, c);
      
      delayMicroseconds(delayTime);
      //printf("%d  %d  %d\n", a,b,c);
    }
  /// fz berechnen
  if(n < fullsteps2/2)         { fz = fz + e*fz/pow(n+2,2);  }
  else if(n >= fullsteps2/2)   { fz = fz - e*fz/pow(fullsteps2-n+2,2);  }
  }
}

void position(int future)
{
   future = future - current;
   current = current + future;
   if (future < 0)      { turn(abs(future), 1); }
   else if (future > 0) { turn(future, 0); }
   else                 {  }  
}

float filterfq = 2.5;
FilterOnePole lowpassFilter(LOWPASS, filterfq);
void loop() 
{
   //Serial.print("duration ");
   //Serial.print(duration);
   //Serial.print(" | val: ");
   //Serial.print(val);
   //Serial.print("\n");
   int filtered = lowpassFilter.input( duration );
   //Serial.print("duration ");
   //Serial.print(duration);
   //Serial.print(" | fitered ");
   //Serial.print(filtered);
   //Serial.print("\n");

   if((filtered > 1000) && (filtered < 10000))
   {
     val = 500000/filtered;
     if (abs(val - pos) > 12) { 
      position(val);  pos = val; 
      }
   //Serial.print("done\n");
   }
   else {  position(val); }
   Serial.print(val);
   //if ((duration > 1000) && (duration < 10000))
   //{  
   //}
   //position(val);  pos = val;
}

ISR(ANALOG_COMP_vect)
{
   current_time = micros();
   duration = current_time - previous_time;
   previous_time = current_time;
}
