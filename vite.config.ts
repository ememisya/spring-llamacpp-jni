import { defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";
import { resolve } from "path";

export default defineConfig({
  // Vite dev/build root
  root: resolve(__dirname, "src/main/javascript"),

  resolve: {
    alias: {
      "@": resolve(__dirname, "src/main/javascript")
    }
  },

  plugins: [vue()],

  css: {
    preprocessorOptions: {
      scss: {
        additionalData: `@use "@/styles/variables.scss";`,
      }
    }
  },

  server: {
    host: true,
    open: true,
    watch: {
      usePolling: true,
      interval: 100
    },
    port: 8080,
    strictPort: true,
    cors: true
  },

  build: {
    outDir: resolve(__dirname, "target/public"),
    emptyOutDir: true
  }
});
