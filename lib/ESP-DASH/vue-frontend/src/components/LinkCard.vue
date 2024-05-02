<template>
  <div class="column is-12-mobile is-4-tablet is-4-desktop is-3-fullhd">
    <div class="card">
      <span class="dot" :class="{ 'active': activity }"></span>
      <div class="card-content">
        <div class="columns is-mobile is-vcentered">
          <div class="column has-text-left">
            <div class="tags mb-0">
              <span class="tag is-warning is-light" v-if="is_leader == 1">
                <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24">
                  <path fill="currentColor" d="m12 17.27l4.15 2.51c.76.46 1.69-.22 1.49-1.08l-1.1-4.72l3.67-3.18c.67-.58.31-1.68-.57-1.75l-4.83-.41l-1.89-4.46c-.34-.81-1.5-.81-1.84 0L9.19 8.63l-4.83.41c-.88.07-1.24 1.17-.57 1.75l3.67 3.18l-1.1 4.72c-.2.86.73 1.54 1.49 1.08z" />
                </svg>
              </span>
              <span class="tag is-danger is-light" v-if="this_device == 1">This Vent</span>
              <span class="tag is-info is-light">ID: {{ id }}</span>
            </div>
            <h6 class="is-size-5 has-text-weight-semibold">{{ name }}</h6>
          </div>
          <div class="column is-narrow">
            <a :href="this_device == 1 ? '/#' : url" :target="this_device == 0 ? '_blank' : ''" class="card-icon py-4 px-5 button  is-responsive is-link has-text-light" key="active">
              <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24">
                <path fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M10 4H6a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2v-4m-8-2l8-8m0 0v5m0-5h-5" />
              </svg> </a>
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