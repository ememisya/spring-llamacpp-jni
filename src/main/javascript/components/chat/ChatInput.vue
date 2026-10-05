<template>
  <v-card class="chat-input">
    <v-text-field
      v-model="localInput"
      type="text"
      class="user-input"
      :placeholder="t('saySomethingPlaceholder')"
      :readonly="disabledModel"
      :disabled="disabledModel"
      @keydown="handleKeydown"
    >
      <template #prepend-inner>
        <v-progress-circular
          v-if="disabledModel"
          class="mr-2"
          indeterminate
        ></v-progress-circular>
      </template>
    </v-text-field>
  </v-card>
</template>

<script setup lang="ts">
  import { ref } from 'vue'
  import { useI18n } from 'vue-i18n'
  const { t } = useI18n()

  defineProps<{
    disabled: boolean
  }>()

  const disabledModel = defineModel<boolean>('disabled')

  const emit = defineEmits<{
    (e: 'send-message', value: string): void
  }>()

  const localInput = ref('')

  // Emit send-message when Enter is pressed
  function handleKeydown(event: KeyboardEvent) {
    if (disabledModel.value) return

    if (event.key === 'Enter' && !event.shiftKey) {
      event.preventDefault()
      const trimmed = localInput.value.trim()
      if (trimmed) {
        emit('send-message', trimmed)
      }
      localInput.value = ''
    }
  }
</script>

<style scoped>
  .chat-input {
    width: auto;
  }
</style>
