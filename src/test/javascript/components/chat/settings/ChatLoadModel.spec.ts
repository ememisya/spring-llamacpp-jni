import { mount } from '@vue/test-utils'
import { describe, it, expect } from 'vitest'
import ChatLoadModel from '@/components/chat/settings/ChatLoadModel.vue'

describe('ChatLoadModel.vue', () => {
  const defaultProps = {
    modelsList: ['Model One', 'Another'],
    disabled: false,
  }

  it('emits model-selected with index when button is clicked', async () => {
    const wrapper = mount(ChatLoadModel, {
      props: defaultProps,
      global: {
        mocks: {
          $t: (msg: string) => msg,
        },
      },
    })

    await wrapper.find('button').trigger('click')

    expect(wrapper.emitted('model-selected')).toBeTruthy()
    expect(wrapper.emitted('model-selected')?.[0]).toEqual([0])
  })
})
