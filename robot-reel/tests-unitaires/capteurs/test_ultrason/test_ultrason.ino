// Test des deux capteurs ultrason HC-SR04.
// Ouvrir le moniteur série à 115200 bauds et approcher la main de chaque capteur.

const int TRIG_1 = 23, ECHO_1 = 22;
const int TRIG_2 = 21, ECHO_2 = 15;

long lireDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  const unsigned long duree = pulseIn(echo, HIGH, 25000);
  if (duree == 0) return -1;  // Pas d'écho reçu.
  return duree * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);
}

void loop() {
  const long d1 = lireDistance(TRIG_1, ECHO_1);
  delay(30);  // Je laisse l'écho du premier capteur disparaître avant de lancer le second.
  const long d2 = lireDistance(TRIG_2, ECHO_2);

  Serial.print("Capteur 1 : ");
  Serial.print(d1);
  Serial.print(" cm | Capteur 2 : ");
  Serial.print(d2);
  Serial.println(" cm");
  delay(300);
}
