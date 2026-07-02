const NOTIFICATION_TIMEOUT = 2000
let pollInterval

/** Handle slider toggle */
function updateMode() {
   if (document.getElementById("learnToggle").checked) _startLearning(); else _stopLearning() // prettier-ignore
}

function _startLearning() {
   // 1. Handle Interface
   document.getElementById("learnToggle").checked = true
   document.getElementById("modeText").innerText = "Mode: Learn (Red)"
   document.getElementById("modeText").style.color = "#d32f2f"

   // 2. Handle Backend state
   fetch("/api/learn/start")

   // 3. Receiver signal at interface
   _pollForSignal()
}

function _stopLearning() {
   // 1. Handle Interface
   document.getElementById("learnToggle").checked = false
   document.getElementById("modeText").innerText = "Mode: Use Control"
   document.getElementById("modeText").style.color = "white"

   // 2. Handle Backend state
   fetch("/api/learn/stop")

   // 3. Receiver signal at interface
   clearInterval(pollInterval)
}

/** Handle (`onClick`) Learning | Transmiting */
function handleClick(btn_id) {
   const isLearning = document.getElementById("learnToggle").checked
   const remote_id = document.getElementById("remoteSelect").value

   if (!isLearning) {
      // 1. Normal mode
      fetch(`/api/transmit?remote=${remote_id}&btn=${btn_id}`).then((r) => !r.ok && showStatus("No signal stored.", "#d32f2f"))
   } else {
      // 1. Learn mode
      fetch(`/api/learn/send?remote=${remote_id}&btn=${btn_id}`)
         .then((response) => response.text())
         .then((text) => {
            showStatus("Saved signal at: " + btn_id, "#4caf50")
            _stopLearning()
         })
   }
}

function _pollForSignal() {
   const showPulses = (pulses) => showStatus(`Caught new signal! (${pulses} pulses). Click a button below to assign.`, "#4caf50")
   pollInterval = setInterval(() => {
      fetch("/api/learn/buffer")
         .then((response) => response.json())
         .then((data) => data.status === "success" && showPulses(data.pulses))
         .catch((error) => showStatus("Polling error: " + error, "#d32f2f"))
   }, 250)
}

// Utils

function showStatus(text, color) {
   const statusEl = document.getElementById("status")
   statusEl.innerText = text
   statusEl.style.backgroundColor = color
   statusEl.style.display = "block"
   setTimeout(() => (statusEl.style.display = "none"), NOTIFICATION_TIMEOUT)
}
