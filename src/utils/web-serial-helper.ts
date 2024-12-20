import {
  setupSerialConnection,
  SerialConnection,
  JsonArray,
} from 'simple-web-serial'

enum EventState {
  READY = 1,
  CHUNK = 2,
  FINISH = 3,
}
type BufferChunk = number[]
type EventCallback = (data?: JsonArray) => void

let bufferSize: number
let buffer: BufferChunk[] = []
let connection: SerialConnection
let customEvents: {[key:string]: Array<EventCallback> } = {}

export const serialInit = (baudRate = 9600, bufSize = 16) => {
  bufferSize = bufSize
  connection = setupSerialConnection({ baudRate })
  connection.on('event-from-arduino', (response) => {
    const [event, ...data] = [...(response as JsonArray)]
    if (event === EventState.CHUNK) {
      if (buffer.length) {
        connection.send('event-to-arduino', [EventState.CHUNK].concat(buffer.shift() as BufferChunk))
      } else {
        connection.send('event-to-arduino', [EventState.FINISH])
      }
    }
    Object.keys(customEvents).forEach((evt: string) => {
      if (evt === event) {
        customEvents[evt].forEach((cb: EventCallback) => {
          cb(data)
        })
      }
    })
  })
  connection.startConnection()
}

export const serialSendData = (data: number[]) => {
  for (let i = 0; i < data.length; i += bufferSize) {
    buffer.push(data.slice(i, i + bufferSize))
  }
  connection.send('event-to-arduino', [EventState.READY])
}

export const serialSend = (event: EventState | string, data: JsonArray = []) => {
  connection.send('event-to-arduino', [event, ...data])
}

export const serialListen = (event: string, callback: EventCallback) => {
  if (customEvents[event]) {
    customEvents[event].push(callback)
  } else {
    customEvents[event] = [callback]
  }
}
