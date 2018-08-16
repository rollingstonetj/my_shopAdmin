<template>
  <div class="rights">
    <!-- 面包屑 -->
    <el-breadcrumb separator-class="el-icon-arrow-right">
      <el-breadcrumb-item :to="{ path: '/' }">首页</el-breadcrumb-item>
      <el-breadcrumb-item>权限管理</el-breadcrumb-item>
      <el-breadcrumb-item>权限列表</el-breadcrumb-item>
    </el-breadcrumb>
      <!-- 表格组件 -->
   <el-table
    ref="singleTable"
    :data="rightsList"
    style="width: 100%">
    <el-table-column
      type="index"
      width="50"
      :index="indexMethod">
    </el-table-column>
    <el-table-column
      property="authName"
      label="权限名称"
      width="180">
    </el-table-column>
    <el-table-column
      property="path"
      label="路径"
      width="180">
    </el-table-column>
    <el-table-column
      property="level"
      label="层级">
      <template slot-scope="scope">
        <span v-if="scope.row.level === '0'">一级</span>
        <span v-else-if="scope.row.level === '1'">二级</span>
        <span v-else>三级</span>
      </template>
    </el-table-column>
  </el-table>
  </div>
</template>

<script>
export default {
  data () {
    return {
      rightsList: []
    }
  },
  methods: {
    // 获取权限列表
    async getRightsList () {
      const res = await this.axios.get('rights/list')
      let {data, meta} = res.data
      if (meta.status === 200) {
        this.rightsList = data
      }
    },
    indexMethod (index) {
      return index * 1
    }
  },
  created () {
    this.getRightsList()
  }
}
</script>

<style>

</style>
