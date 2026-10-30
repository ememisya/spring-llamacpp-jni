import request from '@/utils/request'
import type {
  ChatParams,
  ApiResponse,
  ChatParamsLog,
  ChatParamsLogIdentifierModel,
  GoEmotionClassifier,
} from '@/types'

export function postEncoder(
  data: ChatParams,
): Promise<ApiResponse<GoEmotionClassifier>> {
  return request({
    url: '/chat/encoder',
    method: 'post',
    responseType: 'json',
    data,
  })
}
//curiousity > 19   desire > 24  excitement > 19 fear > 15 optimism > 15   Switch to BPD
// anger > 14 desire > 11 excitement > 17 realization > 14 Switch back to MoE
export function postCompletions(
  data: ChatParams,
  forceNewContext?: boolean,
): Promise<ApiResponse<string>> {
  if (forceNewContext) {
    return request({
      url: '/chat/completions',
      method: 'post',
      responseType: 'text',
      params: { forceNewContext: true },
      data,
    })
  }
  return request({
    url: '/chat/completions',
    method: 'post',
    responseType: 'text',
    data,
  })
}

export function postTokenCount(data: ChatParams): Promise<ApiResponse<number>> {
  return request({
    url: '/chat/token-count',
    method: 'post',
    data,
  })
}

export function postChatParamsLog(
  data: ChatParams,
): Promise<ApiResponse<ChatParamsLog>> {
  return request({
    url: '/chat/chat-params-log',
    method: 'post',
    data,
  })
}

export function getModelsList(): Promise<ApiResponse<string[]>> {
  return request({
    url: '/chat/models-list',
    method: 'get',
  })
}

export function getModelSlot(): Promise<ApiResponse<number>> {
  return request({
    url: '/chat/model-slot',
    method: 'get',
  })
}

export function postReloadModel(
  modelIndex: number,
): Promise<ApiResponse<void>> {
  return request({
    url: '/chat/reload-model',
    method: 'post',
    params: { modelIndex },
  })
}

export function getChatParamsLog(
  id: number,
): Promise<ApiResponse<ChatParamsLog>> {
  return request({
    url: '/chat/chat-params-log',
    method: 'get',
    params: { id },
  })
}

export function deleteChatParamsLog(id: number): Promise<ApiResponse<void>> {
  return request({
    url: '/chat/chat-params-log',
    method: 'delete',
    params: { id },
  })
}

export function getChatParamsLogIdentifiers(
  year: number,
): Promise<ApiResponse<ChatParamsLogIdentifierModel[]>> {
  return request({
    url: '/chat/chat-params-log-identifiers',
    method: 'get',
    params: { year },
  })
}
