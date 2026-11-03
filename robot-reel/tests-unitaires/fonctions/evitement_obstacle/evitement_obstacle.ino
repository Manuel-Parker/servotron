// Test : le robot avance et contourne les obstacles avec ses deux capteurs ultrason.
// Il démarre 3 secondes après la mise sous tension, le temps de le poser au sol.

const int DIR_M1 = 25, PWM_M1 = 26;
const int DIR_M2 = 5, PWM_M2 = 17;
const int TRIG_1 = 23, ECHO_1 = 22;
const int TRIG_2 = 21, ECHO_2 = 15;
const int VITESSE = 128;
const int SEUIL_CM = 20;

void moteurs(int sensM1, int sensM2, int vitesse) {
  digitalWrite(DIR_M1, sensM1);
  digitalWrite(DIR_M2, sensM2);
  ledcWrite(PWM_M1, vitesse);
  ledcWrite(PWM_M2, vitesse);
}

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

void setup() {
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  ledcAttach(PWM_M1, 500, 8);
  ledcAttach(PWM_M2, 500, 8);
  pinMode(TRIG_1, OUTPUT);
  pinMode(ECHO_1, INPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_2, INPUT);
  delay(3000);
}

void loop() {
  const long d1 = lireDistance(TRIG_1, ECHO_1);
  delay(30);  // Je mesure un capteur à la fois pour éviter qu'ils se perturbent.
  const long d2 = lireDistance(TRIG_2, ECHO_2);

  if (d1 < SEUIL_CM || d2 < SEUIL_CM) {
    moteurs(LOW, HIGH, 0);
    delay(1000);

    // Je tourne du côté où l'obstacle est le plus loin.
    if (d1 < d2) moteurs(HIGH, HIGH, VITESSE);
    else moteurs(LOW, LOW, VITESSE);
    delay(500);
  } else {
    moteurs(LOW, HIGH, VITESSE);
  }
}
