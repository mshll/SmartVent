<template>
  <div>
    <div class="container" v-if="devices.length > 0">
      <div class="columns is-mobile is-multiline">
        <link-card v-for="device in devices" :key="device.id" :id="device.id" :name="device.name" :url="device.url" :is_leader="device.is_leader" :this_device="device.this_device"></link-card>
        <!-- for testing -->
        <!-- <link-card id="96f6b870" name="Smart Vent 1" url="https://www.google.com" is_leader="1" this_device="1"></link-card> -->
      </div>
    </div>
    <div class="container" v-else>
      <div class="section">
        <div class="row">
          <div class="columns is-centered has-text-centered" style="font-weight: 400; font-size: 18px">
            <div class="column">
              Loading
              <svg width="24px" height="24px" class="spinner" style="vertical-align: middle;" viewBox="0 0 24 24" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
                <g id="Stockholm-icons-/-Code-/-Loading" stroke="none" stroke-width="1" fill="none" fill-rule="evenodd">
                  <g id="Group">
                    <polygon id="Shape" points="0 0 24 0 24 24 0 24"></polygon>
                  </g>
                  <path d="M12,4 L12,6 C8.6862915,6 6,8.6862915 6,12 C6,15.3137085 8.6862915,18 12,18 C15.3137085,18 18,15.3137085 18,12 C18,10.9603196 17.7360885,9.96126435 17.2402578,9.07513926 L18.9856052,8.09853149 C19.6473536,9.28117708 20,10.6161442 20,12 C20,16.418278 16.418278,20 12,20 C7.581722,20 4,16.418278 4,12 C4,7.581722 7.581722,4 12,4 Z" id="Oval-3" fill="currentColor" opacity="0.3" transform="translate(12.000000, 12.000000) scale(-1, 1) translate(-12.000000, -12.000000) "></path>
                </g>
              </svg>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
// @ts-nocheck
import LinkCard from '@/components/LinkCard.vue';

export default {
  name: 'hub',

  props: ['devices'],

  components: {
    LinkCard
  },

}
</script>