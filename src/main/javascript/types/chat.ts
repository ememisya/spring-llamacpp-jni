export type ChatMessageRole = 'user' | 'assistant' | 'system'

export interface ChatParamsLogIdentifierModel {
  id: number
  timestamp: string
}

export type ChatParamsLogIdentifierMap = Map<
  string,
  ChatParamsLogIdentifierModel[]
>

export interface ChatMessage {
  content: string
  role: ChatMessageRole
  id?: string | number
  emotion?: GoEmotionClassifier
}

export interface GoEmotionClassifier {
  admiration: number
  amusement: number
  anger: number
  annoyance: number
  approval: number
  caring: number
  confusion: number
  curiosity: number
  desire: number
  disappointment: number
  disapproval: number
  disgust: number
  embarrassment: number
  excitement: number
  fear: number
  gratitude: number
  grief: number
  joy: number
  love: number
  nervousness: number
  optimism: number
  pride: number
  realization: number
  relief: number
  remorse: number
  sadness: number
  surprise: number
  neutral: number
}

export interface ChatParamsLog {
  chatParams: ChatParams
  id: number | string
  timestamp: number | string
}

export interface ChatSettings {
  batchSize: number
  contextSize: number
  dryAllowedLength: number
  dryBase: number
  dryMultiplier: number
  dryPenaltyLastN: number
  dynamicTemperature: number
  freqPenalty: number
  maxTokens: number
  minP: number
  penaltyLastN: number
  presencePenalty: number
  repeatPenalty: number
  rngSeed: number
  scratchPad: string
  softLimit: number
  softLimitPenalty: number
  temperature: number
  uBatchSize: number
}

export interface ChatParams extends ChatSettings {
  id: number
  messages: ChatMessage[]
}

export interface ChatState extends ChatSettings {
  chatList: ChatMessage[]
  currentUserName: string
  systemMessage: string
  requestPending: number
}
