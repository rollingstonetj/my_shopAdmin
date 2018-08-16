<template>
  <div class="roles">
    <!-- 面包屑 -->
    <el-breadcrumb separator-class="el-icon-arrow-right">
      <el-breadcrumb-item :to="{ path: '/' }">首页</el-breadcrumb-item>
      <el-breadcrumb-item>权限管理</el-breadcrumb-item>
      <el-breadcrumb-item>角色列表</el-breadcrumb-item>
    </el-breadcrumb>
    <!-- 添加用户 -->
    <el-button type="success" plain class="mr_10 mt_10" @click="showAddDialog">添加角色</el-button>
    <!-- 添加用户的对话框 -->
    <el-dialog
      title="添加角色"
      :visible.sync="addDialogVisible"
      width="40%"
      >
      <el-form :model="addForm" :rules="rules" ref="addForm" label-width="80px">
        <el-form-item label="角色名称" prop="roleName">
          <el-input v-model="addForm.roleName"></el-input>
        </el-form-item>
        <el-form-item label="角色描述" prop="roleDesc">
          <el-input v-model="addForm.roleDesc" @keyup.enter.native="addRole"></el-input>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="addDialogVisible = false">取 消</el-button>
        <el-button type="primary" @click="addRole">确 定</el-button>
      </span>
    </el-dialog>
    <!-- 编辑用户的对话框 -->
    <el-dialog
      title="修改角色"
      :visible.sync="editDialogVisible"
      width="40%"
      >
      <el-form :model="editForm" :rules="rules" ref="editForm" label-width="80px">
        <el-form-item label="角色名称" prop="roleName">
          <el-input v-model="editForm.roleName"></el-input>
        </el-form-item>
        <el-form-item label="角色描述" prop="roleDesc">
          <el-input v-model="editForm.roleDesc" @keyup.enter.native="editRole"></el-input>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="editDialogVisible = false">取 消</el-button>
        <el-button type="primary" @click="editRole">确 定</el-button>
      </span>
    </el-dialog>
    <!-- 分配权限的对话框 -->
     <!-- node-key="id" 每个树节点用来作为唯一标识的属性，整棵树应该是唯一的 可以通过id来设置默认选中-->
    <el-dialog
      title="分配权限"
      :visible.sync="assignDialogVisible"
      width="40%"
      >
      <el-tree
        :data="rightList"
        show-checkbox
        default-expand-all
        node-key="id"
        ref="tree"
        highlight-current
        :props="defaultProps">
      </el-tree>
      <span slot="footer" class="dialog-footer">
        <el-button @click="assignDialogVisible = false">取 消</el-button>
        <el-button type="primary" @click="assignRight">确 定</el-button>
      </span>
    </el-dialog>
    <!-- 表格控件 -->
   <el-table
      :data="rolesList"
      style="width: 100%">
      <el-table-column type="expand">
        <template slot-scope="scope">
          <el-row class="dataTips" v-if="scope.row.children.length === 0">暂无权限</el-row>
          <!-- 一级权限 -->
          <el-row v-for="level1 in scope.row.children" :key="level1.id" class="firstLevel">
            <el-col :span="4">
              <el-tag closable  @close="deleteRight(scope.row,level1.id)">{{level1.authName}}</el-tag>
              <span class="el-icon-arrow-right"></span>
            </el-col>
            <el-col :span="20">
              <!-- 二级权限 -->
              <el-row :span="4" v-for="level2 in level1.children" :key="level2.id" class="secLevel">
                <el-col :span="4">
                  <el-tag type="success" closable @close="deleteRight(scope.row,level2.id)">{{level2.authName}}</el-tag>
                  <span class="el-icon-arrow-right"></span>
                </el-col>
                <el-col :span="20">
                  <!-- 三级权限 -->
                  <el-tag v-for="level3 in level2.children" :key="level3.id" closable type="warning" class="thirdLevel" @close="deleteRight(scope.row,level3.id)">{{level3.authName}}</el-tag>
                </el-col>
              </el-row>
            </el-col>
          </el-row>
        </template>
      </el-table-column>
      <el-table-column
        label="#"
        prop="id" width="100px"
        type="index"
        :index="indexMethod">
      </el-table-column>
      <el-table-column
        label="角色名称"
        width="200px"
        prop="roleName">
      </el-table-column>
      <el-table-column
        label="描述"
        prop="roleDesc"
        width="200px">
      </el-table-column>
      <el-table-column
        label="操作"
        prop="desc"
        >
        <template slot-scope="scope">
          <el-button type="primary" icon="el-icon-edit" size="small" plain @click="showEditDialog(scope.row)"></el-button>
          <el-button type="danger" icon="el-icon-delete" size="small" plain @click="deleteRole(scope.row)"></el-button>
          <el-button type="success" icon="el-icon-check" size="small" plain @click="showAssignRight(scope.row)">分配权限</el-button>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
export default {
  data () {
    return {
      rolesList: [],
      tags: [],
      addForm: {
        roleName: '',
        roleDesc: ''
      },
      editForm: {},
      editDialogVisible: false,
      addDialogVisible: false,
      assignDialogVisible: false,
      rules: {
        roleName: [
          {required: true, message: '角色名称不能为空', trigger: 'blur'}
        ]
      },
      rightList: [],
      defaultProps: {
        children: 'children',
        label: 'authName'
      },
      assignRoleId: ''
    }
  },
  methods: {
    // 获取角色列表
    async getRolesList () {
      const res = await this.axios.get('/roles')
      let {data, meta} = res.data
      if (meta.status === 200) {
        this.rolesList = data
      }
    },
    indexMethod (index) {
      return index * 1
    },
    // 显示对话框
    showAddDialog () {
      this.addDialogVisible = true
    },
    // 添加角色
    addRole () {
      // 表单校验
      this.$refs.addForm.validate(async valid => {
        if (valid) {
          // 发送ajax
          const res = await this.axios.post('roles', this.addForm)
          let {meta} = res.data
          // console.log(meta, data)
          if (meta.status === 201) {
            // 关闭对话框 重置表单 重新渲染 提示信息
            this.addDialogVisible = false
            this.$refs.addForm.resetFields()
            this.getRolesList()
            this.$message.success('添加角色成功')
          }
        } else {
          return false
        }
      })
    },
    // 删除角色
    async deleteRole (role) {
      // 显示messageBox
      try {
        await this.$confirm('你确定要删除此角色吗, 是否继续?', '提示', {
          confirmButtonText: '确定',
          cancelButtonText: '取消',
          type: 'warning'
        })
        const res = await this.axios.delete(`roles/${role.id}`)
        let {meta} = res.data
        // console.log(meta)
        if (meta.status === 200) {
          // 重新渲染 提示用户
          this.getRolesList()
          this.$message.success('删除角色成功')
        }
      } catch (error) {
        this.$message({
          type: 'info',
          message: '已取消删除'
        })
      }
    },
    showEditDialog (role) {
      this.editDialogVisible = true
      // 数据回显
      this.editForm = role
    },
    // 编辑角色
    editRole () {
      // 校验
      this.$refs.editForm.validate(async valid => {
        if (valid) {
          // 发送ajax
          const res = await this.axios.put(`roles/${this.editForm.id}`, this.editForm)
          let {meta} = res.data
          // console.log(meta)
          if (meta.status === 200) {
            // 对话框隐藏 清除表单 渲染当前页 提示信息
            this.editDialogVisible = false
            this.$refs.editForm.resetFields()
            this.getRolesList()
            this.$message.success('修改成功')
          }
        } else {
          return false
        }
      })
    },
    // 删除角色指定权限
    async deleteRight (role, rightId) {
      // console.log(role)
      // 发送ajax请求
      const res = await this.axios.delete(`roles/${role.id}/rights/${rightId}`)
      let {meta, data} = res.data
      if (meta.status === 200) {
        // 把删除后的权限重新赋值给role.children
        role.children = data
        // 提示消息
        this.$message.success('删除成功')
      }
    },
    // 获取所有权限，渲染页面，并将每个角色对应的权限显示出来
    async showAssignRight (role) {
      this.assignDialogVisible = true
      // console.log(role)
      // 发送ajax
      this.assignRoleId = role.id // 需要分配权限的角色的id
      const res = await this.axios.get('rights/tree')
      let {meta, data} = res.data
      // console.log(res.data)
      if (meta.status === 200) {
        this.rightList = data // 获取所有权限并显示
        // 获取到当前角色下的所有三级权限，并渲染到页面中
        const checkedKeys = []
        role.children.forEach(level1 => {
          level1.children.forEach(level2 => {
            level2.children.forEach(level3 => {
              // console.log(level3.id)
              checkedKeys.push(level3.id)
            })
          })
        })
        // console.log(checkedKeys)
        // 设置默认选中
        this.$refs.tree.setCheckedKeys(checkedKeys)
      }
    },
    async assignRight () {
      // 获取要设置权限的角色id
      // console.log(this.assignRoleId)
      // 获取选中的权限id
      let checkedKeys = this.$refs.tree.getCheckedKeys()
      // 获取选中的半选权限id  若不获取半选权限id，则选中状态会渲染不上去，因为最外层的权限没有选中，只选中内部权限，也相当于没有选中
      let halfCheckedKeys = this.$refs.tree.getHalfCheckedKeys()
      // console.log(checkedKeys, halfCheckedKeys)
      // 发送ajax请求
      const res = await this.axios.post(`roles/${this.assignRoleId}/rights`, {
        rids: [...checkedKeys, ...halfCheckedKeys].join()
      })
      let {meta} = res.data
      if (meta.status === 200) {
        // 关闭对话框 重新渲染  提示信息
        this.assignDialogVisible = false
        this.getRolesList()
        this.$message.success('权限修改成功')
      }
    }
  },
  created () {
    this.getRolesList()
  }
}
</script>

<style>
.firstLevel {
  border-bottom: 1px dotted #ccc;
  padding: 10px 0 0px 0;
}
.secLevel {
  margin-bottom: 8px;
}
.thirdLevel {
  margin-right: 5px;
}
.mr_10{
  margin-bottom: 10px;
}
.mt_10{
  margin-top: 10px;
}
</style>
