// Test : le robot recule pendant 2 secondes, puis s'arrête.
// Il démarre 3 secondes après la mise sous tension, le temps de le poser au sol.

const int DIR_M1 = 25, PWM_M1 = 26;
const int DIR_M2 = 5, PWM_M2 = 17;
const int VITESSE = 128;

void setup() {
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  ledcAttach(PWM_M1, 500, 8);
  ledcAttach(PWM_M2, 500, 8);
  delay(3000);

  // Sens inverses de ceux de la marche avant.
  digitalWrite(DIR_M1, HIGH);
  digitalWrite(DIR_M2, LOW);
  ledcWrite(PWM_M1, VITESSE);
  ledcWrite(PWM_M2, VITESSE);
  delay(2000);

  ledcWrite(PWM_M1, 0);
  ledcWrite(PWM_M2, 0);
}

void loop() {}
