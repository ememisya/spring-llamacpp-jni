<template>
  <v-card class="flex-grow-1">
    <v-row>
      <v-col cols="9">
        <slot name="prepend">
          <v-card>Prepend slot</v-card>
        </slot>
      </v-col>
      <v-col cols="2">
        <v-card variant="flat" class="align-center append">
          <v-tooltip :text="messagesLength + ' ' + $t('messagesLengthMessage')">
            <template #activator="{ props }">
              <slot name="append"> </slot>
              <v-progress-linear
                v-bind="props"
                :model-value="currentTokensModel"
                :max="maxTokensModel"
                :indeterminate="!currentTokensModel || currentTokensModel <= 0"
                height="28"
                class="context-progress"
                color="primary"
              >
                <!-- The default slot maps text over the loading bar -->
                <template #default="{ value }">
                  {{ $t('contextProgressTitle') }} {{ Math.ceil(value) }}%
                </template>
              </v-progress-linear>
            </template>
          </v-tooltip>
        </v-card>
      </v-col>
    </v-row>
  </v-card>
</template>

<script setup lang="ts">
  defineProps<{
    editing: number[]
    messagesLength: number
    maxTokens: number
    currentTokens: number
  }>()

  const currentTokensModel = defineModel<number>('currentTokens')
  const maxTokensModel = defineModel<number>('maxTokens')
</script>

<style scoped>
  .context-progress {
    min-width: 90px;
  }

  .append {
    float: right;
    display: block;
    position: absolute;
    right: 5px;
    top: 5px;
  }
</style>
