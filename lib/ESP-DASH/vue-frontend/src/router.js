import Vue from 'vue'
import Router from 'vue-router'

Vue.use(Router);

import Home from './views/Home.vue';
// import Stats from './views/Stats.vue';
import Hub from './views/Hub.vue';
// import Lock from './views/Lock.vue';

export default new Router({
  linkActiveClass: 'is-active',
  routes: [
    /*
    {
      path: '/locked',
      name: 'Locked',
      component: Lock
    },
    */
    {
      path: '/',
      name: 'Home',
      component: Home,
    },
    // {
    //   path: '/statistics',
    //   name: 'Statistics',
    //   component: Stats,
    // },
    {
      path: '/hub',
      name: 'Hub',
      component: Hub,
    },
  ],
});
