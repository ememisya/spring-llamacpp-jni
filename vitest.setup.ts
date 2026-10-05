import { config } from '@vue/test-utils'
import { describe, it, expect, vi, beforeEach } from 'vitest'
import { createVuetify } from 'vuetify'
import * as components from 'vuetify/components'
import * as directives from 'vuetify/directives'
import router from '@/router'
import i18n from '@/plugins/i18n'

// Create a testing Vuetify instance with all components loaded.
const vuetify = createVuetify({
  components,
  directives,
})

config.global.plugins = [vuetify, i18n, router]

if (typeof window !== 'undefined') {
  const createAudioParam = () => ({
    value: 0,
    setValueAtTime: vi.fn(),
    exponentialRampToValueAtTime: vi.fn(),
    linearRampToValueAtTime: vi.fn(),
    setTargetAtTime: vi.fn(),
    setValueCurveAtTime: vi.fn(),
    cancelScheduledValues: vi.fn(),
  })

  const createAudioNode = () => ({
    connect: vi.fn(),
    disconnect: vi.fn(),
    start: vi.fn(),
    stop: vi.fn(),
    frequency: createAudioParam(),
    gain: createAudioParam(),
  })

  const MockAudioContext = vi.fn().mockImplementation(function () {
    return {
      state: 'suspended',
      currentTime: 0,
      destination: {},
      resume: vi.fn().mockResolvedValue(undefined),
      createOscillator: vi.fn().mockImplementation(createAudioNode),
      createGain: vi.fn().mockImplementation(createAudioNode),
    }
  })

  Object.defineProperty(window, 'AudioContext', {
    writable: true,
    value: MockAudioContext,
  })

  Object.defineProperty(window, 'webkitAudioContext', {
    writable: true,
    value: MockAudioContext,
  })

  if (!window.visualViewport) {
    window.visualViewport = {
      addEventListener: () => {},
      removeEventListener: () => {},
      width: 1024,
      height: 768,
      offsetLeft: 0,
      offsetTop: 0,
      pageLeft: 0,
      pageTop: 0,
      scale: 1,
    } as unknown as VisualViewport
  }
}