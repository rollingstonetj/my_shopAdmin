<template>
  <div class="addGoods">
    <el-steps :active="active" finish-status="success">
      <el-step title="步骤 1" description="基本信息"></el-step>
      <el-step title="步骤 2" description="商品图片"></el-step>
      <el-step title="步骤 3" description="商品内容"></el-step>
    </el-steps>
    <!-- tab栏切换 -->
    <el-tabs :tab-position="tabPosition" v-model="activeName" @tab-click="handleClick">
    <el-tab-pane label="用户管理" name="basic">
        <el-form ref="addForm" :model="addForm" :rules="rules">
        <el-form-item label="商品名称" label-width="80px" prop="goods_name">
          <el-input v-model="addForm.goods_name"></el-input>
        </el-form-item>
        <el-form-item label="商品价格"  label-width="80px" prop="goods_price">
          <el-input v-model="addForm.goods_price"></el-input>
        </el-form-item>
        <el-form-item label="商品重量"  label-width="80px" prop="goods_weight">
          <el-input v-model="addForm.goods_weight"></el-input>
        </el-form-item>
        <el-form-item label="商品数量"  label-width="80px" prop="goods_number">
          <el-input v-model="addForm.goods_number"></el-input>
        </el-form-item>
         <el-form-item label="商品分类"  label-width="80px">
          <!-- 级联 -->
          <el-cascader
            :options="options"
            v-model="addForm.goods_cat"
            :props="props"
            @change="handleChange">
          </el-cascader>
        </el-form-item>
        <el-form-item label="是否促销"  label-width="80px">
          <template>
            <el-radio v-model="radio" :label="true">是</el-radio>
            <el-radio v-model="radio" :label="false">否</el-radio>
          </template>
        </el-form-item>
        <el-form-item label-width="80px">
          <el-button type="primary" @click="next(active,'pic')">下一步</el-button>
        </el-form-item>
      </el-form>
    </el-tab-pane>
    <el-tab-pane label="配置管理" name="pic">
      <el-upload
        class="upload-demo"
        action="http://localhost:8888/api/private/v1/upload"
        :on-success="handleSuccess"
        :on-remove="handleRemove"
        list-type="picture"
        :headers="headers">
        <el-button size="small" type="primary">点击上传</el-button>
        <div slot="tip" class="el-upload__tip">只能上传jpg/png文件，且不超过500kb</div>
      </el-upload>
      <el-button type="primary" @click="next(active,'content')">下一步</el-button>
    </el-tab-pane>
    <el-tab-pane label="商品内容" name="content">
        <quill-editor v-model="addForm.goods_introduce"></quill-editor>
        <el-button type="primary" @click="addGood" class="mt_10 mb_10">确定</el-button>
    </el-tab-pane>
  </el-tabs>
  </div>
</template>

<script>
export default {
  data () {
    return {
      active: 0,
      tabPosition: 'left',
      activeName: 'basic',
      addForm: {
        goods_name: '',
        goods_price: '',
        goods_weight: '',
        goods_number: '',
        goods_cat: [],
        pics: [],
        goods_introduce: ''
      },
      options: [],
      props: {
        label: 'cat_name',
        value: 'cat_id'
      },
      radio: true,
      // 用于给图片上传组件的ajax设置token
      headers: {
        Authorization: localStorage.getItem('token')
      },
      rules: {
        goods_name: [
          {required: true, message: '请输入商品名称', trigger: 'blur'}
        ],
        goods_price: [
          {required: true, message: '请输入商品价格', trigger: 'blur'}
        ],
        goods_weight: [
          {required: true, message: '请输入商品重量', trigger: 'blur'}
        ],
        goods_number: [
          {required: true, message: '请输入商品数量', trigger: 'blur'}
        ]
      }
    }
  },
  methods: {
    handleClick (tab, event) {
      // console.log(tab, event)
      // 点击tab栏，让步骤条跟着联动
      this.active = +tab.index
    },
    next (active, activeName) {
      this.active = 1
      this.activeName = activeName
      if (this.active++ > 2) this.active = 0
    },
    handleChange (value) {
      // console.log(value)
      this.addForm.goods_cat = value
    },
    // 上传成功的钩子函数
    handleSuccess (res, file, fileList) {
      // console.log(res, file)
      if (res.meta.status === 200) {
        this.addForm.pics.push({pic: res.data.tmp_path})
        console.log(this.addForm.pics)
      }
    },
    handleRemove (file, fileList) {
      // console.log(file, fileList)
      let tmpPath = file.response.data.tmp_path
      let idx = this.addForm.pics.findIndex(item => item.pic === tmpPath)
      // console.log(idx)
      this.addForm.pics.splice(idx, 1)
    },
    addGood () {
      // 校验
      this.$refs.addForm.validate(async valid => {
        if (valid) {
          // 发送ajax
          const res = await this.axios.post('goods', {
            ...this.addForm,
            goods_cat: this.addForm.goods_cat.join()
          })
          let {meta, data} = res.data
          console.log(meta, data)
          if (meta.status === 201) {
            this.$router.push('goods')
            this.$message.success('添加商品成功')
          }
        } else {
          return false
        }
      })
    }
  },
  async created () {
    // 发送ajax,获取所有分类数据
    const res = await this.axios.get('categories/?type=3')
    let {meta, data} = res.data
    // console.log(meta, data)
    if (meta.status === 200) {
      this.options = data
    }
  }
}
</script>

<style>
.el-upload-list {
  min-height: 100px;
}
.ql-editor {
  min-height: 200px;
}
.quill-editor {
  background: #fff;
}
.mt_10{
  margin-top: 10px;
}
.mt_10 {
  margin-bottom: 10px;
}
</style>
