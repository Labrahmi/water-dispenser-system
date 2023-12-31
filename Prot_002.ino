/*
 * Pin configuration for various modules and sensors:
 * -----------------------------------------------------------------------------------------
 * Module/Sensor   Description                     Arduino Uno   Arduino Mega   Arduino Nano
 * -----------------------------------------------------------------------------------------
 * MFRC522         RST_PIN: 8                      9             5              D9
 *                 SS_PIN: 53                     10            53             D10
 *                 SDA(SS): 10                    10            53             D10
 *                 MOSI: 11 / ICSP-4              11 / ICSP-4   51             D11
 *                 MISO: 12 / ICSP-1              12 / ICSP-1   50             D12
 *                 SCK: 13 / ICSP-3               13 / ICSP-3   52             D13
 * 
 * LEDs            LED_2: 2                       2             -              -
 *                 LED_3: 3                       3             -              -
 *                 LED_4: 4                       4             -              -
 * 
 * TTP223B         TOUCH_PIN_5: 5                 5             -              -
 *                 TOUCH_PIN_6: 6                 6             -              -
 *                 TOUCH_PIN_7: 7                 7             -              -
 * 
 * Ultrasonic      ECHO_PIN: 9                    9             -              -
 * Sensor (HC-SR04) TRIG_PIN: 10                  10            -              -
 * 
 * Relay           RELAY_PIN: 11                  11            -              -
 * 
 * Miscellaneous   MAX_USERS: 10                  -             -              -
 * -----------------------------------------------------------------------------------------
 */

// -------------------------------------------
#include <SPI.h>
#include <MFRC522.h>
#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// -------------------------------------------
#define RST_PIN 8 // MFRC522 RST
#define SS_PIN 53 // MFRC522 SDA
#define LED_2 2 // LED
#define LED_3 3 // LED
#define LED_4 4 // LED
#define TOUCH_PIN_5 5 // TTP223B module
#define TOUCH_PIN_6 6 // TTP223B module
#define TOUCH_PIN_7 7 // TTP223B module
#define ECHO_PIN 9 // capteur ultrason hc-sr04
#define TRIG_PIN 10 // capteur ultrason hc-sr04
#define RELAY_PIN 11 // Relay signal pin
#define MAX_USERS 10 // constant to the max number of users
// -------------------------------------------
typedef struct {
  String uid; // Unique ID
  String description; // description about the card holder
  float balance; // card's balance
} user_t;
// ---------------------
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
user_t users[MAX_USERS]; // Create array of users
// ---------------------
const float fullDistance = 4.5; // distance from the hc-sr04, when water reservoir if full
const float fullQuantity = 100.0; // how much liters in the reservoir when full
const float emptyDistance = 15; // distance from the hc-sr04, when water reservoir if empty
const float emptyQuantity = 0.0; // how much liters in the reservoir when empty
LiquidCrystal_I2C lcd(0x27, 16, 2); // object for the LCD screen, 

// -------------------------------------------

void setup_users() { // setup static users
  for (int i = 0; i < MAX_USERS; i++) {
    if (i == 0) {
      users[i].uid = "C3B873A3"; //
      users[i].description = "White Void Card";
      users[i].balance = 50.0f;
    } else if (i == 1) {
      users[i].uid = "839E660D";
      users[i].description = "Blue Tag";
      users[i].balance = 20.0f;
    } else if (i == 2) {
      users[i].uid = "C43CE422";
      users[i].description = "Youssef's Card";
      users[i].balance = 10.0f;
    } else {
      users[i].uid = "NULL";
      users[i].balance = 0.0f;
    }
  }
}

// -------------------------------------------

float get_balance(String str)
{
  for (int i = 0; i < MAX_USERS; i++)
  {
    if (users[i].uid == str)
    {
      return (users[i].balance);
    }
  }
  return -1;
}

// -------------------------------------------

float get_distance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = (duration * 0.0343) / 2;
  return distance;
}

// -------------------------------------------

float convertToLiters(float distance) {
  // Ensure the distance is within the specified range
  distance = constrain(distance, fullDistance, emptyDistance);
  // Perform linear interpolation
  float slope = (emptyQuantity - fullQuantity) / (emptyDistance - fullDistance);
  float intercept = fullQuantity - slope * fullDistance;
  // Calculate water quantity for the given distance
  float quantity = slope * distance + intercept;
  return quantity;
}

// -------------------------------------------

void decrease_balance(String cardUID, float balance)
{
  for (int i = 0; i < MAX_USERS; i++)
  {
    if (users[i].uid == cardUID)
    {
      users[i].balance -= balance;
    }
  }
}

// -------------------------------------------

void print_card_details(String cardUID)
{
  for (int i = 0; i < MAX_USERS; i++)
  {
    if (users[i].uid == cardUID)
    {
      Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();Serial.println();
      Serial.println("-----------------------------");
      // ------------------
      Serial.print("cardUID: ");
      Serial.println(users[i].uid);
      // --------
      Serial.print("Balance: ");
      Serial.println(users[i].balance);
      // --------
      Serial.print("description: ");
      Serial.println(users[i].description);
      // ------------------
      Serial.println("-----------------------------");
      break;
    }
  }
}

// -------------------------------------------

String get_card_uid()
{
  String cardUID;
  for (byte i = 0; i < mfrc522.uid.size; ++i) {
    char hexBuffer[3];
    sprintf(hexBuffer, "%02X", mfrc522.uid.uidByte[i]);
    cardUID += hexBuffer;
  }
  return cardUID;
}

// -------------------------------------------

void flush_led(int led, int count)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
  }
}

// -------------------------------------------

void setup() {
  pinMode(LED_2, OUTPUT); // led
  pinMode(LED_3, OUTPUT); // led
  pinMode(LED_4, OUTPUT); // led
  pinMode(TOUCH_PIN_5, INPUT); // touch button
  pinMode(TOUCH_PIN_6, INPUT); // touch button
  pinMode(TOUCH_PIN_7, INPUT); // touch button
  pinMode(TRIG_PIN, OUTPUT); // ultrasonic trig
  pinMode(ECHO_PIN, INPUT); // ultrasonic echo
  pinMode(RELAY_PIN, OUTPUT); // relay (pump controller)
  // -------------
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  delay(100); // Optional delay
  // -------------
  setup_users();
}

// -------------------------------------------

bool fill_water(float quantity, int led) {
  Serial.print("The customer has selected: "); // print how much the customer selected
  Serial.print(quantity);
  Serial.println(" litres: ");
  digitalWrite(led, HIGH); // turn on the targeted led [green]
  delay(1000); // wait for a sed befor start filling
  float intial_quantity = convertToLiters(get_distance()); // musure the intial water quantity in the reservoir
  if (intial_quantity >= quantity) { // check if there is enaugh water to serve the client
    while (1) { // yes
      digitalWrite(RELAY_PIN, HIGH); // turn on the relay that controls the pump
      delay(300); // wait a bit
      float new_quantity = convertToLiters(get_distance()); // musure new water quantity in the reservoir
      lcd.clear();
      lcd.print(new_quantity); // print it in LCD screen
      if ((intial_quantity - new_quantity) >= quantity) // condition to stop if requested quantity has been served
        break;
    }
    digitalWrite(RELAY_PIN, LOW); // turn off the relay [pump stops]
  }
  else
    return false; // there's no enaugh water 
  digitalWrite(led, LOW); // turn off green led
  return true;
}

// -------------------------------------------

void handle_filling(String cardUID, float quantity, float price)
{
  if (get_balance(cardUID) >= price) // if the card's balance biger or equal to the required price
  { // yes
    if (fill_water(quantity, LED_2)) // fill the requested quantity, while turning on the green led, and return if the water has been filled successfully?
      decrease_balance(cardUID, quantity); // decrease the balance from the card / executed only when successfully filling the water
  }
  else
  { // no
    Serial.println("Client has no enough Balance on his card!"); // print an error
    flush_led(LED_4, 6); // flush the red LED
  }
}

// -------------------------------------------

void loop() {
  // ------------------------------------------------------------------------
  lcd.clear();
  lcd.print(convertToLiters(get_distance())); // print the water quantity on the reservoir all the time
  // ------------------------------------------------------------------------
  if (!mfrc522.PICC_IsNewCardPresent()) { // no card is present
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) { // no card is present
    return;
  }
  // ------------------------------------------------------------------------
  String cardUID = get_card_uid(); // card is present, filling the card UID [cardUID]
  print_card_details(cardUID); // print all the card Details to the Serial monitor
  // ------------------------------------------------------------------------
  while (1) // loop until user select an option
  {
    flush_led(LED_3, 1); // blue light, user should select an option
    // ------------
    if (digitalRead(TOUCH_PIN_5)) { // [button] first choice: 5 litres, 5Dhs
      handle_filling(cardUID, 5, 5); // logic for filling the water
      break;
    }
    // ------------
    if (digitalRead(TOUCH_PIN_6)) { // [button] second choice: 10 litres, 10Dhs
      handle_filling(cardUID, 10, 10); // logic for filling the water
      break;
    }
    // ------------
    if (digitalRead(TOUCH_PIN_7)) { // [button] third choice: 20 litres, 20Dhs
      handle_filling(cardUID, 20, 20); // logic for filling the water
      break;
    }
    delay(100);
  }
  // ------------------------------------------------------------------------
  delay(300);
}


