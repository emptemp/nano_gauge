#include <Filters.h>

const byte motorPin1 = 8;
const byte motorPin2 = 9;
const byte motorPin3 = 10;
const byte motorPin4 = 11;
const byte inputPin = 7;

int fss = 160;
int delayTime = 4255;

unsigned int a;
unsigned int b;
unsigned int c;
unsigned int d;

bool  ccw=0;
float e=2.71;
int   current=0;
int   pos = 0;


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
  ACSR = B01011010;
  turn(150,1);
  //turn(30,0);
  delay(1000);
}

void turn(int fullsteps, bool ccw)
{
  /// 1 schleifendurchgang entspricht 4 pi rad
  
  /// fz ist fss
  int fz = fss;
  
  /// muss degree/2 mal ausgeführt werden für degree
  for(int n=0; n<fullsteps; n++)
  {
    //Serial.print("n: ");
    //Serial.print(n);
    //Serial.print("\n");
    delayTime = 1000000/(3*fz);
    ///   jede schleife entspricht 2 fullsteps (2° / 4 pi)
    for(int k=5; k>=0; k--)
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
    }
  /// fz berechnen
  if(n < fullsteps/2)         { fz = fz + e*fz/pow(n+2,2);  }
  else if(n >= fullsteps/2)   { fz = fz - e*fz/pow(fullsteps-n+2,2);  }
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

int val;

float filterfq = 5;
FilterOnePole lowpassFilter(LOWPASS, filterfq);

void loop() 
{
  
  if ((duration > 300) && (duration < 11000))
  {
    int filtered = lowpassFilter.input( duration );
    if (filtered > 900) {    val = 113940/filtered; }
    //Serial.print(duration);
    //Serial.print("\n");
    Serial.print(val*1000);
    Serial.print("\t"); 
    Serial.println(filtered);
    position(val); 
  }
  
  //delay(100);
  //position(45);
  //position(46);
  //int rndm = random(1,40);
  //int pos1=70+rndm;
  //int pos2=70-rndm;
  //position(pos1);
  //delay(10);
  //position(pos2);
  //delay(10);
  //for(int i = 20; i > 0; i--)
  //{
  //position(70+i);
  //delay(50);
  //position(70-i);
  //delay(50);  
  //position(70);
  //delay(50);
  //}
  //delay(1000);
}

ISR(ANALOG_COMP_vect)
{
   current_time = micros();
   duration = current_time - previous_time;
   previous_time = current_time;
}
