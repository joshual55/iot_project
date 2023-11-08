import { fileURLToPath, URL } from "node:url";

import { defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue()],
  resolve: {
    alias: {
      "@": fileURLToPath(new URL("./src", import.meta.url)),
    },
  },
  server: {
    proxy: {
      // with options: http://localhost:5173/api/bar-> http://jsonplaceholder.typicode.com/bar
      "/api": {
        target: "https://9otaictrgg.execute-api.us-east-2.amazonaws.com/my-add-face-deployment",
        changeOrigin: true,
        rewrite: (path) => path.replace(/^\/api/, ""),
      }, 
    },
  },
});
