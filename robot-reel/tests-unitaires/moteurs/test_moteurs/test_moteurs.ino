// Test de chaque moteur séparément, dans les deux sens.
// Poser le robot sur un support : les roues ne doivent pas toucher le sol.
// Sert à vérifier le câblage : chaque roue doit tourner vers l'avant puis vers l'arrière.

const int DIR_M1 = 25, PWM_M1 = 26;
const int DIR_M2 = 5, PWM_M2 = 17;
const int VITESSE = 128;

void tourner(int pwm, int dir, int sens, const char *message) {
  Serial.println(message);
  digitalWrite(dir, sens);
  ledcWrite(pwm, VITESSE);
  delay(2000);
  ledcWrite(pwm, 0);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  ledcAttach(PWM_M1, 500, 8);
  ledcAttach(PWM_M2, 500, 8);
}

void loop() {
  // Les moteurs sont montés en miroir : LOW fait avancer le M1, HIGH fait avancer le M2.
  tourner(PWM_M1, DIR_M1, LOW, "Moteur 1 : avant");
  tourner(PWM_M1, DIR_M1, HIGH, "Moteur 1 : arrière");
  tourner(PWM_M2, DIR_M2, HIGH, "Moteur 2 : avant");
  tourner(PWM_M2, DIR_M2, LOW, "Moteur 2 : arrière");
}
