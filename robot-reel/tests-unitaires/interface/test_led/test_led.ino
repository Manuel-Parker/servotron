// Test de la LED RVB : rouge, vert, bleu, puis les couleurs des modes.

const int LED_ROUGE = 2, LED_VERTE = 19, LED_BLEUE = 12;

void led(int r, int v, int b) {
  analogWrite(LED_ROUGE, r);
  analogWrite(LED_VERTE, v);
  analogWrite(LED_BLEUE, b);
  delay(1000);
}

void setup() {
  pinMode(LED_ROUGE, OUTPUT);
  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_BLEUE, OUTPUT);
}

void loop() {
  led(255, 0, 0);    // Évitement d'obstacle
  led(0, 255, 0);    // Suivi de ligne
  led(0, 0, 255);    // Carré
  led(254, 254, 0);  // Cercle
  led(236, 0, 255);  // Triangle
  led(0, 0, 0);
}
