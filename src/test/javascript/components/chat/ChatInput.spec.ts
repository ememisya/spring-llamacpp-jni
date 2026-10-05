import { mount } from '@vue/test-utils'
import { describe, it, expect } from 'vitest'
import ChatInput from '@/components/chat/ChatInput.vue'

describe('ChatInput.vue', () => {
  it('emits send-message when enter is pressed and input is active', async () => {
    const wrapper = mount(ChatInput, {
      props: { disabled: false },
    })

    const input = wrapper.find('input')
    await input.setValue('Hello AI')
    await input.trigger('keydown', { key: 'Enter' })

    expect(wrapper.emitted('send-message')).toBeTruthy()
    expect(wrapper.emitted('send-message')?.[0]).toEqual(['Hello AI'])
  })

  it('does not emit send-message when disabled', async () => {
    const wrapper = mount(ChatInput, {
      props: { disabled: true },
    })

    const input = wrapper.find('input')
    await input.setValue('Hello AI')
    await input.trigger('keydown', { key: 'Enter' })

    expect(wrapper.emitted('send-message')).toBeFalsy()
  })
})
