#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"
#include <MyBase64.h>
#include <WiFiClientSecure.h>

#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22


// Replace with your network credentials
const char* ssid = "Verizon XT1585 2611";
const char* password = "sh@wn0153";

// Replace with your API endpoint
const char* apiEndpoint = "https://aoymgietyhxxhklhhvxw.supabase.co/functions/v1/authorize-user-v2";

// Pin number for the button
const int buttonPin = 15;

// Camera settings
camera_config_t config;
camera_fb_t* fb = NULL;
bool capturePhotoFlag = false;

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Initialize the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 40;
  config.fb_count = 1;

  // Init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 40;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.println("Camera init failed with error");
    return;
  }

  // Configure button pin
  pinMode(buttonPin, INPUT_PULLUP);
}

bool captureInProgress = false;

void loop() {
  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW && !captureInProgress) {
    Serial.println("Button Pressed");
    captureInProgress = true;
    capturePhoto(); // Capture a new photo
  }

  if (captureInProgress && fb != NULL) {
    // Convert the captured photo to base64
    String base64Image = Photo2Base64();

    // Send the base64 image to the server
    if (!base64Image.isEmpty()) {
      sendImageToServer(base64Image);
    }

    // Reset capture flag and release the frame buffer
    captureInProgress = false;
    esp_camera_fb_return(fb);
    fb = NULL;
  }

  // Delay for some time to prevent continuous captures
  delay(100); // Adjust the delay time as needed
}

  // Check if the flag is set to capture a photo
  //if (capturePhotoFlag) {
    
    //capturePhotoFlag = false; // Reset the flag
  //}

  // Delay for some time to prevent continuous captures
  //delay(1000); // Adjust the delay time as needed

void capturePhoto() {
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
  } else {
    Serial.println("Image captured successfully");
  }

  // Encode the captured image as base64
  // Release the frame buffer
  esp_camera_fb_return(fb);

  // Send a POST request with the base64 image data
}


String Photo2Base64() {
    camera_fb_t * fb = esp_camera_fb_get();
    if(!fb) {
      Serial.println("Camera capture failed");
      return "";
    }

    String imageFile = "data:image/jpeg;base64,";

    // Calculate the size of the base64 encoded string
    int outputLength = base64_enc_len(fb->len);
    char *output = (char *)malloc(outputLength + 1); // +1 for null terminator

    // Perform base64 encoding
    base64_encode(output, (char *)fb->buf, fb->len);

    // Append the encoded data to the image file string
    imageFile += String(output);

    // Free the allocated memory for the output buffer
    free(output);

    esp_camera_fb_return(fb);

    Serial.println(imageFile);
    return imageFile;
}

void sendImageToServer(String base64Image) {
  WiFiClientSecure client;
  HTTPClient http;
  http.begin(client, apiEndpoint);
  client.setInsecure();
  // Set the content type and the base64 image as the request body
  http.addHeader("Content-Type", "application/json");
  
  // Add authorization header (replace "YourTokenHere" with your actual token)
  http.addHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFveW1naWV0eWh4eGhrbGhodnh3Iiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY5OTMwMTQ5MCwiZXhwIjoyMDE0ODc3NDkwfQ.ta7C-rii70wvEvEfYTXgRUuKawZe9m-LBHVd-vZw3XU");

  int httpResponseCode = http.POST("{\"imgdata\":\"" + base64Image + "\"}");

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    String response = http.getString();
    Serial.println("Server response: " + response);
  } else {
    Serial.print("HTTP Error: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}