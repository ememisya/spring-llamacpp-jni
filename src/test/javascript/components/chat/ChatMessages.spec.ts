import { mount } from '@vue/test-utils'
import { describe, it, expect } from 'vitest'
import ChatMessages from '@/components/chat/ChatMessages.vue'

describe('ChatMessages.vue', () => {
  it('renders chat messages and typing indicator correctly', () => {
    const messages = [
      { id: 1, role: 'user', content: 'Hi' },
      { id: 2, role: 'assistant', content: 'Hello' },
    ]

    const wrapper = mount(ChatMessages, {
      props: {
        typing: true,
        messages,
        editing: [],
      },
    })

    expect(wrapper.text()).toContain('Hi')
    expect(wrapper.text()).toContain('Hello')
    expect(wrapper.text()).toContain('AI is typing...')
  })

  it('emits delete-message event when delete button is clicked in edit mode', async () => {
    const messages = [{ id: 1, role: 'user', content: 'Hi' }]

    const wrapper = mount(ChatMessages, {
      props: {
        typing: false,
        messages,
        editing: [1],
      },
    })

    const deleteBtn = wrapper.find('.btn-chat')
    await deleteBtn.trigger('click')

    expect(wrapper.emitted('delete-message')).toBeTruthy()
    expect(wrapper.emitted('delete-message')?.[0]).toEqual([1])
  })
})
