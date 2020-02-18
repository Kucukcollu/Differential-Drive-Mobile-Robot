 /**********************************************************************
        Arduino project code for Differential Drive Mobile Robot
                    Ali Aydın KÜÇÜKÇÖLLÜ
                    Oğuzhan ÜNLÜ
                    Efe KURDOĞLU
 **********************************************************************/
 

 #include <SD.h>                                      // The library of SD card module.
 
 //MOSI = pin 11
 //MISO = pin 12
 //SCK = pin 13
 
 int CS_pin = 10;

 const int motorA1  = 7;                              // Digital pins of motor 1.
 const int motorA2  = 6;
  
 const int motorB1  = 5;                              // Digital pins of motor 2.
 const int motorB2  = 4;  

 int e1 = 3;                                          // Enable pins of DC motors.
 int e2 = 9;

 int v1;
 int v2;
 int v3;
 int state;

 int new_data;
 int data;                                            // The measured value of LDR module from analog Input.
 char junk;
 String bluetooth_string = "";

void setup() {
    Serial.begin(9600);                               // Starting code for serial comminication.
    
    pinMode(CS_pin, OUTPUT);

    if (!SD.begin(CS_pin)){                           // Code block to check that is SD card available.
      Serial.println("Card Failure");
      return;
    }
    Serial.println("Card Ready");
  
    pinMode(A0,INPUT);
    
    pinMode(motorA1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorB2, OUTPUT);

    pinMode(e1,OUTPUT);
    pinMode(e2,OUTPUT);
}

void loop() {
  File dataFile = SD.open("data.txt", FILE_WRITE);     // Creates a '.txt' file as 'data.txt'
  if (dataFile)
  {
    data = analogRead(A0);
    dataFile.print("LDR:");
    dataFile.println(data);
    dataFile.close();
    Serial.println(data);
  }
  File data2File = SD.open("velocity.txt",FILE_WRITE); // Creates a '.txt' file as 'velocity.txt'
  if(data2File)
  {
    if (new_data < 85){
      data2File.print("Motor Velocity:");
      data2File.println(v1);
      data2File.close();
      Serial.println(v1);
    }
    else if(85 <= new_data < 170){
      data2File.print("Motor Velocity:");
      data2File.println(v2);
      data2File.close();
     Serial.println(v2);
    }
    else{
      data2File.print("Motor Velocity:");
      data2File.println(v3);
      data2File.close();
      Serial.println(v3);
    }
  }

  File data3File = SD.open("state.txt", FILE_WRITE);   // Creates a '.txt' file as 'state.txt'
  if (data3File){
    if (new_data < 85){
      data3File.print("State:");
      data3File.println(state);
      data3File.close();
      Serial.println(state);
    }
    else if(85 <= new_data < 170){
      data3File.print("State:");
      data3File.println(state);
      data3File.close();
     Serial.println(state);
    }
    else{
      data3File.print("State:");
      data3File.println(state);
      data3File.close();
      Serial.println(state);
    }
  }
  
  else
  {
    Serial.println("Couldn't open log file");
  }
  
  data = analogRead(A0);
  new_data = map(data,0,1023,0,255);
  
  if (new_data < 85){                                 // First level of velocity.
    v1 = 180;
    state = 1;
    analogWrite(e1,180);
    analogWrite(e2,180);
  }
  else if(85 <= new_data < 170){                      // Second level of velocity.
    v2 = 100;
    state = 2;
    analogWrite(e1,255);
    analogWrite(e2,255);
  }
  else if(new_data >= 170){                           // Third level of velocity.
    v3 =0;
    state = 3;
    analogWrite(e1,0);
    analogWrite(e2,0);
  }
  
  Serial.print("LDR value:");
  Serial.println(data);

  

  if(Serial.available()){
    while(Serial.available()){
      char inChar = (char)Serial.read();
      bluetooth_string += inChar;
    }
    Serial.println(bluetooth_string);
    while (Serial.available() > 0){ 
      junk = Serial.read(); 
    }

    if(bluetooth_string == "1"){                      // Robot goes forward.
      digitalWrite(motorA1,HIGH);
      digitalWrite(motorA2,LOW);
      digitalWrite(motorB1,LOW);
      digitalWrite(motorB2,HIGH);

    }

    else if(bluetooth_string == "2"){                 // Robot goes backward.
      digitalWrite(motorA1,LOW);
      digitalWrite(motorA2,HIGH);
      digitalWrite(motorB1,HIGH);
      digitalWrite(motorB2,LOW);

    }

    else if(bluetooth_string == "3"){                 // Robot turns right.
      digitalWrite(motorA1,LOW);
      digitalWrite(motorA2,LOW);
      digitalWrite(motorB1,LOW);
      digitalWrite(motorB2,HIGH);
    
    }
    
    else if(bluetooth_string == "4"){                 // Robot turns left.
      digitalWrite(motorA1,HIGH);
      digitalWrite(motorA2,LOW);
      digitalWrite(motorB1,LOW);
      digitalWrite(motorB2,LOW);
    
    }
    
    else if(bluetooth_string == "5"){                 // Robot stops.
      digitalWrite(motorA1,LOW);
      digitalWrite(motorA2,LOW);
      digitalWrite(motorB1,LOW);
      digitalWrite(motorB2,LOW);
      
    }
    
    bluetooth_string = "";
  }
}
