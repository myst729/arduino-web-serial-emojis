<script setup lang="ts">
import { emojis } from '@/assets'

defineProps<{ emojiSize: number }>()
const emits = defineEmits(['pick'])

const pick = (src: string) => {
  emits('pick', src)
}
</script>

<template>
  <p>
    <img v-for="(emoji, i) in emojis" :key="i + 1" :src="emoji" :alt="`${i + 1}`" @click="pick(emoji)" />
  </p>
</template>

<style scoped>
p {
  display: flex;
  flex-wrap: wrap;
  max-width: 600px;
  user-select: none;
}
img {
  width: 32px;
  height: 32px;
  padding: 4px;
  margin-right: 8px;
  margin-bottom: 8px;
  border: 1px solid #f0f0f0;
  border-radius: 8px;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  user-select: none;
}
img:hover {
  border: 1px solid #d0d0d0;
}
</style>
