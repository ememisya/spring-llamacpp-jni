import { Store } from 'vuex'
import type { ChatState } from '../types' // Adjust path to your state interface

export const localStoragePlugin = (store: Store<ChatState>) => {
  const STORAGE_KEY = 'spring-llamacpp-jni'

  const savedState = localStorage.getItem(STORAGE_KEY)
  if (savedState) {
    try {
      store.replaceState(Object.assign({}, store.state, JSON.parse(savedState)))
    } catch (e) {
      console.debug('Could not load state from localStorage', e)
    }
  }

  store.subscribe((mutation, state) => {
    const { requestPending, ...stateToSave } = state
    localStorage.setItem(STORAGE_KEY, JSON.stringify(stateToSave))
  })
}
