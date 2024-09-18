<script setup lang="ts">
defineProps<{
  color: string,
  size: number,
}>()
</script>

<template>
  <svg :width="size" :height="size" viewBox="0 0 272 272" xmlns="http://www.w3.org/2000/svg">
    <g transform="rotate(90, 136, 136)">
      <path fill="#666" d="M41.74 160.32l71.16 71.27L260.78 70c4.85-5.3 4.15-14-1.59-19.78l-36.39-36.4c-5.74-5.74-14.46-6.45-19.75-1.59z"/>
      <path fill="#999" d="M20.4 238.71l14.23 14.26 78.28-21.39-71.17-71.26-21.34 78.39z"/>
      <path :fill="color" d="M12.11 258.65l8.58 3.84a4.38 4.38 0 0 0 5-.53l9-9-14.29-14.25L9 250.08c-2.24 2.29-.58 6.92 3.11 8.57z"/>
    </g>
  </svg>
</template>
