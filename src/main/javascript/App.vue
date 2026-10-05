<template>
  <v-layout>
    <v-main class="d-flex flex-column fill-height">
      <GlobalSnackbar />
      <router-view />
    </v-main>
  </v-layout>
</template>

<script setup>
  import GlobalSnackbar from '@/components/GlobalSnackbar.vue'
</script>
