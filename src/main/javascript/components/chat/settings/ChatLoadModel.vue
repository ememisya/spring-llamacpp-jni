<template>
  <v-card class="w-100 pa-4" variant="flat">
    <v-card-title class="px-0 pt-0 pb-3">
      {{ $t('loadModelTitle') }}:
    </v-card-title>
    <div class="d-flex align-center w-100">
      <v-select
        v-model="selectedIndex"
        :items="selectItems"
        item-title="title"
        item-value="value"
        :disabled="disabled"
        hide-details
        class="flex-grow-1 mr-2"
      ></v-select>
      <v-btn color="primary" :disabled="disabled" @click="handleSelect">
        {{ $t('loadButtonTitle') }}
      </v-btn>
    </div>
  </v-card>
</template>

<script setup lang="ts">
  import { ref, computed } from 'vue'

  const props = withDefaults(
    defineProps<{
      modelsList: string[]
      disabled?: boolean
    }>(),
    {
      disabled: false,
    },
  )

  const emit = defineEmits<{
    (e: 'model-selected', value: number): void
  }>()

  const selectedIndex = ref<number>(0)

  const selectItems = computed(() =>
    props.modelsList.map((label, index) => ({
      title: label,
      value: index,
    })),
  )

  const handleSelect = (): void => {
    emit('model-selected', selectedIndex.value)
  }
</script>
