export type ChatMessageRole = 'user' | 'assistant' | 'system'

export interface ChatParamsLogIdentifierModel {
  id: number
  timestamp: string
}

// Define the Map type alias for cleaner code
export type ChatParamsLogIdentifierMap = Map<
  string,
  ChatParamsLogIdentifierModel[]
>

export interface ChatMessage {
  content: string
  role: ChatMessageRole
  id?: string | number
}

export interface ChatParamsLog {
  chatParams: ChatParams
  id: number | string
  timestamp: number | string
}

export interface ChatParams {
  batchSize: number
  contextSize: number
  dryAllowedLength: number
  dryBase: number
  dryMultiplier: number
  dryPenaltyLastN: number
  dynamicTemperature: number
  freqPenalty: number
  id: number
  messages: ChatMessage[]
  minP: number
  penaltyLastN: number
  presencePenalty: number
  repeatPenalty: number
  rngSeed: number
  scratchPad: string
  temperature: number
  uBatchSize: number
}

export interface ChatState {
  chatList: ChatMessage[]
  currentUserName: string
  systemMessage: string
  contextSize: number
  batchSize: number
  uBatchSize: number
  dryAllowedLength: number
  dryBase: number
  dryMultiplier: number
  dryPenaltyLastN: number
  dynamicTemperature: number
  freqPenalty: number
  minP: number
  penaltyLastN: number
  presencePenalty: number
  repeatPenalty: number
  rngSeed: number
  scratchPad: string
  temperature: number
  requestPending: boolean
}
