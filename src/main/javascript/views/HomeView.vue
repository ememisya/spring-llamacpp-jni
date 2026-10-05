<template>
  <div class="container">
    <TopBar
      :chat-params-log-identifier-map="chatParamsLogIdentifierMap"
      @toggle-drawer="toggleDrawer"
      @get-chat-params-log="getChatParamsLog"
      @post-chat-params-log="postChatParamsLog"
      @clear-chat="clearChat"
      @get-chat-params-logs="getChatParamsLogs"
      @delete-chat-params-log="deleteChatParamsLog"
    />

    <SideDrawer :drawer="drawer" @update-drawer="updateDrawer">
      <template #content>
        <ChatSettings
          :system-message="systemMessage"
          :scratch-pad="scratchPad"
          :chat-count="chatCount"
          :disabled="requestPending"
          :context-size="contextSize"
          :batch-size="batchSize"
          :u-batch-size="uBatchSize"
          :dry-allowed-length="dryAllowedLength"
          :dry-base="dryBase"
          :dry-multiplier="dryMultiplier"
          :dry-penalty-last-n="dryPenaltyLastN"
          :dynamic-temperature="dynamicTemperature"
          :freq-penalty="freqPenalty"
          :min-p="minP"
          :penalty-last-n="penaltyLastN"
          :presence-penalty="presencePenalty"
          :repeat-penalty="repeatPenalty"
          :rng-seed="rngSeed"
          :temperature="temperature"
          @update-context-size="updateContextSize"
          @update-batch-size="updateBatchSize"
          @update-ubatch-size="updateUBatchSize"
          @update-scratchpad="updateScratchPad"
          @update-dry-allowed-length="updateDryAllowedLength"
          @update-dry-base="updateDryBase"
          @update-dry-multiplier="updateDryMultiplier"
          @update-dry-penalty-last-n="updateDryPenaltyLastN"
          @update-dynamic-temperature="updateDynamicTemperature"
          @update-freq-penalty="updateFreqPenalty"
          @update-min-p="updateMinP"
          @update-penalty-last-n="updatePenaltyLastN"
          @update-presence-penalty="updatePresencePenalty"
          @update-repeat-penalty="updateRepeatPenalty"
          @update-rng-seed="updateRngSeed"
          @update-temperature="updateTemperature"
          @update-system-message="updateSystemMessage"
        />
      </template>
    </SideDrawer>

    <ChatMessages
      :messages="messages"
      :editing="editing"
      :typing="requestPending"
      @edit-message="editMessage"
      @delete-message="deleteMessage"
      @update-message-content="updateMessageContent"
    />

    <BottomBar>
      <ChatBottom :messages-length="chatCount" :editing="editing">
        <template #prepend>
          <ChatInput :disabled="requestPending" @send-message="sendMessage" />
        </template>
        <template #append>
          <label
            class="text-body-small d-inline-flex align-center cursor-pointer"
          >
            <input v-model="forceNewContext" type="checkbox" class="mr-2" />
            {{ $t('forceNewContextCheckboxTitle') }}
          </label>
        </template>
      </ChatBottom>
    </BottomBar>
  </div>
</template>

<script setup lang="ts">
  import { ref, computed } from 'vue'
  import { useStore } from 'vuex'
  import { showSnackbar } from '@/composables/useSnackbar'
  import { key } from '@/store'
  import {
    ChatMessage,
    ChatParams,
    ChatParamsLog,
    ChatParamsLogIdentifierMap,
  } from '@/types'
  import { useI18n } from 'vue-i18n'
  const { t } = useI18n()

  import TopBar from '@/components/layout/TopBar.vue'
  import BottomBar from '@/components/layout/BottomBar.vue'
  import SideDrawer from '@/components/layout/SideDrawer.vue'

  import ChatSettings from '@/components/chat/settings/ChatSettings.vue'

  import ChatBottom from '@/components/chat/ChatBottom.vue'
  import ChatInput from '@/components/chat/ChatInput.vue'
  import ChatMessages from '@/components/chat/ChatMessages.vue'

  import * as T from '@/store/chat.types'
  import {
    doChat,
    doLoadChatParamsLog,
    doSaveChatParams,
    doDeleteChatParamsLog,
    getChatParamsLogIdentifiers,
  } from '@/api/chat'
  import { scrollToBottom, playBeep } from '@/composables'

  const chatParamsLogIdentifierMap = ref<ChatParamsLogIdentifierMap>(new Map())

  // Store
  const store = useStore(key)

  // UI state
  const editing = ref<number[]>([false])
  const drawer = ref(false)

  const forceNewContext = ref(false)

  // Derived state
  const requestPending = computed(() => store.getters.requestPending)
  const messages = computed(() => store.getters.chatList)
  const chatCount = computed(() => store.getters.chatCount)

  const systemMessage = computed(() => store.getters.systemMessage)
  const scratchPad = computed(() => store.getters.scratchPad)

  const contextSize = computed(() => store.getters.contextSize)
  const batchSize = computed(() => store.getters.batchSize)
  const uBatchSize = computed(() => store.getters.uBatchSize)

  const dryAllowedLength = computed(() => store.getters.dryAllowedLength)
  const dryBase = computed(() => store.getters.dryBase)
  const dryMultiplier = computed(() => store.getters.dryMultiplier)
  const dryPenaltyLastN = computed(() => store.getters.dryPenaltyLastN)
  const dynamicTemperature = computed(() => store.getters.dynamicTemperature)
  const freqPenalty = computed(() => store.getters.freqPenalty)
  const minP = computed(() => store.getters.minP)
  const penaltyLastN = computed(() => store.getters.penaltyLastN)
  const presencePenalty = computed(() => store.getters.presencePenalty)
  const repeatPenalty = computed(() => store.getters.repeatPenalty)
  const rngSeed = computed(() => store.getters.rngSeed)
  const temperature = computed(() => store.getters.temperature)

  function constructRequest(): ChatParams {
    const generatedSystemMessage: ChatMessage = {
      role: 'system',
      content: systemMessage.value,
    }
    return {
      messages: [generatedSystemMessage, ...messages.value],
      scratchPad: scratchPad.value,
      contextSize: contextSize.value,
      batchSize: batchSize.value,
      uBatchSize: uBatchSize.value,
      dryAllowedLength: dryAllowedLength.value,
      dryBase: dryBase.value,
      dryMultiplier: dryMultiplier.value,
      dryPenaltyLastN: dryPenaltyLastN.value,
      dynamicTemperature: dynamicTemperature.value,
      freqPenalty: freqPenalty.value,
      minP: minP.value,
      penaltyLastN: penaltyLastN.value,
      presencePenalty: presencePenalty.value,
      repeatPenalty: repeatPenalty.value,
      rngSeed: rngSeed.value,
      temperature: temperature.value,
    }
  }

  // Drawer
  function toggleDrawer() {
    console.log('HomeView toggleDrawer b = ' + drawer.value)
    drawer.value = !drawer.value
    console.log('HomeView toggleDrawer a = ' + drawer.value)
  }

  function updateDrawer(value: boolean) {
    console.log('HomeView toggleDrawer = ' + value)
    drawer.value = value
  }

  // System message
  function updateSystemMessage(value: string) {
    store.dispatch(T.SET_SYSTEM_MESSAGE_ACTION, value)
  }

  // Model settings
  function updateContextSize(value: number) {
    store.dispatch(T.SET_CONTEXT_SIZE_ACTION, value)
  }

  function updateBatchSize(value: number) {
    store.dispatch(T.SET_BATCH_SIZE_ACTION, value)
  }

  function updateUBatchSize(value: number) {
    store.dispatch(T.SET_UBATCH_SIZE_ACTION, value)
  }

  // Scratchpad
  function updateScratchPad(value: string) {
    store.dispatch(T.SET_SCRATCHPAD_ACTION, value)
  }

  function updateDryAllowedLength(value: number) {
    store.dispatch(T.SET_DRY_ALLOWED_LENGTH_ACTION, value)
  }

  function updateDryBase(value: number) {
    store.dispatch(T.SET_DRY_BASE_ACTION, value)
  }

  function updateDryMultiplier(value: number) {
    store.dispatch(T.SET_DRY_MULTIPLIER_ACTION, value)
  }

  function updateDryPenaltyLastN(value: number) {
    store.dispatch(T.SET_DRY_PENALTY_LAST_N_ACTION, value)
  }

  function updateDynamicTemperature(value: number) {
    store.dispatch(T.SET_DYNAMIC_TEMPERATURE_ACTION, value)
  }

  function updateFreqPenalty(value: number) {
    store.dispatch(T.SET_FREQ_PENALTY_ACTION, value)
  }

  function updateMinP(value: number) {
    store.dispatch(T.SET_MIN_P_ACTION, value)
  }

  function updatePenaltyLastN(value: number) {
    store.dispatch(T.SET_PENALTY_LAST_N_ACTION, value)
  }

  function updatePresencePenalty(value: number) {
    store.dispatch(T.SET_PRESENCE_PENALTY_ACTION, value)
  }

  function updateRepeatPenalty(value: number) {
    store.dispatch(T.SET_REPEAT_PENALTY_ACTION, value)
  }

  function updateRngSeed(value: number) {
    store.dispatch(T.SET_RNG_SEED_ACTION, value)
  }

  function updateTemperature(value: number) {
    store.dispatch(T.SET_TEMPERATURE_ACTION, value)
  }

  // Chat message operations
  async function sendMessage(text: string) {
    store.dispatch(T.ADD_MESSAGE_ACTION, {
      id: -1 * Math.floor(Math.random() * 65000) + 1,
      role: 'user',
      content: text,
    })
    scrollToBottom('chat-messages')
    await doChatFlow()
    scrollToBottom('chat-messages')
  }

  function editMessage(id: number) {
    if (id !== null) {
      if (!editing?.value?.includes(id)) {
        editing.value.push(id)
      }
    }
  }

  function updateMessageContent({ id, content }) {
    store.dispatch(T.UPDATE_MESSAGE_ACTION, { id, content })
    if (id !== null) {
      const indexOfIndex = editing?.value?.indexOf(id)
      if (indexOfIndex !== -1) {
        editing?.value?.splice(indexOfIndex, 1)
      }
    }
  }

  function deleteMessage(id: number) {
    store.dispatch(T.DELETE_MESSAGE_ACTION, { id })
    if (id !== null) {
      const indexOfIndex = editing?.value?.indexOf(id)
      if (indexOfIndex !== -1) {
        editing?.value?.splice(indexOfIndex, 1)
      }
    }
  }

  async function getChatParamsLog(id: number) {
    const data: ChatParamsLog = await doLoadChatParamsLog(id)
    store.dispatch(T.SET_STATE_FROM_REQUEST_ACTION, data.chatParams)
    showSnackbar(t('loadedStateMessage'), 'success', 2500)
  }

  async function deleteChatParamsLog(id: number) {
    await doDeleteChatParamsLog(id)
    for (const [, logArray] of chatParamsLogIdentifierMap.value.entries()) {
      const index = logArray.findIndex((item) => item.id === id)
      if (index !== -1) {
        logArray.splice(index, 1)
        break
      }
    }
  }

  async function postChatParamsLog() {
    const chatParamsLog: ChatParamsLog =
      await doSaveChatParams(constructRequest())
    chatParamsLogIdentifierMap.value
      .get(new Date().getFullYear())
      ?.push(chatParamsLog)
    showSnackbar(t('savedStateMessage'), 'success', 2500)
  }

  async function getChatParamsLogs(year: number) {
    if (!chatParamsLogIdentifierMap.value.has(year)) {
      const currentYear = await getChatParamsLogIdentifiers(year)
      chatParamsLogIdentifierMap.value.set(year, currentYear)
    }
  }

  function clearChat() {
    store.dispatch(T.CLEAR_CHAT_ACTION)
    showSnackbar(t('clearedStateMessage'), 'success', 2500)
  }

  async function doChatFlow() {
    store.dispatch(T.SET_REQUEST_PENDING_ACTION, true)

    try {
      const response = await doChat(constructRequest(), forceNewContext.value)
      store.dispatch(T.ADD_MESSAGE_ACTION, {
        id: -1 * Math.floor(Math.random() * 65000) + 1,
        role: 'assistant',
        content: response,
      })
      playBeep()
    } finally {
      store.dispatch(T.SET_REQUEST_PENDING_ACTION, false)
    }
  }
</script>

<style scoped lang="scss">
  .container {
  }
</style>
