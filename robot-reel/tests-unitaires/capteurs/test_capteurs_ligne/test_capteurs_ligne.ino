// Test des trois capteurs de ligne.
// Ouvrir le moniteur série à 115200 bauds et passer une bande noire sous chaque capteur.
// 1 = le capteur voit la ligne.

const int LIGNE_GAUCHE = 32, LIGNE_CENTRE = 35, LIGNE_DROITE = 33;

void setup() {
  Serial.begin(115200);
  pinMode(LIGNE_GAUCHE, INPUT);
  pinMode(LIGNE_CENTRE, INPUT);
  pinMode(LIGNE_DROITE, INPUT);
}

void loop() {
  Serial.print("Gauche : ");
  Serial.print(digitalRead(LIGNE_GAUCHE));
  Serial.print(" | Centre : ");
  Serial.print(digitalRead(LIGNE_CENTRE));
  Serial.print(" | Droite : ");
  Serial.println(digitalRead(LIGNE_DROITE));
  delay(200);
}
