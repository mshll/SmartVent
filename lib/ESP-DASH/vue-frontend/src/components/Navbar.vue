<template>
  <div class="card">
    <div class="card-content">
      <nav class="navbar" role="navigation" aria-label="main navigation">
        <div class="navbar-brand">
          <router-link class="navbar-item" to="/" style="flex-grow: 1; background-color: transparent;">
            <h1 class="title is-size-4 has-text-weight-bold has-text-black"> {{ stats.device_name }} </h1>
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
              Dashboard
            </router-link>
            <router-link class="navbar-item" @click.native="open = false" to="/hub">
              <span v-if="stats.devices_count > 1" class="badge is-black has-text-weight-bold" style="padding: 0.5rem 0.4rem; line-height: 0px; border: none;">{{ stats.devices_count }}</span>
              Devices Hub
            </router-link>
            <!-- <router-link class="navbar-item" @click.native="open = false" to="/statistics">
            Statistics
          </router-link> -->
          </div>
        </div>
      </nav>
    </div>
  </div>
</template>

<script>
import Socket from '../socket';

export default {
  props: ['connected', 'stats'],

    data() {
      return {
        open: false,
      }
    },

  mounted() {
    Socket.send(JSON.stringify({
      "command": "getStats"
    }));
  }
}
</script>
