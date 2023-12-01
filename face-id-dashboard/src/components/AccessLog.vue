<script setup>
import { supabase } from '@/supabase'
import { ref } from 'vue';
const entryLogs = ref([]);
const fetchEntryLogs = async () => {

  const { data, error } = await supabase
    .from('entry_history')
    .select('*')

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
    <div class="grid grid-cols-3 border-b py-3">
      <div>
        <h4>Entry Time</h4>
      </div>
      <div>
        <h4>Name</h4>
      </div>
      <div>
        <h4>Status</h4>
      </div>
    </div>
    <div v-for="entryLog in entryLogs"
      :key="entryLog.id"
      class="grid grid-cols-3 py-3">
      <div>
        <p>{{ entryLog.entry_time }}</p>
      </div>
      <div>
        <p>{{ entryLog.first_name + " " +  entryLog.last_name }}</p>
      </div>
      <div>
        <p v-if="entryLog.entry_status == 'authorized'" class="text-green-400">Authorized</p>
        <p v-else class="text-red-400">Unauthorized</p>
      </div>
    </div>
  </div>
</template>