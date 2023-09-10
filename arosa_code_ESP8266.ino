#include <FirebaseESP8266.h>
#include <FirebaseJson.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
// Paramètres de connexion WiFi
const char* ssid = "NOM_DU_RESEAU_WIFI";
const char* password = "MOT_DE_PASSE_DU_RESEAU_WIFI";

// Paramètres de connexion à la base de données en temps réel Firebase
const char* firebaseHost = "NOM_DU_PROJET.firebaseio.com";
const char* firebaseAuth = "TOKEN_AUTHENTIFICATION";

// Structure pour stocker les données d'une vanne
struct Vanne {
  String duration;
  String Type;
  bool State;
  String TimeEnd;
  String TimeStart;
  bool setTime;
};

// Structure pour stocker les données d'une pompe
struct Pompe {
  String duration;
  String Type;
  bool State;
  int distributedwater;
  String TimeEnd;
  String TimeStart;
  bool setTime;
};

// Structure pour stocker les données d'une plante
struct Plante {
  int Humidity;
  String name;
};

// Tableaux pour stocker les vannes, les pompes et les plantes
Vanne vannes[3];
Pompe pompes[1];
Plante plantes[2];
int pinspompes = [13];
int pinsvannes = [0,2,4];
String pinshumidities = ["A0","A3","A6"];
String pinsdist = ["A0"];
FirebaseData firebaseData;
int previousMinute = 0;
String uid = "UdUNauKxfwTYR5JasUmqC0yMoLy2";
// Fonction de configuration WiFi
void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connexion au réseau WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnexion WiFi établie !");
}

// Fonction de configuration de la connexion Firebase
void setupFirebase() {
  Firebase.begin(firebaseHost, firebaseAuth);
  Serial.println("Connexion à Firebase établie !");
}

// Fonction pour récupérer les données des vannes depuis Firebase
void getVannesData() {
  for (int i = 0; i < 3; i++) {
    String path = uid + "Vannes/" + String(i);
    // Firebase.getString(firebaseData, path);

    // Convertir le JSON en structure Vanne
    // Assurez-vous que la structure du JSON correspond à la structure de la Vanne
    // Utilisez une bibliothèque JSON appropriée pour analyser le JSON, comme ArduinoJson
    // Ici, nous supposerons que le JSON est déjà correctement formaté
    vannes[i].State = Firebase.getBool(firebaseData, path + "/State");
    vannes[i].TimeEnd = Firebase.getString(firebaseData, path + "/TimeEnd");
    vannes[i].TimeStart = Firebase.getString(firebaseData, path + "/TimeStart");
    vannes[i].setTime = Firebase.getBool(firebaseData, path + "/setTime");
  }
}

// Fonction pour récupérer les données des pompes depuis Firebase
void getPompesData() {
  for (int i = 0; i < 1; i++) {
    String path =uid + "Pompes/" + String(i);
    // Firebase.getString(firebaseData, path);

    // Convertir le JSON en structure Pompe
    // Assurez-vous que la structure du JSON correspond à la structure de la Pompe
    // Utilisez une bibliothèque JSON appropriée pour analyser le JSON, comme ArduinoJson
    // Ici, nous supposerons que le JSON est déjà correctement formaté
    pompes[i].State = Firebase.getBool(firebaseData, path + "/State");
    pompes[i].TimeEnd = Firebase.getString(firebaseData, path + "/TimeEnd");
    pompes[i].TimeStart = Firebase.getString(firebaseData, path + "/TimeStart");
    pompes[i].setTime = Firebase.getBool(firebaseData, path + "/setTime");
    pompes[i].distributedwater = Firebase.getInt(firebaseData, path + "/distributedwater");
   ;
  }
}

// Fonction utilitaire pour contrôler les vannes et les pompes
void controlSystem() {
  // Récupérer l'heure actuelle
 int currentHour = hour();
  int currentMinute = minute();

  // Parcourir les vannes
  for (int i = 0; i < vannes.length; i++) {
    // Vérifier si la vanne doit être activée
    if (!vannes[i].setTime && vannes[i].TimeStart.toInt() <= currentHour && vannes[i].TimeEnd.toInt() > currentHour) {
      // Activer la vanne
      // Code pour activer la vanne sur l'ESP8266
      // vannes[i].setTime = true;
      vannes[i].State = true;
      digitalWrite(pinsvannes[i],vannes[i].State);
      // Mettre à jour la valeur de la vanne dans Firebase
      String path = uid + "Vannes/" + String(i) + "/State";
      Firebase.setBool(firebaseData, path.c_str(), true);
      // path = uid + "Vannes/" + String(i) + "/setTime";
      // Firebase.setBool(firebaseData, path.c_str(), true);
    }
    else if (vannes[i].setTime && vannes[i].TimeEnd.toInt() <= currentHour) {
      // Désactiver la vanne
      // Code pour désactiver la vanne sur l'ESP8266
      // vannes[i].setTime = false;
      vannes[i].State = false;
      digitalWrite(pinsvannes[i],vannes[i].State);
      // Mettre à jour la valeur de la vanne dans Firebase
      String path = uid + "Vannes/" + String(i) + "/State";
      Firebase.setBool(firebaseData, path.c_str(), false);
      // path = uid + "Vannes/" + String(i) + "/setTime";
      // Firebase.setBool(firebaseData, path.c_str(), false);
    }
  }

  // Parcourir les pompes
  for (int i = 0; i < pompes.length; i++) {
    // Vérifier si la pompe doit être activée
    if (!pompes[i].setTime && pompes[i].TimeStart.toInt() <= currentHour && pompes[i].TimeEnd.toInt() > currentHour) {
      // Activer la pompe
      // Code pour activer la pompe sur l'ESP8266
      // pompes[i].setTime = true;
      pompes[i].State = true;
      digitalWrite(pinspompes[i],pompes[i].State);
      // Mettre à jour la valeur de la pompe dans Firebase
      String path = uid + "Pompes/" + String(i) + "/State";
      Firebase.setBool(firebaseData, path.c_str(), true);
      // path = uid + "Pompes/" + String(i) + "/setTime";
      // Firebase.setBool(firebaseData, path.c_str(), true);
    }
    else if (pompes[i].setTime && pompes[i].TimeEnd.toInt() <= currentHour) {
      // Désactiver la pompe
      // Code pour désactiver la pompe sur l'ESP8266
      // pompes[i].setTime = false;
      pompes[i].State = false;
      digitalWrite(pinspompes[i],pompes[i].State);
      // Mettre à jour la valeur de la pompe dans Firebase
      String path = uid + "Pompes/" + String(i) + "/State";
      Firebase.setBool(firebaseData, path.c_str(), false);
      // path = uid + "Pompes/" + String(i) + "/setTime";
      // Firebase.setBool(firebaseData, path.c_str(), false);
    }
    
}
void setdata(){
  for (int i=0,i<pompes.length,i++){
    int distributedwater = analog(pinsdist[i]);
    pompes[i].distributedwater = distributedwater
    String path = uid + "Pompes/" + String(i) + "/distributedwater";
    Firebase.setInt(path, distributedwater);
  }
  for (int i = 0; i < 2; i++) {
    // Superviser l'humidité de la plante
    int humidity = analogRead(pinshumidities[i]); // Lire la valeur de l'humidité depuis le capteur

    // Mettre à jour la valeur d'humidité dans la structure Plante
    plantes[i].Humidity = humidity;

    // Mettre à jour la valeur d'humidité dans Firebase
    String path = uid + "Plants/" + String(i) + "/Humidity";
    Firebase.setInt(path, humidity);
  }
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupFirebase();
  pinMode(13,OUTPUT);
  pinMode(0,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  // getVannesData();
  // getPompesData();
  // getPlantesData();
  for(int i=0,i<pinspompes.length,i++) digitalWrite(pinspompes[i],LOW);
  for(int i=0,i<pinsvannes.length,i++) digitalWrite(pinsvannes[i],LOW);
}

void loop() {
  getVannesData();
  getPompesData();
  //pompes
  for (int i=0,i<pompes.length,i++){
  if(pompes[i].settime == false){
    if (pompes[i].state) {
    digitalWrite(pinspompes[i], HIGH);
  } else {
    digitalWrite(pinspompes[i], LOW);
  }
  }
  // Exécuter le système de contrôle toutes les minutes
  else {if (minute() != previousMinute) {
    controlSystem();
    previousMinute = minute();
  }}}
  //vannes
  for (int i=0,i<vannes.length,i++){
  if(vannes[i].settime == false){
    if (vannes[i].state) {
    digitalWrite(pinsvannes[i], HIGH);
  } else {
    digitalWrite(pinsvannes[i], LOW);
  }
  }
  // Exécuter le système de contrôle toutes les minutes
  else {if (minute() != previousMinute) {
    controlSystem();
    previousMinute = minute();
  }}}
  setdata();
  // Autres tâches à effectuer...
}
