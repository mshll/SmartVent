<template>
  <div class="column is-12-mobile is-6-tablet">
    <div class="card">
      <span class="dot" :class="{ 'active': activity }"></span>
      <div class="card-content">
        <div class="columns is-mobile is-vcentered">
          <div class="column has-text-left">
            <h6 class="is-size-4 has-text-weight-bold">{{ name }}</h6>
            <div class="tags my-2 has-text-weight-semibold">
              <span class="tag" v-if="is_leader == 1" title="Host Device">
                <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24">
                  <path fill="currentColor" d="m12 17.27l4.15 2.51c.76.46 1.69-.22 1.49-1.08l-1.1-4.72l3.67-3.18c.67-.58.31-1.68-.57-1.75l-4.83-.41l-1.89-4.46c-.34-.81-1.5-.81-1.84 0L9.19 8.63l-4.83.41c-.88.07-1.24 1.17-.57 1.75l3.67 3.18l-1.1 4.72c-.2.86.73 1.54 1.49 1.08z" />
                </svg>
              </span>
              <span class="tag" v-if="this_device == 1">This Device</span>
              <div class="tags mb-0 has-addons">
                <span class="tag is-black">ID</span>
                <span class="tag">{{ id }}</span>
              </div>
            </div>
            <div class="is-fullwidth has-background-black has-text-white is-size-6" style="border-radius: 0.35rem">
              <a class="px-5 py-3 is-link has-text-weight-medium has-text-white" style="display: flex; justify-content: center; align-items: center;" :href="this_device == 1 ? '#/' : url" :target="this_device == 0 ? '_blank' : ''" key="active">
                <small>View Dashboard</small>
                <span class="icon-text">
                  <svg xmlns="http://www.w3.org/2000/svg" width="20" height="20" viewBox="0 0 24 24">
                    <path fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="m10 17l5-5l-5-5" />
                  </svg>
                </span>
              </a>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  props: ['name', 'url', 'is_leader', 'id', 'this_device'],

  data() {
    return {
      activity: true
    }
  },

  watch: {
    'name': function () {
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