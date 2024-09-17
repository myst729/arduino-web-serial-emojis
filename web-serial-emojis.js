import { setupSerialConnection } from './simple-web-serial.js'

const connection = setupSerialConnection({ baudRate: 9600 })
const cells = document.querySelectorAll('span')
const p = document.querySelector('p')
const canvas = document.querySelector('canvas')
const buttons = document.querySelectorAll('button')
const context = canvas.getContext('2d', { alpha: false, desynchronized: true, willReadFrequently: true })
const emojis = []
const buffer = []
const bufferSize = 16
const events = { START: 1, DATA: 2, END: 3 }

for (let i = 0; i < window.ROW; i++) {
  for (let j = 0; j < window.COL; j++) {
    let emoji = document.createElement('img')
    emoji.src = `${window.BASE}/${i}_${j}.png`
    emojis.push(emoji)
    p.appendChild(emoji)
  }
}
p.style.width = `${window.COL * 50}px`

const drawCells = ({ data }) => {
  const pixels = []
  cells.forEach((cell, i) => {
    const [r, g, b, a] = data.slice(i * 4, i * 4 + 4)
    cell.style.backgroundColor = `rgb(${r}, ${g}, ${b})`
    pixels.push(r << 16 | g << 8 | b)
  })
  return pixels
}

const sendData = (data) => {
  for (let i = 0; i < data.length; i += bufferSize) {
    buffer.push(data.slice(i, i + bufferSize))
  }
  connection.send('event-to-arduino', [events.START])
}

connection.on('event-from-arduino', (data) => {
  if (data === events.DATA) {
    if (buffer.length) {
      connection.send('event-to-arduino', [events.DATA].concat(buffer.shift()))
    } else {
      connection.send('event-to-arduino', [events.END])
    }
  }
})

buttons[0].addEventListener('click', async () => {
  connection.startConnection()
})

buttons[1].addEventListener('click', async () => {
  sendData(Array(64).fill(0))
})

emojis.forEach((em) => {
  em.addEventListener('click', () => {
    context.drawImage(em, 0, 0, 8, 8)
    const imageData = context.getImageData(0, 0, 8, 8)
    const pixelData = drawCells(imageData)
    sendData(pixelData)
  }, false)
})
