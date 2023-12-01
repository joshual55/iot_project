import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/login',
      name: 'login',
      component: () => import('../views/Login.vue')
    },
    {
      path: '/',
      name: 'Home',
      component: () => import('../views/Home.vue')
    },
    {
      path: '/add-user',
      name: 'Add User',
      component: () => import('../views/AddUser.vue')
    },
    {
      path: '/verify-code',
      name: 'Verify Code',
      component: () => import('../views/VerifyCode.vue')
    }
  ]
})

export default router
