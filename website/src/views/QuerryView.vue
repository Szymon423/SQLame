<template>
	<div class="view-container">
		<div class="query-container">
			<h1 class="text-2xl mb-4">Query</h1>
			<Codemirror v-model="jsonInput" placeholder="Code goes here..." :autofocus="true" :indent-with-tab="true"
				:tab-size="2" class="codemirror-editor" />
			<button type="submit"
				class="w-full px-4 py-2 my-4 font-bold text-white bg-gray-600 rounded hover:bg-gray-700 focus:outline-none focus:ring-2 focus:ring-gray-500">
				Submit
			</button>
		</div>

		<div class="response-container">
			<h1 class="text-2xl mb-4">Response</h1>
			<Codemirror v-model="jsonOutput" placeholder="Output will be here..." :autofocus="true"
				:indent-with-tab="true" :tab-size="2" class="codemirror-editor" />
		</div>
	</div>
</template>

<script lang="ts">
import { defineComponent, ref } from 'vue'
import { Codemirror } from 'vue-codemirror'

export default defineComponent({
	name: 'QueryView',
	components: {
		Codemirror
	},
	setup() {
		const jsonInput = ref('{\n    "name": "example",\n    "value": 42\n}')
		const jsonOutput = ref('{\n    "response": "example response"\n}')

		return {
			jsonInput,
			jsonOutput
		}
	}
})
</script>

<style scoped>
.view-container {
	display: flex;
	flex-direction: column;
	height: 100vh;
}

.query-container,
.response-container {
	flex: 1;
	display: flex;
	flex-direction: column;
	padding: 1rem;
}

.query-container {
	height: 200px;
	flex: 2;
}

.response-container {
	flex: 1;
}

.codemirror-editor {
	flex: 1;
	overflow: auto;
}
</style>