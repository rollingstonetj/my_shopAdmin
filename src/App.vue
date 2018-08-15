<template>
  <div id="app">
    <router-view/>
  </div>
</template>
<script>
export default {
}
</script>
<style>
  *{
    margin: 0;
    padding: 0;
    list-style: none;
  }
  html,
  body{
    height: 100%;
    width: 100%;
  }
  #app{
    width: 100%;
    height: 100%;
  }
  .el-breadcrumb {
  height: 48px;
  background: #d4dae0;
  line-height: 48px;
  padding-left: 10px;
  font-size: 16px;
}
</style>
