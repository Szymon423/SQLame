<template>
	<div class="view-container">
		<div class="query-container">
			<h1 class="text-2xl mb-4">Query</h1>
			<Codemirror v-model="jsonInput" :options="codeMirrorOptions" class="codemirror-container"></Codemirror>
			<button type="submit"
				class="w-full px-4 py-2 my-4 font-bold text-white bg-gray-600 rounded hover:bg-gray-700 focus:outline-none focus:ring-2 focus:ring-blue-500">
				Submit
			</button>
		</div>

		<div class="response-container">
			<h1 class="text-2xl mb-4">Response</h1>
			<Codemirror v-model="jsonOutput" :options="codeMirrorOptionsReadOnly" class="codemirror-container">
			</Codemirror>
		</div>
	</div>
</template>

<script lang="ts">
import { defineComponent, ref } from 'vue'
import { Codemirror } from 'vue-codemirror'
import { basicSetup } from 'codemirror'

export default defineComponent({
	name: 'QueryView',
	components: {
		Codemirror
	},
	setup() {
		const jsonInput = ref('{\n    "name": "example",\n    "value": 42\n}')
		const jsonOutput = ref('{\n    "response": "example response"\n}')
		const codeMirrorOptions = {
			autofocus: true,
			disabled: false,
			indentWithTab: true,
			tabSize: 2,
			placeholder: 'Code goes here...',
			extensions: [basicSetup]
		}
		const codeMirrorOptionsReadOnly = {
			...codeMirrorOptions,
			readOnly: 'nocursor' // Ustawienie pola jako tylko do odczytu
		}

		return {
			jsonInput,
			jsonOutput,
			codeMirrorOptions,
			codeMirrorOptionsReadOnly
		}
	}
})
</script>

<style scoped>
.view-container {
	display: grid;
	grid-template-rows: 2fr 1fr;
	height: 100vh;
}

.query-container,
.response-container {
	padding: 1rem;
}

.codemirror-container {
	height: 100%;
	overflow: auto;
	border: 1px solid #d1d5db;
	/* Dodanie obramowania */
	border-radius: 0.375rem;
	/* Dodanie zaokrąglenia */
}
</style>