import { nextTick } from 'vue'

export function scrollToBottom(cssClass: string, smooth: boolean = true) {
  nextTick(() => {
    const targetElement = document.getElementById(cssClass)
    if (targetElement) {
      targetElement.scrollIntoView({
        block: 'end',
        behavior: smooth ? 'smooth' : 'auto',
      })
    } else {
      window.scrollTo({
        top: document.body.scrollHeight,
        behavior: smooth ? 'smooth' : 'auto',
      })
    }
  })
}
