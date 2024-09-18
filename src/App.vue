<script setup lang="ts">
import { onMounted, ref } from 'vue'
import CellCanvas from '@/components/CellCanvas.vue'
import EmojiPicker from '@/components/EmojiPicker.vue'
import { serialInit, serialListen, serialSend } from '@/utils/web-serial-helper'

const EMOJI_SIZE = 8
const CELL_SIZE = 40
const emojiSrc = ref('')

const draw = (src: string) => {
  emojiSrc.value = src
}

onMounted(() => {
  serialListen('new-emoji-received', () => {
    console.log('The device sent a custom serial event.') // test
  })
})
</script>

<template>
  <CellCanvas :emoji-size="EMOJI_SIZE" :cell-size="CELL_SIZE" :src-url="emojiSrc" @ready="serialSend" />
  <EmojiPicker :emoji-size="EMOJI_SIZE" @pick="draw" />
  <button @click="serialInit(9600)">Connect</button>
  <button @click="draw('')">Clear</button>
  <a href="./web-serial-emojis.ino" download>Download Arduino Sketch</a>
</template>

<style scoped>
button {
  margin: 10px;
}
</style>
