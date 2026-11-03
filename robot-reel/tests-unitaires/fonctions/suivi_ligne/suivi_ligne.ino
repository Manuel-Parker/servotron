// Test : suivi d'une ligne noire au sol avec les trois capteurs.
// Poser le robot sur la ligne : il démarre 3 secondes après la mise sous tension.

const int DIR_M1 = 25, PWM_M1 = 26;
const int DIR_M2 = 5, PWM_M2 = 17;
const int LIGNE_GAUCHE = 32, LIGNE_CENTRE = 35, LIGNE_DROITE = 33;
const int VITESSE = 128;

void moteurs(int sensM1, int sensM2) {
  digitalWrite(DIR_M1, sensM1);
  digitalWrite(DIR_M2, sensM2);
  ledcWrite(PWM_M1, VITESSE);
  ledcWrite(PWM_M2, VITESSE);
}

void setup() {
  pinMode(DIR_M1, OUTPUT);
  pinMode(DIR_M2, OUTPUT);
  ledcAttach(PWM_M1, 500, 8);
  ledcAttach(PWM_M2, 500, 8);
  pinMode(LIGNE_GAUCHE, INPUT);
  pinMode(LIGNE_CENTRE, INPUT);
  pinMode(LIGNE_DROITE, INPUT);
  delay(3000);
}

void loop() {
  // Un capteur à l'état haut voit la ligne.
  const bool g = digitalRead(LIGNE_GAUCHE);
  const bool c = digitalRead(LIGNE_CENTRE);
  const bool d = digitalRead(LIGNE_DROITE);

  if (g && !d) moteurs(LOW, LOW);         // Ligne à gauche : je tourne à gauche.
  else if (!g && d) moteurs(HIGH, HIGH);  // Ligne à droite : je tourne à droite.
  else if (c) moteurs(LOW, HIGH);         // Ligne au centre : j'avance.
  else moteurs(HIGH, LOW);                // Ligne perdue : je recule pour la retrouver.
}
