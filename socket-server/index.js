const express = require("express");
const app = express();
const http = require("http");
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
// import from node moduels
app.use(express.static(__dirname + "/"));

server.listen(3000, () => {
  console.log("listening on *:3000");
});

io.on("connection", async (socket) => {
  socket.on("user_authorized", async () => {
    console.log('Unlocking the door!')
    //
    // Impklement logic to check db and verify user is allowed to unlock door
    //
    io.emit("unlock_door", true);
  });
});
