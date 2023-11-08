<script setup>
import Navbar from '@/components/Navbar.vue';
import AccessLog from  '@/components/AccessLog.vue';
import Users from '@/components/Users.vue';
</script>
<template>
  <div class="dashboard-landing">
    <Navbar />
    <Users class="mb-14"/>
    <AccessLog />
  </div>
</template>