<template>
  <div class="add">
      <!-- 面包屑 -->
    <el-breadcrumb separator-class="el-icon-arrow-right">
      <el-breadcrumb-item :to="{ path: '/' }">首页</el-breadcrumb-item>
      <el-breadcrumb-item>商品管理</el-breadcrumb-item>
      <el-breadcrumb-item>商品列表</el-breadcrumb-item>
    </el-breadcrumb>
    <el-button type="primary" class="mt_10 mb_10" @click="$router.push('goods-add')">添加商品</el-button>
    <!-- 表格 -->
    <el-table :data="goodList">
      <el-table-column type="index" width="50"  :index="indexMethod"></el-table-column>
      <el-table-column label="商品名称" prop="goods_name"></el-table-column>
      <el-table-column label="商品价格" prop="goods_price"></el-table-column>
      <el-table-column label="商品重量" prop="goods_weight"></el-table-column>
      <el-table-column label="创建时间" prop="add_time">
        <template slot-scope="scope">
          {{scope.row.add_time | dateFilter}}
        </template>
      </el-table-column>
      <el-table-column label="操作">
        <template slot-scope="scope">
          <el-button type="primary" icon="el-icon-edit" size="small" plain ></el-button>
          <el-button type="danger" icon="el-icon-delete" size="small" plain></el-button>
        </template>
      </el-table-column>
    </el-table>
    <!-- 分页 -->
     <el-pagination
      @size-change="handleSizeChange"
      @current-change="handleCurrentChange"
      :current-page="current"
      :page-sizes="[10, 20, 30, 40]"
      :page-size="pageSize"
      layout="total, sizes, prev, pager, next, jumper"
      :total="total">
    </el-pagination>
  </div>
</template>

<script>
export default {
  data () {
    return {
      goodList: [],
      query: '',
      current: 1,
      pageSize: 10,
      total: 0
    }
  },
  methods: {
    async getGoodList () {
      const res = await this.axios.get('goods', {
        params: {
          query: this.query,
          pagenum: this.current,
          pagesize: this.pageSize
        }
      })
      let {meta, data} = res.data
      // console.log(meta, data)
      if (meta.status === 200) {
        this.goodList = data.goods
        this.total = data.total
      }
    },
    indexMethod (index) {
      return (this.current - 1) * this.pageSize + index + 1
    },
    handleSizeChange (val) {
      this.pageSize = val
      this.current = 1
      this.getGoodList()
    },
    handleCurrentChange (val) {
      this.current = val
      this.getGoodList()
    }
  },
  created () {
    this.getGoodList()
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
