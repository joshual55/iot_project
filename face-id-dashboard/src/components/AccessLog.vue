<script setup>
import { supabase } from '@/supabase'
import { ref } from 'vue';
const entryLogs = ref([]);
const fetchEntryLogs = async () => {
  const { data, error } = await supabase
  .from('entry_history')
  .select();
  if (error) {
    console.log(error);
  } else {
    console.log(data);
    entryLogs.value = data;
  }
};
fetchEntryLogs()
</script>
<template>
  <div class="access-log-component container mx-auto">
    <div class="user-header my-6">
      <h2 class="text-2xl font-medium">Entry History</h2>
      <p>Access to door lock history</p>
    </div>
    <div class="grid grid-cols-5 border-b py-3">
      <div>
        <h4>Entry Time</h4>
      </div>
      <div>
        <h4>User ID</h4>
      </div>
      <div>
        <h4>Unlocked</h4>
      </div>
      <div>
        <h4>Two Factor Success</h4>
      </div>
      <div>
        <h4>Face Recognized</h4>
      </div>
    </div>
  </div>
</template>