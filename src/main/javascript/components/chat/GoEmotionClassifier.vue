<template>
  <v-card class="pa-1 my-2 emotion-card" variant="outlined" density="compact">
    <div class="emotions-grid">
      <div
        v-for="(val, key) in emotion"
        :key="key"
        class="emotion-item"
        :title="`${key}: ${typeof val === 'number' ? val.toFixed(1) : val}%`"
      >
        <span class="label text-truncate d-inline-block">{{
          String(key)
        }}</span>
        <div class="track">
          <div class="fill" :style="{ width: `${Number(val)}%` }"></div>
        </div>
      </div>
    </div>
  </v-card>
</template>

<script setup lang="ts">
  import type { GoEmotionClassifier } from '@/types' // Adjust path to your types if needed

  defineProps<{
    emotion: GoEmotionClassifier
  }>()
</script>

<style scoped lang="scss">
  .emotion-card {
    font-size: 10px;
    background: rgba(var(--v-theme-surface), 0.5) !important;
  }

  .emotions-grid {
    display: grid;
    grid-template-columns: repeat(4, minmax(0, 1fr));
    gap: 2px 4px;
  }

  .emotion-item {
    display: flex;
    flex-direction: column;
  }

  .label {
    word-break: break-all !important;
    clear: both;
    font-size: 9px;
    line-height: 1;
    color: rgba(var(--v-theme-on-surface), 0.7);
    text-transform: lowercase;
    white-space: nowrap;
  }

  .track {
    height: 3px;
    background: rgba(var(--v-theme-on-surface), 0.1);
    border-radius: 1px;
    margin-top: 1px;
  }

  .fill {
    height: 100%;
    background: rgb(var(--v-theme-primary));
  }
</style>
