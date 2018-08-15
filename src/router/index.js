import Vue from 'vue'
import Router from 'vue-router'

import Login from '@/components/Login'
import Home from '@/components/Home'
import Users from '@/components/Users'
import Roles from '@/components/Roles'
import Rights from '@/components/Rights'

Vue.use(Router)

const router = new Router({
  routes: [
    {
      path: '/',
      redirect: '/login'
    },
    {
      path: '/login',
      name: 'login',
      component: Login
    },
    {
      path: '/home',
      name: 'home',
      component: Home,
      children: [
        {
          path: '/users',
          name: 'users',
          component: Users
        },
        {
          path: '/roles',
          name: 'roles',
          component: Roles
        },
        {
          path: '/rights',
          name: 'rights',
          component: Rights
        }
      ]
    }
  ]
})
router.beforeEach((to, from, next) => {
  // if (to.path === '/login') {
  //   next()
  //   return false
  // }
  let token = localStorage.getItem('token')
  if (to.path === '/login' || token) {
    next()
  } else {
    next('login')
  }
})
export default router
