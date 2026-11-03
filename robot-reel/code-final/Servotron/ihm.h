// Page web de l'IHM, copie de robot-reel/ihm/index.html.
#pragma once
#include <pgmspace.h>

const char PAGE_IHM[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Contrôle du Robot Mobile</title>
<style>
  body { font-family: Arial, sans-serif; text-align: center; background: linear-gradient(135deg, #1e3c72, #2a5298);
         color: white; margin: 0; min-height: 100vh; user-select: none; -webkit-user-select: none; }
  h1 { margin-top: 20px; }
  .control-panel { margin-top: 50px; display: flex; justify-content: space-between; align-items: center; padding: 0 20px; }
  .left-panel { display: flex; flex-direction: column; align-items: flex-start; gap: 10px; }
  .left-panel button { margin-bottom: 15px; }
  .right-panel { display: flex; flex-direction: column; align-items: flex-end; gap: 20px; }
  .mode-container { display: flex; gap: 10px; }
  .mode-container > button, .mode-container .dropdown { margin-right: 10px; }
  .horizontal-movement { display: flex; gap: 10px; }
  .horizontal-movement button { margin-right: 15px; }
  button { width: 150px; height: 80px; font-size: 20px; border: none; border-radius: 10px; cursor: pointer; transition: 0.2s;
           display: flex; align-items: center; justify-content: center; box-shadow: 0 4px 6px rgba(0,0,0,0.2); touch-action: none; }
  button:active, button.pressed { transform: scale(0.9); }
  button svg { width: 24px; height: 24px; }
  .btn-avancer { background: #4CAF50; color: white; }
  .btn-reculer { background: #f44336; color: white; }
  .btn-droite, .btn-gauche { background: #008CBA; color: white; }
  .btn-stop { background: #555555; color: white; width: 100px; margin-top: 20px; gap: 6px; }
  .btn-stop svg { width: 16px; height: 16px; }
  .btn-mode { background: #ff9800; color: white; padding: 10px 20px; }
  .dropdown { position: relative; }
  .dropdown-content { display: none; position: absolute; background-color: #f1f1f1; min-width: 160px;
                      box-shadow: 0px 8px 16px rgba(0,0,0,0.2); z-index: 1; border-radius: 5px; }
  .dropdown:hover .dropdown-content, .dropdown.open .dropdown-content { display: block; }
  .dropdown-content button { width: 100%; height: auto; background-color: #ff9800; color: white; border-radius: 0;
                             box-shadow: none; padding: 10px; justify-content: flex-start; font-size: 18px; }
  .dropdown-content button:hover { background-color: #f44336; }
  input[type="range"] { width: 80%; margin: 20px 0; }
</style>
</head>
<body>
  <h1>Contrôle du Robot Mobile</h1>

  <div class="control-panel">
    <div class="left-panel">
      <button class="btn-avancer" data-maintien="avancer" aria-label="Avancer"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"><path d="M12 19V5M5 12l7-7 7 7"/></svg></button>
      <button class="btn-reculer" data-maintien="reculer" aria-label="Reculer"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"><path d="M12 5v14M19 12l-7 7-7-7"/></svg></button>
    </div>

    <div class="right-panel">
      <div class="mode-container">
        <div class="dropdown" id="formes">
          <button class="btn-mode" id="ouvrirFormes">Formes</button>
          <div class="dropdown-content">
            <button data-commande="cercle">Cercle</button>
            <button data-commande="carre">Carré</button>
            <button data-commande="triangle">Triangle</button>
          </div>
        </div>
        <button class="btn-mode" data-commande="suiviLigne">Suivi de Ligne</button>
        <button class="btn-mode" data-commande="detectionObstacle">Evitement d'Obstacle</button>
      </div>

      <div class="horizontal-movement">
        <button class="btn-gauche" data-maintien="gauche" aria-label="Gauche"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"><path d="M19 12H5M12 19l-7-7 7-7"/></svg></button>
        <button class="btn-droite" data-maintien="droite" aria-label="Droite"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"><path d="M5 12h14M12 5l7 7-7 7"/></svg></button>
      </div>

      <button class="btn-stop" data-commande="stop"><svg viewBox="0 0 24 24" fill="currentColor"><rect x="4" y="4" width="16" height="16"/></svg> Stop</button>
    </div>
  </div>

  <div class="speed-control">
    <label for="vitesse">Vitesse</label>
    <input type="range" id="vitesse" min="0" max="100" value="50" step="1">
    <span id="valeurVitesse">50</span>%
  </div>

<script>
  const envoyer = route => fetch('/' + route).catch(() => {});

  // Déplacements : le robot roule tant que le bouton reste appuyé.
  document.querySelectorAll('[data-maintien]').forEach(bouton => {
    const appui = e => {
      e.preventDefault();
      bouton.classList.add('pressed');
      envoyer(bouton.dataset.maintien);
    };
    const relache = () => {
      if (!bouton.classList.contains('pressed')) return;
      bouton.classList.remove('pressed');
      envoyer('stop');
    };
    bouton.addEventListener('pointerdown', appui);
    bouton.addEventListener('pointerup', relache);
    bouton.addEventListener('pointerleave', relache);
    bouton.addEventListener('pointercancel', relache);
  });

  // Modes et bouton Stop.
  const formes = document.getElementById('formes');
  document.querySelectorAll('[data-commande]').forEach(bouton => {
    bouton.addEventListener('click', () => {
      envoyer(bouton.dataset.commande);
      formes.classList.remove('open');
    });
  });

  // Sur téléphone il n'y a pas de survol : le menu Formes s'ouvre aussi au clic.
  document.getElementById('ouvrirFormes').addEventListener('click', e => {
    e.stopPropagation();
    formes.classList.toggle('open');
  });
  document.addEventListener('click', () => formes.classList.remove('open'));

  // Vitesse : j'envoie la valeur au relâchement pour ne pas saturer l'ESP32.
  const curseur = document.getElementById('vitesse');
  const valeur = document.getElementById('valeurVitesse');
  curseur.addEventListener('input', () => { valeur.textContent = curseur.value; });
  curseur.addEventListener('change', () => envoyer('speed?value=' + curseur.value));
  envoyer('speed?value=' + curseur.value);
</script>
</body>
</html>
)rawliteral";
