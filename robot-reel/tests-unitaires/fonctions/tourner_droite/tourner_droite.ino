// Test : le robot pivote à droite sur place, d'environ 90°.
// Ajuster DUREE_90_MS si l'angle n'est pas bon.

const int DIR_M1 = 25, PWM_M1 = 26;
const int DIR_M2 = 5, PWM_M2 = 17;
const int VITESSE = 128;
const int DUREE_90_MS = 690;

void setup() {
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  ledcAttach(PWM_M1, 500, 8);
  ledcAttach(PWM_M2, 500, 8);
  delay(3000);

  // Le M1 tourne vers l'arrière et le M2 vers l'avant.
  digitalWrite(DIR_M1, HIGH);
  digitalWrite(DIR_M2, HIGH);
  ledcWrite(PWM_M1, VITESSE);
  ledcWrite(PWM_M2, VITESSE);
  delay(DUREE_90_MS);

  ledcWrite(PWM_M1, 0);
  ledcWrite(PWM_M2, 0);
}

void loop() {}
