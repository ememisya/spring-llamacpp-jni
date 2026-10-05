import { mount } from '@vue/test-utils'
import { describe, it, expect } from 'vitest'
import { VLayout } from 'vuetify/components' // Import VBtn for querying
import { h } from 'vue'
import BottomBar from '@/components/layout/BottomBar.vue'

describe('BottomBar.vue', () => {
  it('renders default slot content', () => {
    const wrapper = mount(VLayout, {
      slots: {
        default: () =>
          h(
            BottomBar,
            {}, // 1st Arg: Component, 2nd Arg: Props Object (Empty if none)
            {
              default: () => 'Custom Bottom Bar Data', // 3rd Arg: Slots Object
            },
          ),
      },
    })

    expect(wrapper.text()).toContain('Custom Bottom Bar Data')
  })
})
