import { mount } from '@vue/test-utils'
import { describe, it, expect } from 'vitest'
import ChatParamsSelect from '@/components/chat/ChatParamsSelect.vue'

describe('ChatParamsSelect.vue', () => {
  it('emits load-data on mount with the current year', () => {
    const wrapper = mount(ChatParamsSelect, {
      props: {
        chatParamsLogIdentifierMap: new Map(),
      },
      global: {
        stubs: {
          VCalendar: true,
        },
      },
    })

    const currentYear = new Date().getFullYear()
    expect(wrapper.emitted('get-chat-params-logs')).toBeTruthy()
    expect(wrapper.emitted('get-chat-params-logs')?.[0]).toEqual([currentYear])
  })
})
