/* 
Name: Elliott Starosta
Date: January 19, 2024
Description: Summative Code of RFID reader food items anaylsis 
*/

// Libaries
#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <SPI.h>

// Define pin assignments
#define JOYSTICK_X_PIN A0
#define JOYSTICK_BUTTON_PIN 8

// RFID
#define RFID_SS_PIN 10
#define RFID_RST_PIN 8

// Define LCD pins
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define RFID reader
MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);

// Menu items
const char* menuItems[] = { "Scan", "Add", "Delete", "Analyze Meal", "RESET MEAL"};

// Define RFID related constants
const int RFID_LENGTH = 11;
const int MAX_FOODS = 20;

// Defining return food array
String returnFoodsArray[MAX_FOODS];
int returnFoodsCount = 0;

// Dict struct for Food items
struct Food {
  char id[RFID_LENGTH];
  String name;
};
// Initalize foods
Food foods[MAX_FOODS];
int foodsCount = 0;

// Current position in the menu
int currentPosition = 0;

// Joystick variables
int initialJoystickXValue = 0;
int lastJoystickXValue = 0;

// Item not scanned but then gets scanned bool
bool hasBeenScanned = false;

// Vars for delete func
bool isDone = false;
int arrayRemoveIndex;

// Var to split sentence response
const int maxParts = 5; 


void setup() {
  // Initialize LCD and Serial communication
  lcd.begin(16, 2);
  Serial.begin(9600);

  // Set up RFID sensor
  SPI.begin();
  mfrc522.PCD_Init();
  delay(20);

  // Set up joystick button pin as input with pull-up resistor
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);

  // Read initial joystick position
  initialJoystickXValue = analogRead(JOYSTICK_X_PIN);
  lastJoystickXValue = initialJoystickXValue;

  // Initial display of the menu
  mainMenu();
}

void loop() {

  // Constantly reading joystick
  int joystickX = analogRead(JOYSTICK_X_PIN);

  // Check for joystick movement and update menu accordingly
  if (joystickX < 412 && currentPosition > 0 && lastJoystickXValue >= 412) {
    currentPosition--;
    mainMenu();
  } else if (joystickX > 612 && currentPosition < 4 && lastJoystickXValue <= 612) {
    currentPosition++;
    mainMenu();
  }

  // Check for button press
  if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
    executeMenuItem();
    delay(200);  // Debounce delay
  }
  // Update last joystick value
  lastJoystickXValue = joystickX;
}

// Update the displayed menu on the LCD (main menu function)
void mainMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> " + String(menuItems[currentPosition])); // Display menu item on first row

  // If there's another item, display it on the second row
  if (currentPosition < 4) {
    lcd.setCursor(0, 1);
    lcd.print(menuItems[currentPosition + 1]); 
  }
}

// Execute the action associated with the selected menu item
void executeMenuItem() {
  lcd.clear();

  if (currentPosition == 0) {  // Scan option
    lcd.print("Scan Tag...");
    // Wait to get UID
    String uid = waitForRFID();
    
    // Allows for Python processing
    getFoodName(uid);
    
    currentPosition = 0;
    mainMenu();
  } else if (currentPosition == 1) {  // Add new item
    addExecute();
    currentPosition = 0;
    mainMenu();
  } else if (currentPosition == 2) {  // Delete
    deleteExecute();
  } else if (currentPosition == 3) {  // Meal
    // Gives code to Python
    returnFoodNames();
    
    lcd.print("Please Wait...");
    // Waits to hear back
    String response = serialRead();
    
    int intValue = response.toInt(); 

    if (intValue == 1) { // If no meal in array
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error: EAT-404");
      lcd.setCursor(0, 1);
      lcd.print("Empty Meal Plan");
      delay(2500);
      currentPosition = 0;
      mainMenu();
    } else { // Show response with analysis
      splitAndStore(response, 3000);
      delay(1000);  
    }
  } else { // Reset
    lcd.print("RESETTING...");
    clearFoodsArray();
  }
}

// Add food to dict
void addFood(const char* uid, String name) {
  if (foodsCount < MAX_FOODS) {
    // Shift existing elements to make room for the new one
    for (int i = foodsCount - 1; i >= 0; i--) {
      foods[i + 1] = foods[i];
    }

    // Add the new element at the beginning
    strncpy(foods[0].id, uid, RFID_LENGTH - 1);
    foods[0].id[RFID_LENGTH - 1] = '\0';  // Null-terminate the string
    foods[0].name = name;

    foodsCount++;
  }
}
// Split incoming string and display it moving up 
void splitAndStore(String input, int delayTime) {

  input.trim(); // no white space 

  String parts[maxParts];   // Array to store the parts
  int inputLength = input.length();
  int partIndex = 0;

  // loop over each 16 chars and adds it to a part
  for (int startPos = 0; startPos < inputLength; startPos += 16) {
    String currentPart = input.substring(startPos, min(startPos + 16, inputLength));

    // Check if the current part is empty, and break out of the loop
    if (currentPart.length() == 0) {
      break;
    }

    parts[partIndex++] = currentPart; // Adds to array
    if (partIndex >= maxParts) {
      // Break if the maximum number of parts is reached
      break;
    }
  }
  // Display parts going upward
  for (int i = 0; i < maxParts - 1; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(parts[i]); // Print first row

    lcd.setCursor(0, 1); // Print second row
    lcd.print(parts[i + 1]);

    delay(delayTime);
  }
  delay(1000);
  currentPosition = 0;
  mainMenu();
}


// Show food array for python code
void returnFoodNames() {
  Serial.print("Food in Meal: ");
  for (int i = 0; i < returnFoodsCount; i++) {
    Serial.print(returnFoodsArray[i]);
    if (i < returnFoodsCount - 1) {
      Serial.print(", ");
    }
  }
  Serial.println();
}
// Getting food name coresponding to uid 
void getFoodName(String uid) {
  bool found = false;

  // Find the name corresponding to the UID in foods
  String scannedFoodName;
  for (int i = 0; i < foodsCount; i++) {
    if (String(foods[i].id) == uid) {
      scannedFoodName = foods[i].name;
      found = true;
      break;
    }
  }

  // If the UID is not found in the foods array
  if (!found) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error: Not found");
    lcd.setCursor(0, 1);
    lcd.print("in foods");
    delay(2500);
    scannedFoodName = addScannedItem(uid);
  }

  // Check if the name corresponding to the UID is already in returnFoodsArray
  for (int i = 0; i < returnFoodsCount; i++) {
    if (returnFoodsArray[i] == scannedFoodName) {
      // The corresponding name is already present, display a message
      lcd.clear();
      lcd.print("Already Scanned");
      delay(2000);
      return;
    }
  }

  // The corresponding name is not present, add it to returnFoodsArray
  returnFoodsArray[returnFoodsCount] = scannedFoodName;
  returnFoodsCount++;

  // Display the scanned food name on the LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scanned:");
  lcd.setCursor(0, 1);
  lcd.print(scannedFoodName);

  delay(2000);
  mainMenu(); // return back to menu
}


// Adding an item to dict func
void addExecute() {
  String uid;

  // Prompt the user to scan the RFID tag
  lcd.clear();
  lcd.print("Scan RFID Tag");

  // Wait for RFID tag to be scanned
  uid = waitForRFID();

  // Check if the UID is already in the foods array
  for (int i = 0; i < foodsCount; i++) {
    if (String(foods[i].id) == uid) {
      // UID is already present, display an error message
      lcd.clear();
      lcd.print("Already exists");
      delay(2000);
      mainMenu();
      return;
    }
  }

  String foodItem = addScannedItem(uid);  // Asigns uid to food item 

  // Display a confirmation message
  lcd.clear();
  lcd.print("Added: " + foodItem);
  delay(2000);


  // Return to the main menu
  mainMenu();
}

// Reading the serial 
String serialRead() {
  String newString = "";
  while (newString.length() == 0) {
    if (Serial.available()) {
      delay(300);
      newString = Serial.readString();
    }
  }
  return newString;
}

// RFID sensor scanner 
String waitForRFID() {
  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    // No new card present or failed to read the card, keep waiting
    delay(100);
  }
  // Takes UID and turns it into HEX
  unsigned long hex_num = 0;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    hex_num <<= 8;
    hex_num |= mfrc522.uid.uidByte[i];
  }

  mfrc522.PICC_HaltA();  // Stop reading
  return String(hex_num, HEX);
}

// Cleans inout and makes it store well 
String cleanString(String input) {

  //Remove leading and trailing whitespace
  input.trim();

  //Remove non-alphanumeric characters
  for (int i = 0; i < input.length(); i++) {
    if (!isAlphaNumeric(input[i])) {
      input.remove(i, 1);
      i--;  // Adjust the index after removal
    }
  }

  //Remove all numeric characters
  for (int i = 0; i < input.length(); i++) {
    if (isDigit(input[i])) {
      input.remove(i, 1);
      i--;  // Adjust the index after removal
    }
  }

  //Capitalize the first letter
  if (input.length() > 0) {
    input.setCharAt(0, toupper(input.charAt(0)));
  }

  return input;
}

// Adding food item func 
String addScannedItem(String uid) {
  // Prompt the user to enter a name
  lcd.clear();
  lcd.print("Enter Name: ");
  Serial.println("Scan");

  String foodItem = serialRead();
  foodItem = foodItem.substring(0, 16);
  foodItem = cleanString(foodItem);

  // Add the scanned RFID and entered name to the array
  addFood(uid.c_str(), foodItem);

  return foodItem;
}


// Delete menu items main loop and logic func
void deleteExecute() {
  // Initial delay for stability
  delay(300);

  // Initialize the position in the deletion menu
  int currentListPos = 0;
  delMenu(currentListPos);

  // Main loop for handling deletion
  while (true) {
    // Read the X-axis value from the joystick
    int joystickX = analogRead(JOYSTICK_X_PIN);

    // Check for joystick movement and update menu accordingly
    if (joystickX < 412 && currentListPos > 0 && lastJoystickXValue >= 412) {
      currentListPos--;
      delMenu(currentListPos);
    } else if (joystickX > 612 && lastJoystickXValue <= 612) {
      currentListPos++;
      delMenu(currentListPos);
    }

    // Update the last joystick X-axis value
    lastJoystickXValue = joystickX;
    arrayRemoveIndex = currentListPos;

    // Check if the deletion process is done
    if (isDone) {
      // Check for joystick button press to exit deletion mode
      if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
        // Reset position and update the menu
        currentPosition = 0;
        mainMenu();
        delay(200); // Debounce delay
        isDone = false;
        break; // Exit the deletion loop
      }
    }

    // If deletion process is not done
    if (!isDone) {
      // Check for joystick button press to confirm deletion
      if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
        // Delete the item at the specified index
        deleteItemAtIndex(arrayRemoveIndex);

        // Clear the LCD, display "Deleted," and wait for a moment
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Deleted");
        delay(1000);

        // Update the deletion menu
        delMenu(currentListPos);
      }
    }
  }
}

// Update the displayed menu on the LCD 
int delMenu(int currentListPos) {
  lcd.clear();

  if (currentListPos == returnFoodsCount - 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("> " + String(returnFoodsArray[currentListPos]));
    lcd.setCursor(0, 1);
    lcd.print("DONE");
  } else if (currentListPos >= returnFoodsCount ) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("> DONE");
    isDone = true;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("> " + String(returnFoodsArray[currentListPos]));
    lcd.setCursor(0, 1);
    lcd.print(String(returnFoodsArray[currentListPos + 1]));
  }
}

// Delete menu object from array
void deleteItemAtIndex(int indexToDelete) {
   
    // Shift elements to fill the gap created by deleting the item
    for (int i = indexToDelete; i < returnFoodsCount - 1; i++) {
        returnFoodsArray[i] = returnFoodsArray[i + 1];
    }
    // Update the returnFoodsCount
    returnFoodsCount--;
    currentPosition = 0;
    mainMenu();
}

// Reset logic clearing array
void clearFoodsArray() {
    for (int i = 0; i < MAX_FOODS; i++) {
        returnFoodsArray[i] = String();  // Clear each String object
    }
    returnFoodsCount = 0;  // Reset the count to zero

    delay(2000);
    currentPosition = 0;
    mainMenu(); // Go back to main menu
}