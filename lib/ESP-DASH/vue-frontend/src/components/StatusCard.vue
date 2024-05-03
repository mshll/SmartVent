<template>
  <div class="column is-12-mobile is-4-tablet is-4-desktop is-3-fullhd">
    <div class="card">
      <span class="dot" :class="{'active': activity}"></span>
      <div class="card-content">
        <div class="columns is-mobile">
          <div class="column is-narrow">
            <div class="card-icon pb-4 pt-5 px-5 has-background-success-light has-text-success" v-if="card.symbol === 'success'">
              <svg width="28" height="28" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
                <g fill="none" fill-rule="evenodd"><path d="M0 0h24v24H0z"></path><circle fill="currentColor" opacity=".3" cx="12" cy="12" r="10"></circle><path d="M16.769 7.818a1 1 0 011.462 1.364l-7 7.5a1 1 0 01-1.382.077l-3.5-3a1 1 0 011.302-1.518l2.772 2.376 6.346-6.8z" fill="currentColor"></path></g>
              </svg>
            </div>
            <div class="card-icon pb-4 pt-5 px-5 has-background-danger-light has-text-danger" v-else-if="card.symbol === 'danger'">
              <svg width="28" height="28" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
                <g fill="none" fill-rule="evenodd"><path d="M0 0h24v24H0z"></path><circle fill="currentColor" opacity=".3" cx="12" cy="12" r="10"></circle><rect fill="currentColor" x="11" y="7" width="2" height="8" rx="1"></rect><rect fill="currentColor" x="11" y="16" width="2" height="2" rx="1"></rect></g>
              </svg>
            </div>
            <div class="card-icon pb-4 pt-5 px-5 has-background-warning-light has-text-warning" v-else-if="card.symbol === 'warning'">
              <svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 24 24">
                <path fill="currentColor" d="M12 3c-2.31 0-3.77 2.587-6.688 7.762l-.364.644c-2.425 4.3-3.638 6.45-2.542 8.022S6.214 21 11.636 21h.728c5.422 0 8.134 0 9.23-1.572s-.117-3.722-2.542-8.022l-.364-.645C15.77 5.587 14.311 3 12 3" opacity="0.5"/><path fill="currentColor" d="M12 7.25a.75.75 0 0 1 .75.75v5a.75.75 0 0 1-1.5 0V8a.75.75 0 0 1 .75-.75M12 17a1 1 0 1 0 0-2a1 1 0 0 0 0 2"/>
              </svg>
            </div>
            <div class="card-icon pb-4 pt-5 px-5 has-bg-white has-text-black" v-else>
              <svg width="28" height="28" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
                <g fill="none" fill-rule="evenodd">
                  <path d="M0 0h24v24H0z"></path>
                  <circle fill="currentColor" opacity=".3" cx="12" cy="12" r="10"></circle>
                  <rect fill="currentColor" x="11" y="10" width="2" height="7" rx="1"></rect>
                  <rect fill="currentColor" x="11" y="7" width="2" height="2" rx="1"></rect>
                </g>
              </svg>
            </div>
          </div>
          <div class="column">
            <h6 class="is-size-6 has-text-black has-text-weight-semibold">{{card.name}}</h6>
            <h2 class="is-size-3 has-text-weight-bold" :class="{ 'has-text-success': card.symbol === 'success', 'has-text-danger': card.symbol === 'danger', 'has-text-warning': card.symbol === 'warning', 'has-text-black': !card.symbol }">
              {{card.value}}</h2>
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
      },
      'card.symbol':function() {
        this.activity = true;
        setTimeout(() => {this.activity = false}, 100);
      },
    },

    mounted() {
      setTimeout(() => {
        this.activity = false
      }, 500);
    }
  }
</script>