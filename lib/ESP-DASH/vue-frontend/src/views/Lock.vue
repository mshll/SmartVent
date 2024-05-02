<template>
    <div class="container" style="margin: 4.55rem 1rem;">
        <div class="columns is-mobile is-multiline is-centered">
            <div class="column is-12">
                <div class="columns is-mobile is-centered">
                    <div class="column has-text-centered">
                    Smart Vent
                </div>
                </div>

            </div>
            <div class="column is-10-mobile is-4-desktop">
                <div class="card">
                    <div class="card-content">
                        <header class="columns is-gapless is-mobile">
                            <svg width="24px" height="24px" viewBox="0 0 24 24" version="1.1"
                                xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
                                <g id="Stockholm-icons-/-Code-/-Lock-circle" stroke="none" stroke-width="1" fill="none"
                                    fill-rule="evenodd">
                                    <rect id="bound" x="0" y="0" width="24" height="24"></rect>
                                    <circle id="Oval-5" fill="#335EEA" opacity="0.3" cx="12" cy="12" r="10"></circle>
                                    <path
                                        d="M14.5,11 C15.0522847,11 15.5,11.4477153 15.5,12 L15.5,15 C15.5,15.5522847 15.0522847,16 14.5,16 L9.5,16 C8.94771525,16 8.5,15.5522847 8.5,15 L8.5,12 C8.5,11.4477153 8.94771525,11 9.5,11 L9.5,10.5 C9.5,9.11928813 10.6192881,8 12,8 C13.3807119,8 14.5,9.11928813 14.5,10.5 L14.5,11 Z M12,9 C11.1715729,9 10.5,9.67157288 10.5,10.5 L10.5,11 L13.5,11 L13.5,10.5 C13.5,9.67157288 12.8284271,9 12,9 Z"
                                        id="Combined-Shape" fill="#335EEA"></path>
                                </g>
                            </svg>
                            <p class="column" style="margin-left: 0.5rem; line-height: 1.2;">Locked</p>
                        </header>
                        <br />
                        <div class="has-text-dark">
                            <div class="field is-horizontal">
                                <div class="field-body">
                                    <div class="field">
                                        <p class="control">
                                            <input class="input" type="password" placeholder="Key" disabled>
                                        </p>
                                    </div>
                                </div>
                            </div>
                            <br />
                            <button class="button is-primary is-light is-fullwidth is-loading" disabled>Unlock</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>