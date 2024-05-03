<template>
  <div class="column is-12-mobile is-4-tablet is-4-desktop is-3-fullhd">
    <div class="card">
      <span class="dot" :class="{ 'active': activity }"></span>
      <div class="card-content">
        <div class="columns is-mobile is-vcentered">
          <div class="column has-text-left">
            <h6 class="is-size-6 has-text-black has-text-weight-semibold">{{ card.name }}</h6>
          </div>
          <div class="column is-narrow">
            <div class="py-3">
              <input class="is-checkradio is-large is-black" id="button-radio" type="radio" name="button-radio" :checked="pushButtonClicked" @click="sendClickEvent">
              <label for="button-radio" class="mx-0"></label>
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
      activity: true,
      pushButtonClicked: false
    }
  },

  methods: {
    sendClickEvent() {
      this.pushButtonClicked = true;
      EventBus.$emit('pushButtonClicked', { id: this.card.id, value: !this.card.value });
      this.activity = true;
      setTimeout(() => {
        this.activity = false;
      }, 100);
      setTimeout(() => {
        this.pushButtonClicked = false;
      }, 200);
    }
  },

  mounted() {
    setTimeout(() => { this.activity = false }, 500);
  }
}
</script>