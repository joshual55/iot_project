<template>
  <div class="navbar-component">
    <h4 class="font-medium text-blue-600">🐊 Node ICU</h4>
    <ul class="navbar-nav">
      <li><router-link to="/">Dashboard</router-link></li>
      <li><router-link to="/">Settings</router-link></li>
      <li class="button"><router-link to="/add-user">Add User</router-link></li>
    </ul>
  </div>
</template>
<style lang="scss" scoped>
.navbar-component {
  @apply border-b border-slate-300 flex justify-between items-center px-4 py-4 shadow-sm;
  .navbar-nav {
    @apply flex;
    li {
      @apply mx-2;
      a {
        @apply text-slate-500 hover:text-slate-500 duration-200;
        &:hover {
          @apply text-orange-500;
        }
      }
    }
  }
}
</style>