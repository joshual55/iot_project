<script setup>
import { supabase } from '@/supabase'
import { ref } from 'vue';
const users = ref([]);
const fetchUsers = async () => {
  const { data, error } = await supabase
  .from('users')
  .select();
  if (error) {
    console.log(error);
  } else {
    console.log(data);
    users.value = data;
  }
};
fetchUsers()
</script>
<template>
  <div class="user-component container mx-auto">
    <div class="user-header my-6">
      <h2 class="text-2xl font-medium">All Users</h2>
      <p>View all the currently registered users</p>
    </div>
    <div class="grid grid-cols-4 border-b py-3">
      <div>
        <h4>First Name</h4>
      </div>
      <div>
        <h4>Last Name</h4>
      </div>
      <div>
        <h4>Phone Number</h4>
      </div>
      <div>
        <h4>Actions</h4>
      </div>
    </div>
    <div v-for="user in users" :key="user">
      <div class="grid grid-cols-4 py-3">
        <div>
          <p>{{ user.first_name }}</p>
        </div>
        <div>
          <p>{{ user.last_name }}</p>
        </div>
        <div>
          <p>{{ user.phone }}</p>
        </div>
        <div>
          <button class="button">Delete</button>
        </div>
      </div>
    </div>
  </div>
</template>