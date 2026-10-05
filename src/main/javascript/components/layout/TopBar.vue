<template>
  <v-app-bar color="teal-darken-4" class="top-bar">
    <template #image>
      <v-img
        gradient="to top right, rgba(19,84,122,.8), rgba(128,208,199,.8)"
      />
    </template>
    <template #prepend>
      <v-app-bar-nav-icon @click.stop="toggleDrawer" />
    </template>
    <v-app-bar-title>{{ $t('appTitle') }}</v-app-bar-title>
    <v-btn icon="mdi-delete" @click="emitClearChat"> </v-btn>
    <v-btn icon="mdi-content-save" @click="emitPostChatParamsLog"> </v-btn>

    <v-menu :close-on-content-click="false" location="bottom end">
      <!-- 2. Use the activator slot for your trigger button -->
      <template #activator="{ props }">
        <v-btn icon="mdi-reload" v-bind="props"></v-btn>
      </template>
      <v-card min-width="80vw" max-width="90vw" class="pa-4">
        <ChatParamsSelect
          :chat-params-log-identifier-map="chatParamsLogIdentifierMap"
          @get-chat-params-log="emitGetChatParamsLog"
          @delete-chat-params-log="emitDeleteChatParamsLog"
          @get-chat-params-logs="emitGetChatParamsLogs"
        />
      </v-card>
    </v-menu>

    <v-btn @click="toggleLanguage">
      {{ locale.toUpperCase() }}
    </v-btn>
  </v-app-bar>
</template>

<script setup lang="ts">
  import { useI18n } from 'vue-i18n'
  import { ChatParamsLogIdentifierMap } from '@/types'
  import type { CalendarEvent } from 'vuetify/labs/VCalendar'

  const { locale } = useI18n()
  const { t } = useI18n()

  import ChatParamsSelect from '@/components/chat/ChatParamsSelect.vue'

  const toggleLanguage = () => {
    locale.value = locale.value === 'en' ? 'tr' : 'en'
    document.title = t('appTitle')
    localStorage.setItem('user-locale', locale.value)
  }

  defineProps<{
    chatParamsLogIdentifierMap: ChatParamsLogIdentifierMap
  }>()

  // Emits
  const emit = defineEmits<{
    (e: 'get-chat-params-log', id: number): void
    (e: 'post-chat-params-log'): void
    (e: 'delete-chat-params-log', id: number): void
    (e: 'get-chat-params-logs', year: number): void
    (e: 'clear-chat'): void
    (e: 'toggle-drawer'): void
  }>()

  // Method to toggle
  function toggleDrawer() {
    emit('toggle-drawer')
  }

  function emitGetChatParamsLog(selectedEvent: CalendarEvent) {
    emit('get-chat-params-log', selectedEvent?.id)
  }

  function emitDeleteChatParamsLog(selectedEvent: CalendarEvent) {
    emit('delete-chat-params-log', selectedEvent?.id)
  }

  function emitGetChatParamsLogs(year: number) {
    emit('get-chat-params-logs', year)
  }

  function emitPostChatParamsLog() {
    emit('post-chat-params-log')
  }

  function emitClearChat() {
    emit('clear-chat')
  }
</script>

<style scoped>
  .top-bar {
    align-content: normal;
  }
</style>
