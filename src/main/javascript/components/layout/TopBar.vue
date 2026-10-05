<template>
  <v-app-bar color="teal-darken-4" class="top-bar">
    <template #image>
      <v-img :gradient="currentGradient" />
    </template>
    <template #prepend>
      <v-app-bar-nav-icon @click.stop="toggleDrawer" />
    </template>
    <v-app-bar-title>{{ $t('appTitle') }}</v-app-bar-title>
    <v-btn icon="mdi-delete" @click="emitClearChat"> </v-btn>
    <v-btn icon="mdi-content-save" @click="emitPostChatParamsLog"> </v-btn>

    <v-menu :close-on-content-click="false" location="bottom end">
      <template #activator="{ props }">
        <v-btn icon="mdi-sync" v-bind="props"></v-btn>
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

    <!-- Settings Menu with Switches -->
    <v-menu :close-on-content-click="false" location="bottom end">
      <template #activator="{ props }">
        <v-btn icon="mdi-cog" v-bind="props"></v-btn>
      </template>
      <v-card min-width="200px" class="pa-3">
        <v-switch
          v-model="isDarkTheme"
          :label="`Theme: ${isDarkTheme ? 'Dark' : 'Light'}`"
          density="compact"
          hide-details
          class="mb-2"
          @change="toggleTheme"
        />
        <v-switch
          v-model="isPrimaryLocale"
          :label="`Language: ${locale.toUpperCase()}`"
          density="compact"
          hide-details
          @change="toggleLanguage"
        />
      </v-card>
    </v-menu>
  </v-app-bar>
</template>

<script setup lang="ts">
  import { ref, computed } from 'vue'
  import { useI18n } from 'vue-i18n'
  import { useTheme } from 'vuetify'
  import { ChatParamsLogIdentifierMap } from '@/types'
  import type { CalendarEvent } from 'vuetify/labs/VCalendar'

  const { locale, t } = useI18n()
  const theme = useTheme()

  const isDarkTheme = ref(theme.global.current.value.dark)
  const isPrimaryLocale = ref(locale.value === 'tr')

  const currentGradient = computed(() => {
    return isDarkTheme.value
      ? 'to top right, rgba(10, 30, 45, 0.9), rgba(20, 60, 55, 0.9)'
      : 'to top right, rgba(19, 84, 122, 0.8), rgba(128, 208, 199, 0.8)'
  })

  import ChatParamsSelect from '@/components/chat/ChatParamsSelect.vue'

  const toggleLanguage = () => {
    locale.value = isPrimaryLocale.value ? 'tr' : 'en'
    document.title = t('appTitle')
    localStorage.setItem('user-locale', locale.value)
  }

  const toggleTheme = () => {
    const newTheme = isDarkTheme.value ? 'dark' : 'light'
    theme.change(newTheme)
    localStorage.setItem('user-theme', theme.global.name.value)
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
