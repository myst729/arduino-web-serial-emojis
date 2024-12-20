const emojiModules = import.meta.glob<Record<string, string>>('@/assets/emojis/*.png', { eager: true })
export const emojis = Object.values(emojiModules).map(emojiModule => emojiModule.default)
