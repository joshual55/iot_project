// Follow this setup guide to integrate the Deno language server with your editor:
// https://deno.land/manual/getting_started/setup_your_environment
// This enables autocomplete, go to definition, etc.


const postAWSURL = 'https://k9nqp1eh2c.execute-api.us-east-2.amazonaws.com/add-face'
async function sendImageToAWS(image:string) {
  //uses fetch to post the image and get the response back from the AWS API on whether a face was added
  const response = await fetch(postAWSURL, {
    method: 'POST',
    body: JSON.stringify({imgdata: image}),
  });
  const data = await response.json();
  if (data.statusCode === 200) {
    const jsonMessage = JSON.stringify({ message: "Face Added Successfully!!", status: "200" });
    return jsonMessage;
  } else {
    const jsonMessage = JSON.stringify({ message: "Failed to Add Face Data!!", error: "400" });
    return jsonMessage;
  }
}


const getAWSURL = 'https://eo54wpbomc.execute-api.us-east-2.amazonaws.com/facial-search'
async function getRecognitionResponse(image:string) {
  //uses fetch to send an image and check to see if the face matches
  try {
    const response = await fetch(postAWSURL, {
    method: 'POST',
    body: JSON.stringify({imgdata: image}),
  });
  const data = await response.json();

    //If the face was recognized then send the repsonse gotten from the server
    if (data.statusCode === 200) {
      const jsonMessage = JSON.stringify({ message: "Face recognized successfully!!",  status: "200"});
      return jsonMessage;
    } else {
      const jsonMessage = JSON.stringify({ message: "Face recognition failed", status: "404"});
      return jsonMessage;
    }
  } catch (error) {
    console.error('Error fetching response', error.message);
  }
}


// To invoke:
// curl -i --location --request POST 'http://127.0.0.1:54321/functions/v1/' \
//   --header 'Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZS1kZW1vIiwicm9sZSI6ImFub24iLCJleHAiOjE5ODM4MTI5OTZ9.CRXP1A7WOeoJeXxjNni43kdQwgnWNReilDMblYTn_I0' \
//   --header 'Content-Type: application/json' \
//   --data '{"name":"Functions"}'


//GET: https://6i5mkkszhl.execute-api.us-east-2.amazonaws.com/default/facial-search
//POST: https://e69bykvnx9.execute-api.us-east-2.amazonaws.com/default/add-face