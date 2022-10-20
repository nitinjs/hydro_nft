//Particle photon sketch
//advanced efficient hydroponic nft system
//released under GPL
//by Nitin Sawant
//nitin@nitinsawant.com

int r1 = D0;//relay

int t1 = D1;//trigger ss1
int e1 = D2;//echo ss1
int t2 = D3;//trigger ss2
int e2 = D4;//echo ss2

float distance1, distance2;
float upper_level,lower_level;

int bucket_height=40;//max height of bucket
int min_water = 5;
int max_water = 35;

bool ON = true;
bool OFF = false;
void setup() {
  Serial.begin(115200);
  pinMode(e1,INPUT);//echo pin of ultraSonic
  pinMode(t1,OUTPUT);//trig pin of ultraSonic
  
  pinMode(e2,INPUT);//echo pin of ultraSonic
  pinMode(t2,OUTPUT);//trig pin of ultraSonic
  
  pinMode(r1,OUTPUT);// relay
}

float GetWaterLevel(int dist){
  return bucket_height - dist;
}

float CheckWaterLevelSS1() //distance calculaion...
{
  digitalWrite(t1,HIGH);
  delayMicroseconds(8);
  digitalWrite(t1,LOW);
  delayMicroseconds(2);
  
  float duration1 = pulseIn(e1,HIGH);
  distance1 = (duration1*0.0343)/2;
  //Serial.println("SS1: "+ String(distance1)+" cm");
  if(distance1>bucket_height){
    distance1=bucket_height;
  }

  return GetWaterLevel(distance1);
}

float CheckWaterLevelSS2() //distance calculaion...
{
  digitalWrite(t2,HIGH);
  delayMicroseconds(8);
  digitalWrite(t2,LOW);
  delayMicroseconds(2);
  
  float duration2 = pulseIn(e2,HIGH);
  distance2 = (duration2*0.0343)/2;
  //Serial.println("SS2: "+ String(distance2)+" cm");
  if(distance2>bucket_height){
    distance2=bucket_height;
  }

  return GetWaterLevel(distance2);
}

String GetPumpSwitchStatus(bool on){
  if(on){
    Serial.println("Switching pump: ON"); 
  }else{
    Serial.println("Switching pump: OFF"); 
  }
}

void SwitchPump(bool on){
  GetPumpSwitchStatus(on); 
  if(on){//turn on
    if(!PumpIsOn()){ 
      digitalWrite(r1,HIGH);// Pump On...
    }
  }else{//rurn off
    if(PumpIsOn()){
      digitalWrite(r1,LOW);// pump off...
    }
  }  
}

bool PumpIsOn(){
  bool o = (digitalRead(r1)==1);
  if(o){
    Serial.println("Pump is: ON");    
  }else{
    Serial.println("Pump is: OFF");    
  } 
  return o;  
}

void loop(){  
  lower_level = CheckWaterLevelSS2();  
  Serial.println("Lower Level: "+ String(lower_level)+" cm");
  
  delay(1000);
}
void loop2XXX() {
  upper_level = CheckWaterLevelSS1();
  lower_level = CheckWaterLevelSS2();
  Serial.println("SS1: "+ String(distance1)+" cm, " + "SS2: "+ String(distance2)+" cm");
  Serial.println("Upper Level: "+ String(upper_level)+" cm, " + "Lower Level: "+ String(lower_level)+" cm");

  bool action_done = false;
  if(lower_level<=min_water){//if water lower in bucket is finished
    SwitchPump(OFF);
    action_done = true;
  }else //if there is water in lower bucket
    if(upper_level<=min_water){//if water in upper bucket is finished
      SwitchPump(ON);
      action_done = true;
  }

  if(!action_done){
    PumpIsOn();
    Serial.println("PROGRAM IS IDLE");
  }
  
  delay(1000);
}
