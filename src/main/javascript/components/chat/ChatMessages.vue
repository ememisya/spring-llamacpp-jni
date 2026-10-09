<template>
  <v-list id="chat-messages" class="chat-messages">
    <v-list-item
      v-for="(item, index) in messagesModel"
      :key="index"
      :class="item.role"
    >
      <!-- AI avatar on the left -->
      <template v-if="item.role === 'assistant'" #prepend>
        <v-avatar size="48" color="deep-purple-lighten-2">
          <span class="text-white font-weight-bold text-display-large">{{
            $t('aiNickname')
          }}</span>
        </v-avatar>
      </template>

      <!-- Chat bubble -->
      <v-card
        class="chat-bubble"
        :class="item.role"
        :color="item.role === 'assistant' ? 'grey-lighten-3' : 'blue-lighten-4'"
        elevation="2"
        rounded="lg"
      >
        <v-card-text class="text">
          <div
            class="text-field"
            :contenteditable="isEditing(item.id)"
            @click="startEditing(item.id)"
            @blur="item.content = ($event.target as HTMLElement).innerText"
            v-text="item.content"
          ></div>
          <hr v-if="isEditing(item.id)" />
          <v-row>
            <v-col class="text-right mt-2">
              <v-btn
                v-if="isEditing(item.id)"
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
                v-if="isEditing(item.id)"
                variant="plain"
                class="btn-chat float-right"
                size="small"
                @click="updateMessageContent(item.id, item.content)"
              >
                <v-icon>mdi-check-circle-outline</v-icon>
                {{ $t('doneButtonTitle') }}
              </v-btn>
            </v-col>
          </v-row>
        </v-card-text>
      </v-card>

      <!-- User avatar on the right -->
      <template v-if="item.role === 'user'" #append>
        <v-avatar size="48" color="blue-lighten-2">
          <span class="text-white font-weight-bold text-display-large">{{
            $t('userNickname')
          }}</span>
        </v-avatar>
      </template>
    </v-list-item>

    <v-list-item v-if="typing" key="-1" class="assistant">
      <template #prepend>
        <v-avatar size="48" color="deep-purple-lighten-2">
          <span class="text-white font-weight-bold text-display-large">{{
            $t('aiNickname')
          }}</span>
        </v-avatar>
      </template>
      <v-card
        class="assistant chat-bubble"
        color="grey-lighten-3"
        elevation="2"
        rounded="lg"
      >
        <v-card-text class="text">
          <v-progress-circular
            indeterminate
            size="16"
            width="2"
            color="primary"
            class="mr-2"
          ></v-progress-circular>

          <span class="animate-pulse">{{ $t('aiIsTypingMessage') }}</span>
        </v-card-text>
      </v-card>
    </v-list-item>
  </v-list>
</template>

<script setup lang="ts">
  import { defineProps, defineEmits } from 'vue'
  import { ChatMessage } from '@/types'

  defineProps<{
    typing: boolean
    messages: ChatMessage[]
    editing: number[]
  }>()

  const messagesModel = defineModel<ChatMessage[]>('messages')
  const editingModel = defineModel<number[]>('editing')

  const isEditing = (id) => editingModel?.value?.includes(id)

  const emit = defineEmits<{
    (e: 'edit-message', id: number): void
    (e: 'delete-message', id: number): void
    (
      e: 'update-message-content',
      payload: { id: number; content: string },
    ): void
  }>()

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

<style scoped>
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
    align-items: flex-start;
    margin-bottom: 8px;
  }

  .assistant {
    justify-content: flex-start;
  }

  .user {
    justify-content: flex-end;
  }

  .chat-bubble {
    max-width: 60%;
    min-width: 210px;
  }

  .chat-bubble.assistant {
    float: left;
  }

  .chat-bubble.user {
    float: right;
  }

  .text-field {
    padding: 5px;
  }

  /* Simple CSS helper to give the text itself a gentle flashing breath effect */
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
</style>
