<template>
  <v-bottom-navigation height="56" app class="bottom-bar">
    <slot>
      <!-- Fallback actions -->
      <v-card>Bottom Bar</v-card>
    </slot>
  </v-bottom-navigation>
</template>

<script setup lang="ts">
  import { onMounted } from 'vue'

  onMounted(() => {
    console.debug('✅ Component mounted')
  })
</script>

<style scoped>
  .bottom-bar {
    align-content: normal;
  }
</style>
