<template>
  <div class="column is-12-mobile is-4-tablet is-4-desktop is-4-fullhd">
    <div class="card">
      <span class="dot" :class="{'active': activity}"></span>
      <div class="card-content">
        <div class="columns is-mobile is-vcentered">
          <div class="column has-text-left">
            <h6 class="is-size-7 has-text-grey has-text-weight-medium mb-1 is-family-monospace">{{ card.symbol }}</h6>
            <h2 class="is-size-5 has-text-black has-text-weight-semibold">{{ card.name }}</h2>
          </div>
          <div class="column is-narrow">
            <!-- <transition name="btn-fade" mode="out-in"> -->
            <div class="pt-2">
              <input :id="'btn-switch-' + card.id" type="checkbox" :name="'button-radio-' + card.id" class="switch is-medium is-rounded is-black" :checked="card.value == 1" @change="sendClickEvent">
              <label :for="'btn-switch-' + card.id"></label>
            </div>
            <!-- </transition> -->
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
    props:['card'],

    data(){
      return{
        activity: true
      }
    },

    methods:{
      sendClickEvent(){
        EventBus.$emit('buttonClicked', { id: this.card.id, value: !this.card.value });
        this.activity = true;
        setTimeout(() => { this.activity = false }, 100);
      }
    },

    mounted(){
      setTimeout(() => { this.activity = false }, 500);
    }
}
</script>