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
              <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 256 256">
                <path fill="currentColor" d="M128 24a104 104 0 1 0 104 104A104.12 104.12 0 0 0 128 24m88 104a87.61 87.61 0 0 1-3.33 24h-38.51a157.44 157.44 0 0 0 0-48h38.51a87.61 87.61 0 0 1 3.33 24m-114 40h52a115.11 115.11 0 0 1-26 45a115.27 115.27 0 0 1-26-45m-3.9-16a140.84 140.84 0 0 1 0-48h59.88a140.84 140.84 0 0 1 0 48ZM40 128a87.61 87.61 0 0 1 3.33-24h38.51a157.44 157.44 0 0 0 0 48H43.33A87.61 87.61 0 0 1 40 128m114-40h-52a115.11 115.11 0 0 1 26-45a115.27 115.27 0 0 1 26 45m52.33 0h-35.62a135.28 135.28 0 0 0-22.3-45.6A88.29 88.29 0 0 1 206.37 88Zm-98.74-45.6A135.28 135.28 0 0 0 85.29 88H49.63a88.29 88.29 0 0 1 57.96-45.6M49.63 168h35.66a135.28 135.28 0 0 0 22.3 45.6A88.29 88.29 0 0 1 49.63 168m98.78 45.6a135.28 135.28 0 0 0 22.3-45.6h35.66a88.29 88.29 0 0 1-57.96 45.6" />
              </svg>
            </div>
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
    'card.name': function () {
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