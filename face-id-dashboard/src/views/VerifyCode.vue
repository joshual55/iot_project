<script setup>
import { supabase } from '@/supabase'
import { ref, unref } from 'vue';
const code = ref('');
const handleVerifyCode = async () => {
  const { data, error } = await supabase.auth.verifyOtp({
    phone: '+14072748119',
    token: unref(code),
    type: 'sms'
  })
  if (error) {
    console.log(error);
  } else {
    console.log(data);
  }
}
</script>
<template>

  <div class="verify-code">
    <input v-model="code" type="text" placeholder="Enter code" />
    <button @click="handleVerifyCode">Verify</button>
  </div>
</template>