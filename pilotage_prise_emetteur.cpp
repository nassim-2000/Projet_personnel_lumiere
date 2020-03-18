/*====================================================
*  Programme permettant de piloter des prises
*  télécommandées à l'aide d'émetteur 433Mhz
*====================================================*/

/*bibliothèques requises*/
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

Servo monServo;

const int gpio = 8;   //déclaration pin 8 --> sortie gpio du esp8266

void setup() {
  Serial.begin(9600);
  pinMode(gpio,INPUT);    //déclarer pin 8 (sortie esp8266) --> entrée
  mySwitch.enableTransmit(10);   //Emetteur branché sur pin 10
}


void loop() {
/*pilotage du de la prise télécommandée à l'aide de l'envoi du signal*/
  if(gpio==1){
    mySwitch.send( 5510485, 24);    //envoi trame du signal prise --> ON
    delay(2000);
  }else if(gpio==0){
    mySwitch.send( 5510657, 24);    //envoi trame du signal prise --> OFF
    delay(2000);
  }
}
