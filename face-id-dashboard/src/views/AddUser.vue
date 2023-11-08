<script setup>
import Navbar from '@/components/Navbar.vue';
import { supabase } from '@/supabase'
import axios from 'axios';
import { ref, unref } from 'vue';

const firstName = ref('');
const lastName = ref('');
const phoneNumber = ref('');
const profilePicture = ref('');
const errorMessage = ref('');
const successMessage = ref('');
const handleCreateUser = async () => {
  if( !unref(firstName) || !unref(lastName) || !unref(phoneNumber) ) {
    errorMessage.value = 'Please fill out all fields';
  } else {
    const { data, error } = await supabase
    .from('users')
    .insert([
      { first_name: unref(firstName), last_name: unref(lastName), phone: unref(phoneNumber) },
    ])
    .select();
    if (error) {
      successMessage.value = '';
      errorMessage.value = error.message;
    } else {
      errorMessage.value = '';
      successMessage.value = 'User created successfully! Recogizing faces...';
    }
  }
};


const handleFileChange = (event) => {
  const file = event.target.files[0];
  if (file) {
    const reader = new FileReader();
    reader.onload = (e) => {
      profilePicture.value = e.target.result;
    };
    reader.readAsDataURL(file);
  }
};

const handleFaceRegonition = async () => {
  axios.post('/api', {
    imgdata: unref(profilePicture),
  })
  .then((res) => {
    console.log(res);
  })
};
</script>

<template>
  <div class="dashboard-landing">
    <Navbar />
    <h1 class="text-center text-xl font-bold mb-4 mt-20">Add New User</h1>
    <div class="form-wrapper w-[500px] mx-auto gap-y-3 flex flex-col">
      <div class="grid grid-cols-2 gap-2">
        <input type="text"
          placeholder="First Name" v-model="firstName" />
        <input type="text"
          placeholder="Last Name" v-model="lastName"/>
      </div>
      <input type="text"
        placeholder="Phone Number" v-model="phoneNumber" />
      <input type="file"
        @change="handleFileChange"
        accept="image/*" />
      <button @click="handleCreateUser">Create User</button>
      <button @click="handleFaceRegonition">Recognize Face</button>
      <p class="text-red-500 text-center">{{ errorMessage }}</p>
      <p class="text-green-500 text-center">{{ successMessage }}</p>
    </div>
  </div>
</template>

<style lang="scss" scoped>
.form-wrapper {
  input {
    @apply border border-slate-300 p-2 rounded-lg w-full;
  }

  button {
    @apply bg-blue-500 hover:bg-blue-600 text-white rounded-lg p-2;
  }
}</style>
