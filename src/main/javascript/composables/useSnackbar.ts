// snackbarService.ts
import { reactive } from 'vue'

export type SnackbarColor = 'success' | 'info' | 'warning' | 'error'

export interface SnackbarState {
  show: boolean
  message: string
  color: SnackbarColor
  timeout: number
}

export const snackbarState = reactive<SnackbarState>({
  show: false,
  message: '',
  color: 'info',
  timeout: 3000,
})

/**
 * Show a Vuetify snackbar
 * @param message - The text to display
 * @param color - Snackbar color (success, info, warning, error)
 * @param timeout - Auto-hide delay in ms
 */
export function showSnackbar(
  message: string,
  color: SnackbarColor = 'info',
  timeout = 3000,
): void {
  snackbarState.message = message
  snackbarState.color = color
  snackbarState.timeout = timeout
  snackbarState.show = true
}
