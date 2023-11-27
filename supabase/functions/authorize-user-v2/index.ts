// Follow this setup guide to integrate the Deno language server with your editor:
// https://deno.land/manual/getting_started/setup_your_environment
// This enables autocomplete, go to definition, etc.

console.log("Hello from Functions!")
const searchFaceEndpoint = 'https://eo54wpbomc.execute-api.us-east-2.amazonaws.com/facial-search'

Deno.serve(async (req) => {
  try {
    const { imgdata } = await req.json();

    const requestData = JSON.stringify({ imgdata: imgdata });

    const response = await fetch(searchFaceEndpoint, {
      method: 'POST',
      body: requestData,
      headers: {
        'Content-Type': 'application/json'
      }
    });

    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }

    const responseJSON = await response.json();
    //
    // TODO: Use supabse SDK to take responseJSON.data.FaceMatches[0].FaceId and find user in database
    //
    return new Response(
      JSON.stringify(responseJSON.data.FaceMatches),
      { headers: { "Content-Type": "application/json" } },
    );
  } catch (error) {
    return new Response(
      JSON.stringify({ error: error.message }),
      { status: 500, headers: { "Content-Type": "application/json" } }
    );
  }
})

// To invoke:
// curl -i --location --request POST 'http://127.0.0.1:54321/functions/v1/' \
//   --header 'Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZS1kZW1vIiwicm9sZSI6ImFub24iLCJleHAiOjE5ODM4MTI5OTZ9.CRXP1A7WOeoJeXxjNni43kdQwgnWNReilDMblYTn_I0' \
//   --header 'Content-Type: application/json' \
//   --data '{"name":"Functions"}'
