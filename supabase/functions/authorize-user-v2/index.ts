// Follow this setup guide to integrate the Deno language server with your editor:
// https://deno.land/manual/getting_started/setup_your_environment
// This enables autocomplete, go to definition, etc.
import { createClient } from "https://esm.sh/@supabase/supabase-js@2";

const searchFaceEndpoint =
  "https://eo54wpbomc.execute-api.us-east-2.amazonaws.com/facial-search";

Deno.serve(async (req) => {
  try {
    const { imgdata } = await req.json();

    const requestData = JSON.stringify({ imgdata: imgdata });

    const response = await fetch(searchFaceEndpoint, {
      method: "POST",
      body: requestData,
      headers: {
        "Content-Type": "application/json",
      },
    });

    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }

    const responseJSON = await response.json();
    //
    // Use supabse SDK to take responseJSON.data.FaceMatches[0].FaceId and find user in database
    //
    const supabaseClient = createClient(
      Deno.env.get("SUPABASE_URL") ?? "",
      Deno.env.get("SUPABASE_ANON_KEY") ?? "",
      {
        global: {
          headers: { Authorization: req.headers.get("Authorization")! },
        },
      }
    );

    // Database queries will have RLS policies enforced
    const { data, error } = await supabaseClient.from("users").select("*").eq('face_id', responseJSON.data.FaceMatches[0].Face.FaceId);
    
    //
    // TODO: Implement handler to send messsage via SMS
    //

    let responseObject = {
      face: responseJSON.data.FaceMatches[0],
      user: data, // Make sure 'data' is defined and accessible here
      message_sent: null // Initialize with a default value
    };
    
    const signIn = async (number) => {
      try {
        let { data, error } = await supabaseClient.auth.signInWithOtp({
          phone: number
        });
    
        if (error) throw error;
        
        // If sign in is successful, return a success message
        return { success: true, message: 'OTP sent successfully' };
      } catch (error) {
        console.error(error);
        // If there is an error, return the error
        return { success: false, message: error.message };
      }
    }
    
    signIn(data[0].phone).then((response) => {
      // Update responseObject within the .then block
      responseObject.message_sent = response.message;
      
    });

    //
    // TODO: Implement response that will determine if user is authorized or not.
    //
    return new Response(JSON.stringify(responseObject), {
      headers: { "Content-Type": "application/json" },
    });

  } catch (error) {
    return new Response(JSON.stringify({ error: error.message }), {
      status: 500,
      headers: { "Content-Type": "application/json" },
    });
  }
});

// To invoke:
// curl -i --location --request POST 'http://127.0.0.1:54321/functions/v1/' \
//   --header 'Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZS1kZW1vIiwicm9sZSI6ImFub24iLCJleHAiOjE5ODM4MTI5OTZ9.CRXP1A7WOeoJeXxjNni43kdQwgnWNReilDMblYTn_I0' \
//   --header 'Content-Type: application/json' \
//   --data '{"name":"Functions"}'
