//POWER SWITCH definitions
int powerSwitch_Pin = 12;

//US defintions
#include <Ultrasonic.h>
#define TRIGGER_PIN  13
#define ECHO_PIN     12
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

//MOTOR definitions
int E1 = 5;   //left motor PWM   
int M1 = 6;   //left motor 
int E2 = 9;   //right motor PWM                   
int M2 = 8;   //right motor

//LINE defintions 
int rightFrontLine_Pin = 5; //connected to analog in 
int frontLine_Led = 13;
int rightFrontLine_Value;
int frontLine_Value;

//GLOBAL definitions
//arena official diameter mini category: 77 cm
float trigDist = 38.5f;
boolean trigToken;
float t1, t2, t3;
float maxTime;
boolean noBreak;

void setup(){
	Serial.begin(9600);
	//startup delay
	Serial.println("Welcome to Neothug");
	pinMode(M1, OUTPUT);   
	pinMode(M2, OUTPUT); 
	bool text = false;

	Serial.println("Starting in 5 seconds...");
	delay(3380);
}



void SensorsReport(){
	US();
	delay(500);
	
	Serial.print("FrontLine sensor value: ");
	Serial.println(analogRead(rightFrontLine_Pin));
	
	delay(500);
	
	if(FrontLine()){
		Serial.println("Frontline is detected!");
	}
	else{
		Serial.println("No frontline is detected");
	}
	delay(500);
  
	
	delay(500);
}

void TestMotors(){ 
	int value; 
	for(value = 0 ; value <= 255; value+=5){ 
		digitalWrite(M1,LOW);   
		digitalWrite(M2, LOW);       
		analogWrite(E1, value);   //PWM Speed control 
		analogWrite(E2, value);   //PWM Speed control 
		delay(30); 
	}  
}



void loop(){  
	//FrontLine true when white, false when black in real conditions
	//main code

	//get current values
	if(!trigToken){
		t1 = US();
		delay(5);
		t2 = US();
		delay(5);
		t3 = US();
		delay(5);
	}

	if(!FrontLine()){
		if ((t1 < trigDist && t2 < trigDist && t3 < trigDist) || trigToken){ 
			trigToken = false; 
			maxTime = millis() + 3500.0f;
			while(!FrontLine() && millis() < maxTime){
				Forward();
			}
		}
		else{
			while(!FrontLine() && US() > trigDist){
				RotateCW();
			}
			Stop();
			if (US() < trigDist){
				trigToken = true;
			}
		}
	}

	if(FrontLine()){
	Stop();
	AvoidBorder();
	}
}

float US(){
	//US
	float cmMsec, inMsec;
	long microsec = ultrasonic.timing();

	cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
	Serial.print("MS: ");
	Serial.print(microsec);
	Serial.print(", Distance (cm): ");
	Serial.println(cmMsec);
	return cmMsec;
}

boolean FrontLine(){
	//frontLine get data
	rightFrontLine_Value = analogRead(rightFrontLine_Pin);
	  
	if(rightFrontLine_Value < 500){
    //testing:
    //return false;
		//actual conditions:
		return true;
	}
	else{
		return false;
	}
}

void Stop(){
	digitalWrite(M1,LOW);
	digitalWrite(M2, LOW);
	analogWrite(E1, 0);
	analogWrite(E2, 0);
}

void Backward(){
	digitalWrite(M1,HIGH);
	digitalWrite(M2, HIGH);
	analogWrite(E1, 250);
	analogWrite(E2, 250);
}

void Forward(){
	digitalWrite(M1,LOW);
	digitalWrite(M2, LOW);
	analogWrite(E1, 250);
	analogWrite(E2, 250);
}

void RotateCW(){
	digitalWrite(M1,LOW);
	digitalWrite(M2, HIGH);
	analogWrite(E1, 255);
	analogWrite(E2, 255);
}

void RotateCCW(){
	digitalWrite(M1,HIGH);
	digitalWrite(M2, LOW);    
	analogWrite(E1, 255);
	analogWrite(E2, 255);
}

void RotateCWavg(){
	digitalWrite(M1,LOW);
	digitalWrite(M2, HIGH);
	analogWrite(E1, 255);
	analogWrite(E2, 255);
}

void AvoidBorder(){
	Stop();
  Backward();
	delay(250);
	RotateCWavg();
	delay(1250);
	//Stop();
	//delay(25);
	Forward();
	//delay(450);
	//Stop();
	//delay(25);
}

//not used 
void EvasiveManeuver(){
	//when facing enemy for too long
	Stop();
	delay(75);
	RotateCW();
	delay(700);
	Stop();
	delay(75);
	noBreak = false;
}




