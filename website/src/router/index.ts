import { createRouter, createWebHistory, RouteRecordRaw } from 'vue-router'
import UsersView from '../views/UsersView.vue'
import ConfigurationView from '../views/ConfigurationView.vue'
import LogsView from '../views/LogsView.vue'
import DataView from '../views/DataView.vue'
import AboutView from '../views/AboutView.vue'
import QuerryView from '../views/QuerryView.vue'
import LoginView from '../views/LoginView.vue'

const routes: Array<RouteRecordRaw> = [
  { path: '/', redirect: '/login' },
  { path: '/login', name: 'Login', component: LoginView },
  { path: '/users', name: 'Users', component: UsersView },
  { path: '/configuration', name: 'Configuration', component: ConfigurationView },
  { path: '/logs', name: 'Logs', component: LogsView },
  { path: '/data', name: 'Data', component: DataView },
  { path: '/about', name: 'About', component: AboutView },
  { path: '/querry', name: 'Querry', component: QuerryView },
  
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

export default router
