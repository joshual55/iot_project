<script setup>
import Navbar from '@/components/Navbar.vue';
import { supabase } from '@/supabase'
import axios from 'axios';
import { ref, unref } from 'vue';

const firstName = ref('');
const lastName = ref('');
const email = ref('');
const password = ref('');
const phoneNumber = ref('');
const profilePicture = ref('');
const errorMessage = ref('');
const successMessage = ref('');
const faceId = ref('');
const handleCreateUser = async () => {
  if (!unref(firstName) || !unref(lastName) || !unref(phoneNumber)) {
    errorMessage.value = 'Please fill out all fields';
  } else {
    await handleFaceRegonition();
    //
    // Verify that the user has uploaded a picture
    //
    if (!unref(faceId)) {
      errorMessage.value = 'Please upload a profile picture';
      return;
    } else {
      const { data, error } = await supabase.auth.signUp(
        {
          phone: unref(phoneNumber),
          password: unref(password),
        }
      )
      // Update the user's face_id in the public users table.
      const { data: updateData, error: updateError } = await supabase
        .from('users')
        .update({ face_id: unref(faceId), first_name: unref(firstName), last_name: unref(lastName) })

        .eq('id', data.user.id)
      if (updateError) {
        console.error('Error updating user:', updateError);
        return;
      }
      if (error) {
        successMessage.value = '';
        errorMessage.value = error.message;
      } else {
        errorMessage.value = '';
        successMessage.value = 'User created successfully! Faces recognized!';
      }
    }

  }
};

const test2faLogin = async () => {
  const { data, error } = await supabase.auth.signInWithOtp({
    phone: '+14072748119',
  })

}

const handleFileChange = (event) => {
  const file = event.target.files[0];
  if (file) {
    const reader = new FileReader();
    reader.onload = (e) => {
      // remove data:image/jpeg;base64, from the string
      profilePicture.value = e.target.result.replace(/^data:image\/(png|jpg|jpeg);base64,/, '');
      handleFaceRegonition();
    };
    reader.readAsDataURL(file);

  }
};

const handleFaceRegonition = async () => {
  //
  // Send the image to AWS Rekognition and get the FaceId
  //
  axios.post('/api', {
    imgdata: unref(profilePicture),
  })
    .then((res) => {
      if (res.data) {
        faceId.value = res.data.data.FaceRecords[0].Face.FaceId;
      }
    })
};
</script>

<template>
  <div class="dashboard-landing">
    <Navbar />
    <h1 class="text-center text-xl font-bold mb-4 mt-20">Add New User</h1>
    <div class="form-wrapper w-[500px] mx-auto gap-y-6 flex flex-col">
      <div class="grid grid-cols-2 gap-2">
        <div>
          <input type="text"
            placeholder="First Name"
            v-model="firstName" />
          <div class="text-xs">Please enter your first name.</div>
        </div>
        <div>
          <input type="text"
            placeholder="Last Name"
            v-model="lastName" />
          <div class="text-xs">Please enter your last name.</div>
        </div>
      </div>
      <div>
        <input type="text"
          placeholder="Phone Number"
          v-model="phoneNumber" />
        <div class="text-xs">Please enter number with no special characters, and include country code. (e.g 13218008577)</div>

      </div>
      <input type="password"
        placeholder="Password"
        v-model="password" />
      <input type="file"
        @change="handleFileChange"
        accept="image/*" />
      <p v-if="faceId"
        class="text-xs text-green-600 text-right">Upload Successful!</p>
      <button @click="handleCreateUser"
        :class="{ 'cursor-not-allowed': faceId.length < 1 }">Create User</button>
      <p class="text-red-500 text-center">{{ errorMessage }}</p>
      <!-- -->
      <p class="text-green-500 text-center">{{ successMessage }}</p>

    </div>
  </div>
</template>

<style lang="scss" scoped>
.form-wrapper {
  input {
    @apply border border-slate-300 p-2 rounded-lg w-full mb-1;
  }

  button {
    @apply bg-blue-500 hover:bg-blue-600 text-white rounded-lg p-2;
  }
}
</style>
