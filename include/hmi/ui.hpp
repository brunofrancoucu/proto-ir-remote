// Web interface
#pragma once
#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>ESP32 Remote</title>
  <style>
    body {
      background-color: #121212;
      color: white;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      margin: 0;
      padding: 20px 0;
    }
    /* El "cuerpo" del control remoto */
    .remote-body {
      background-color: #2a2a2a;
      padding: 30px 20px;
      border-radius: 40px;
      box-shadow: 0 15px 25px rgba(0,0,0,0.8), inset 0 2px 5px rgba(255,255,255,0.1);
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 15px;
      width: 240px;
    }
    button {
      background-color: #404040;
      border: none;
      color: white;
      padding: 20px 0;
      font-size: 16px;
      font-weight: bold;
      border-radius: 15px;
      cursor: pointer;
      box-shadow: 0 5px 10px rgba(0,0,0,0.5);
      transition: all 0.1s ease;
      touch-action: manipulation;
    }
    button:active {
      transform: translateY(4px);
      box-shadow: 0 1px 2px rgba(0,0,0,0.5);
      background-color: #555;
    }
    .btn-power {
      background-color: #d32f2f;
      grid-column: span 2;
      border-radius: 30px;
    }
    
    /* D-PAD CIRCULAR */
    .d-pad {
      grid-column: span 2;
      display: grid;
      grid-template-areas: 
        ". up ."
        "left center right"
        ". down .";
      gap: 4px;
      width: 170px;
      height: 170px;
      margin: 10px auto;
      background-color: #1e1e1e; /* Fondo oscuro del círculo */
      border-radius: 50%; /* Lo hace redondo */
      padding: 12px;
      box-shadow: inset 0 4px 10px rgba(0,0,0,0.7), 0 2px 5px rgba(255,255,255,0.05);
    }
    .d-pad button {
      padding: 0;
      border-radius: 8px;
      background-color: #383838;
      display: flex;
      justify-content: center;
      align-items: center;
      font-size: 18px;
      box-shadow: 0 3px 6px rgba(0,0,0,0.4);
    }
    .d-pad button:active {
      background-color: #666;
    }
    /* Asignación de áreas y bordes redondeados para seguir la forma circular */
    .d-pad .d-up { grid-area: up; border-radius: 30px 30px 8px 8px; }
    .d-pad .d-down { grid-area: down; border-radius: 8px 8px 30px 30px; }
    .d-pad .d-left { grid-area: left; border-radius: 30px 8px 8px 30px; }
    .d-pad .d-right { grid-area: right; border-radius: 8px 30px 30px 8px; }
    .d-pad .d-center { 
      grid-area: center; 
      border-radius: 50%; 
      background-color: #505050; 
      font-size: 14px;
    }
    
    /* Botones Macro */
    .macro-row {
      background-color: #222;
      border-radius: 12px;
    }
  </style>
</head>
<body>

  <div class="remote-body">
    <button class="btn-power" onclick="sendCmd('power')">POWER</button>
    
    <div class="d-pad">
      <button class="d-up" onclick="sendCmd('up')">▲</button>
      <button class="d-left" onclick="sendCmd('left')">◀</button>
      <button class="d-center" onclick="sendCmd('ok')">OK</button>
      <button class="d-right" onclick="sendCmd('right')">▶</button>
      <button class="d-down" onclick="sendCmd('down')">▼</button>
    </div>
    
    <button onclick="sendCmd('vol_up')">Vol +</button>
    <button onclick="sendCmd('ch_up')">Ch +</button>
    
    <button onclick="sendCmd('vol_down')">Vol -</button>
    <button onclick="sendCmd('ch_down')">Ch -</button>
    
    <button class="macro-row" onclick="sendCmd('m1')">M1</button>
    <button class="macro-row" onclick="sendCmd('m2')">M2</button>
  </div>

  <script>
    function sendCmd(command) {
      fetch('/ir?btn=' + command)
        .then(response => {
            if('vibrate' in navigator) navigator.vibrate(50);
        })
        .catch(error => console.error('Error:', error));
    }
  </script>

</body>
</html>
)rawliteral";