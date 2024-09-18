<script setup lang="ts">
import { onMounted, ref, useTemplateRef } from 'vue'
import EmojiCanvas from '@/components/EmojiCanvas.vue'
import EmojiPicker from '@/components/EmojiPicker.vue'
import { serialInit, serialListen, serialSend } from '@/utils/web-serial-helper'

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

onMounted(() => {
  serialListen('new-emoji-received', () => {
    console.log('The device sent a custom serial event.') // test
  })
})
</script>

<template>
  <EmojiCanvas :emoji-size="EMOJI_SIZE" :cell-size="CELL_SIZE" :src-url="emojiSrc" @ready="serialSend" ref="emojiCanvasRef" />
  <EmojiPicker :emoji-size="EMOJI_SIZE" @pick="draw" />
  <button @click="serialInit(9600)">Connect</button>
  <button @click="clear">Clear</button>
  <a href="./web-serial-emojis.ino" download>Download Arduino Sketch</a>
</template>

<style scoped>
button,
a {
  margin: 0 10px;
}
</style>
