#include <SoftwareSerial.h>
#include <Adafruit_FONA.h>

// Pin definitions
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4
#define FONA_RI_INTERRUPT 0

int resval = 0;
int respin = A3;  // Analog pin for the water level sensor

// Define the FONA object
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

// Phone numbers and alert message
char PHONE_1[] = "+91 9597933964";
char PHONE_2[] = "+91 6383392630";
char tempalert[] = "Flood Alert";

void setup() {
  Serial.begin(115200);  // Corrected baud rate
  delay(5000);           // Give time for serial monitor to open

  fonaSS.begin(9600);    // Initialize FONA serial
  if (!fona.begin(fonaSS)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);  // Halt the program if FONA is not found
  }

  fona.println("AT+CSMP=17,167,0,0");  // Set SMS text mode parameters
}

void loop() {
  resval = analogRead(respin);  // Read water level sensor value
  Serial.print("Water Level: ");
  Serial.println(resval);

  if (resval > 200) {  // Check if water level exceeds threshold
    Serial.println("Flood Alert");
    make_multi_call();
    send_multi_sms();
  }

  delay(10000);  // Delay to avoid flooding with alerts
}

void send_multi_sms() {
  if (PHONE_1[0] != '\0') {  // Check if PHONE_1 is not empty
    Serial.print("Sending SMS to Phone 1: ");
    Serial.println(PHONE_1);
    fona.sendSMS(PHONE_1, tempalert);
    delay(20000);  // Delay to allow SMS sending
  }

  if (PHONE_2[0] != '\0') {  // Check if PHONE_2 is not empty
    Serial.print("Sending SMS to Phone 2: ");
    Serial.println(PHONE_2);
    fona.sendSMS(PHONE_2, tempalert);
    delay(20000);  // Delay to allow SMS sending
  }
}

void make_multi_call() {
  if (PHONE_1[0] != '\0') {  // Check if PHONE_1 is not empty
    Serial.print("Calling Phone 1: ");
    Serial.println(PHONE_1);
    make_call(PHONE_1);
  }

  if (PHONE_2[0] != '\0') {  // Check if PHONE_2 is not empty
    Serial.print("Calling Phone 2: ");
    Serial.println(PHONE_2);
    make_call(PHONE_2);
  }
}

void make_call(const char* phone) {
  Serial.println("Calling...");
  fona.println("ATD" + String(phone) + ";");  // Dial the number
  delay(20000);  // Allow the call to go through
  fona.println("ATH");  // Hang up the call
  delay(1000);  // Short delay after hanging up
}
