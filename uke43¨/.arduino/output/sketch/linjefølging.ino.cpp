#include <Arduino.h>
#line 1 "/Users/oleleire/Documents/ESI/Prosjektfag/Pololu Zumo 32u4/uke43¨/linjefølging/linjefølging.ino"
#include <Zumo32U4.h>

Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;

int16_t lastError = 0;
const uint16_t maxSpeed = 200;  //Dette er maksfarten som motorene vil kjøre 

#define NUM_SENSORS 5 
unsigned int lineSensorValues[NUM_SENSORS]; //Lager et array med sensorverdiene 


void kalibrer(){

delay(1000); //Delay på et sekund før den første for-løkken starter 
for(uint16_t i=0; i <= 10; i++) 
//En for-løkke som teller fra 0 til 9
{
    if (i <= 5){ // Visst i<=5 kjører bilen den ene veien og kalibrer 
    motors.setSpeeds(-150,150);
    delay(500);
    motors.setSpeeds(0,0);
    lineSensors.calibrate();
    //delay(1000);
    }

    if (i >= 5){ //Visst i>=5 kjører bilen den andre veien og kalibrer 
    motors.setSpeeds(150,-150);
    lineSensors.calibrate();
    delay(500);
    motors.setSpeeds(0,0);
    //delay(1000);
    }
}



}



void linjefolging(){

  int16_t position = lineSensors.readLine(lineSensorValues); //Definerer en ny variabel position og setter den lik sensorverdiene 
  int16_t error = position - 2000; //Error (Avviket) er lik sensorverdien - 2000 (2000 er senter)
  int16_t speedDifference = error / 4 + 6 * (error - lastError); //speedDifference er lik avvik/4 + 6* (avvik- forrige avvik)
  lastError = error;

  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference; //Definerer leftSpeed 
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference; //Definerer rightSpeed 

 //Høyre og venstre motor får forskjellig fart avhengig av speedDifference (Svinger enten høyre eller venstre)
 //Constrain cutter verdien til tall mellom 0 og maxSpeed (200)
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed); 
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);


  motors.setSpeeds(leftSpeed, rightSpeed);
}




void setup(){

lineSensors.initFiveSensors();
kalibrer(); //Kaller funksjonen kalibrer (Kun en gang, hver gang man skrur på bilen)


}

void loop(){
linjefolging();

}


