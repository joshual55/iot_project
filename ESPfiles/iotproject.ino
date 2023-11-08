#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"
#include <MyBase64.h>

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
const char* ssid = "2000-2004@Gainesville_place";
const char* password = "TwJK4b7XZ";

// Replace with your server information
const char* server = "jsonplaceholder.typicode.com";
const int serverPort = 443; // HTTPs default port

// Replace with your API endpoint
const char* apiEndpoint = "/your-api-endpoint";

// Pin number for the button
const int buttonPin = 15;

// Camera settings
camera_config_t config;
camera_fb_t* fb = NULL;
bool capturePhotoFlag = false;

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);

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

void loop() {
  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    // Button is pressed, set the flag to capture a photo
    Serial.println("Button Pressed");
    capturePhotoFlag = true;
  }

  // Check if the flag is set to capture a photo
  if (capturePhotoFlag) {
    capturePhoto();
    Serial.println(Photo2Base64());
    capturePhotoFlag = false; // Reset the flag
  }

  // Delay for some time to prevent continuous captures
  //delay(1000); // Adjust the delay time as needed
}

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

    return imageFile;
}


String urlencode(String str) {
  const char *msg = str.c_str();
  const char *hex = "0123456789ABCDEF";
  String encodedMsg = "";
  while (*msg != '\0') {
    if (('a' <= *msg && *msg <= 'z') || ('A' <= *msg && *msg <= 'Z') || ('0' <= *msg && *msg <= '9') || *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~') {
      encodedMsg += *msg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[(unsigned char)*msg >> 4];
      encodedMsg += hex[*msg & 0xf];
    }
    msg++;
  }
  return encodedMsg;
}

