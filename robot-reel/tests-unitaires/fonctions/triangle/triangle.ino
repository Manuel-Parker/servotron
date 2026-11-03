// Test : le robot trace un triangle, une seule fois.
// Les durées viennent de mes essais sur le robot : à ajuster selon votre sol et votre batterie.

const int DIR_M1 = 25, PWM_M1 = 26;
const int DIR_M2 = 5, PWM_M2 = 17;
const int VITESSE = 128;

void avancer(int duree) {
  digitalWrite(DIR_M1, LOW);
  digitalWrite(DIR_M2, HIGH);
  ledcWrite(PWM_M1, VITESSE);
  ledcWrite(PWM_M2, VITESSE);
  delay(duree);
}

void droite(int duree) {
  digitalWrite(DIR_M1, HIGH);
  digitalWrite(DIR_M2, HIGH);
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

  avancer(2000);
  droite(800);
  avancer(2000);
  droite(900);
  avancer(2350);

  ledcWrite(PWM_M1, 0);
  ledcWrite(PWM_M2, 0);
}

void loop() {}
