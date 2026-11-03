// Test : le robot trace un cercle, une seule fois.
// Une roue tourne plus vite que l'autre. L'écart règle le rayon, la durée règle la longueur du tour.

const int DIR_M1 = 25, PWM_M1 = 26;
const int DIR_M2 = 5, PWM_M2 = 17;
const int VITESSE_M1 = 220;
const int VITESSE_M2 = 85;
const int DUREE_TOUR_MS = 4500;

void setup() {
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  ledcAttach(PWM_M1, 500, 8);
  ledcAttach(PWM_M2, 500, 8);
  delay(3000);

  digitalWrite(DIR_M1, LOW);
  digitalWrite(DIR_M2, HIGH);
  ledcWrite(PWM_M1, VITESSE_M1);
  ledcWrite(PWM_M2, VITESSE_M2);
  delay(DUREE_TOUR_MS);

  ledcWrite(PWM_M1, 0);
  ledcWrite(PWM_M2, 0);
}

void loop() {}
