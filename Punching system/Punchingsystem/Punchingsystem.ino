#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 8  // Define the LED pin

MFRC522 mfrc522(SS_PIN, RST_PIN);

String tagNames[][2] = {
    {"D1 6F 4D 24", "Person1"},
    {"E0 D7 4A 1F", "Person2"},
    {"05 36 C5 2B 16 B0 C1", "Person3"},
    {"05 34 6D CF 08 B0 C1","Person4"}
    // Add more tag names and IDs as needed
};
int numTags = sizeof(tagNames) / sizeof(tagNames[0]);

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    pinMode(LED_PIN, OUTPUT);  // Initialize the LED pin as an output
    Serial.println("Put your card to the reader...");
    Serial.println();
}

void loop() {
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent()) {
        delay(500); // Add a delay to avoid multiple detections in quick succession
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        delay(500); // Add a delay to avoid multiple detections in quick succession
        return;
    }

    Serial.print("UID tag :");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();

    bool accessGranted = false;
    String tagName = "";

    for (int i = 0; i < numTags; i++) {
        if (content.substring(1) == tagNames[i][0]) {
            Serial.println("Authorized access");
            Serial.println();
            accessGranted = true;
            tagName = tagNames[i][1];
            digitalWrite(LED_PIN, HIGH);  // Turn on the LED
            break;
        }
    }

    if (accessGranted) {
        Serial.println("Access granted");

        // Print timestamp and tag name
        Serial.print("Timestamp: ");
        Serial.print(millis()); // Add a timestamp to the log
        Serial.print(" - Person: "); 
        Serial.println(tagName);

        delay(1000); // Keep the LED on for 1 seconds
        digitalWrite(LED_PIN, LOW);  // Turn off the LED
    } else {
        Serial.println("Access denied");
        // Add your access denied logic here

        delay(500); // Delay before checking for a new card
    }
}
