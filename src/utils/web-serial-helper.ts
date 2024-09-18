import { setupSerialConnection } from 'simple-web-serial'

type BufferChunk = number[]
enum DataEvent {
  PREPARE = 1,
  READY = 2,
  END = 3,
}

let connection: any
let bufferSize: number
let buffer: BufferChunk[] = []
let customEvents: any = {}

export const serialInit = (baudRate = 9600, bufSize = 16) => {
  bufferSize = bufSize
  connection = setupSerialConnection({ baudRate })
  connection.on('event-from-arduino', (event: DataEvent | string) => {
    if (event === DataEvent.READY) {
      if (buffer.length) {
        connection.send('event-to-arduino', [DataEvent.READY].concat(buffer.shift() as BufferChunk))
      } else {
        connection.send('event-to-arduino', [DataEvent.END])
      }
    }
    Object.keys(customEvents).forEach((e: string) => {
      if (e === event) {
        customEvents[e].forEach((cb: () => void) => {
          cb()
        })
      }
    })
  })
  connection.startConnection()
}

export const serialSend = (data: number[]) => {
  for (let i = 0; i < data.length; i += bufferSize) {
    buffer.push(data.slice(i, i + bufferSize))
  }
  connection?.send('event-to-arduino', [DataEvent.PREPARE])
}

export const serialListen = (event: string, callback: () => void) => {
  if (customEvents[event]) {
    customEvents[event].push(callback)
  } else {
    customEvents[event] = [callback]
  }
}
