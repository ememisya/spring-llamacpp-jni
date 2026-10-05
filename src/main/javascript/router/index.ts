import i18n from '../plugins/i18n' // Your vue-i18n instance
import { createRouter, createWebHistory, RouteRecordRaw } from 'vue-router'
import HomeView from '../views/HomeView.vue'

const titleKey: string = 'appTitle'

const routes: Array<RouteRecordRaw> = [
  {
    path: '/',
    name: 'HomeView',
    component: HomeView,
  },
]

const router = createRouter({
  history: createWebHistory(),
  routes,
})

router.beforeEach(() => {
  if (titleKey) {
    document.title = i18n.global.t(titleKey) as string
  } else {
    document.title = 'Chat Application'
  }
})

export default router
