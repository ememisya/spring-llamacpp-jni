import { mount } from '@vue/test-utils'
import { describe, it, expect } from 'vitest'
import { VLayout } from 'vuetify/components' // Import VBtn for querying
import { h } from 'vue'
import SideDrawer from '@/components/layout/SideDrawer.vue'

describe('SideDrawer.vue', () => {
  it('renders provided slot content', () => {
    const wrapper = mount(VLayout, {
      slots: {
        default: () =>
          h(
            SideDrawer,
            {}, // 1st Arg: Component, 2nd Arg: Props Object (Empty if none)
            {
              content: () => 'Menu Items', // 3rd Arg: Slots Object
            },
          ),
      },
    })

    expect(wrapper.text()).toContain('Menu Items')
  })
})
