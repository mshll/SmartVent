<template>
<div class="card">
  <div class="card-content">
    <nav class="navbar" role="navigation" aria-label="main navigation">
      <div class="navbar-brand">
        <router-link class="navbar-item" to="/">
          <h1 class="title is-4">Smart Vent</h1>
        </router-link>

        <a role="button" class="navbar-burger burger" aria-label="menu" :aria-expanded="open" :class="{'is-active': open}" @click.prevent="open = !open">
          <span aria-hidden="true"></span>
          <span aria-hidden="true"></span>
          <span aria-hidden="true"></span>
        </a>
      </div>

      <div class="navbar-menu" :class="{ 'is-active': open }">
        <div class="navbar-end">
          <router-link class="navbar-item" @click.native="open = false" to="/" exact>
            Overview
          </router-link>
          <router-link class="navbar-item" @click.native="open = false" to="/statistics">
            Statistics
          </router-link>
        </div>
      </div>
    </nav>
  </div>
</div>
</template>

<script>
  export default {
    props: ['connected', 'stats'],

    data() {
      return {
        open: false,
      }
    },
  }
</script>
