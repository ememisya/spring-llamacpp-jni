import request from '@/utils/request'
import type {
  ChatParams,
  ApiResponse,
  ChatParamsLog,
  ChatParamsLogIdentifierModel,
} from '@/types'

export function doChat(
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

export function doSaveChatParams(
  data: ChatParams,
): Promise<ApiResponse<ChatParamsLog>> {
  return request({
    url: '/chat/chat-params-log',
    method: 'post',
    data,
  })
}

export function getLastChatParamsLog(): Promise<ApiResponse<ChatParamsLog>> {
  return request({
    url: '/chat/last',
    method: 'get',
  })
}

export function doLoadChatParamsLog(
  id: number,
): Promise<ApiResponse<ChatParamsLog>> {
  return request({
    url: '/chat/chat-params-log',
    method: 'get',
    params: { id },
  })
}

export function doDeleteChatParamsLog(id: number): Promise<ApiResponse<void>> {
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
