import { mount } from '@vue/test-utils'
import { nextTick, reactive } from 'vue'
import { describe, it, expect, vi, beforeEach, afterEach } from 'vitest'
import GlobalSnackbar from '@/components/GlobalSnackbar.vue'

vi.mock('@/composables/useSnackbar', () => {
  return {
    snackbarState: reactive({
      show: true,
      message: 'Settings saved successfully',
      color: 'success',
      timeout: 3000,
    }),
  }
})

describe('GlobalSnackbar.vue', () => {
  let mainTarget: HTMLDivElement

  beforeEach(() => {
    // 1. Create a physical element in the DOM for the overlay container to mount onto
    mainTarget = document.createElement('div')
    mainTarget.setAttribute('class', 'v-overlay-container')
    document.body.appendChild(mainTarget)
  })

  afterEach(() => {
    // Clean up the DOM after the test run
    document.body.innerHTML = ''
  })

  it('displays the message from the composable state', async () => {
    const wrapper = mount(GlobalSnackbar, {
      attachTo: document.body, // 2. Force full DOM mounting so teleport works natively
      global: {
        stubs: {
          // 3. FORCE disable stubbing for teleport and overlays
          teleport: false,
          VSnackbar: false,
          VOverlay: false,
        },
      },
    })

    await nextTick()

    // 4. Since it teleports out of the wrapper wrapper into document.body,
    // query document.body directly to look for the message!
    expect(document.body.innerHTML).toContain('Settings saved successfully')

    // Clean up attachment
    wrapper.unmount()
  })
})
