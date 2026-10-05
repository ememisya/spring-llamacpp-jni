import { fileURLToPath, URL } from 'node:url'
import { defineConfig } from 'vitest/config'
import vue from '@vitejs/plugin-vue'

export default defineConfig({
  plugins: [vue()],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src/main/javascript', import.meta.url))
    }
  },
  test: {
    environment: 'happy-dom',
    css: true,
    pool: 'threads',
    setupFiles: ['./vitest.setup.ts'],
    server: {
      deps: {
        inline: ['vuetify'],
      },
    },
    include: ['src/test/javascript/**/*.{test,spec}.{js,mjs,cjs,ts,mts,cts,jsx,tsx}'],
  },
})
