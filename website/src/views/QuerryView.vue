<template>
  <div class="p-4">
    <h1 class="text-2xl mb-4">Query</h1>
    <Codemirror
      v-model="jsonInput"
      :options="codeMirrorOptions"
      class="h-96 border border-gray-300"
    ></Codemirror>
  </div>
</template>

<script lang="ts">
import { defineComponent, ref } from 'vue'
import { Codemirror } from 'vue-codemirror'

export default defineComponent({
  name: 'QuerryView',
  components: {
    Codemirror
  },
  setup() {
    const jsonInput = ref('{\n    "name": "example",\n    "value": 42\n}')
    const codeMirrorOptions = {
      mode: 'application/json',
      indentWithTab: true,
      tabSize: 2,
      theme: 'default'
    }

    return {
      jsonInput,
      codeMirrorOptions
    }
  }
})
</script>

<style scoped>
/* Dodatkowe stylizacje, jeśli są potrzebne */
</style>
