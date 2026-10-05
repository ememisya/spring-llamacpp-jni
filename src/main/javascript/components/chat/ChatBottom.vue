<template>
  <v-card class="flex-grow-1">
    <v-row>
      <v-col cols="9">
        <slot name="prepend">
          <v-card>Prepend slot</v-card>
        </slot>
      </v-col>
      <v-col cols="2">
        <v-row>
          <v-col>
            <v-card variant="flat" class="align-center append">
              <slot name="append"> </slot>
            </v-card>
          </v-col>
        </v-row>
        <v-row>
          <v-col
            ><v-card variant="flat" class="align-center messages">
              {{ messagesLength }} {{ $t('messagesLengthMessage') }}
            </v-card></v-col
          >
        </v-row>
      </v-col>
    </v-row>
  </v-card>
</template>

<script setup lang="ts">
  import { defineProps } from 'vue'

  defineProps<{
    editing: number[]
    messagesLength: number
  }>()
</script>

<style scoped>
  .sidebar-number-input {
    align-content: normal;
  }

  .messages {
    float: right;
    display: block;
    position: absolute;
    right: 5px;
    bottom: 5px;
  }

  .append {
    float: right;
    display: block;
    position: absolute;
    right: 5px;
    top: 5px;
  }
</style>
