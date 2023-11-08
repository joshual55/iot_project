// Follow this setup guide to integrate the Deno language server with your editor:
// https://deno.land/manual/getting_started/setup_your_environment
// This enables autocomplete, go to definition, etc.

// Download the helper library from https://www.twilio.com/docs/node/install
// Find your Account SID and Auth Token at twilio.com/console
// and set the environment variables. See http://twil.io/secure

const accountSid = process.env.ACCOUNT_SID;
const authToken = process.env.AUTH_TOKEN;
const serviceID = process.env.VERIFY_SERVICE_SID;
const client = require('twilio')(accountSid, authToken);

async function get2FAResponse(phone:string) {
  client.verify.v2.services(serviceID)
    .verifications
    .create({
      to: phone,
      channel: 'sms'})
    .then(verification => {
      console.log(verification.sid);
      const json2FA = JSON.stringify({ status: verification.status, valid: verification.valid});
      return json2FA;
    });
}

// console.log("Hello from Functions!")

// Deno.serve(async (req) => {
//   const { base64string } = await req.json()
//   const data = {
//     message: `Hello ${name}!`,
//   }

//   return new Response(
//     JSON.stringify(data),
//     { headers: { "Content-Type": "application/json" } },
//   )
// })

// To invoke:
// curl -i --location --request POST 'http://127.0.0.1:54321/functions/v1/' \
//   --header 'Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZS1kZW1vIiwicm9sZSI6ImFub24iLCJleHAiOjE5ODM4MTI5OTZ9.CRXP1A7WOeoJeXxjNni43kdQwgnWNReilDMblYTn_I0' \
//   --header 'Content-Type: application/json' \
//   --data '{"name":"Functions"}'
