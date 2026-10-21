import request from '@/utils/request'
import type {
  ChatParams,
  ApiResponse,
  ChatParamsLog,
  ChatParamsLogIdentifierModel,
} from '@/types'

export function postCompletions(
  data: ChatParams,
  forceNewContext?: boolean,
): Promise<ApiResponse<string>> {
  if (forceNewContext) {
    return request({
      url: '/chat/completions',
      method: 'post',
      params: { forceNewContext: true },
      data,
    })
  }
  return request({
    url: '/chat/completions',
    method: 'post',
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
