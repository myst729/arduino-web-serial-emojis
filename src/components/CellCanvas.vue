<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue'

const props = defineProps<{
  emojiSize: number,
  cellSize: number,
  srcUrl: string,
}>()
const emits = defineEmits(['ready'])

const WHITE_COLOR = 0xffffff
const imageRef = ref<null | HTMLImageElement>(null)
const canvasRef = ref<null | HTMLCanvasElement>(null)
const context = ref<null | CanvasRenderingContext2D>(null)
const colors = ref<number[]>(Array(props.emojiSize ** 2).fill(WHITE_COLOR))
const hexVals = computed(() => colors.value.map(c => `#${c.toString(16).padStart(6, '0')}`))

const draw = () => {
  context.value?.drawImage(imageRef.value as CanvasImageSource, 0, 0, props.emojiSize, props.emojiSize)
  const imageData = context.value?.getImageData(0, 0, props.emojiSize, props.emojiSize)
  const pixelData = imageData?.data || []
  colors.value = colors.value.map((_c, i) => {
    const [r, g, b, _a] = pixelData.slice(i * 4, i * 4 + 4)
    return r << 16 | g << 8 | b
  })
  emits('ready', colors.value)
}

watch(
  () => props.srcUrl,
  (newVal: string) => {
    if (!newVal) {
      colors.value = colors.value.map(() => WHITE_COLOR)
      emits('ready', colors.value)
    }
  }
)

onMounted(() => {
  if (canvasRef.value) {
    context.value = canvasRef.value.getContext('2d', { alpha: false, desynchronized: true, willReadFrequently: true })
  }
})
</script>

<template>
  <div :style="{ width: `${emojiSize * cellSize + emojiSize - 1}px` }">
    <span v-for="i in emojiSize ** 2" :key="i" :style="{ backgroundColor: hexVals[i - 1] }"></span>
  </div>
  <canvas :height="emojiSize" :width="emojiSize" ref="canvasRef"></canvas>
  <img :src="srcUrl" alt="emoji" @load="draw" ref="imageRef" />
</template>

<style scoped>
div {
  background-color: #e0e0e0;
  border: 1px solid #e0e0e0;
  display: grid;
  grid-template-columns: repeat(v-bind(emojiSize), calc(v-bind(cellSize) * 1px));
  grid-template-rows: repeat(v-bind(emojiSize), calc(v-bind(cellSize) * 1px));
  gap: 1px;
  user-select: none;
}
span {
  background-color: #fff;
  width: calc(v-bind(cellSize) * 1px);
  height: calc(v-bind(cellSize) * 1px);
  user-select: none;
}
canvas,
img {
  display: none;
  user-select: none;
}
</style>
