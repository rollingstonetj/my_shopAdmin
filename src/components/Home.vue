<template>
<div class="home">
  <el-container>
    <!-- 头部 -->
    <el-header>
      <div class="logo">
      </div>
      <div class="logout">
        欢迎光临
        <a href="javascript:;" @click="logout">退出</a>
      </div>
      <div class="title">
        <h3>电商后台管理系统</h3>
      </div>
    </el-header>
    <el-container>
      <!-- 侧边栏 -->
      <el-aside width="200px">
      <el-menu
      unique-opened
      router
      default-active="2"
      class="el-menu-vertical-demo"
      background-color="#545c64"
      text-color="#fff"
      active-text-color="#ffd04b">
        <el-submenu index="1">
          <template slot="title">
            <i class="el-icon-location"></i>
            <span>用户管理</span>
          </template>
          <el-menu-item index="users">
            <i class="el-icon-menu"></i>
            <span slot="title">用户列表</span>
          </el-menu-item>
        </el-submenu>
        <el-submenu index="">
          <template slot="title">
            <i class="el-icon-location"></i>
            <span>权限管理</span>
          </template>
          <el-menu-item index="roles">
            <i class="el-icon-menu"></i>
            <span slot="title">角色列表</span>
          </el-menu-item>
           <el-menu-item index="rights">
            <i class="el-icon-menu"></i>
            <span slot="title">权限列表</span>
          </el-menu-item>
        </el-submenu>
      </el-menu>
      </el-aside>
      <el-main>
        <router-view></router-view>
      </el-main>
    </el-container>
  </el-container>
</div>
</template>
<script>
export default {
  methods: {
    async logout () {
      // 点击退出，出现消息提示
      try {
        await this.$confirm('您确定要退出系统吗, 是否继续?', '提示', {
          confirmButtonText: '确定',
          cancelButtonText: '取消',
          type: 'warning'
        })
        this.$message({
          type: 'success',
          message: '退出成功!'
        })
        localStorage.removeItem('token')
        // 跳转到登录页
        this.$router.push('login')
      } catch (e) {
        this.$message({
          type: 'info',
          message: '退出取消了'
        })
      }
    }
  }
}
</script>
<style lang="less">
  .home {
    height: 100%;
    .el-container {
      height: 100%;
    }
  }
  .el-header{
    background: hsl(208, 21%, 75%);
    height: 60px;
    .logo {
      width: 180px;
      height: 60px;
      background: url(../assets/logo.png) no-repeat center center;
      background-size: contain;
      float: left;
    }
    .title {
      font-size: 24px;
      font-weight: 700;
      color: #fff;
      height: 60px;
      line-height: 60px;
      text-align: center;
      overflow: hidden;
    }
    .logout {
      float: right;
      width: 180px;
      height: 60px;
      line-height: 60px;
      font-weight: 700;
      font-size: 16px;
      a{
        color: #b07a17;
        padding-left: 5px;
      }
    }
  }
  .el-aside{
    background: #545c64;
  }
  .el-main{
    background: #eaeef1;
  }
  .el-menu{
    width: 200px;
  }
</style>
