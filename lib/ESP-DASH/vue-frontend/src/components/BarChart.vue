<template>
  <div class="column is-12-mobile is-6-tablet is-6-desktop">
    <div class="card">
      <span class="dot" :class="{'active': activity}"></span>
      <div class="card-content">
        <div class="columns is-mobile is-vcentered">
          <div class="column is-narrow has-text-primary">
            <div class="card-icon pb-1 pt-2 px-2 has-bg-white has-text-black">
              <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24">
                <g fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="2">
                  <path d="M3 4.5v15h17" />
                  <path d="m19 9l-5 5l-4-4l-3 3" opacity=".6" />
                </g>
              </svg>
            </div>
          </div>
          <div class="column">
            <h6 class="is-size-6 has-text-black has-text-weight-semibold" v-html="chart.name.replace('CO2', 'CO<sub>2</sub>') "></h6>
          </div>
        </div>
        <div class="columns">
          <div class="column is-12">
            <line-chartjs :chart-data="chartData" :options="options" height="250px"></line-chartjs>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
  import LineChartjs from './Charts/Bar';

  export default {
    props: ['chart'],

    components: {
      LineChartjs
    },

    data() {
      return {
        activity: true,
        options: {
          responsive: true,
          aspectRatio: 1,
          height: 200,
          legend: {
            display: false
          },
          scales: {
            xAxes: [{
              barPercentage: 0.6,
              gridLines: {
                display: false
              }
            }],
          }
        }
      }
    },

    watch: {
      'chart.x_axis': {
        deep: true,
        handler() {
          if (this.activity === false) {
            this.activity = true;
            setTimeout(() => {
              this.activity = false
            }, 100);
          }
        }
      },
      'chart.y_axis': {
        deep: true,
        handler() {
          if (this.activity === false) {
            this.activity = true;
            setTimeout(() => {
              this.activity = false
            }, 100);
          }
        }
      },
    },

    computed: {
      chartData() {
        return {
          labels: this.chart.x_axis,
          datasets: [{
            label: '',
            backgroundColor: '#2B2B2B',
            data: this.chart.y_axis
          }]
        }
      }
    },

    mounted() {
      setTimeout(() => {
        this.activity = false
      }, 500);
    }
  }
</script>