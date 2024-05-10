<template>
  <div class="column is-12-mobile is-4-tablet is-4-desktop is-4-fullhd">
    <div class="card">
      <span class="dot" :class="{ 'active': activity }"></span>
      <div class="card-content">
        <div class="columns is-mobile">
          <div class="column is-narrow">
            <div class="card-icon pb-4 pt-5 px-5 has-bg-white has-text-black">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 24 24">
                <path fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M3 8h7a3 3 0 1 0-3-3M4 16h11a3 3 0 1 1-3 3M2 12h17a3 3 0 1 0-3-3" />
              </svg>
            </div>
          </div>
          <div class="column">
            <h6 class="is-size-6 has-text-black has-text-weight-semibold" v-html="card.name.replace('CO2', 'CO<sub>2</sub>') "></h6>
            <h2 class="is-size-3 has-text-black has-text-weight-bold">{{ card.value }} <small class="is-size-5 has-text-weight-normal has-text-grey">{{ card.symbol }}</small></h2>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  props: ['card'],

  data() {
    return {
      activity: true
    }
  },

  watch: {
    'card.value': function () {
      this.activity = true;
      setTimeout(() => { this.activity = false }, 100);
    }
  },

  mounted() {
    setTimeout(() => {
      this.activity = false
    }, 500);
  }
}
</script>