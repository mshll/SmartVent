<template>
  <div class="column is-12-mobile is-4-tablet is-4-desktop is-4-fullhd">
    <div class="card">
      <span class="dot" :class="{'active': activity}"></span>
      <div class="card-content">
        <div class="columns is-mobile">
          <div class="column is-narrow">
            <div class="card-icon pb-4 pt-5 px-5 has-bg-white has-text-black">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 512 512">
                <path fill="currentColor" d="M258.6 0c-1.7 0-3.4.1-5.1.5C168 17 115.6 102.3 130.5 189.3c2.9 17 8.4 32.9 15.9 47.4L32 224h-2.6C13.2 224 0 237.2 0 253.4c0 1.7.1 3.4.5 5.1C17 344 102.3 396.4 189.3 381.5c17-2.9 32.9-8.4 47.4-15.9L224 480v2.6c0 16.2 13.2 29.4 29.4 29.4c1.7 0 3.4-.1 5.1-.5c85.5-16.5 137.9-101.8 123-188.8c-2.9-17-8.4-32.9-15.9-47.4L480 288h2.6c16.2 0 29.4-13.2 29.4-29.4c0-1.7-.1-3.4-.5-5.1c-16.5-85.5-101.8-137.9-188.8-123c-17 2.9-32.9 8.4-47.4 15.9L288 32v-2.6C288 13.2 274.8 0 258.6 0M256 224a32 32 0 1 1 0 64a32 32 0 1 1 0-64" />
              </svg>
            </div>
          </div>
          <div class="column">
            <h6 class="is-size-6 has-text-black has-text-weight-semibold is-capitalized">{{ card.name }}</h6>
            <h2 class="is-size-3 has-text-black has-text-weight-bold">{{ card.value }} <small class="is-size-5 has-text-weight-normal has-text-grey">{{ card.symbol }}</small></h2>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  props:['card'],

  data(){
    return{
      activity: true
    }
  },

  watch:{
    'card.value':function() {
      this.activity = true;
      setTimeout(() => {this.activity = false}, 100);
    }
  },

  mounted(){
    setTimeout(() => { this.activity = false }, 500);
  }
}
</script>
