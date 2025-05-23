#include <WiFi.h>
#include <WebServer.h>

#define r_pwm 33
#define l_pwm 32
#define RF 13
#define LF 26
#define RB 12
#define LB 27

#define BUZZER 25
#define FRONT_LIGHT 14
#define REAR_LIGHT 15

int speed = 150;

const char* ssid = "RoboCar_AP";
const char* password = "12345678";

WebServer server(80);

// === Fungsi Stop ===
void stopC() {
  digitalWrite(RF, LOW);
  digitalWrite(LF, LOW);
  digitalWrite(RB, LOW);
  digitalWrite(LB, LOW);
  analogWrite(l_pwm, 0);
  analogWrite(r_pwm, 0);
}

// === Setup Webserver ===
void setupServer() {
  // Interface Web
  server.on("/", []() {
    server.send(200, "text/html",
      "<h2>RoboCar Controller</h2>"
      "<button onclick=\"fetch('/forward')\">Maju</button> "
      "<button onclick=\"fetch('/backward')\">Mundur</button><br><br>"
      "<button onclick=\"fetch('/left')\">Kiri</button> "
      "<button onclick=\"fetch('/right')\">Kanan</button><br><br>"
      "<button onclick=\"fetch('/stop')\">Stop</button><br><br>"
      "<button onclick=\"fetch('/buzzer/on')\">Buzzer ON</button> "
      "<button onclick=\"fetch('/buzzer/off')\">Buzzer OFF</button><br><br>"
      "<button onclick=\"fetch('/light/front/on')\">Lampu Depan ON</button> "
      "<button onclick=\"fetch('/light/front/off')\">Lampu Depan OFF</button><br><br>"
      "<button onclick=\"fetch('/light/rear/on')\">Lampu Belakang ON</button> "
      "<button onclick=\"fetch('/light/rear/off')\">Lampu Belakang OFF</button>"
    );
  });

  // Kontrol arah
  server.on("/forward", []() {
    digitalWrite(RF, HIGH); digitalWrite(LF, HIGH);
    digitalWrite(RB, LOW);  digitalWrite(LB, LOW);
    analogWrite(r_pwm, speed);
    analogWrite(l_pwm, speed);
    server.send(200, "text/plain", "Forward");
  });

  server.on("/backward", []() {
    digitalWrite(RF, LOW); digitalWrite(LF, LOW);
    digitalWrite(RB, HIGH); digitalWrite(LB, HIGH);
    analogWrite(r_pwm, speed);
    analogWrite(l_pwm, speed);
    server.send(200, "text/plain", "Backward");
  });

  server.on("/left", []() {
    digitalWrite(RF, HIGH); digitalWrite(LF, HIGH);
    digitalWrite(RB, LOW);  digitalWrite(LB, LOW);
    analogWrite(r_pwm, 255);
    analogWrite(l_pwm, 150);
    server.send(200, "text/plain", "Left");
  });

  server.on("/right", []() {
    digitalWrite(RF, HIGH); digitalWrite(LF, HIGH);
    digitalWrite(RB, LOW);  digitalWrite(LB, LOW);
    analogWrite(r_pwm, 150);
    analogWrite(l_pwm, 255);
    server.send(200, "text/plain", "Right");
  });

  server.on("/stop", []() {
    stopC();
    server.send(200, "text/plain", "Stopped");
  });

  // Buzzer
  server.on("/buzzer/on", []() {
    digitalWrite(BUZZER, HIGH);
    server.send(200, "text/plain", "Buzzer ON");
  });

  server.on("/buzzer/off", []() {
    digitalWrite(BUZZER, LOW);
    server.send(200, "text/plain", "Buzzer OFF");
  });

  // Lampu Depan
  server.on("/light/front/on", []() {
    digitalWrite(FRONT_LIGHT, HIGH);
    server.send(200, "text/plain", "Front Light ON");
  });

  server.on("/light/front/off", []() {
    digitalWrite(FRONT_LIGHT, LOW);
    server.send(200, "text/plain", "Front Light OFF");
  });

  // Lampu Belakang
  server.on("/light/rear/on", []() {
    digitalWrite(REAR_LIGHT, HIGH);
    server.send(200, "text/plain", "Rear Light ON");
  });

  server.on("/light/rear/off", []() {
    digitalWrite(REAR_LIGHT, LOW);
    server.send(200, "text/plain", "Rear Light OFF");
  });

  server.begin();
}

// === Setup Utama ===
void setup() {
  Serial.begin(115200);

  // Pin Mode
  pinMode(RF, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(r_pwm, OUTPUT);
  pinMode(l_pwm, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(FRONT_LIGHT, OUTPUT);
  pinMode(REAR_LIGHT, OUTPUT);

  // Inisialisasi kondisi awal
  stopC();
  digitalWrite(BUZZER, LOW);
  digitalWrite(FRONT_LIGHT, LOW);
  digitalWrite(REAR_LIGHT, LOW);

  // Jalankan WiFi AP
  WiFi.softAP(ssid, password);
  Serial.println("WiFi Access Point started");
  Serial.println(WiFi.softAPIP());

  setupServer();
}

// === Loop ===
void loop() {
  server.handleClient(); // tangani permintaan web
}
