<template>
  <div class="column is-12-mobile is-4-tablet is-4-desktop is-3-fullhd">
    <div class="card">
      <span class="dot" :class="{'active': activity}"></span>
      <div class="card-content">
        <div class="columns is-mobile">
          <div class="column is-narrow">
            <div class="card-icon pb-4 pt-5 px-5 has-bg-white has-text-black">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 256 256">
                <path fill="currentColor" d="M174 47.75a254.19 254.19 0 0 0-41.45-38.3a8 8 0 0 0-9.18 0A254.19 254.19 0 0 0 82 47.75C54.51 79.32 40 112.6 40 144a88 88 0 0 0 176 0c0-31.4-14.51-64.68-42-96.25m9.85 105.59a57.6 57.6 0 0 1-46.56 46.55a8.75 8.75 0 0 1-1.29.11a8 8 0 0 1-1.32-15.89c16.57-2.79 30.63-16.85 33.44-33.45a8 8 0 0 1 15.78 2.68Z" />
              </svg>
            </div>
          </div>
          <div class="column">
            <h6 class="is-size-6 has-text-black has-text-weight-semibold">{{ card.name }}</h6>
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

    watch:{
      'card.value':function() {
        this.activity = true;
        setTimeout(() => {this.activity = false}, 100);
      }
    },

    mounted() {
      setTimeout(() => {
        this.activity = false
      }, 500);
    }
  }
</script>