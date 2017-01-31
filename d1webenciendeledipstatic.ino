
#include <ESP8266WiFi.h>


IPAddress arduino_ip ( 192,  168,   1,  99);
IPAddress dns_ip     (  8,   8,   8,   8);
IPAddress gateway_ip ( 192,  168,   1,   1);
IPAddress subnet_mask(255, 255, 255,   0);

const char* ssid = "ssid";
const char* password = "password";







int ledPin = 2; // GPIO2
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);


  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Conexión a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Conectado a ");
  Serial.println(ssid);


   WiFi.config(arduino_ip, gateway_ip, subnet_mask);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");

  // Start the server
  server.begin();
  Serial.println("Servidor inicializado ");

  // Print the IP address
  Serial.print("URL: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("nuevo cliente");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, LOW);
    value = LOW;
  }



  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html><head><meta http-equiv='Content-Type' content='text/html; charset=utf-8' /></head>");

  client.print("Led está: ");

  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\">Encender LED</a> <br>");
  client.println("<a href=\"/LED=OFF\">Apagar LED</a><br>");
  client.println("</html>");

  delay(1);
  Serial.println("Cliente desconectado");
  Serial.println("");

}
