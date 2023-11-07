#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"

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
const char* ssid = "***";
const char* password = "***";

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

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

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
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
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
  if (digitalRead(buttonPin) == LOW) {
    // Button is pressed, capture a photo
    capturePhoto();
  }
}

void capturePhoto() {
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
  } else {
    Serial.println("Image captured successfully");
  }

  // Encode the captured image as base64
  String base64Image = base64Encode(fb->buf, fb->len);

  // Release the frame buffer
  esp_camera_fb_return(fb);

  // Send a POST request with the base64 image data
  sendImageToServer(base64Image);
}


String base64Encode(uint8_t *data, size_t len) {
  const char* base64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String base64 = "";

  for (size_t i = 0; i < len; i += 3) {
    uint32_t octet_a = i < len ? data[i] : 0;
    uint32_t octet_b = (i + 1) < len ? data[i + 1] : 0;
    uint32_t octet_c = (i + 2) < len ? data[i + 2] : 0;

    base64 += base64chars[(octet_a >> 2) & 0x3F];
    base64 += base64chars[((octet_a << 4) | (octet_b >> 4)) & 0x3F];
    base64 += base64chars[((octet_b << 2) | (octet_c >> 6)) & 0x3F];
    base64 += base64chars[octet_c & 0x3F];
  }

  return base64;
}

void sendImageToServer(String base64Image) {
  HTTPClient http;
  http.begin(server, serverPort, apiEndpoint);

  // Set the content type and the base64 image as the request body
  http.addHeader("Content-Type", "application/json");
  String json = "{\"image\":\"" + base64Image + "\"}";

  int httpResponseCode = http.POST(json);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("HTTP Error");
  }

  http.end();
  delay(5000); // Delay before capturing another photo
}
