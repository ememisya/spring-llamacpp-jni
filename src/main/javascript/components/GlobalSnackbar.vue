<template>
  <v-snackbar
    v-model="state.show"
    :timeout="state.timeout"
    :color="state.color"
    location="bottom center"
    rounded
  >
    {{ state.message }}
    <template #actions>
      <v-btn variant="text" @click="state.show = false">{{
        $t('closeButtonTitle')
      }}</v-btn>
    </template>
  </v-snackbar>
</template>

<script setup lang="ts">
  import { snackbarState as state } from '../composables/useSnackbar'
</script>
