<template>
  <div class="categories">
    <!-- 面包屑 -->
    <el-breadcrumb separator-class="el-icon-arrow-right">
      <el-breadcrumb-item :to="{ path: '/' }">首页</el-breadcrumb-item>
      <el-breadcrumb-item>商品管理</el-breadcrumb-item>
      <el-breadcrumb-item>分类列表</el-breadcrumb-item>
    </el-breadcrumb>
    <!-- 添加分类按钮 -->
    <el-button type="success" plain class="mt_10 mb_10" @click="showAddDialog">添加分类</el-button>
    <!-- 添加表格插件 -->
    <!-- 设置加载动画效果 -->
    <el-table
      :data="catList"
      v-loading="loading"
      style="width: 100%">
      <el-table-tree-column
        prop="cat_name"
        label="分类名称"
        child-key="children"
        :indent-size="20"
        tree-key="cat_id"
        level-key="cat_level"
        parent-key="cat_pid">
      </el-table-tree-column>
      <el-table-column
        prop="cat_deleted"
        label="是否有效">
        <template slot-scope="scope">
          <span v-if="scope.row.cat_deleted">无效</span>
          <span v-else>有效</span>
        </template>
      </el-table-column>
      <el-table-column
        prop="cat_level"
        label="排序">
      </el-table-column>
       <el-table-column
        prop=""
        label="操作">
        <template slot-scope="scope">
          <el-button type="primary" icon="el-icon-edit" size="small" plain @click="showEditDialog(scope.row)"></el-button>
          <el-button type="danger" icon="el-icon-delete" size="small" plain @click="delCat(scope.row)"></el-button>
        </template>
      </el-table-column>
    </el-table>
    <!-- 添加分类的对话框 -->
     <el-dialog
      title="添加分类"
      :visible.sync="addDialogVisible"
      width="40%"
      >
      <el-form :model="addForm" :rules="rules" ref="addForm" label-width="80px">
        <el-form-item label="分类名称" prop="cat_name">
          <el-input v-model="addForm.cat_name"></el-input>
        </el-form-item>
        <el-form-item label="父级名称" prop="cat_pid">
          <!-- 挖坑  级联插件-->
          <el-cascader
            :options="options"
            change-on-select
            clearable
            :props="props"
            v-model="addForm.cat_pid"
          ></el-cascader>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="addDialogVisible = false">取 消</el-button>
        <el-button type="primary" @click="addCat">确 定</el-button>
      </span>
    </el-dialog>
    <!-- 编辑分类的对话框 -->
    <el-dialog
      title="编辑分类"
      :visible.sync="editDialogVisible"
      width="40%"
      >
      <el-form :model="editForm" :rules="rules" ref="editForm" label-width="80px">
        <el-form-item label="分类名称" prop="cat_name">
          <el-input v-model="editForm.cat_name"></el-input>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="editDialogVisible = false">取 消</el-button>
        <el-button type="primary" @click="editCat">确 定</el-button>
      </span>
    </el-dialog>
    <!-- 分页功能 -->
    <el-pagination
      @size-change="handleSizeChange"
      @current-change="handleCurrentChange"
      :current-page="this.current"
      :page-sizes="[10, 20, 30, 40]"
      :page-size="100"
      layout="total, sizes, prev, pager, next, jumper"
      :total="total">
    </el-pagination>
  </div>
</template>

<script>
import ElTreeGrid from 'element-tree-grid'
// console.log(ElTreeGrid.name)  // el-table-tree-column
// import Vue from 'vue'
// Vue.component('el-table-tree-column',ElTreeGrid)
export default {
  data () {
    return {
      catList: [],
      current: 1,
      pageSize: 10,
      total: 0,
      loading: true,
      addDialogVisible: false,
      addForm: {
        cat_name: '',
        cat_pid: []
      },
      props: {
        value: 'cat_id',
        label: 'cat_name',
        children: 'children'
      },
      rules: {
        cat_name: [
          {required: true, message: '请输入分类名称', trigger: 'blur'}
        ]
      },
      options: [],
      editForm: {
        cat_name: '',
        cat_id: ''
      },
      editDialogVisible: false
    }
  },
  methods: {
    async getCatList () {
      // 发送ajax,获取数据
      const res = await this.axios.get('categories/', {
        params: {
          type: 3,
          pagenum: this.current, // 当前页码
          pagesize: this.pageSize // 当前页的条数
        }
      })
      let {meta, data} = res.data
      if (meta.status === 200) {
        this.loading = false
        this.catList = data.result
        this.total = data.total
        // console.log(data)
      }
    },
    handleSizeChange (val) {
      console.log(val)
      this.loading = true
      this.pageSize = val
      // 始终先跳到第一页
      this.current = 1
      this.getCatList()
    },
    handleCurrentChange (val) {
      // 修改当前页
      this.loading = true
      this.current = val
      // 重新渲染
      this.getCatList()
    },
    // 显示添加分类的对话框
    async showAddDialog () {
      this.addDialogVisible = true
      // 获取options中的数据  只需要两级数据
      const res = await this.axios.get('categories/?type=2')
      let {meta, data} = res.data
      // console.log(data)
      if (meta.status === 200) {
        this.options = data
      }
    },
    // 添加分类
    addCat () {
      // 校验表单
      this.$refs.addForm.validate(async valid => {
        if (valid) {
          // 校验成功
          // 发送ajax请求
          const {cat_name: catName, cat_pid: catPid} = this.addForm
          // console.log(catName, catPid)
          const res = await this.axios.post('categories', {
            // 注意，如果不给0，一级分类添加不上
            cat_pid: catPid[catPid.length - 1] || 0,
            cat_name: catName,
            cat_level: catPid.length
          })
          let {meta} = res.data
          // console.log(meta, data)
          if (meta.status === 201) {
            // 对话框隐藏
            // 重新渲染
            // 提示消息
            this.addDialogVisible = false
            // 重置表单
            this.$refs.addForm.resetFields()
            if (catPid.length === 0) {
              this.total++
              this.current = Math.ceil(this.total / this.pageSize)
            }
            this.getCatList()
            this.$message.success('创建角色成功')
          }
        } else {
          return false
        }
      })
    },
    // 删除分类
    async delCat (cat) {
      // console.log(cat_id)
      // 发送ajax请求
      try {
        await this.$confirm('此操作将永久删除此角色, 是否继续?', '提示', {
          confirmButtonText: '确定',
          cancelButtonText: '取消',
          type: 'warning'
        })
        const res = await this.axios.delete(`categories/${cat.cat_id}`)
        let {meta} = res.data
        // console.log(meta)
        if (meta.status === 200) {
          // 重新渲染 提示信息
          if (this.catList.length === 1) {
            this.current--
          }
          this.getCatList()
          this.$message.success('删除分类成功')
        }
        this.$message({
          type: 'success',
          message: '删除成功!'
        })
      } catch (error) {
        this.$message.error('删除角色失败')
      }
    },
    showEditDialog (cat) {
      this.editDialogVisible = true
      // 分类名称回显
      // console.log(cat)
      this.editForm.cat_name = cat.cat_name
      this.editForm.cat_id = cat.cat_id
    },
    // 编辑分类
    editCat () {
      // 表单校验
      this.$refs.editForm.validate(async valid => {
        if (!valid) {
          return false
        }
        // 发送Ajax请求
        // console.log(this.editForm.cat_id)
        const res = await this.axios.put(`categories/${this.editForm.cat_id}`, this.editForm)
        let {meta} = res.data
        // console.log(meta, data)
        if (meta.status === 200) {
          // 对话框隐藏 重新渲染 提示消息
          this.editDialogVisible = false
          this.getCatList()
          this.$message.success('更新角色成功')
        }
      })
    }
  },
  created () {
    this.getCatList()
  },
  components: {
    [ElTreeGrid.name]: ElTreeGrid
  }
}
</script>

<style>
.mt_10{
  margin-top: 10px;
}
.mb_10{
  margin-bottom: 10px;
}
</style>
