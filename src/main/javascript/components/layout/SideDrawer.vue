<template>
  <v-navigation-drawer
    v-model="drawerModel"
    temporary
    class="side-drawer"
    @update:model-value="handleDrawerUpdate"
  >
    <v-sheet elevation="2">
      <slot name="content">
        <v-card>content</v-card>
      </slot>
      <v-divider />
    </v-sheet>
  </v-navigation-drawer>
</template>

<script setup lang="ts">
  import { defineModel } from 'vue'

  defineProps<{
    drawer: boolean
  }>()

  // Two-way binding for drawer state
  const drawerModel = defineModel<boolean>('drawer')

  const emit = defineEmits<{
    (e: 'update-drawer', open: boolean): void
  }>()

  function handleDrawerUpdate(open: number) {
    console.log('SideDrawer handleDrawerUpdate = ' + open)
    emit('update-drawer', open)
  }
</script>

<style scoped>
  .side-drawer {
    align-content: normal;
  }
</style>
