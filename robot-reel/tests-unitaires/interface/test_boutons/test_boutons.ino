// Test des quatre boutons du couvercle.
// Ouvrir le moniteur série à 115200 bauds et appuyer sur chaque bouton.

const int BOUTONS[] = {13, 14, 27, 18};
const char *NOMS[] = {"Suivi de ligne", "Obstacle", "Carré", "Stop"};
bool etatPrecedent[] = {false, false, false, false};

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) pinMode(BOUTONS[i], INPUT_PULLUP);
}

void loop() {
  for (int i = 0; i < 4; i++) {
    const bool appuye = digitalRead(BOUTONS[i]) == LOW;  // Bouton appuyé = état bas.
    if (appuye && !etatPrecedent[i]) {
      Serial.print("Bouton : ");
      Serial.println(NOMS[i]);
    }
    etatPrecedent[i] = appuye;
  }
  delay(20);
}
