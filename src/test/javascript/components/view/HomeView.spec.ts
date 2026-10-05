import { mount, flushPromises } from '@vue/test-utils'
import { describe, it, expect, vi, beforeEach, type MockInstance } from 'vitest'

import { VLayout } from 'vuetify/components'
import HomeView from '@/views/HomeView.vue'
import TopBar from '@/components/layout/TopBar.vue'
import ChatInput from '@/components/chat/ChatInput.vue'
import ChatSettings from '@/components/chat/settings/ChatSettings.vue'

import * as T from '@/store/chat.types'
import * as chatApi from '@/api/chat'
import { store, key } from '@/store'
import request from '@/utils/request'
import MockAdapter from 'axios-mock-adapter'

const mockAxios = new MockAdapter(request)

describe('HomeView.vue', () => {
  let dispatchSpy: MockInstance

  beforeEach(() => {
    mockAxios.reset()
    mockAxios.onPost('/chat/completions').reply(200, 'Mocked response')
    mockAxios.onGet(/\/chat\/.*log.*/).reply(200, [])

    vi.clearAllMocks()

    // Spies on doChat while calling the original code
    vi.spyOn(chatApi, 'doChat')

    dispatchSpy = vi.spyOn(store, 'dispatch').mockImplementation(async () => {})

    store.state.chatList = [
      { id: 1, role: 'assistant', content: 'Hello World' },
    ]
    store.state.requestPending = false
  })

  const mountHomeView = () => {
    return mount(VLayout, {
      slots: {
        default: HomeView,
      },
      global: {
        plugins: [[store, key]],
        stubs: {
          ChatParamsSelect: true,
        },
      },
    })
  }

  it('renders the view successfully', () => {
    const wrapper = mountHomeView()
    expect(wrapper.findComponent(HomeView).exists()).toBe(true)
  })

  it('dispatches clear chat action when TopBar emits clear-chat', async () => {
    const wrapper = mountHomeView()
    const homeView = wrapper.findComponent(HomeView)
    const topBar = homeView.findComponent(TopBar)

    await topBar.vm.$emit('clear-chat')

    expect(dispatchSpy).toHaveBeenCalledWith(T.CLEAR_CHAT_ACTION)
  })

  it('triggers send message flow when ChatInput emits send-message', async () => {
    const wrapper = mountHomeView()
    const homeView = wrapper.findComponent(HomeView)
    const chatInput = homeView.findComponent(ChatInput)

    await chatInput.vm.$emit('send-message', 'Hello AI')
    await flushPromises()

    expect(dispatchSpy).toHaveBeenCalledWith(T.ADD_MESSAGE_ACTION, {
      role: 'user',
      id: expect.any(Number),
      content: 'Hello AI',
    })
    expect(chatApi.doChat).toHaveBeenCalled()
  })

  it('dispatches system message update when ChatSettings emits update-system-message', async () => {
    const wrapper = mountHomeView()
    const homeView = wrapper.findComponent(HomeView)
    const chatSettings = homeView.findComponent(ChatSettings)

    await chatSettings.vm.$emit('update-system-message', 'New System Message')

    expect(dispatchSpy).toHaveBeenCalledWith(
      T.SET_SYSTEM_MESSAGE_ACTION,
      'New System Message',
    )
  })
})
