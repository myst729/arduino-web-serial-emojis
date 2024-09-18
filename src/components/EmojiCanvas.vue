<script setup lang="ts">
import { computed, onMounted, ref, useTemplateRef } from 'vue'
import PenIcon from '@/components/PenIcon.vue'

const props = defineProps<{
  emojiSize: number,
  cellSize: number,
  srcUrl: string,
}>()
const emits = defineEmits(['ready'])

const DEFAULT_COLOR = 0x0
const imageRef =  useTemplateRef('imageRef')
const canvasRef =  useTemplateRef('canvasRef')
const context = ref<null | CanvasRenderingContext2D>(null)
const colors = ref<number[]>(Array(props.emojiSize ** 2).fill(DEFAULT_COLOR))
const penState = ref({ color: '#42b983', x: 0, y: 0, active: false, visibility: 'hidden', lastCell: -1 })
const hexVals = computed(() => colors.value.map(c => `#${c.toString(16).padStart(6, '0')}`))

const sendSignal = () => {
  emits('ready', colors.value)
}

const drawCell = (cell: HTMLElement) => {
  if (cell.tagName.toLowerCase() === 'span') {
    const index = Number(cell.dataset.index) - 1
    colors.value[index] = Number.parseInt(penState.value.color.slice(1), 16)
    if (penState.value.lastCell !== index) {
      penState.value.lastCell = index
      sendSignal()
    }
  }
}

const drawEmoji = () => {
  context.value?.drawImage(imageRef.value as CanvasImageSource, 0, 0, props.emojiSize, props.emojiSize)
  const imageData = context.value?.getImageData(0, 0, props.emojiSize, props.emojiSize)
  const pixelData = imageData?.data || []
  colors.value = colors.value.map((_c, i) => {
    const [r, g, b, _a] = pixelData.slice(i * 4, i * 4 + 4)
    return r << 16 | g << 8 | b
  })
  sendSignal()
}

const clear = () => {
  colors.value = colors.value.map(() => DEFAULT_COLOR)
  sendSignal()
}

const updatePenState = (e: MouseEvent, state = {}) => {
  penState.value = { ...penState.value, ...state, x: e.clientX, y: e.clientY }
  if (penState.value.active && penState.value.visibility === 'visible') {
    drawCell(e.target as HTMLElement)
  }
}

onMounted(() => {
  if (canvasRef.value) {
    context.value = canvasRef.value.getContext('2d', { alpha: false, desynchronized: true, willReadFrequently: true })
  }
})

defineExpose({ clear })
</script>

<template>
  <article>
    <div
      :style="{ width: `${emojiSize * cellSize + emojiSize - 1}px` }"
      @mousemove="updatePenState"
      @mouseenter="updatePenState($event, { visibility: 'visible' })"
      @mouseleave="updatePenState($event, { visibility: 'hidden' })"
      @mousedown="updatePenState($event, { active: true })"
      @mouseup="updatePenState($event, { active: false })"
    >
      <span v-for="i in emojiSize ** 2" :key="i" :style="{ backgroundColor: hexVals[i - 1] }" :data-index="i">{{ i }}</span>
    </div>
    <section>
      <p>Pick a color and draw</p>
      <input type="color" v-model="penState.color" />
    </section>
  </article>
  <PenIcon class="pen-icon" :size="64" :color="penState.color" :style="{ left: `${penState.x - 5}px`, top: `${penState.y - 5}px` }" />
  <canvas :height="emojiSize" :width="emojiSize" ref="canvasRef"></canvas>
  <img :src="srcUrl" alt="emoji" @load="drawEmoji" ref="imageRef" />
</template>

<style scoped>
article {
  display: flex;
}
div {
  background-color: #e0e0e0;
  border: 1px solid #e0e0e0;
  display: grid;
  grid-template-columns: repeat(v-bind(emojiSize), calc(v-bind(cellSize) * 1px));
  grid-template-rows: repeat(v-bind(emojiSize), calc(v-bind(cellSize) * 1px));
  gap: 1px;
  user-select: none;
  cursor: none;
}
span {
  background-color: #000;
  width: calc(v-bind(cellSize) * 1px);
  height: calc(v-bind(cellSize) * 1px);
  user-select: none;
}
section {
  margin-left: 30px;
  text-align: initial;
}
.pen-icon {
  position: fixed;
  pointer-events: none;
  visibility: v-bind('penState.visibility');
}
canvas,
img {
  display: none;
  user-select: none;
}
</style>
