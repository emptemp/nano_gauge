
int motorPin1 = 8;
int motorPin2 = 9;
int motorPin3 = 10;
int motorPin4 = 11;
int fss = 235;
int delayTime = 4255;

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  turn(960,1);
}

void turn(int partsteps, bool ccw)
{
  int a;
  int b;
  int c;
  int d;

  /// 1 schleifendurchgang entspricht 2pi rad
  ///  partsteps durch 6 ergibt die doppelten fullsteps
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
  //if(fz < 600)         { fz = fz + fz/(3*n);  }
  //else if(n >= fullsteps2 - 7)   { fz = fz - fz/(3*(fullsteps2-n);  }
    
  /// fz berechnen
  if(n < fullsteps2/2)         { fz = fz + fz/pow(2, n+1);  }
  else if(n >= fullsteps2/2)   { fz = fz - fz/pow(2, fullsteps2-n);  }
  }
}
void loop() 
{  
  for(int k = 0; k<=30; k++)
  {
  turn(12,0);
  delay(1000);
  }
  
  for(int k = 0; k<=30; k++)
  {
  turn(12,1);
  delay(1000);
  }
}
