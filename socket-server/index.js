require('dotenv').config();

const express = require("express");
const app = express();
const http = require("http");
const server = http.createServer(app);
const { Server } = require("socket.io");
const supabase = require("./supabase");
const io = require("socket.io")(server, {
  cors: {
   origin: "*",
   methods: ["GET", "POST"],
   transports: ["websocket", "polling"],
   credentials: true,
  },
   allowEIO3: true,
  });
// import from node moduels
app.use(express.static(__dirname + "/"));

server.listen(80, () => {
  console.log("listening on *:80");
});

io.on("connection", async (socket) => {
  socket.on("status", async (msg) => {
    console.log('Status: ', msg)
  });
  socket.on("user_authorized", async (msg) => {
    console.log('Unlocking the door!')

    console.log('User: ', JSON.parse(msg).code)
    //
    // Impklement logic to check db and verify user is allowed to unlock door
    //
    const code = JSON.parse(msg).code;
    const phone = JSON.parse(msg).phone;

    const { data, error } = await supabase.auth.verifyOtp({
      phone,
      token: code,
      type: 'sms'
    });
    if (error) {
      console.log('error: ', error);
      return;
    } else {
      io.emit("unlock_door", "Let's unlock the door!");
      console.log(`[unlock-success] - unlocking for ${data.user.id}` );
    }
  });
});
