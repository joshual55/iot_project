<script setup>
import Navbar from '@/components/Navbar.vue';
import { supabase } from '@/supabase'
import { ref } from 'vue';
import { io } from 'socket.io-client';

const code = ref('');
const phone = ref('');
const socketURL = 'http://ec2-18-118-47-118.us-east-2.compute.amazonaws.com';
let socket;

const handleVerifyCode = () => {
  // Create a Socket.IO connection
  socket = io(socketURL);

  // Handle connection
  socket.on('connect', () => {
    console.log('Connected to Socket.IO server');

    // Emit the user_authorized event
    socket.emit('user_authorized', JSON.stringify({ code: code.value, phone: phone.value }));
  });

  // Handle other events
  socket.on('message', (data) => {
    console.log('Message from server:', data);
  });

  socket.on('disconnect', () => {
    console.log('Disconnected from Socket.IO server');
  });

  socket.on('connect_error', (error) => {
    console.error('Connection error:', error);
  });
}
</script>


<template>
  <div class="verify-code">
    <Navbar />
    <div class="form-wrapper">
      <div>
        <div class="text-xs text-slate-500 mb-2">Please enter your phone number</div>
        <input v-model="phone"
          type="text"
          placeholder="Enter phone number (e.g 13218008577)" />
      </div>
      <div>
        <div class="text-xs text-slate-500 mb-2">Please enter your 2FA Code</div>
        <input v-model="code"
          type="text"
          placeholder="Enter code" />
      </div>
      <button @click="handleVerifyCode">Unlock Door</button>
    </div>
  </div>
</template>
<style lang="scss" scoped>
input {
  @apply border border-slate-300 p-2 rounded-lg w-full mb-4;
}

button {
  @apply bg-blue-500 hover:bg-blue-600 text-white rounded-lg p-2 w-full;

}

.form-wrapper {
  max-width: 300px;
  @apply mx-auto mt-20;
}
</style>