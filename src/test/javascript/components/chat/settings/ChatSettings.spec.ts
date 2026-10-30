import { mount } from '@vue/test-utils'
import { describe, it, expect } from 'vitest'
import ChatSettings from '@/components/chat/settings/ChatSettings.vue'

describe('ChatSettings.vue', () => {
  const defaultProps = {
    systemMessage: 'Initial System Message',
    scratchPad: '',
    chatCount: 0,
    disabled: false,
    contextSize: 2048,
    batchSize: 512,
    uBatchSize: 512,
    maxTokens: 512,
    softLimit: 0,
    softLimitPenalty: 1.0,
    dryAllowedLength: 2,
    dryBase: 1.75,
    dryMultiplier: 0.0,
    dryPenaltyLastN: 0,
    dynamicTemperature: 0,
    freqPenalty: 0,
    minP: 0.05,
    penaltyLastN: 64,
    presencePenalty: 0,
    repeatPenalty: 1.1,
    rngSeed: -1,
    temperature: 0.8,
  }

  it('emits update-system-message when textarea value changes', async () => {
    const wrapper = mount(ChatSettings, {
      props: defaultProps,
    })

    const textareas = wrapper.findAll('textarea')
    await textareas[0].setValue('Updated System Message')

    expect(wrapper.emitted('update-system-message')).toBeTruthy()
    expect(wrapper.emitted('update-system-message')?.[0]).toEqual([
      'Updated System Message',
    ])
  })

  it('emits update events for maxTokens, softLimit, and softLimitPenalty', async () => {
    const wrapper = mount(ChatSettings, {
      props: defaultProps,
    })

    await wrapper.vm.$emit('update-max-tokens', 1024)
    await wrapper.vm.$emit('update-soft-limit', 500)
    await wrapper.vm.$emit('update-soft-limit-penalty', 5.0)

    expect(wrapper.emitted('update-max-tokens')?.[0]).toEqual([1024])
    expect(wrapper.emitted('update-soft-limit')?.[0]).toEqual([500])
    expect(wrapper.emitted('update-soft-limit-penalty')?.[0]).toEqual([5.0])
  })
})
