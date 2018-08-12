<template>
  <div class="login">
   <el-form ref="form" :rules="rules" status-icon :model="form" label-width="80px">
      <img src="../assets/1.jpg" alt="">
      <el-form-item label="用户名" prop="username">
        <el-input v-model="form.username"></el-input>
      </el-form-item>
      <el-form-item label="密码" prop="password">
        <el-input type="password" v-model="form.password"></el-input>
      </el-form-item>
      <el-form-item>
        <el-button type="primary" @click="login">登录</el-button>
        <el-button @click="reset">重置</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>
<script>
import axios from 'axios'
export default {
  data () {
    return {
      form: {
        username: '',
        password: ''
      },
      rules: {
        username: [
          { required: true, message: '请输入用户名', trigger: 'change' },
          { min: 3, max: 6, message: '长度在 3 到 6 个字符', trigger: 'change' }
        ],
        password: [
          { required: true, message: '请输入密码', trigger: 'change' },
          { min: 6, max: 12, message: '长度在 6 到 12 个字符', trigger: 'change' }
        ]
      }
    }
  },
  methods: {
    login () {
    //  表单校验
      this.$refs.form.validate((valid) => {
        if (valid) {
          axios({
            method: 'post',
            url: 'http://localhost:8888/api/private/v1/login',
            data: this.form
          }).then(res => {
            if (res.data.meta.status === 200) {
              this.$message({
                message: '恭喜你，登录成功',
                type: 'success'
              })
              // 跳转到主页   登录成功,记录token
              let token = res.data.data.token
              localStorage.setItem('token', token)
              this.$router.push('home')
            } else {
              // 消息提示，提示用户名或者密码错误
              this.$message.error('用户名或者密码错误')
            }
          })
        } else {
          return false
        }
      })
    },
    reset () {
      this.$refs.form.resetFields()
    }
  }
}
</script>
<style lang="less">
.login{
  width: 100%;
  height: 100%;
  background: #2d434c;
  overflow: hidden;
  .el-form{
    position: relative;
    width: 400px;
    background: #ffffff;
    margin:200px auto;
    padding:75px 40px 15px 40px;
    border-radius:20px;
  }
  img{
    width: 100px;
    height: 100px;
    border-radius: 50%;
    position: absolute;
    top: -70px;
    left: 50%;
    transform: translateX(-50%);
    padding: 10px;
    background: #fff;
  }
  .el-button + .el-button{
    margin-left: 80px;
  }
}
</style>
