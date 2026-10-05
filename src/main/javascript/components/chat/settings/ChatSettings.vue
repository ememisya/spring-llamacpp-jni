<template>
  <v-sheet class="chat-settings">
    <v-card class="pa-5">
      <v-card-title>{{ $t('systemMessageTitle') }}</v-card-title>
      <v-textarea
        v-model="localSystemMessage"
        class="sidebar-textarea"
        :placeholder="$t('systemMessagePlaceholder')"
        @update:model-value="emitUpdate"
      />
    </v-card>
    <v-card class="pa-5">
      <v-card-title>{{ $t('scratchPadTitle') }}</v-card-title>
      <v-textarea
        v-model="localScratchPad"
        class="sidebar-textarea"
        :placeholder="$t('scratchPadPlaceholder')"
        @update:model-value="emitScratchPad"
      />
    </v-card>

    <v-card class="pa-5">
      <v-card-title>{{ $t('chatSettingsTitle') }}</v-card-title>

      <div>
        {{ $t('dryAllowedLengthTitle') }}
        <v-number-input
          v-model="localDryAllowedLength"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitDryAllowedLength"
        />
      </div>

      <div>
        {{ $t('dryBaseTitle') }}
        <v-number-input
          v-model="localDryBase"
          :precision="null"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitDryBase"
        />
      </div>

      <div>
        {{ $t('dryMultiplierTitle') }}
        <v-number-input
          v-model="localDryMultiplier"
          :precision="null"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitDryMultiplier"
        />
      </div>

      <div>
        {{ $t('dryPenaltyLastNTitle') }}
        <v-number-input
          v-model="localDryPenaltyLastN"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitDryPenaltyLastN"
        />
      </div>

      <div>
        {{ $t('dynamicTemperatureTitle') }}
        <v-number-input
          v-model="localDynamicTemperature"
          :precision="null"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitDynamicTemperature"
        />
      </div>

      <div>
        {{ $t('freqPenaltyTitle') }}
        <v-number-input
          v-model="localFreqPenalty"
          :precision="null"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitFreqPenalty"
        />
      </div>

      <div>
        {{ $t('minPTitle') }}
        <v-number-input
          v-model="localMinP"
          :precision="null"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitMinP"
        />
      </div>

      <div>
        {{ $t('penaltyLastNTitle') }}
        <v-number-input
          v-model="localPenaltyLastN"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitPenaltyLastN"
        />
      </div>

      <div>
        {{ $t('presencePenaltyTitle') }}
        <v-number-input
          v-model="localPresencePenalty"
          :precision="null"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitPresencePenalty"
        />
      </div>

      <div>
        {{ $t('repeatPenaltyTitle') }}
        <v-number-input
          v-model="localRepeatPenalty"
          :precision="null"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitRepeatPenalty"
        />
      </div>

      <div>
        {{ $t('rngSeedTitle') }}
        <v-number-input
          v-model="localRngSeed"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitRngSeed"
        />
      </div>

      <div>
        {{ $t('temperatureTitle') }}
        <v-number-input
          v-model="localTemperature"
          :precision="null"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitTemperature"
        />
      </div>

      <div>
        {{ $t('contextSizeTitle') }}
        <v-number-input
          v-model="localContextSize"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitContextSize"
        />
      </div>
      <!-- Batch size -->
      <div>
        {{ $t('batchSizeTitle') }}
        <v-number-input
          v-model="localBatchSize"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitBatchSize"
        />
      </div>
      <!-- UBatch size -->
      <div>
        {{ $t('uBatchSizeTitle') }}
        <v-number-input
          v-model="localUBatchSize"
          type="number"
          density="compact"
          class="sidebar-number-input"
          @update:model-value="emitUBatchSize"
        />
      </div>
    </v-card>
  </v-sheet>
</template>

<script setup lang="ts">
  import { watch, defineProps, defineEmits } from 'vue'

  const props = defineProps<{
    systemMessage: string
    scratchPad: string
    chatCount: number
    disabled: boolean
    contextSize: number
    batchSize: number
    uBatchSize: number
    dryAllowedLength: number
    dryBase: number
    dryMultiplier: number
    dryPenaltyLastN: number
    dynamicTemperature: number
    freqPenalty: number
    minP: number
    penaltyLastN: number
    presencePenalty: number
    repeatPenalty: number
    rngSeed: number
    temperature: number
  }>()

  const emit = defineEmits<{
    (e: 'update-system-message', value: string): void
    (e: 'update-scratchpad', value: string): void
    (e: 'update-context-size', value: number): void
    (e: 'update-batch-size', value: number): void
    (e: 'update-ubatch-size', value: number): void
    (e: 'update-dry-allowed-length', value: number): void
    (e: 'update-dry-base', value: number): void
    (e: 'update-dry-multiplier', value: number): void
    (e: 'update-dry-penalty-last-n', value: number): void
    (e: 'update-dynamic-temperature', value: number): void
    (e: 'update-freq-penalty', value: number): void
    (e: 'update-min-p', value: number): void
    (e: 'update-penalty-last-n', value: number): void
    (e: 'update-presence-penalty', value: number): void
    (e: 'update-repeat-penalty', value: number): void
    (e: 'update-rng-seed', value: number): void
    (e: 'update-temperature', value: number): void
  }>()

  // Local copy to avoid mutating props
  const localSystemMessage = defineModel<string>('systemMessage')
  // Local state mirrors props
  const localScratchPad = defineModel<string>('scratchPad')
  // Local synced refs
  const localContextSize = defineModel<number>('contextSize')
  const localBatchSize = defineModel<number>('batchSize')
  const localUBatchSize = defineModel<number>('uBatchSize')
  const localDryAllowedLength = defineModel<number>('dryAllowedLength')
  const localDryBase = defineModel<number>('dryBase')
  const localDryMultiplier = defineModel<number>('dryMultiplier')
  const localDryPenaltyLastN = defineModel<number>('dryPenaltyLastN')
  const localDynamicTemperature = defineModel<number>('dynamicTemperature')
  const localFreqPenalty = defineModel<number>('freqPenalty')
  const localMinP = defineModel<number>('minP')
  const localPenaltyLastN = defineModel<number>('penaltyLastN')
  const localPresencePenalty = defineModel<number>('presencePenalty')
  const localRepeatPenalty = defineModel<number>('repeatPenalty')
  const localRngSeed = defineModel<number>('rngSeed')
  const localTemperature = defineModel<number>('temperature')

  // Sync when parent updates
  watch(
    () => props.systemMessage,
    (newVal) => {
      localSystemMessage.value = newVal
    },
  )

  function emitUpdate() {
    emit('update-system-message', localSystemMessage.value)
  }

  // Emitters
  function emitScratchPad() {
    emit('update-scratchpad', localScratchPad.value)
  }

  // Emitters
  function emitContextSize() {
    emit('update-context-size', Number(localContextSize.value))
  }

  function emitBatchSize() {
    emit('update-batch-size', Number(localBatchSize.value))
  }

  function emitUBatchSize() {
    emit('update-ubatch-size', Number(localUBatchSize.value))
  }

  function emitDryAllowedLength() {
    emit('update-dry-allowed-length', Number(localDryAllowedLength.value))
  }

  function emitDryBase() {
    emit('update-dry-base', Number(localDryBase.value))
  }

  function emitDryMultiplier() {
    emit('update-dry-multiplier', Number(localDryMultiplier.value))
  }

  function emitDryPenaltyLastN() {
    emit('update-dry-penalty-last-n', Number(localDryPenaltyLastN.value))
  }

  function emitDynamicTemperature() {
    emit('update-dynamic-temperature', Number(localDynamicTemperature.value))
  }

  function emitFreqPenalty() {
    emit('update-freq-penalty', Number(localFreqPenalty.value))
  }

  function emitMinP() {
    emit('update-min-p', Number(localMinP.value))
  }

  function emitPenaltyLastN() {
    emit('update-penalty-last-n', Number(localPenaltyLastN.value))
  }

  function emitPresencePenalty() {
    emit('update-presence-penalty', Number(localPresencePenalty.value))
  }

  function emitRepeatPenalty() {
    emit('update-repeat-penalty', Number(localRepeatPenalty.value))
  }

  function emitRngSeed() {
    emit('update-rng-seed', Number(localRngSeed.value))
  }

  function emitTemperature() {
    emit('update-temperature', Number(localTemperature.value))
  }
</script>

<style scoped>
  .chat-settings {
    align-content: normal;
  }
</style>
