<script lang="ts" setup>
import { ref } from 'vue';
const email = ref('');
const password = ref('');
const handleLoginSubmit = () => {
  console.log('[handle-login-submit]', email.value, password.value);
  // TOOD: Handle form validation
}
</script>
<template>
  <div class="login-component">
    <h2 class="text-center text-2xl font-medium">Login</h2>
    <h4 class="text-center">Login to manage your Face ID lock</h4>
    <form class="auth-form">
      <input type="text" placeholder="Email" v-model="email" />
      <input type="password" placeholder="Password" v-model="password" />
      <button @click="handleLoginSubmit">Login</button>
    </form>
  </div>
</template>
<style lang="scss" scoped>
.login-component {
  @apply mt-24;
}
.auth-form {
  width: 400px;
  @apply mx-auto mt-8;
  input {
    @apply px-2 rounded-lg mb-4 border-solid border-slate-400 border w-full py-2 duration-200;
    &:focus {
      @apply outline-none border-orange-500;
    }
  }
  button {
    @apply bg-blue-500 text-white rounded-lg px-4 py-2 w-full duration-200;
    &:hover {
      @apply bg-orange-600;
    }
  }
}
</style>