import {
    assert,
    assertExists,
    assertEquals,
  } from 'https://deno.land/std@0.192.0/testing/asserts.ts'
import { createClient, SupabaseClient } from 'https://esm.sh/@supabase/supabase-js@2.23.0'
import { delay } from 'https://deno.land/x/delay@v0.2.0/mod.ts'

// Set up the configuration for the Supabase client
const supabaseUrl = Deno.env.get('SUPABASE_URL') ?? ''
const supabaseKey = Deno.env.get('SUPABASE_ANON_KEY') ?? ''
const options = {
  auth: {
    autoRefreshToken: false,
    persistSession: false,
    detectSessionInUrl: false,
  },
}

// Test the creation and functionality of the Supabase client
const testsendImageToAWS = async () => {
  var client: SupabaseClient = createClient(supabaseUrl, supabaseKey, options)

  // Verify if the Supabase URL and key are provided
  if (!supabaseUrl) throw new Error('supabaseUrl is required.')
  if (!supabaseKey) throw new Error('supabaseKey is required.')

  // Test a simple query to the database
  const { data: func_data, error: func_error } = await client.functions.invoke('json-to-aws', {
    body: { image: 'put image here'}
  })
  if (func_error) {
    throw new Error('Invalid response: ' + func_error.message)
  }
  assertEquals(func_data.message, 'Face Added Successfully!!')
}

const testgetRecognitionResponse = async () => {
    var client: SupabaseClient = createClient(supabaseUrl, supabaseKey, options)
  
    // Verify if the Supabase URL and key are provided
    if (!supabaseUrl) throw new Error('supabaseUrl is required.')
    if (!supabaseKey) throw new Error('supabaseKey is required.')
  
    // Test a simple query to the database
    const { data: func_data, error: func_error } = await client.functions.invoke('json-to-aws', {
      body: { image: 'put image here'}
    })
    if (func_error) {
      throw new Error('Invalid response: ' + func_error.message)
    }
    assertEquals(func_data.message, 'Face recognized successfully!!')
  }

DelayNode.test('Getting 2FA Response', testsendImageToAWS)
DelayNode.test('Getting 2FA Response', testgetRecognitionResponse)

