import Vue from 'vue'
import Router from 'vue-router'

// 按需加载
// import Login from '@/components/Login'
const Login = () => import(/* webpackChunkName: 'login' */'@/components/Login')
// import Home from '@/components/Home'
const Home = () => import(/* webpackChunkName: 'home' */'@/components/Home')
// import Users from '@/components/users/Users'
const Users = () => import(/* webpackChunkName: 'users' */'@/components/users/Users')
// import Roles from '@/components/rights/Roles'
const Roles = () => import(/* webpackChunkName: 'roles' */'@/components/rights/Roles')
// import Rights from '@/components/rights/Rights'
const Rights = () => import(/* webpackChunkName: 'rights' */'@/components/rights/Rights')
// import Categories from '@/components/product/Categories'
const Categories = () => import(/* webpackChunkName: 'categories' */'@/components/product/Categories')
// import Goods from '@/components/product/Goods'
const Goods = () => import(/* webpackChunkName: 'goods' */'@/components/product/Goods')
// import AddGoods from '@/components/product/Add'
const AddGoods = () => import(/* webpackChunkName: 'goods' */'@/components/product/Add')

Vue.use(Router)

const router = new Router({
  routes: [
    {
      path: '/',
      redirect: 'home'
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
        },
        {
          path: '/categories',
          name: 'categories',
          component: Categories
        },
        {
          path: '/goods',
          name: 'goods',
          component: Goods
        },
        {
          path: '/goods-add',
          name: 'goods-add',
          component: AddGoods
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
