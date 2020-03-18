/*====================================================
*  Programme permettant de piloter sortie gpio
*  ESP8266 à l'aide d'un serveur sur le WEB
*====================================================*/

/*bibliothèques requises*/
#include <ESP8266WiFi.h>

/*infos de connexion réseau*/
const char* ssid = "SFR-D128";
const char* password = "ermadaceyfredkublip8";

WiFiServer server(80);

void setup() {
  /*Ouverture port serie en 115200 baud*/
  Serial.begin(115200);
  delay(10);

  /*déclaration et init GPIO2 --> sortie*/
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  /*-----------Affichage sur le moniteur série-------------
  Serial.println();    //nécessaire à l'init du moniteur
  Serial.print("Connecting to ");
  Serial.println(ssid);
  -------------------------------------------------------*/

  /*Connexion au réseau WIFI avec ssid et mdp prédéfinis*/
  WiFi.begin(ssid, password);

  /*Loop de connexion | sortie de boucle si connexion réussie | A AMELIORER /!\ */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("...");  //visualisation du chargement
  }
  Serial.println("");
  Serial.println("WiFi connecté");  //info messsage connexion réussie

  /*Procédure serveur*/
  server.begin();
  Serial.println("Serveur démarré");  //info message serveur démarré

  /*------Affichage de l'adresse IP sur le moniteur----------
  Serial.println(WiFi.localIP());   //info message adresse IP
  ---------------------------------------------------------*/
}

void loop() {
  /*vérifier si un client est connecté*/
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  /*connexion d'un nouveau client*/
  Serial.println("nouveau client détecté");
  while(!client.available()){
    delay(1);
  }

  /*lecture de la requête*/
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  int val;  //variable état ampoule
  if (req.indexOf("/gpio/0") != -1){   //teste si requete = requete pour éteindre l'ampoule
    val = 0;
  }
  else if (req.indexOf("/gpio/1") != -1){   //teste si requete = requete pour allumer l'ampoule
    val = 1;
  }
  else{
  Serial.println("invalid request");    //requete invalide
  client.stop();
  return;
  }

  digitalWrite(2, val);   //adapter l'état de la sortie gpio2 à l'état de l'ampoule
  client.flush();

  /*création de la réponse*/
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  /*envoi de la réponse*/
  client.print(s);
}
