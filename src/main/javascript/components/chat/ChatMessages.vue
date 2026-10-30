<template>
  <v-list id="chat-messages" class="chat-messages">
    <v-list-item
      v-for="(item, index) in messagesModel"
      :key="index"
      :class="item.role"
    >
      <!-- AI avatar on the left -->
      <template v-if="item.role === 'assistant'" #prepend>
        <v-avatar size="48" class="chat-avatar">
          <span class="text-white font-weight-bold text-display-large">{{
            $t('aiNickname')
          }}</span>
        </v-avatar>
      </template>

      <!-- Bubble and Tail Wrapper -->
      <div
        :class="[
          'd-flex align-center w-100',
          item.role === 'user' ? 'justify-end' : 'justify-start',
        ]"
      >
        <!-- Assistant Tail -->
        <ChatTail v-if="item.role === 'assistant'" role="assistant" />

        <!-- Chat bubble -->
        <v-card class="chat-bubble custom-bubble" elevation="2" rounded="lg">
          <v-card-text class="text">
            <div
              v-if="item.content?.trim().length > 0"
              ref="textFields"
              class="text-field"
              :contenteditable="isEditing(item.id)"
              @paste="handlePaste"
              @click="startEditing(item.id)"
              v-text="item.content"
            ></div>
            <div
              v-else
              ref="textFields"
              contenteditable="true"
              class="text-field empty-text-field"
              @click="startEditing(item.id)"
            ></div>
            <hr v-if="isEditing(item.id)" />
            <GoEmotionClassifier
              v-if="item.emotion && isEditing(item.id)"
              :emotion="item.emotion"
            />
            <v-row v-if="isEditing(item.id)">
              <v-col class="text-right mt-2">
                <v-btn
                  class="btn-chat float-left"
                  variant="plain"
                  size="small"
                  @click="deleteMessage(item.id)"
                >
                  <v-icon>mdi-delete</v-icon> {{ $t('deleteButtonTitle') }}
                </v-btn>
                <v-spacer />
                <v-spacer />
                <v-btn
                  variant="plain"
                  class="btn-chat float-right"
                  size="small"
                  @click="
                    updateMessageContent(
                      item.id,
                      $refs.textFields[index].innerText,
                    )
                  "
                >
                  <v-icon>mdi-check-circle-outline</v-icon>
                  {{ $t('doneButtonTitle') }}
                </v-btn>
              </v-col>
            </v-row>
          </v-card-text>
        </v-card>

        <!-- User Tail -->
        <ChatTail v-if="item.role === 'user'" role="user" />
      </div>

      <!-- User avatar on the right -->
      <template v-if="item.role === 'user'" #append>
        <v-avatar size="48" class="chat-avatar">
          <span class="text-white font-weight-bold text-display-large">{{
            $t('userNickname')
          }}</span>
        </v-avatar>
      </template>
    </v-list-item>

    <v-list-item v-if="typing" key="-1" class="assistant">
      <template #prepend>
        <v-avatar size="48" class="chat-avatar">
          <span class="text-white font-weight-bold text-display-large">{{
            $t('aiNickname')
          }}</span>
        </v-avatar>
      </template>
      <div class="d-flex align-center justify-start w-100">
        <ChatTail role="assistant" />
        <v-card
          class="assistant chat-bubble custom-bubble"
          elevation="2"
          rounded="lg"
        >
          <v-card-text class="text">
            <span class="animate-pulse font-italic"
              >{{ $t('aiIsTypingMessage')
              }}<span class="typing-dots"
                ><span>.</span><span>.</span><span>.</span></span
              ></span
            >
          </v-card-text>
        </v-card>
      </div>
    </v-list-item>
  </v-list>
</template>

<script setup lang="ts">
  import { h } from 'vue'
  import { ChatMessage } from '@/types'

  import GoEmotionClassifier from '@/components/chat/GoEmotionClassifier.vue'

  defineProps<{
    typing: boolean
    messages: ChatMessage[]
    editing: number[]
  }>()

  const messagesModel = defineModel<ChatMessage[]>('messages')
  const editingModel = defineModel<number[]>('editing')

  const isEditing = (id) => editingModel?.value?.includes(id)

  const ChatTail = (props: { role: 'user' | 'assistant' }) => {
    const isUser = props.role === 'user'
    return h(
      'svg',
      {
        class: ['chat-tail', isUser ? 'user-tail' : 'assistant-tail'],
        viewBox: '0 0 14 18',
        width: '14',
        height: '18',
      },
      [
        h('path', {
          d: isUser ? 'M0 0V18L14 9L0 0Z' : 'M14 0V18L0 9L14 0Z',
          fill: 'var(--bubble-color)',
        }),
      ],
    )
  }

  const emit = defineEmits<{
    (e: 'edit-message', id: number): void
    (e: 'delete-message', id: number): void
    (
      e: 'update-message-content',
      payload: { id: number; content: string },
    ): void
  }>()

  function handlePaste(event: ClipboardEvent) {
    event.preventDefault()

    // Extract only plain text from the clipboard
    const text = event.clipboardData?.getData('text/plain') || ''

    // Insert the clean text at the current cursor position preserving undo history
    document.execCommand('insertText', false, text)
  }

  function startEditing(id: number) {
    emit('edit-message', id)
  }

  function deleteMessage(id: number) {
    emit('delete-message', id)
  }

  function updateMessageContent(id: number, content: string) {
    emit('update-message-content', { id, content })
  }
</script>

<style scoped lang="scss">
  .v-theme--light {
    --app-user-bubble: #{$user-bubble-light};
    --app-assistant-bubble: #{$assistant-bubble-light};
    --app-user-avatar: #{$user-avatar-light};
    --app-assistant-avatar: #{$assistant-avatar-light};
  }

  .v-theme--dark {
    --app-user-bubble: #{$user-bubble-dark};
    --app-assistant-bubble: #{$assistant-bubble-dark};
    --app-user-avatar: #{$user-avatar-dark};
    --app-assistant-avatar: #{$assistant-avatar-dark};
  }

  .v-theme--dark.v-list {
    background-color: transparent;
  }

  .chat-messages {
    width: 100%;
    overflow-y: auto;
    padding: 16px;
    display: flex;
    height: 50%;
    flex-direction: column;
    scroll-margin-bottom: 80px;
  }

  .v-list-item {
    display: flex !important;
    align-items: center;
    margin-bottom: 8px;

    &.user {
      justify-content: flex-end;
      --bubble-color: var(--app-assistant-bubble);
      --avatar-color: var(--app-user-avatar);
    }

    &.assistant {
      justify-content: flex-start;
      --bubble-color: var(--app-user-bubble);
      --avatar-color: var(--app-assistant-avatar);
    }
  }

  .chat-bubble {
    max-width: 60%;
    min-width: 210px;
    background-color: var(--bubble-color) !important;
  }

  .chat-avatar {
    background-color: var(--avatar-color) !important;
  }

  .chat-tail {
    flex-shrink: 0;
    z-index: 2;
  }

  .text-field {
    padding: 5px;
  }

  .empty-text-field {
    min-width: 150px;
  }

  .animate-pulse {
    animation: pulse-effect 2s cubic-bezier(0.4, 0, 0.6, 1) infinite;
  }

  @keyframes pulse-effect {
    0%,
    100% {
      opacity: 1;
    }
    50% {
      opacity: 0.4;
    }
  }

  .typing-dots span {
    animation: blink 1.4s infinite both;
  }

  .typing-dots span:nth-child(2) {
    animation-delay: 0.2s;
  }

  .typing-dots span:nth-child(3) {
    animation-delay: 0.4s;
  }

  @keyframes blink {
    0% {
      opacity: 0.2;
    }
    20% {
      opacity: 1;
    }
    100% {
      opacity: 0.2;
    }
  }
</style>
