// Servotron : programme final du robot
// Carte : ESP32, cœur Arduino 3.x
// Le robot se pilote depuis l'IHM web ou les boutons du couvercle.

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include "ihm.h"

// ---------- Wi-Fi ----------
// À remplir avec votre réseau. S'il est introuvable, le robot crée le sien.
const char *WIFI_SSID = "VOTRE_WIFI";
const char *WIFI_MDP = "VOTRE_MOT_DE_PASSE";
const char *ROBOT_SSID = "Servotron";
const char *ROBOT_MDP = "servotron";

// ---------- Broches ----------
const int DIR_M1 = 25, PWM_M1 = 26;
const int DIR_M2 = 5, PWM_M2 = 17;

const int LIGNE_GAUCHE = 32, LIGNE_CENTRE = 35, LIGNE_DROITE = 33;

const int TRIG_1 = 23, ECHO_1 = 22;
const int TRIG_2 = 21, ECHO_2 = 15;

const int LED_ROUGE = 2, LED_VERTE = 19, LED_BLEUE = 12;

const int BTN_LIGNE = 13, BTN_OBSTACLE = 14, BTN_CARRE = 27, BTN_STOP = 18;

// ---------- Réglages ----------
const int PWM_FREQUENCE = 500;
const int PWM_RESOLUTION = 8;

// Les durées des formes ont été réglées à cette vitesse, je ne la lie donc pas au curseur.
const int VITESSE_FORME = 128;
const int CERCLE_M1 = 220, CERCLE_M2 = 85;

const int SEUIL_OBSTACLE_CM = 20;
const unsigned long PAUSE_OBSTACLE_MS = 1000;
const unsigned long ROTATION_OBSTACLE_MS = 500;

// ---------- Formes ----------
// Une forme est une suite d'actions chronométrées : A = avancer, D = droite, C = cercle.
struct Etape {
  char action;
  unsigned long duree;
};

const Etape CARRE[] = {{'A', 2000}, {'D', 690}, {'A', 2000}, {'D', 690},
                       {'A', 2000}, {'D', 690}, {'A', 2000}, {'D', 690}};
const Etape TRIANGLE[] = {{'A', 2000}, {'D', 800}, {'A', 2000}, {'D', 900}, {'A', 2350}};
const Etape CERCLE[] = {{'C', 4500}};

// ---------- État du robot ----------
enum Mode { ARRET, MANUEL, LIGNE, OBSTACLE, FORME };
Mode mode = ARRET;

int vitesse = 128;

const Etape *forme = nullptr;
int nbEtapes = 0;
int etape = 0;
unsigned long debutEtape = 0;

enum EtatObstacle { AVANCE, PAUSE, ROTATION };
EtatObstacle etatObstacle = AVANCE;
unsigned long debutObstacle = 0;
long distance1 = 400, distance2 = 400;
bool mesurerCapteur1 = true;
unsigned long derniereMesure = 0;

WebServer serveur(80);

// ---------- Moteurs ----------
// Vitesse positive = marche avant. Les moteurs sont montés en miroir,
// c'est pour ça que leurs sens de rotation sont inversés.
void moteurs(int v1, int v2) {
  digitalWrite(DIR_M1, v1 >= 0 ? LOW : HIGH);
  digitalWrite(DIR_M2, v2 >= 0 ? HIGH : LOW);
  ledcWrite(PWM_M1, abs(v1));
  ledcWrite(PWM_M2, abs(v2));
}

void avancer(int v) { moteurs(v, v); }
void reculer(int v) { moteurs(-v, -v); }
void droite(int v) { moteurs(-v, v); }
void gauche(int v) { moteurs(v, -v); }

// ---------- LED ----------
void led(int r, int v, int b) {
  analogWrite(LED_ROUGE, r);
  analogWrite(LED_VERTE, v);
  analogWrite(LED_BLEUE, b);
}

void clignotementDemarrage() {
  for (int i = 0; i < 5; i++) {
    led(255, 0, 0);
    delay(500);
    led(0, 255, 0);
    delay(500);
    led(0, 0, 255);
    delay(500);
  }
  led(0, 0, 0);
}

// ---------- Changement de mode ----------
void arreter() {
  moteurs(0, 0);
  led(0, 0, 0);
  mode = ARRET;
}

void lancerForme(const Etape *f, int n, int r, int v, int b) {
  forme = f;
  nbEtapes = n;
  etape = 0;
  debutEtape = millis();
  led(r, v, b);
  mode = FORME;
}

void lancerCarre() { lancerForme(CARRE, 8, 0, 0, 255); }
void lancerTriangle() { lancerForme(TRIANGLE, 5, 236, 0, 255); }
void lancerCercle() { lancerForme(CERCLE, 1, 254, 254, 0); }

// Appuyer une deuxième fois sur le bouton du mode l'arrête.
void basculerLigne() {
  if (mode == LIGNE) {
    arreter();
    return;
  }
  arreter();
  led(0, 255, 0);
  mode = LIGNE;
}

void basculerObstacle() {
  if (mode == OBSTACLE) {
    arreter();
    return;
  }
  arreter();
  led(255, 0, 0);
  etatObstacle = AVANCE;
  distance1 = distance2 = 400;
  mode = OBSTACLE;
}

// ---------- Formes ----------
void executerForme() {
  const Etape &e = forme[etape];
  if (e.action == 'A') avancer(VITESSE_FORME);
  else if (e.action == 'D') droite(VITESSE_FORME);
  else moteurs(CERCLE_M1, CERCLE_M2);

  if (millis() - debutEtape >= e.duree) {
    etape++;
    debutEtape = millis();
    if (etape >= nbEtapes) arreter();
  }
}

// ---------- Suivi de ligne ----------
// Un capteur à l'état haut voit la ligne.
void suivreLigne() {
  const bool g = digitalRead(LIGNE_GAUCHE);
  const bool c = digitalRead(LIGNE_CENTRE);
  const bool d = digitalRead(LIGNE_DROITE);

  if (g && !d) gauche(vitesse);
  else if (!g && d) droite(vitesse);
  else if (c) avancer(vitesse);
  else reculer(vitesse);  // Ligne perdue : je recule pour la retrouver.
}

// ---------- Détection d'obstacle ----------
// Le délai de 25 ms évite de bloquer le robot quand l'écho ne revient pas.
long lireDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  const unsigned long duree = pulseIn(echo, HIGH, 25000);
  if (duree == 0) return 400;  // Pas d'écho : rien devant le capteur.
  return duree * 0.034 / 2;
}

// Je mesure un capteur à la fois, sinon chacun capte l'écho de l'autre.
void mesurerDistances() {
  if (millis() - derniereMesure < 30) return;
  derniereMesure = millis();
  if (mesurerCapteur1) distance1 = lireDistance(TRIG_1, ECHO_1);
  else distance2 = lireDistance(TRIG_2, ECHO_2);
  mesurerCapteur1 = !mesurerCapteur1;
}

void eviterObstacles() {
  mesurerDistances();

  switch (etatObstacle) {
    case AVANCE:
      if (distance1 < SEUIL_OBSTACLE_CM || distance2 < SEUIL_OBSTACLE_CM) {
        moteurs(0, 0);
        debutObstacle = millis();
        etatObstacle = PAUSE;
      } else {
        avancer(vitesse);
      }
      break;

    case PAUSE:
      if (millis() - debutObstacle >= PAUSE_OBSTACLE_MS) {
        // Je tourne du côté où l'obstacle est le plus loin.
        if (distance1 < distance2) droite(vitesse);
        else gauche(vitesse);
        debutObstacle = millis();
        etatObstacle = ROTATION;
      }
      break;

    case ROTATION:
      if (millis() - debutObstacle >= ROTATION_OBSTACLE_MS) etatObstacle = AVANCE;
      break;
  }
}

// ---------- Boutons ----------
// Je réagis à l'appui, pas au maintien. La lecture toutes les 20 ms suffit à filtrer les rebonds.
bool appuiDetecte(int broche, bool &etatPrecedent) {
  const bool appuye = digitalRead(broche) == LOW;
  const bool front = appuye && !etatPrecedent;
  etatPrecedent = appuye;
  return front;
}

void lireBoutons() {
  static unsigned long derniereLecture = 0;
  static bool ligne = false, obstacle = false, carre = false, stop = false;
  if (millis() - derniereLecture < 20) return;
  derniereLecture = millis();

  if (appuiDetecte(BTN_STOP, stop)) arreter();
  if (appuiDetecte(BTN_LIGNE, ligne)) basculerLigne();
  if (appuiDetecte(BTN_OBSTACLE, obstacle)) basculerObstacle();
  if (appuiDetecte(BTN_CARRE, carre)) lancerCarre();
}

// ---------- IHM web ----------
void repondre() { serveur.send(200, "text/plain", "ok"); }

void manuel(void (*mouvement)(int)) {
  led(0, 0, 0);
  mode = MANUEL;
  mouvement(vitesse);
  repondre();
}

void configurerServeur() {
  serveur.on("/", [] { serveur.send_P(200, "text/html; charset=utf-8", PAGE_IHM); });

  serveur.on("/avancer", [] { manuel(avancer); });
  serveur.on("/reculer", [] { manuel(reculer); });
  serveur.on("/gauche", [] { manuel(gauche); });
  serveur.on("/droite", [] { manuel(droite); });
  serveur.on("/stop", [] { arreter(); repondre(); });

  serveur.on("/suiviLigne", [] { basculerLigne(); repondre(); });
  serveur.on("/detectionObstacle", [] { basculerObstacle(); repondre(); });
  serveur.on("/carre", [] { lancerCarre(); repondre(); });
  serveur.on("/triangle", [] { lancerTriangle(); repondre(); });
  serveur.on("/cercle", [] { lancerCercle(); repondre(); });

  // Le curseur de l'IHM envoie un pourcentage.
  serveur.on("/speed", [] {
    vitesse = map(constrain(serveur.arg("value").toInt(), 0, 100), 0, 100, 0, 255);
    repondre();
  });

  serveur.onNotFound([] { serveur.send(404, "text/plain", "Page introuvable"); });
  serveur.begin();
}

void connecterWifi() {
  WiFi.begin(WIFI_SSID, WIFI_MDP);
  const unsigned long debut = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - debut < 10000) delay(250);

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("IHM : http://");
    Serial.println(WiFi.localIP());
  } else {
    WiFi.softAP(ROBOT_SSID, ROBOT_MDP);
    Serial.print("Réseau du robot : ");
    Serial.print(ROBOT_SSID);
    Serial.print("  IHM : http://");
    Serial.println(WiFi.softAPIP());
  }
}

// ---------- Programme principal ----------
void setup() {
  Serial.begin(115200);

  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  ledcAttach(PWM_M1, PWM_FREQUENCE, PWM_RESOLUTION);
  ledcAttach(PWM_M2, PWM_FREQUENCE, PWM_RESOLUTION);
  moteurs(0, 0);

  pinMode(LIGNE_GAUCHE, INPUT);
  pinMode(LIGNE_CENTRE, INPUT);
  pinMode(LIGNE_DROITE, INPUT);

  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);

  pinMode(LED_ROUGE, OUTPUT);
  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_BLEUE, OUTPUT);

  pinMode(BTN_LIGNE, INPUT_PULLUP);
  pinMode(BTN_OBSTACLE, INPUT_PULLUP);
  pinMode(BTN_CARRE, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);

  clignotementDemarrage();
  connecterWifi();
  configurerServeur();
}

void loop() {
  serveur.handleClient();
  lireBoutons();

  if (mode == LIGNE) suivreLigne();
  else if (mode == OBSTACLE) eviterObstacles();
  else if (mode == FORME) executerForme();
}
