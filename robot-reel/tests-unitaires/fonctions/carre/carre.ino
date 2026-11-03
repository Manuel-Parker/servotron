// Test : le robot trace un carré, une seule fois.
// Le robot n'a pas d'encodeurs : tout est chronométré. Ajuster les deux durées selon votre sol et votre batterie.

const int DIR_M1 = 25, PWM_M1 = 26;
const int DIR_M2 = 5, PWM_M2 = 17;
const int VITESSE = 128;
const int DUREE_COTE_MS = 2000;
const int DUREE_90_MS = 690;

void moteurs(int sensM1, int sensM2, int duree) {
  digitalWrite(DIR_M1, sensM1);
  digitalWrite(DIR_M2, sensM2);
  ledcWrite(PWM_M1, VITESSE);
  ledcWrite(PWM_M2, VITESSE);
  delay(duree);
}

void setup() {
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  ledcAttach(PWM_M1, 500, 8);
  ledcAttach(PWM_M2, 500, 8);
  delay(3000);

  for (int i = 0; i < 4; i++) {
    moteurs(LOW, HIGH, DUREE_COTE_MS);  // Avancer
    moteurs(HIGH, HIGH, DUREE_90_MS);   // Tourner à droite
  }

  ledcWrite(PWM_M1, 0);
  ledcWrite(PWM_M2, 0);
}

void loop() {}
