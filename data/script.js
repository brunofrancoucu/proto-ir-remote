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

function sendCmd(command) {
   fetch("/ir?btn=" + command)
      .then((response) => {
         if ("vibrate" in navigator) navigator.vibrate(50)
      })
      .catch((error) => console.error("Error:", error))
}
