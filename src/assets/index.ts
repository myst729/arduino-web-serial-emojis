const emojisModules = import.meta.glob('@/assets/emojis/*.png', { eager: true })
export const emojis = Object.values(emojisModules).map((module: any) => module.default)
