<template>
  <div :class="isLoginRoute ? 'screen-login-container bg-gray-900' : 'screen-container'">
    <SidebarComponent v-if="!isLoginRoute" />
    <div :class="isLoginRoute ? 'login-content bg-gray-900' : 'flex-1 p-4'">
      <router-view />
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent, computed } from 'vue'
import { useRoute } from 'vue-router'
import SidebarComponent from './components/SidebarComponent.vue'

export default defineComponent({
  name: 'App',
  components: {
    SidebarComponent
  },
  setup() {
    const route = useRoute()
    const isLoginRoute = computed(() => route.path === '/login')

    return {
      isLoginRoute
    }
  }
})
</script>

<style>
body {
  margin: 0;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, 'Noto Sans', sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol', 'Noto Color Emoji';
  height: 100vh;
  width: 100vw;
  overflow: hidden;
}

.screen-container {
  display: grid;
  grid-template-columns: 1fr 6fr;
  height: 100vh;
}

.screen-login-container {
  display: flex;
  height: 100vh;
  width: 100vw;
  justify-content: center;
  align-items: center;
  background-color: #1a202c; /* Ustaw tło na ciemny kolor, aby pasowało do reszty strony */
}

.login-content {
  width: 100%;
  max-width: 400px; /* Ustaw maksymalną szerokość dla formularza logowania */
  padding: 2rem;
  background-color: #2d3748; /* Tło formularza logowania */
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}
</style>
