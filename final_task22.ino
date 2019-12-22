#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
int bluetoothTx = 7;
int bluetoothRx = 8;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
String state;
String dist;// string to store incoming message from bluetooth
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
int Elev_x[10]={0,0,0,0,0,0,0,0,0,0};
int Elev_y[10]={0,0,0,0,0,0,0,0,0,0};
int Elev_z[10]={0,0,0,0,0,0,0,0,0,0};
int Elevator[10]={0,0,0,0,0,0,0,0,0,0};
int temp[10]={0,0,0,0,0,0,0,0,0,0};
int r1=0;
int r2=0;
//stepper
int output1=2;
int output2=3;
int output3=4;
int output4=5;
char Disten_x=1;
char Disten_y=1;
char Disten_z=1;
char nstop;
int count_x=0;
int count_y=0;
int count_z=0;
int result_x=0;
int result_y=0;
int result_z=0;
char golden_rule=0;
char b;
//time time_stop;
// time_stop=delay(1000);
char b_value(char call,char Dist,char curr);
//gold rule give me the nearest elevator to the call
char gold_rule(char Dist,char curr,char call, char elev);
//main function
void choose_elevator(char call, char going);
char pass (char Dist ,char curr, char call, char elev);
void add_pass (char Dist ,char curr, char call, char elev);
 
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(output1, OUTPUT);
pinMode(output2, OUTPUT);
pinMode(output3, OUTPUT);
pinMode(output4, OUTPUT);
lcd.begin(16, 2);
lcd.setCursor(0,0);
bluetooth.begin(9600);



}

//How to add call if pass?!
//Duplicate requests?

void loop() {
 stepper();
 bluetooth_func();

lcd_display();
//int r1 = random(1,5);
//int r2 = random(1,5);
Serial.print("Call is ");
Serial.println(r2);
Serial.print("Going to ");
Serial.println(r1);
if(!(r1==0&& r2==0) ){
choose_elevator(r2, r1);
}

Serial.print(" x: ");
 for(char i=0;i<10;i++){
 
 Serial.print(Elev_x[i]);
 }
 Serial.println("  ");
 Serial.print(" y: ");
  for(char i=0;i<10;i++){
    
 Serial.print(Elev_y[i]);
 }
 Serial.println("  ");

  Serial.print(" z: ");
  for(char i=0;i<10;i++){
   
 Serial.print(Elev_z[i]);
 }
 Serial.println("  ");

// delay(2000);
 
 /*for (char i=0; i<9; i++)
 {
  Elev_x[i]=Elev_x[i+1];
  Elev_y[i]=Elev_y[i+1];
  Elev_z[i]=Elev_z[i+1];
  }
  if (count_x!=0) 
    {count_x--;}
  if (count_y!=0) 
    {count_y--;}
  if (count_z!=0) 
    {count_z--;}
    */
    
 
}

// function to show if there is bounce or not
char b_value(char Call,char Dist,char Curr){
  if (((Dist>Curr)&&(Curr>Call)) || ((Dist<Curr)&&(Curr<Call))) 
  {
    b=1;
   }
else 
 {
  b=0;
  }

  }
  // calculate number of stops
char pass (char Dist ,char curr, char call, char elev)
{ //Serial.println(elev_array);
  int p=0; //Will it pass on the call?
  int nstop1=0;
  if(((Dist>curr)&&(curr<call)&&(call<Dist)))
  { p=1;
    for(char i=0;i<10 ;i++)
    {
      if((Elevator[i]<call)&&(Elevator[i] != 0))
      {
        nstop1++;
        }
      }
      
    }
    if(((Dist<curr)&&(curr>call)&&(call>Dist)))
    { p=1;

      for(char i=0;i<10 ;i++)
    {
      if((Elevator[i]>call)&&(Elevator[i] != 0))
      {
        nstop1++;
        }
      }
      }
      //Serial.println(nstop1);
        if (p==1)
  {return nstop1;}
  else { switch (elev)
 {
  case 'x': return(count_x);
  case 'y': return(count_y);
  case 'z': return(count_z);
  }}

  }

  
//our golden rule
char gold_rule(char Dist,char curr,char call, char elev)
  {
 switch (elev)
 {
  case 'x': for (int i=0; i<=9; i++) {Elevator[i] = Elev_x[i];}
  case 'y': for (int i=0; i<=9; i++) {Elevator[i] = Elev_y[i];}
  case 'z': for (int i=0; i<=9; i++) {Elevator[i] = Elev_z[i];}
  }
 
    b=b_value(call,Dist,curr);
   nstop=pass(Dist,curr,call, elev);
   //time_stop=nstop*delay(5000);
     golden_rule=abs((Dist-curr)+(call-Dist))*2+2*b*abs(Dist-curr)*2+ nstop*5;
      return golden_rule;
  }


void choose_elevator(char call, char going)
{ 
    if(Elev_x[0]==0)
    {    
       Elev_x[count_x++]=call;
       Elev_x[count_x++]=going;
    }
    
    else if(Elev_y[0]==0)
    {
        Elev_y[count_y++]=call;        
        Elev_y[count_y++]=going;
    }
    else if(Elev_z[0]==0)
    {
          Elev_z[count_z++]=call;
          Elev_z[count_z++]=going;
    }
    else
    {
          Disten_x=Elev_x[count_x];
          Disten_y=Elev_y[count_y];
          Disten_z=Elev_z[count_z];

         
          result_x= gold_rule(Disten_x,Elev_x[0],call, 'x');
          result_x+= gold_rule(Disten_x,call, going, 'x');
          result_y= gold_rule(Disten_y,Elev_y[0],call, 'y');
          result_y+= gold_rule(Disten_y,call, going, 'y');
          result_z= gold_rule(Disten_z,Elev_z[0],call, 'z');
          result_z+= gold_rule(Disten_z, call, going, 'z');
          Serial.println("Result of x: ");
          Serial.println(result_x);
          Serial.println("Result of y: ");
          Serial.println(result_y);
          Serial.println("Result of z: ");
          Serial.println(result_z);
          //show the min number of steps  
          char min_1=min(result_x,result_y);
          char min_2=min(min_1,result_z); 
          Serial.println(" ");        
          if (min_2==result_x)
          {
            //if(((Dist<curr)&&(curr>call)&&(call>Dist))||((Dist>curr)&&(curr<call)&&(call<Dist))) {p=1;}
            Elev_x[count_x++]=call;
            Elev_x[count_x++]=going;
            
           }
            else if(min_2==result_y)
            {
              //if(((Dist<curr)&&(curr>call)&&(call>Dist))||((Dist>curr)&&(curr<call)&&(call<Dist))) {p=1;}
              Elev_y[count_y++]=call;
              Elev_y[count_y++]=going;
              
            }
            else
            {   //if(((Dist<curr)&&(curr>call)&&(call>Dist))||((Dist>curr)&&(curr<call)&&(call<Dist))) {p=1;}
                Elev_z[count_z++]=call;
                Elev_z[count_z++]=going;
                
            }
       }

       
     if (count_x>=10) {count_x=0;}
     if (count_y>=10) {count_y=0;} 
     if (count_z>=10) {count_z=0;}
     
                   
       }

   void lcd_display()
    {
         lcd.setCursor(0,0);
         lcd.print("Elev_x");
          delay(3000);
          
         lcd.setCursor(7,0);
         lcd.print(Elev_x[0]);
         delay(300);
        
        lcd.setCursor(0,1);
         lcd.print("Elev_y");
          delay(3000);
          
         lcd.setCursor(7,1);
         lcd.print(Elev_y[0]);
         delay(300);
         
        lcd.setCursor(8,0);
         lcd.print("Elev_z");
          delay(3000);
          
          lcd.setCursor(15,0);
         lcd.print(Elev_z[0]);
         delay(300);
      }



void stepper()
{
  //up
 if(Elev_x[1]>Elev_x[0]){
       if( (Elev_x[1]-Elev_x[0])==1)
       {
        for(int j=0;j<230;j++){clockwise(); }
      
        }
        else if( (Elev_x[1]-Elev_x[0])==2)
       {
        for(int j=0;j<460;j++){clockwise(); }
      
        }
        else if((Elev_x[1]-Elev_x[0])==3)
        {
          for(int j=0;j<690;j++){clockwise(); }
          }
      
           else if((Elev_x[1]-Elev_x[0])==4)
        {
          for(int j=0;j<920;j++){clockwise(); }
          }
          
        }


        //down
    if(Elev_x[1]<Elev_x[0]){
       if( (Elev_x[0]-Elev_x[1])==1)
       {
        for(int j=0;j<230;j++){anticlockwise(); }
      
        }
        else if( (Elev_x[0]-Elev_x[1])==2)
       {
        for(int j=0;j<460;j++){anticlockwise(); }
      
        }
        else if((Elev_x[0]-Elev_x[1])==3)
        {
          for(int j=0;j<690;j++){anticlockwise(); }
          }
      
           else if((Elev_x[0]-Elev_x[1])==4)
        {
          for(int j=0;j<920;j++){anticlockwise(); }
          }
          
        }
}   
void anticlockwise(){

    int n;
          //for(n=0;n<2;n=n+1)
          //{
      
           
             
           
       digitalWrite(output1,LOW);
       digitalWrite(output2,LOW);
       digitalWrite(output3,LOW);
       digitalWrite(output4,HIGH);
        delay(10);   

        digitalWrite(output1,LOW);
       digitalWrite(output2,LOW);
       digitalWrite(output3,HIGH);
       digitalWrite(output4,LOW);
     delay(10);
        digitalWrite(output1,LOW);
       digitalWrite(output2,HIGH);
       digitalWrite(output3,LOW);
       digitalWrite(output4,LOW);
     
             delay(10);
        digitalWrite(output1,HIGH);
       digitalWrite(output2,LOW);
       digitalWrite(output3,LOW);
       digitalWrite(output4,LOW);
  
              delay(10);
      
          //}
      }

      void clockwise(){

    int n;
          //for(n=0;n<2;n=n+1)
          //{
       digitalWrite(output1,HIGH);
       digitalWrite(output2,LOW);
       digitalWrite(output3,LOW);
       digitalWrite(output4,LOW);
  
           
              delay(10);

        digitalWrite(output1,LOW);
       digitalWrite(output2,HIGH);
       digitalWrite(output3,LOW);
       digitalWrite(output4,LOW);
     
             delay(10);


               digitalWrite(output1,LOW);
       digitalWrite(output2,LOW);
       digitalWrite(output3,HIGH);
       digitalWrite(output4,LOW);
    
            delay(10);
       digitalWrite(output1,LOW);
       digitalWrite(output2,LOW);
       digitalWrite(output3,LOW);
       digitalWrite(output4,HIGH);
              delay(10);
      
          //}
      }

 void bluetooth_func()
 {


  while (bluetooth.available()>0){  //Check if there is an available byte to read
  //delay(10); //Delay added to make thing stable 
  unsigned int c = bluetooth.read(); //Conduct a serial read
  state += c ;
 // dist=c+1;
  
  if (state.length() ==2){
    int d = state.toInt();
     r1 = d%10;//destin
     r2 = (d/10)%10;// call
    

  Serial.println("destination");
  Serial.print(r1);
  Serial.println("position");
  Serial.println(r2);
  
  state="";}
 

}
  
  }     


   

