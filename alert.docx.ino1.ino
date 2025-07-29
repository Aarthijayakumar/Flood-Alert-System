#include <SoftwareSerial.h>
#include <Adafruit_FONA.h>

// Pin definitions
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

int resval = 0;
int respin = A3;  // Analog pin for water level sensor

// FONA object
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

// Phone numbers and alert message
char PHONE_1[] = "+919597933964";
char PHONE_2[] = "+916383392630";
char tempalert[] = "Flood Alert";

void setup() {
  Serial.begin(115200);
  delay(5000);

  fonaSS.begin(9600);
  if (!fona.begin(fonaSS)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

  fona.println("AT+CSMP=17,167,0,0");  // SMS config
}

void loop() {
  resval = analogRead(respin);
  Serial.print("Water Level: ");
  Serial.println(resval);

  if (resval > 200) {
    Serial.println("Flood Alert Triggered!");
    make_multi_call();
    send_multi_sms();
    delay(60000);  // Delay 1 min to prevent spamming
  }

  delay(5000);  // Regular sensor polling every 5 sec
}

void send_multi_sms() {
  if (PHONE_1[0] != '\0') {
    Serial.print("Sending SMS to Phone 1: ");
    Serial.println(PHONE_1);
    fona.sendSMS(PHONE_1, tempalert);
    delay(20000);
  }

  if (PHONE_2[0] != '\0') {
    Serial.print("Sending SMS to Phone 2: ");
    Serial.println(PHONE_2);
    fona.sendSMS(PHONE_2, tempalert);
    delay(20000);
  }
}

void make_multi_call() {
  if (PHONE_1[0] != '\0') {
    Serial.print("Calling Phone 1: ");
    Serial.println(PHONE_1);
    make_call(PHONE_1);
  }

  if (PHONE_2[0] != '\0') {
    Serial.print("Calling Phone 2: ");
    Serial.println(PHONE_2);
    make_call(PHONE_2);
  }
}

void make_call(const char* phone) {
  Serial.println("Calling...");
  fona.print("ATD");
  fona.print(phone);
  fona.println(";");
  delay(20000);
  fona.println("ATH");
  delay(1000);
}
