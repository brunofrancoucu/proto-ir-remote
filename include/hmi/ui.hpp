#pragma once
#include <Arduino.h>

// Mimified and stored in flash memory
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
      height: 100vh;
      margin: 0;
    }
    /* El "cuerpo" del control remoto */
    .remote-body {
      background-color: #2a2a2a;
      padding: 30px 20px;
      border-radius: 40px;
      box-shadow: 0 15px 25px rgba(0,0,0,0.8), inset 0 2px 5px rgba(255,255,255,0.1);
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 20px;
      width: 220px;
    }
    button {
      background-color: #404040;
      border: none;
      color: white;
      padding: 20px 0;
      font-size: 16px;
      font-weight: bold;
      border-radius: 20px;
      cursor: pointer;
      box-shadow: 0 5px 10px rgba(0,0,0,0.5);
      transition: all 0.1s ease;
      touch-action: manipulation; /* Evita zoom al hacer doble tap */
    }
    button:active {
      transform: translateY(4px);
      box-shadow: 0 1px 2px rgba(0,0,0,0.5);
      background-color: #555;
    }
    .btn-power {
      background-color: #d32f2f;
      grid-column: span 2; /* Ocupa las dos columnas */
      border-radius: 30px;
    }
    .btn-wide {
      grid-column: span 2;
    }
  </style>
</head>
<body>

  <div class="remote-body">
    <button class="btn-power" onclick="sendCmd('power')">POWER</button>
    
    <button onclick="sendCmd('vol_up')">Vol +</button>
    <button onclick="sendCmd('ch_up')">Ch +</button>
    
    <button onclick="sendCmd('vol_down')">Vol -</button>
    <button onclick="sendCmd('ch_down')">Ch -</button>
    
    <button class="btn-wide" onclick="sendCmd('mystery')">Misterio</button>
  </div>

  <script>
    // calls route /ir?btn=COMANDO in ESP32
    function sendCmd(command) {
      fetch('/ir?btn=' + command)
        .then(response => {
            if('vibrate' in navigator) navigator.vibrate(50); // Feedback
        })
        .catch(error => console.error('Error:', error));
    }
  </script>

</body>
</html>
)rawliteral";