<template>
  <div class="users">
    <!-- 面包屑 -->
    <el-breadcrumb separator-class="el-icon-arrow-right">
      <el-breadcrumb-item :to="{ path: '/' }">首页</el-breadcrumb-item>
      <el-breadcrumb-item>用户列表</el-breadcrumb-item>
      <el-breadcrumb-item>用户管理</el-breadcrumb-item>
    </el-breadcrumb>
    <!-- 搜索框 -->
      <el-input placeholder="请输入内容" v-model="query" class="input-with-select" @keyup.enter.native="search">
        <el-button slot="append" icon="el-icon-search" @click="search"></el-button>
      </el-input>
      <el-button type="success" @click="showAddDialog">添加用户</el-button>
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
              inactive-color="#ff4949"
              @change="changeState(scope.row)">
            </el-switch>
          </template>
      </el-table-column>
        <el-table-column
        prop=""
        label="操作">
         <template slot-scope="scope">
           <el-button type="primary" icon="el-icon-edit" size="small" plain @click="showEditDialog(scope.row)"></el-button>
           <el-button type="danger" icon="el-icon-delete" size="small" plain @click="deleteUser(scope.row)"></el-button>
           <el-button type="success" icon="el-icon-check" size="small" plain @click="showAssignDialog">分配角色</el-button>
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
    <!-- 添加用户的对话框插件 -->
   <el-dialog
    title="添加用户"
    :visible.sync="addDialogVisible"
    width="40%"
    >
    <el-form :model="addForm" :rules="rules" ref="addForm" label-width="80px">
      <el-form-item label="用户名" prop="username">
        <el-input v-model="addForm.username"></el-input>
      </el-form-item>
       <el-form-item label="密码" prop="password">
        <el-input v-model="addForm.password"  @keyup.enter.native="addUser"></el-input>
      </el-form-item>
       <el-form-item label="邮箱" prop="email">
        <el-input v-model="addForm.email"></el-input>
      </el-form-item>
       <el-form-item label="手机号" prop="mobile">
        <el-input v-model="addForm.mobile" @keyup.enter.native="addUser"></el-input>
      </el-form-item>
    </el-form>
    <span slot="footer" class="dialog-footer">
      <el-button @click="addDialogVisible = false">取 消</el-button>
      <el-button type="primary" @click="addUser">确 定</el-button>
    </span>
  </el-dialog>
  <!-- 编辑用户的对话框插件 -->
    <el-dialog
    title="修改用户"
    :visible.sync="editDialogVisible"
    width="40%"
    >
    <el-form :model="editForm" :rules="rules" ref="editForm" label-width="80px">
      <el-form-item label="用户名">
        <el-tag type="info">{{this.editForm.username}}</el-tag>
      </el-form-item>
       <el-form-item label="邮箱" prop="email">
        <el-input v-model="editForm.email"></el-input>
      </el-form-item>
       <el-form-item label="手机号" prop="mobile">
        <el-input v-model="editForm.mobile" @keyup.enter.native="editUser"></el-input>
      </el-form-item>
    </el-form>
    <span slot="footer" class="dialog-footer">
      <el-button @click="editDialogVisible = false">取 消</el-button>
      <el-button type="primary" @click="editUser">确 定</el-button>
    </span>
  </el-dialog>
  <!-- 显示分配角色的对话框 -->
  <el-dialog :v-model="assignForm" label-width="80px">
    <span slot="footer" class="dialog-footer">
      <el-button @click="assignDialogVisible = false">取 消</el-button>
      <el-button type="primary" @click="editUser">确 定</el-button>
    </span>
  </el-dialog>>
  </div>
</template>

<script>
// import axios from 'axios'
export default {
  data () {
    return {
      current: 1,
      pageSize: 2,
      userList: [],
      total: 0,
      query: '',
      addDialogVisible: false,
      editDialogVisible: false,
      addForm: {
        username: '',
        password: '',
        mobile: '',
        email: '',
        id: ''
      },
      editForm: {
        username: '',
        mobile: '',
        email: '',
        id: ''
      },
      rules: {
        username: [
          { required: true, message: '请输入用户名', trigger: 'blur' },
          { min: 3, max: 6, message: '长度在 3 到 6 个字符', trigger: 'blur' }
        ],
        password: [
          { required: true, message: '请输入密码', trigger: 'blur' },
          { min: 6, max: 12, message: '长度在 6 到 12 个字符', trigger: 'blur' }
        ],
        email: [
          { type: 'email', message: '请输入正确的邮箱格式', trigger: 'blur' }
        ],
        mobile: [
          { pattern: /^1\d{10}$/, message: '请输入正确的手机格式', trigger: 'blur' }
        ]
      },
      assignForm:[],
      assignDialogVisible: false
    }
  },
  methods: {
    async getUserlist () {
      const res = await this.axios({
        method: 'get',
        url: 'users',
        params: {
          query: this.query,
          pagenum: this.current,
          pagesize: this.pageSize
        }
      })
      let {meta, data} = res.data
      if (meta.status === 200) {
        // 获取用户列表成功  获取数据
        this.userList = data.users
        this.total = data.total
      }
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
      this.current = 1
      this.getUserlist()
    },
    async changeState (user) {
      let res = await this.axios.put(`users/${user.id}/state/${user.mg_state}`)
      let {status} = res.data.meta
      if (status === 200) {
        this.$message.success('修改用户状态成功')
      } else {
        this.$message.error('修改用户状态失败')
      }
    },
    async deleteUser (user) {
      await this.$confirm('你确定要删除此用户吗?, 是否继续?', '提示', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      })
      // 发送ajax请求
      try {
        const res = await this.axios.delete(`users/${user.id}`)
        let {status} = res.data.meta
        if (status === 200) {
          if (this.userList.length === 1) {
            this.current--
          }
          // 重新渲染当前页
          this.getUserlist()
          this.$message.success('删除用户成功')
        } else {
          this.$message.error('删除用户失败')
        }
      } catch (error) {
        this.$message({
          type: 'info',
          message: '已取消删除'
        })
      }
    },
    showAddDialog () {
      // 让对话框显示
      this.addDialogVisible = true
    },
    addUser () {
      // 校验表单
      this.$refs.addForm.validate(async (valid) => {
        if (valid) {
          // 发送ajax请求
          const res = await this.axios.post('users', this.addForm)
          this.addForm.id = res.data.id
          let {status} = res.data.meta
          if (status === 201) {
            // console.log(123)
            // 关闭对话框，重置表单，重新渲染当前页==>始终渲染最后一页
            this.addDialogVisible = false
            this.$refs.addForm.resetFields()
            this.total++
            this.current = Math.ceil(this.total / this.pageSize)
            this.getUserlist()
          } else {
            this.$message.error(res.data.meta.msg)
          }
        } else {
          return false
        }
      })
    },
    showEditDialog (user) {
      this.editDialogVisible = true
      this.editForm.username = user.username
      this.editForm.mobile = user.mobile
      this.editForm.email = user.email
      this.editForm.id = user.id
    },
    editUser () {
      // 进行表单校验
      this.$refs.editForm.validate(async (valid) => {
        if (valid) {
          // console.log('校验成功')
          // 发送ajax请求，更新数据
          const res = await this.axios.put(`users/${this.editForm.id}`, this.editForm)
          let {status} = res.data.meta
          if (status === 200) {
            // 1.重新渲染当前页
            // 2.隐藏对话框
            // 3.提示修改成功
            this.getUserlist()
            this.editDialogVisible = false
            this.$message.success('更新成功')
          } else {
            this.$message.error('更新失败')
          }
        } else {
          console.log('校验失败')
          return false
        }
      })
    },
    // 显示分配角色的对话框
    showAssignDialog () {
      this.assignDialogVisible = true
    }
  },
  created () {
    this.getUserlist()
  }
}
</script>

<style>
.input-with-select {
  margin-top: 10px;
  margin-bottom: 10px;
  width: 300px;
}
.el-pagination{
 margin-top: 10px;
 height: 35px;
 padding-top: 9px;
 background: lightsteelblue;
}
</style>
