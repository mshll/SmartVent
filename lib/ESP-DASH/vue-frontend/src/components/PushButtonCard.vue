<template>
  <div class="column is-12-mobile is-4-tablet is-4-desktop is-3-fullhd">
    <div class="card">
      <span class="dot" :class="{ 'active': activity }"></span>
      <div class="card-content">
        <div class="columns is-mobile is-vcentered">
          <div class="column has-text-left">
            <h6 class="is-size-6 has-text-muted has-text-weight-medium">{{ card.name }}</h6>
          </div>
          <div class="column is-narrow">
            <div class="card-icon py-4 px-5 button is-large is-responsive is-link has-text-light" style="cursor: pointer;" @click="sendClickEvent" key="active">
              <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"><path fill="currentColor" d="M12 22q-2.075 0-3.9-.788t-3.175-2.137q-1.35-1.35-2.137-3.175T2 12q0-2.075.788-3.9t2.137-3.175q1.35-1.35 3.175-2.137T12 2q2.075 0 3.9.788t3.175 2.137q1.35 1.35 2.138 3.175T22 12q0 2.075-.788 3.9t-2.137 3.175q-1.35 1.35-3.175 2.138T12 22m0-2q3.35 0 5.675-2.325T20 12q0-3.35-2.325-5.675T12 4Q8.65 4 6.325 6.325T4 12q0 3.35 2.325 5.675T12 20m0-8"/></svg>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
// @ts-nocheck
import EventBus from '@/event-bus.js';

export default {
  props: ['card'],

  data() {
    return {
      activity: true
    }
  },

  methods: {
    sendClickEvent() {
      EventBus.$emit('pushButtonClicked', { id: this.card.id, value: !this.card.value });
      this.activity = true;
      setTimeout(() => { this.activity = false }, 100);
    }
  },

  mounted() {
    setTimeout(() => { this.activity = false }, 500);
  }
}
</script>