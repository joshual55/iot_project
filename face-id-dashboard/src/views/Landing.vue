<template>
  <div class="landing flex flex-col pt-20">
    <h1 class="text-center text-4xl mb-4">🐊</h1>
    <h2 class="text-2xl font-medium mb-4">Welcome to Node ICU</h2>
    <p class="mb-6">Node ICU is a Node.js and Vue.js application that allows you to control a door lock with your face and 2FA.</p>
    <h4>What would you like to do?</h4>
    <div class="flex flex-col mx-auto">
      <router-link to="/dashboard"
        class="button">View Dashboard</router-link>
      <router-link to="/add-user"
        class="button">Add User</router-link>
      <router-link to="/verify-code"
        class="button">Verify Code</router-link>
    </div>
  </div>
</template>
<style lang="scss" scoped>
h2 {
  @apply text-center text-blue-500;
}

p {
  @apply text-center text-slate-500;
}

h4 {
  @apply text-center text-orange-500;
}

.button {
  width: 200px;
  @apply bg-blue-600 text-blue-100 rounded-md py-2 px-4 my-3 text-center;
}
</style>