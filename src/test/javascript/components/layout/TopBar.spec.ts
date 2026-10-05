import { mount } from '@vue/test-utils'
import { describe, it, expect } from 'vitest'
import { VLayout, VBtn } from 'vuetify/components' // Import VBtn for querying
import TopBar from '@/components/layout/TopBar.vue'
import { nextTick } from 'vue'
import { h } from 'vue'

describe('TopBar.vue', () => {
  it('emits clear-chat when delete button is clicked', async () => {
    const wrapper = mount(VLayout, {
      slots: {
        default: () =>
          h(TopBar, {
            chatParamsLogIdentifierMap: new Map(), // Props go directly inside h() for a slot
          }),
      },
    })

    // Find the TopBar component instance first
    const topBar = wrapper.findComponent(TopBar)
    const buttons = topBar.findAllComponents(VBtn)

    const clearBtn = buttons.find((btn) => btn.props('icon') === 'mdi-delete')

    if (!clearBtn) throw new Error('Could not find Clear button')
    // Trigger the click event
    await clearBtn.trigger('click')
    // Wait for Vue and Vuetify to settle their internal event pipelines
    await nextTick()

    expect(topBar.emitted('clear-chat')).toBeTruthy()
  })

  it('emits save-chat when save button is clicked', async () => {
    const wrapper = mount(VLayout, {
      slots: {
        default: () =>
          h(TopBar, {
            chatParamsLogIdentifierMap: new Map(), // Props go directly inside h() for a slot
          }),
      },
    })

    // Find the TopBar component instance first
    const topBar = wrapper.findComponent(TopBar)
    const buttons = topBar.findAllComponents(VBtn)

    const saveBtn = buttons.find(
      (btn) => btn.props('icon') === 'mdi-content-save',
    )

    if (!saveBtn) throw new Error('Could not find Clear button')
    // Trigger the click event
    await saveBtn.trigger('click')
    // Wait for Vue and Vuetify to settle their internal event pipelines
    await nextTick()

    expect(topBar.emitted('post-chat-params-log')).toBeTruthy()
  })
})
