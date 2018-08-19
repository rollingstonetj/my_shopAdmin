import ElementUI from 'element-ui'
import 'element-ui/lib/theme-chalk/index.css'

// 富文本编辑器的样式
import 'quill/dist/quill.core.css'
import 'quill/dist/quill.snow.css'
import 'quill/dist/quill.bubble.css'

import Vue from 'vue'
import App from './App'
import router from './router'


import axios from 'axios'
import moment from 'moment'
// 导入富文本编辑器
import VueQuillEditor from 'vue-quill-editor'
Vue.use(VueQuillEditor)
Vue.config.productionTip = false
Vue.use(ElementUI)

// 定义过滤器
Vue.filter('dateFilter', (input, format = 'YYYY-MM-DD HH:mm:ss') => {
  return moment(input).format(format)
})

/* eslint-disable no-new */
Vue.prototype.axios = axios

// 1.拦截器
// 添加请求拦截器
axios.interceptors.request.use(function (config) {
  // 在发送请求之前做些什么
  // console.log('haha')
  config.baseURL = 'http://localhost:8888/api/private/v1/'
  config.headers.Authorization = localStorage.getItem('token')
  return config
}, function (error) {
  // 对请求错误做些什么
  return Promise.reject(error)
})
// 2.配置axios的默认值
// axios.defaults.baseURL = 'http://localhost:8888/api/private/v1/'
// axios.defaults.headers.common['Authorization'] = localStorage.getItem('token')

new Vue({
  el: '#app',
  router,
  components: { App },
  template: '<App/>'
})
