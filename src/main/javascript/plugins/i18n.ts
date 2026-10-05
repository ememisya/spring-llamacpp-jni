import { createI18n } from 'vue-i18n'

import en from '../locales/en.json'
import tr from '../locales/tr.json'

// Get saved locale or default to 'en'
const savedLocale = localStorage.getItem('user-locale') || 'en'

export default createI18n({
  legacy: false,
  locale: savedLocale,
  fallbackLocale: 'en',
  messages: {
    en,
    tr,
  },
})
