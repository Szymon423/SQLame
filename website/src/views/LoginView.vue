<template>
    <div class="h-screen flex items-center justify-center bg-gray-900 text-white">
      <div class="w-full max-w-md p-8 space-y-6 bg-gray-800 rounded-lg shadow-md">
        <!-- Logo -->
        <div class="flex flex-col items-center">
          <img src="@/assets/logo.png" alt="Logo"/>
          <h2 class="mt-4 text-2xl font-bold">Login</h2>
        </div>
        <!-- Login Form -->
        <form @submit.prevent="login" class="space-y-4">
          <div>
            <label for="name" class="block text-sm font-medium">name</label>
            <input
              id="name"
              v-model="name"
              type="name"
              required
              class="w-full px-3 py-2 mt-1 bg-gray-700 border border-gray-600 rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
            />
          </div>
          <div>
            <label for="password" class="block text-sm font-medium">Password</label>
            <input
              id="password"
              v-model="password"
              type="password"
              required
              class="w-full px-3 py-2 mt-1 bg-gray-700 border border-gray-600 rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
            />
          </div>
          <div>
            <button
              type="submit"
              class="w-full px-4 py-2 font-bold text-white bg-blue-600 rounded hover:bg-blue-700 focus:outline-none focus:ring-2 focus:ring-blue-500"
            >
              Log In
            </button>
          </div>
        </form>
      </div>
    </div>
  </template>
  
  <script lang="ts">
  import { defineComponent, ref } from 'vue'
  
  export default defineComponent({
    name: 'LoginView',
    setup() {
      const name = ref('')
      const password = ref('')
  
      const login = () => {
        // Logika logowania
        console.log('name:', name.value)
        console.log('Password:', password.value)
      }
  
      return {
        name,
        password,
        login
      }
    }
  })
  </script>
  
  <style scoped>
  /* Dodatkowe stylizacje, jeśli są potrzebne */
  </style>
  