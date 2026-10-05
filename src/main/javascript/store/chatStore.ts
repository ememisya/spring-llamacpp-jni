import { ActionContext } from 'vuex'
import * as T from './chat.types'
import { ChatMessage, ChatState, ChatParams } from '@/types'

export const ChatStore = {
  // =========================
  // STATE (typed)
  // =========================
  state: (): ChatState => ({
    chatList: [],

    currentUserName: '',
    systemMessage: '',

    contextSize: 18432,
    batchSize: 4096,
    uBatchSize: 512,
    requestPending: false,
    dryAllowedLength: 2,
    dryBase: 1.75,
    dryMultiplier: 0.523,
    dryPenaltyLastN: 512,
    dynamicTemperature: 0.1465,
    freqPenalty: 0.05,
    minP: 0.05,
    penaltyLastN: 64,
    presencePenalty: 0.023,
    repeatPenalty: 1.0543,
    rngSeed: 42,
    scratchPad: '',
    temperature: 0.8023,
  }),

  // =========================
  // GETTERS (typed)
  // =========================
  getters: {
    chatCount(state: ChatState): number {
      return state.chatList.length
    },

    chatList(state: ChatState): ChatMessage[] {
      return state.chatList
    },

    systemMessage(state: ChatState): string {
      return state.systemMessage
    },

    dynamicTemperature(state: ChatState): number {
      return state.dynamicTemperature
    },

    dryPenaltyLastN(state: ChatState): number {
      return state.dryPenaltyLastN
    },

    dryMultiplier(state: ChatState): number {
      return state.dryMultiplier
    },

    dryBase(state: ChatState): number {
      return state.dryBase
    },

    dryAllowedLength(state: ChatState): number {
      return state.dryAllowedLength
    },

    requestPending(state: ChatState): boolean {
      return state.requestPending
    },

    uBatchSize(state: ChatState): number {
      return state.uBatchSize
    },

    batchSize(state: ChatState): number {
      return state.batchSize
    },

    contextSize(state: ChatState): number {
      return state.contextSize
    },

    temperature(state: ChatState): number {
      return state.temperature
    },

    scratchPad(state: ChatState): string {
      return state.scratchPad
    },

    rngSeed(state: ChatState): number {
      return state.rngSeed
    },

    repeatPenalty(state: ChatState): number {
      return state.repeatPenalty
    },

    presencePenalty(state: ChatState): number {
      return state.presencePenalty
    },

    penaltyLastN(state: ChatState): number {
      return state.penaltyLastN
    },

    minP(state: ChatState): number {
      return state.minP
    },

    freqPenalty(state: ChatState): number {
      return state.freqPenalty
    },

    isRequestPending(state: ChatState): boolean {
      return state.requestPending
    },
  },

  // =========================
  // ACTIONS (typed)
  // =========================
  actions: {
    [T.DELETE_MESSAGE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      payload: { id: number },
    ): void {
      commit(T.DELETE_MESSAGE, payload)
    },

    [T.ADD_MESSAGE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      message: ChatMessage,
    ): void {
      commit(T.ADD_MESSAGE, message)
    },

    [T.UPDATE_MESSAGE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      payload: { id: string; content: string },
    ): void {
      commit(T.UPDATE_MESSAGE, payload)
    },

    [T.SET_REQUEST_PENDING_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: boolean,
    ): void {
      commit(T.SET_REQUEST_PENDING, value)
    },

    [T.SET_SYSTEM_MESSAGE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      message: string,
    ): void {
      commit(T.SET_SYSTEM_MESSAGE, message)
    },

    [T.SET_STATE_FROM_REQUEST_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      request: ChatParams,
    ): void {
      commit(T.SET_STATE_FROM_REQUEST, request)
    },

    [T.SET_CONTEXT_SIZE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ): void {
      commit(T.SET_CONTEXT_SIZE, value)
    },

    [T.SET_BATCH_SIZE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ): void {
      commit(T.SET_BATCH_SIZE, value)
    },

    [T.SET_UBATCH_SIZE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ): void {
      commit(T.SET_UBATCH_SIZE, value)
    },

    [T.CLEAR_CHAT_ACTION]({
      commit,
    }: ActionContext<ChatState, ChatState>): void {
      commit(T.CLEAR_CHAT)
    },
    [T.SET_DRY_ALLOWED_LENGTH_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_DRY_ALLOWED_LENGTH, value)
    },

    [T.SET_DRY_BASE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_DRY_BASE, value)
    },

    [T.SET_DRY_MULTIPLIER_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_DRY_MULTIPLIER, value)
    },

    [T.SET_DRY_PENALTY_LAST_N_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_DRY_PENALTY_LAST_N, value)
    },

    [T.SET_DYNAMIC_TEMPERATURE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_DYNAMIC_TEMPERATURE, value)
    },

    [T.SET_FREQ_PENALTY_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_FREQ_PENALTY, value)
    },

    [T.SET_MIN_P_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_MIN_P, value)
    },

    [T.SET_PENALTY_LAST_N_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_PENALTY_LAST_N, value)
    },

    [T.SET_PRESENCE_PENALTY_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_PRESENCE_PENALTY, value)
    },

    [T.SET_REPEAT_PENALTY_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_REPEAT_PENALTY, value)
    },

    [T.SET_RNG_SEED_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_RNG_SEED, value)
    },

    [T.SET_TEMPERATURE_ACTION](
      { commit }: ActionContext<ChatState, ChatState>,
      value: number,
    ) {
      commit(T.SET_TEMPERATURE, value)
    },

    generateRandomId(length: number = 12): string {
      const chars =
        'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'
      return Array.from({ length }, () =>
        chars.charAt(Math.floor(Math.random() * chars.length)),
      ).join('')
    },
  },

  // =========================
  // MUTATIONS (typed)
  // =========================
  mutations: {
    [T.SET_DRY_ALLOWED_LENGTH](state: ChatState, value: number) {
      state.dryAllowedLength = value
    },

    [T.SET_DRY_BASE](state: ChatState, value: number) {
      state.dryBase = value
    },

    [T.SET_DRY_MULTIPLIER](state: ChatState, value: number) {
      state.dryMultiplier = value
    },

    [T.SET_DRY_PENALTY_LAST_N](state: ChatState, value: number) {
      state.dryPenaltyLastN = value
    },

    [T.SET_DYNAMIC_TEMPERATURE](state: ChatState, value: number) {
      state.dynamicTemperature = value
    },

    [T.SET_FREQ_PENALTY](state: ChatState, value: number) {
      state.freqPenalty = value
    },

    [T.SET_MIN_P](state: ChatState, value: number) {
      state.minP = value
    },

    [T.SET_PENALTY_LAST_N](state: ChatState, value: number) {
      state.penaltyLastN = value
    },

    [T.SET_PRESENCE_PENALTY](state: ChatState, value: number) {
      state.presencePenalty = value
    },

    [T.SET_REPEAT_PENALTY](state: ChatState, value: number) {
      state.repeatPenalty = value
    },

    [T.SET_RNG_SEED](state: ChatState, value: number) {
      state.rngSeed = value
    },

    [T.SET_TEMPERATURE](state: ChatState, value: number) {
      state.temperature = value
    },

    [T.ADD_MESSAGE](state: ChatState, message: ChatMessage): void {
      state.chatList.push(message)
    },

    [T.DELETE_MESSAGE](state: ChatState, payload: { id: number }): void {
      const { id } = payload
      const indexOfIndex = state.chatList.findIndex((m) => m.id === id)
      if (indexOfIndex !== -1) {
        state.chatList.splice(indexOfIndex, 1)
      }
    },

    [T.UPDATE_MESSAGE](
      state: ChatState,
      payload: { id: string; content: string },
    ): void {
      const msg = state.chatList.find((m) => m.id === payload.id)
      if (msg) msg.content = payload.content
    },

    [T.SET_REQUEST_PENDING](state: ChatState, value: boolean): void {
      state.requestPending = value
    },

    [T.SET_SYSTEM_MESSAGE](state: ChatState, message: string): void {
      state.systemMessage = message
    },

    [T.SET_CONTEXT_SIZE](state: ChatState, value: number): void {
      state.contextSize = value
    },

    [T.SET_BATCH_SIZE](state: ChatState, value: number): void {
      state.batchSize = value
    },

    [T.SET_STATE_FROM_REQUEST](state: ChatState, value: ChatParams): void {
      state.batchSize = value.batchSize
      state.contextSize = value.contextSize
      state.dryAllowedLength = value.dryAllowedLength
      state.dryBase = value.dryBase
      state.dryMultiplier = value.dryMultiplier
      state.dryPenaltyLastN = value.dryPenaltyLastN
      state.dynamicTemperature = value.dynamicTemperature
      state.freqPenalty = value.freqPenalty
      state.minP = value.minP
      state.penaltyLastN = value.penaltyLastN
      state.presencePenalty = value.presencePenalty
      state.repeatPenalty = value.repeatPenalty
      state.rngSeed = value.rngSeed
      state.scratchPad = value.scratchPad
      state.temperature = value.temperature
      state.uBatchSize = value.uBatchSize

      const systemMessage = value.messages.find((m) => m.role === 'system')
      if (systemMessage) {
        state.systemMessage = systemMessage.content
        state.chatList = value.messages.filter((m) => m.role !== 'system')
      }
    },

    [T.SET_UBATCH_SIZE](state: ChatState, value: number): void {
      state.uBatchSize = value
    },

    [T.CLEAR_CHAT](state: ChatState): void {
      state.chatList = []
    },
  },
}
