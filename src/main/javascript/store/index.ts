import { ChatState } from '@/types'
import { InjectionKey } from 'vue'
import { createStore, Store } from 'vuex'
import { ChatStore } from './chatStore'

export const key: InjectionKey<Store<ChatState>> = Symbol()

export const store = createStore<storeTypes>(ChatStore)
