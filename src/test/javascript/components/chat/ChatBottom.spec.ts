import { mount } from '@vue/test-utils'
import { describe, it, expect } from 'vitest'
import ChatBottom from '@/components/chat/ChatBottom.vue'

describe('ChatBottom.vue', () => {
  it('displays correct message length count', () => {
    const wrapper = mount(ChatBottom, {
      props: {
        editing: [],
        currentTokens: 5,
        maxTokens: 10,
        messagesLength: 3,
      },
    })

    expect(wrapper.text()).toContain('50%')
  })

  it('renders custom prepend slot content', () => {
    const wrapper = mount(ChatBottom, {
      props: { editing: [], messagesLength: 0 },
      slots: {
        prepend: 'Custom Content',
      },
    })

    expect(wrapper.text()).toContain('Custom Content')
  })
})
