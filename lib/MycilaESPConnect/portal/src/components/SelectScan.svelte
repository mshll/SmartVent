<script>
  import { createEventDispatcher } from 'svelte';
  const dispatch = createEventDispatcher();

  export let access_points;

  const signalWeakSVG = `<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"><path fill="currentColor" d="M15.53 17.46L12 21l-3.53-3.54c.9-.9 2.15-1.46 3.53-1.46s2.63.56 3.53 1.46"/></svg>`;
  const signalModerateSVG = `<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"><path fill="currentColor" d="M12 10c3.03 0 5.78 1.23 7.76 3.22l-2.12 2.12A7.967 7.967 0 0 0 12 13c-2.2 0-4.2.9-5.64 2.35l-2.12-2.12C6.22 11.23 8.97 10 12 10m0 6c-1.38 0-2.63.56-3.53 1.46L12 21l3.53-3.54A4.98 4.98 0 0 0 12 16"/></svg>`;
  const signalStrongSVG = `<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"><path fill="currentColor" d="m1 9l2 2c4.97-4.97 13.03-4.97 18 0l2-2C16.93 2.93 7.08 2.93 1 9m8 8l3 3l3-3a4.237 4.237 0 0 0-6 0m-4-4l2 2a7.074 7.074 0 0 1 10 0l2-2C15.14 9.14 8.87 9.14 5 13"/></svg>`;
  const signalVeryStrongSVG = `<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"><path fill="currentColor" d="m1 9l2 2c4.97-4.97 13.03-4.97 18 0l2-2C16.93 2.93 7.08 2.93 1 9m8 8l3 3l3-3a4.237 4.237 0 0 0-6 0m-4-4l2 2a7.074 7.074 0 0 1 10 0l2-2C15.14 9.14 8.87 9.14 5 13"/></svg>`;

  function signalIcon(signal) {
    if (signal < 25) {
      return signalWeakSVG;
    } else if (signal < 50) {
      return signalModerateSVG;
    } else if (signal < 75) {
      return signalStrongSVG;
    } else {
      return signalVeryStrongSVG;
    }
  }
</script>

<div class="container d-flex flex-columns">
  <div class="row h-100">
    <div class="column" style="margin-bottom: 3rem;">
      <div class="row clickable-row" on:click={() => dispatch('select', { ap_mode: true, ssid: '' })}>
        <div class="column">
          <div class="container">
                <div class="row flex-rows">
                  <div class="column w-100">
                    Access Point Mode
                  </div>
                  <div class="column w-auto" style="display: inline-flex;">
                    <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"><path fill="currentColor" d="M4.93 4.93A9.969 9.969 0 0 0 2 12c0 2.76 1.12 5.26 2.93 7.07l1.41-1.41A7.938 7.938 0 0 1 4 12c0-2.21.89-4.22 2.34-5.66zm14.14 0l-1.41 1.41A7.955 7.955 0 0 1 20 12c0 2.22-.89 4.22-2.34 5.66l1.41 1.41A9.969 9.969 0 0 0 22 12c0-2.76-1.12-5.26-2.93-7.07M7.76 7.76A5.98 5.98 0 0 0 6 12c0 1.65.67 3.15 1.76 4.24l1.41-1.41A3.99 3.99 0 0 1 8 12c0-1.11.45-2.11 1.17-2.83zm8.48 0l-1.41 1.41A3.99 3.99 0 0 1 16 12c0 1.11-.45 2.11-1.17 2.83l1.41 1.41A5.98 5.98 0 0 0 18 12c0-1.65-.67-3.15-1.76-4.24M12 10a2 2 0 0 0-2 2a2 2 0 0 0 2 2a2 2 0 0 0 2-2a2 2 0 0 0-2-2"/></svg>
                  </div>
                </div>
          </div>
        </div>
      </div>
      {#if access_points.length > 0}
        {#each access_points as ap}
          <div class="row clickable-row" on:click={() => dispatch('select', { ssid: ap.name, open: ap.open })}>
            <div class="column">
              <div class="container">
                <div class="row flex-rows">
                  <div class="column w-100">
                    {ap.name}
                  </div>
                  <div class="column w-auto" style="display: inline-flex; display: inline-flex; gap: 1rem;">
                    {#if !ap.open}
                      <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" style="fill: currentColor"><path d="M20,12c0-1.103-0.897-2-2-2h-1V7c0-2.757-2.243-5-5-5S7,4.243,7,7v3H6c-1.103,0-2,0.897-2,2v8c0,1.103,0.897,2,2,2h12 c1.103,0,2-0.897,2-2V12z M9,7c0-1.654,1.346-3,3-3s3,1.346,3,3v3H9V7z"></path></svg>
                      <!-- <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24"><path fill="currentColor" d="M18 8h-1V6c0-2.76-2.24-5-5-5S7 3.24 7 6v2H6c-1.1 0-2 .9-2 2v10c0 1.1.9 2 2 2h12c1.1 0 2-.9 2-2V10c0-1.1-.9-2-2-2m-6 9c-1.1 0-2-.9-2-2s.9-2 2-2s2 .9 2 2s-.9 2-2 2m3.1-9H8.9V6c0-1.71 1.39-3.1 3.1-3.1c1.71 0 3.1 1.39 3.1 3.1z"/></svg> -->
                    {/if}
                    {@html signalIcon(ap.signal)}
                  </div>
                </div>
              </div>
            </div>
          </div>
        {/each}
      {:else}
        <div class="row">
          <div class="column">
            <div class="container">
              <div class="row flex-rows">
                <div class="column text-center text-muted">No access points in range</div>
              </div>
            </div>
          </div>
        </div>
      {/if}
    </div>
  </div>
  <div class="row">
    <div class="column">
      <button class="button w-100" on:click={() => dispatch('refresh')}>Refresh</button>
    </div>
  </div>
</div>
