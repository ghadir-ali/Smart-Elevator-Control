#include <SoftwareSerial.h>

int bluetoothTx = 7;
int bluetoothRx = 8;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
String state; // string to store incoming message from bluetooth
int s,f;

void setup() {
  Serial.begin(9600); // serial communication started

bluetooth.begin(9600);

}
//-----------------------------------------------------------------------//  
void loop() {
  while (bluetooth.available()>0){  //Check if there is an available byte to read
  //delay(10); //Delay added to make thing stable 
  unsigned int c = bluetooth.read(); //Conduct a serial read
  state += c ;
  


  if (state.length() ==2){
    int d = state.toInt();

  Serial.println(d);
  
  state="";}
 

}}
