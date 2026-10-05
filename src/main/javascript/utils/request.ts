import axios, { type AxiosInstance, type AxiosResponse } from 'axios'
import { showSnackbar } from '@/composables/useSnackbar'

const instance: AxiosInstance = axios.create({
  baseURL: 'http://localhost:8081/v1/',
  timeout: 3000000,
})

instance.interceptors.request.use(
  (config) => {
    return config
  },
  (error) => {
    console.error('Request error:', error)
    return Promise.reject(error)
  },
)

instance.interceptors.response.use(
  (response: AxiosResponse) => {
    return response.data
  },
  (error) => {
    console.error('Response error:', error)

    if (error.response) {
      const status = error.response.status
      switch (status) {
        case 401:
          showSnackbar('You need to authenticate!', 'error', 2500)

          break
        case 403:
          showSnackbar('Absolutely haram!', 'error', 2500)
          break
        case 404:
          showSnackbar('Page not found!', 'error', 2500)
          break
        case 500:
          showSnackbar('Internal error!', 'error', 2500)
          break
        default:
          showSnackbar(`Unknown error: ${error.message}`, 'error', 2500)
      }
    } else {
      showSnackbar('Did not receive response!', 'error', 2500)
    }

    return Promise.reject(error)
  },
)

export default instance
