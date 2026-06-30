let pressTimer
const LONG_PRESS_MS = 800 // 800 milliseconds to activate learning mode
let currentLearningButton = ""

// 1. Button logic
function onButtonDown(btnName) {
   // Start the timer when touching the screen or clicking
   pressTimer = setTimeout(() => {
      openLearnModal(btnName)
   }, LONG_PRESS_MS)
}

function onButtonUp(btnName) {
   // If released before 800ms, cancel the long-press and trigger the normal command
   if (pressTimer) {
      clearTimeout(pressTimer)
      sendCmd(btnName) // Your original fetch() function
   }
}

// 2. Modal control
function openLearnModal(btnName) {
   pressTimer = null // Prevents the normal click from firing
   currentLearningButton = btnName

   // Show the modal in the DOM (assuming you have a <dialog> or absolute-positioned <div>)
   document.getElementById("learnModal").style.display = "flex"
   document.getElementById("modalTitle").innerText = `Waiting for signal for: ${btnName}`

   // Notify the ESP32 to start listening
   fetch(`/api/learn/start?btn=${btnName}`)

   // Start polling the ESP32 to check whether it already received something
   pollForSignal()
}

function closeLearnModal() {
   document.getElementById("learnModal").style.display = "none"
   fetch("/api/learn/cancel") // Notify the ESP32 to abort
}

function updateMode() {
   const isLearning = document.getElementById("learnToggle").checked
   document.getElementById("modeText").innerText = isLearning ? "Mode: Learn (Red)" : "Mode: Use Control"
   document.getElementById("modeText").style.color = isLearning ? "#d32f2f" : "white"
}

function showStatus(text, color) {
   const statusEl = document.getElementById("status")
   statusEl.innerText = text
   statusEl.style.backgroundColor = color
   statusEl.style.display = "block"
   setTimeout(() => (statusEl.style.display = "none"), 3000)
}

function sendCmd(btn_id) {
   if ("vibrate" in navigator) navigator.vibrate(50)

   const isLearning = document.getElementById("learnToggle").checked
   const remote_id = document.getElementById("remoteSelect").value
   const endpoint = isLearning ? "/learn" : "/ir"

   if (isLearning) {
      showStatus("Point Controller to record...", "#ff9800")
   }

   fetch(`${endpoint}?remote=${remote_id}&btn=${btn_id}`)
      .then((response) => response.text())
      .then((text) => {
         if (isLearning) {
            showStatus("Saved.", "#4caf50")
            document.getElementById("learnToggle").checked = false // auto-off
            updateMode()
         }
      })
      .catch((error) => {
         showStatus("Connection error.", "#d32f2f")
         console.error(error)
      })
}
