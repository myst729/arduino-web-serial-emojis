<script setup lang="ts">
import { onMounted, ref, useTemplateRef } from 'vue'
import EmojiCanvas from '@/components/EmojiCanvas.vue'
import EmojiPicker from '@/components/EmojiPicker.vue'
import { serialInit, serialListen, serialSend, serialSendData } from '@/utils/web-serial-helper'

const EMOJI_SIZE = 8
const CELL_SIZE = 40
const emojiSrc = ref('')
const emojiCanvasRef = useTemplateRef('emojiCanvasRef')

const draw = (src: string) => {
  emojiSrc.value = src
}

const clear = () => {
  emojiCanvasRef.value?.clear()
}

const download = () => {
  serialSend('download-event', ['Arduino sketch is downloading.'])
}

onMounted(() => {
  serialListen('unknown-event', (data = []) => {
    const [msg, ...payload] = [...data]
    console.log('UNKNOWN', {msg, payload })
  })
})
</script>

<template>
  <EmojiCanvas :emoji-size="EMOJI_SIZE" :cell-size="CELL_SIZE" :src-url="emojiSrc" @ready="serialSendData" ref="emojiCanvasRef" />
  <EmojiPicker :emoji-size="EMOJI_SIZE" @pick="draw" />
  <button @click="serialInit(9600)">Connect</button>
  <button @click="clear">Clear</button>
  <a href="./web-serial-emojis.ino" download @click="download">Download Arduino Sketch</a>
</template>

<style scoped>
button,
a {
  margin: 0 10px;
}
</style>
