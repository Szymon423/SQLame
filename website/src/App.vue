<template>
  <div class="screen-container">
    <SidebarComponent />
    <div class="flex-1 p-4">
      <router-view />
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import SidebarComponent from './components/SidebarComponent.vue'

export default defineComponent({
  name: 'App',
  components: {
    SidebarComponent
  }
})
</script>

<style>
body {
  margin: 0;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, 'Noto Sans', sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol', 'Noto Color Emoji';
}

.screen-container {
  display: grid;
  grid-template-columns: 1fr 6fr;
}
</style>
