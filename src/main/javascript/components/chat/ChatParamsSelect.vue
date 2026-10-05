<template>
  <v-container fluid>
    <v-row class="mb-4 align-center">
      <v-col cols="12" sm="6" class="d-flex justify-center align-center">
        <v-select
          v-model="selectedMonth"
          :items="monthsList"
          item-title="text"
          item-value="value"
          variant="plain"
          hide-details
          density="compact"
          class="text-h5 font-weight-bold mx-2 selected-month"
        ></v-select>

        <v-select
          v-model="selectedYear"
          :items="yearsList"
          variant="plain"
          hide-details
          density="compact"
          class="text-h5 font-weight-bold mx-2 selected-year"
        ></v-select>
      </v-col>

      <v-col cols="12" sm="3" class="d-flex justify-end">
        <v-select
          v-model="calendarView"
          :items="[
            { title: t('monthViewTitle'), value: 'month' },
            { title: t('dayViewTitle'), value: 'day' },
          ]"
          label="View"
          variant="outlined"
          density="compact"
          hide-details
          style="min-width: 100px"
        ></v-select>
      </v-col>
      <v-col cols="12" sm="3" class="d-flex align-center">
        <v-btn
          icon="mdi-chevron-left"
          variant="text"
          @click="calendarRef?.prev()"
        ></v-btn>
        <v-btn
          v-tooltip="t('todayButtonTooltip')"
          icon="mdi-calendar"
          size="small"
          @click="setToday"
        ></v-btn>
        <v-btn
          icon="mdi-chevron-right"
          variant="text"
          @click="calendarRef?.next()"
        ></v-btn>
      </v-col>
    </v-row>

    <v-sheet>
      <v-calendar
        ref="calendarRef"
        v-model="focusDate"
        :locale="locale"
        :type="calendarView"
        :events="formattedEvents"
        color="primary"
        @click:date="switchToDayView"
        @click:more="switchToDayView"
      >
        <template #event="{ event }">
          <div
            class="v-event-drag-wrapper px-2 text-white cursor-pointer fill-height d-flex align-center"
            style="width: 100%; font-size: 14px"
            @click.stop="showEventDetails(event)"
          >
            <strong>{{ event.title }}</strong>
          </div>
        </template>
      </v-calendar>
    </v-sheet>

    <v-dialog v-model="detailsDialog" max-width="400px">
      <v-card v-if="selectedEvent">
        <v-card-title
          class="bg-primary text-white d-flex justify-space-between align-center"
        >
          <span>{{ selectedEvent.title }}</span>
          <v-icon>mdi-calendar-clock</v-icon>
        </v-card-title>
        <v-card-text class="pt-4">
          <p class="mb-2">
            <strong>{{ $t('databaseIDTitle') }}</strong> {{ selectedEvent.id }}
          </p>
          <p class="mb-2">
            <strong>{{ $t('loggedTimeTitle') }}</strong>
            {{ formatReadableTime(selectedEvent.start) }}
          </p>
        </v-card-text>
        <v-card-actions>
          <v-btn color="success" @click="emitGetChatParamsLog(selectedEvent)">
            {{ $t('loadStateButtonTitle') }}
          </v-btn>
          <v-btn color="error" @click="emitDeleteChatParamsLog(selectedEvent)">
            {{ $t('deleteStateButtonTitle') }}
          </v-btn>
          <v-spacer></v-spacer>
          <v-btn
            color="primary"
            variant="text"
            @click="detailsDialog = false"
            >{{ $t('closeButtonTitle') }}</v-btn
          >
        </v-card-actions>
      </v-card>
    </v-dialog>
  </v-container>
</template>

<script setup lang="ts">
  import { ref, computed, watch, onMounted } from 'vue'
  import { VCalendar } from 'vuetify/components/VCalendar'
  import type { CalendarEvent } from 'vuetify/labs/VCalendar'
  import { MonthItem, ChatParamsLogIdentifierMap } from '@/types'
  import { useI18n } from 'vue-i18n'
  const { locale } = useI18n()
  const { t } = useI18n()

  //Props
  defineProps<{
    chatParamsLogIdentifierMap: ChatParamsLogIdentifierMap
  }>()

  // Emits
  const emit = defineEmits<{
    (e: 'get-chat-params-log', calendarEvent: CalendarEvent): void
    (e: 'delete-chat-params-log', calendarEvent: CalendarEvent): void
    (e: 'get-chat-params-logs', year: number): void
  }>()

  const chatParamsLogIdentifierMapModel =
    defineModel<ChatParamsLogIdentifierMap>('chatParamsLogIdentifierMap')

  const calendarRef = ref(null)
  const calendarView = ref('month')
  const focusDate = ref(new Date())
  const detailsDialog = ref(false)
  const selectedEvent = ref(null)

  onMounted(() => {
    emit('get-chat-params-logs', new Date(focusDate.value).getFullYear())
  })

  watch(
    () => focusDate.value?.getFullYear(),
    (newYear) => {
      if (newYear) {
        emit('get-chat-params-logs', newYear)
      }
    },
  )

  const monthsList: MonthItem[] = [
    { text: t('month0Name'), value: 0 },
    { text: t('month1Name'), value: 1 },
    { text: t('month2Name'), value: 2 },
    { text: t('month3Name'), value: 3 },
    { text: t('month4Name'), value: 4 },
    { text: t('month5Name'), value: 5 },
    { text: t('month6Name'), value: 6 },
    { text: t('month7Name'), value: 7 },
    { text: t('month8Name'), value: 8 },
    { text: t('month9Name'), value: 9 },
    { text: t('month10Name'), value: 10 },
    { text: t('month11Name'), value: 11 },
  ]

  const yearsList = computed<number[]>(() => {
    const currentYear = new Date().getFullYear()
    const startYear = currentYear - 10
    const endYear = currentYear + 1
    const years = []
    for (let y = startYear; y <= endYear; y++) {
      years.push(y)
    }
    return years
  })

  const selectedMonth = computed({
    get: () =>
      focusDate.value
        ? new Date(focusDate.value).getMonth()
        : new Date().getMonth(),
    set: (val) => {
      const d = new Date(focusDate.value || new Date())
      d.setMonth(val)
      focusDate.value = d
    },
  })

  const selectedYear = computed({
    get: () =>
      focusDate.value
        ? new Date(focusDate.value).getFullYear()
        : new Date().getFullYear(),
    set: (val) => {
      const d = new Date(focusDate.value || new Date())
      d.setFullYear(val)
      focusDate.value = d
    },
  })

  const formattedEvents = computed<CalendarEvent[]>(() => {
    // 1. Guard against an empty, null, or undefined Map
    if (
      !chatParamsLogIdentifierMapModel.value ||
      chatParamsLogIdentifierMapModel.value.size === 0
    )
      return []

    // 2. Extract all the arrays from the map values, flatten them, and map to CalendarEvents
    return Array.from(chatParamsLogIdentifierMapModel.value.values())
      .flatMap((logArray) => logArray) // Flattens the nested arrays into one single list
      .map((item) => {
        const cleanTimestampString = item.timestamp.split('.')[0]

        return {
          title: `${t('savedStateTitle')} (${item.id})`,
          start: cleanTimestampString,
          end: cleanTimestampString,
          allDay: false,
          id: item.id,
          rawTimestamp: item.timestamp,
        }
      })
  })

  const switchToDayView = (event, eventPayload) => {
    const targetDateStr =
      eventPayload?.date ||
      eventPayload?.id ||
      eventPayload?.value ||
      eventPayload
    if (!targetDateStr) return

    const parts = String(targetDateStr).split(' ')[0].split('-')
    if (parts.length === 3) {
      const localDate = new Date(parts[0], parts[1] - 1, parts[2])
      focusDate.value = localDate
      calendarView.value = 'day'
    }
  }

  const showEventDetails = (eventObj) => {
    if (eventObj) {
      selectedEvent.value = eventObj
      detailsDialog.value = true
    }
  }

  const formatReadableTime = (dateStr) => {
    if (!dateStr) return ''
    return new Date(dateStr).toLocaleString(locale.value, {
      dateStyle: 'medium',
      timeStyle: 'short',
    })
  }

  const setToday = () => {
    focusDate.value = new Date()
  }

  function emitGetChatParamsLog(selectedEvent: CalendarEvent) {
    detailsDialog.value = false
    emit('get-chat-params-log', selectedEvent)
  }

  function emitDeleteChatParamsLog(selectedEvent: CalendarEvent) {
    detailsDialog.value = false
    emit('delete-chat-params-log', selectedEvent)
  }
</script>

<style scoped>
  .selected-month {
    min-width: 150px;
    text-align: right;
  }

  .selected-year {
    min-width: 80px;
  }
</style>
