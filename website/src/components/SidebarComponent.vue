<template>
  <div class="h-screen bg-gray-900 text-white flex flex-col justify-between">
    <!-- Logo -->
    <div class="p-4 flex flex-col items-center">
      <img src="@/assets/logo.png" alt="Logo"/>
    </div>
    
    <!-- Navigation Links -->
    <nav class="flex-1 mt-4">
      <router-link to="/users" class="block p-2 hover:bg-gray-700">Users</router-link>
      <router-link to="/data" class="block p-2 hover:bg-gray-700">Data</router-link>
      <router-link to="/querry" class="block p-2 hover:bg-gray-700">Querry</router-link>
      <router-link to="/configuration" class="block p-2 hover:bg-gray-700">Configuration</router-link>
      <router-link to="/logs" class="block p-2 hover:bg-gray-700">Logs</router-link>
      <router-link to="/about" class="block p-2 hover:bg-gray-700">About</router-link>
    </nav>

    <!-- Napis na samym dole -->
    <div class="p-4 text-left">
      <h2 class="text-xl font-bold">Current User</h2>
      <!-- Kółko z pierwszą literą nazwy użytkownika i nazwa użytkownika -->
      <div class="flex items-center justify-between mt-2">
        <div class="flex items-center">
          <div class="bg-gray-700 text-white rounded-full h-10 w-10 flex items-center justify-center">
            <span class="text-lg font-bold">A</span>
          </div>
          <span class="ml-2 text-lg">Admin</span>
        </div>
        <!-- Ikona wylogowania -->
        <button @click="logout" class="ml-4 text-gray-400 hover:text-white">
          <font-awesome-icon :icon="['fas', 'sign-out-alt']" />
        </button>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import { library } from '@fortawesome/fontawesome-svg-core'
import { faSignOutAlt } from '@fortawesome/free-solid-svg-icons'
import { FontAwesomeIcon } from '@fortawesome/vue-fontawesome'

library.add(faSignOutAlt)

export default defineComponent({
  name: 'SidebarComponent',
  components: {
    FontAwesomeIcon
  },
  methods: {
    logout() {
      // Logika wylogowania
      console.log('User logged out')
    }
  }
})
</script>

<style scoped>
.router-link-active {
  background-color: #4a5568;
}
</style>
