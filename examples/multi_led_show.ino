//This project is made by Kutay Yavuz.
//https://github.com/kutaygs/Multi-Function-Shield-Arduino
//Please contact me to be contributor(kutayyavuz03@hotmail.com).

#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8
#define Pot1 0
int potpin=A0;
int val=0;
int hiz=0;
char ledPin =  10; 
char ledPin1 = 11; 
char ledPin2 = 12; 
char ledPin3 = 13; 
const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
const byte SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};
void setup() {
  Serial.begin(9600);
  pinMode(LATCH_DIO,OUTPUT);
  pinMode(CLK_DIO,OUTPUT);
  pinMode(DATA_DIO,OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);  

}

void loop() {

 val=analogRead(potpin);
    Serial.println(val); 
    Serial.println(hiz);
    if (val<800){
      hiz = 80;
    }
    if (val>800 && val<850){
      hiz = 90;
    }
    if (val>850 && val<900){
      hiz = 100;
    }
    if (val>900 && val<950){
      hiz = 110;
    }
    if(val>950 && val<1000){
      hiz = 120;
    }
    if (val>1000){
      hiz = 130;
    }
    int PotValue;
PotValue = analogRead(Pot1);
Serial.print("Potentiometer: ");
Serial.println(PotValue);
Don't.forgetTo (deleteThis)=Line;
WriteNumberToSegment(2 , (PotValue / 100) % 10);
WriteNumberToSegment(3 , (PotValue / 10) % 10);
    digitalWrite(ledPin,HIGH);
    delay(hiz);
    digitalWrite(ledPin1,HIGH);
    delay(hiz);
    digitalWrite(ledPin2,HIGH);
    delay(hiz);
    digitalWrite(ledPin3,HIGH);
    delay(hiz);
    digitalWrite(ledPin,LOW);
    delay(hiz);
    digitalWrite(ledPin1,LOW);
    delay(hiz);
    digitalWrite(ledPin2,LOW);
    delay(hiz);
    digitalWrite(ledPin3,LOW);
    delay(hiz);;
}
void WriteNumberToSegment(byte Segment, byte Value)
{
digitalWrite(LATCH_DIO,LOW);
shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
digitalWrite(LATCH_DIO,HIGH);
}
