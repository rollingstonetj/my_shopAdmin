<template>
  <div class="users">
    <!-- 面包屑 -->
    <el-breadcrumb separator-class="el-icon-arrow-right">
      <el-breadcrumb-item :to="{ path: '/' }">首页</el-breadcrumb-item>
      <el-breadcrumb-item>用户列表</el-breadcrumb-item>
      <el-breadcrumb-item>用户管理</el-breadcrumb-item>
    </el-breadcrumb>
    <!-- 搜索框 -->
      <el-input placeholder="请输入内容" v-model="query" class="input-with-select">
        <el-button slot="append" icon="el-icon-search" @click="search"></el-button>
      </el-input>
      <el-button type="success" plain>成功按钮</el-button>
    <!-- 表格组件 -->
    <el-table
      :data="userList"
      style="width: 100%">
      <el-table-column
        prop="username"
        label="姓名"
        width="180">
      </el-table-column>
      <el-table-column
        prop="email"
        label="邮箱"
        width="180">
      </el-table-column>
      <el-table-column
        prop="mobile"
        label="电话">
      </el-table-column>
        <el-table-column
        prop="mg_state"
        label="用户状态">
        <!-- 自定义列模板 -->
          <template slot-scope="scope">
            <el-switch
              v-model="scope.row.mg_state"
              active-color="#13ce66"
              inactive-color="#ff4949">
            </el-switch>
          </template>
      </el-table-column>
        <el-table-column
        prop=""
        label="操作">
         <template slot-scope="scope">
           <el-button type="primary" icon="el-icon-edit" size="small" plain></el-button>
           <el-button type="danger" icon="el-icon-delete" size="small" plain></el-button>
           <el-button type="success" icon="el-icon-check" size="small" plain>分配角色</el-button>
         </template>
      </el-table-column>
    </el-table>
    <!-- 分页插件 -->
    <el-pagination
      background
      :page-sizes="[2,4,6]"
      layout="total,sizes,prev, pager, next,jumper"
      :total="total"
      :page-size="pageSize"
      :current-page="current"
      @current-change="handleCurrentChange"
      @size-change="handleSizeChange"
      >
    </el-pagination>
  </div>
</template>

<script>
import axios from 'axios'
export default {
  data () {
    return {
      current: 1,
      pageSize: 2,
      userList: [],
      total: 0,
      query: ''
    }
  },
  methods: {
    getUserlist () {
      axios({
        method: 'get',
        url: 'http://localhost:8888/api/private/v1/users',
        params: {
          query: this.query,
          pagenum: this.current,
          pagesize: this.pageSize
        },
        headers: {'Authorization': localStorage.getItem('token')}
      }).then(res => {
        let meta = res.data.meta
        let data = res.data.data
        // console.log(data)
        if (meta.status === 200) {
          // 获取用户列表成功  获取数据
          this.userList = data.users
          this.total = data.total
        }
      })
    },
    handleCurrentChange (val) {
      this.current = val
      this.getUserlist()
    },
    handleSizeChange (val) {
      // console.log(val)
      this.pageSize = val
      this.current = 1
      this.getUserlist()
    },
    search () {
      this.getUserlist()
    }
  },
  created () {
    this.getUserlist()
  }

}
</script>

<style>
.el-breadcrumb {
  height: 48px;
  background: #d4dae0;
  line-height: 48px;
  padding-left: 10px;
  font-size: 18px;
}
.input-with-select {
  margin-top: 10px;
  margin-bottom: 10px;
  width: 300px;
}
.el-pagination{
 margin-top: 10px;
 background: lightsteelblue;
}
</style>
