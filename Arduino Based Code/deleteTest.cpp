#include <LiquidCrystal.h>

// Define pin assignments
#define JOYSTICK_X_PIN A0
#define JOYSTICK_BUTTON_PIN 9

// Define LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Menu items
const char* menuItems[] = {"Scan", "Add", "Delete", "List of Items", "Analyze Meal"};

// Define RFID related constants
const int RFID_LENGTH = 9;
const int MAX_FOODS = 10;

// Define a struct for storing food information
struct Food {
  char id[RFID_LENGTH];
  String name;
};

// Create an array to store food items
Food foods[MAX_FOODS];
// Track the count of food items in the array
int foodsCount = 0;

// Return Food array for AI
String returnFoodsArray[MAX_FOODS];
int returnFoodsCount = 0;

// Current position in the menu
int currentPosition = 4;

// Joystick variables
int initialJoystickXValue = 0;
int lastJoystickXValue = 0;

// Flags to control program flow
bool isScrolling = false;
bool isDelete = false;

void setup() {
  // Initialize LCD and Serial communication
  lcd.begin(16, 2);
  Serial.begin(9600);

  // Set up joystick button pin as input with pull-up resistor
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);

  // Read initial joystick position
  initialJoystickXValue = analogRead(JOYSTICK_X_PIN);
  lastJoystickXValue = initialJoystickXValue;

  // Initial display of the menu
  updateMenu();

  // Add example foods to the array
  addExampleFoods();

  // Print the food array
  printFoodArray();
  
  // Display the names of foods in the meal
  returnFoodNames();
}

void loop() {
  int joystickX = analogRead(JOYSTICK_X_PIN);

  if (!isScrolling) {
    // Check for joystick movement and update menu accordingly
    if (joystickX < 412 && currentPosition > 0 && lastJoystickXValue >= 412) {
      currentPosition--;
      updateMenu();
    } else if (joystickX > 612 && currentPosition < 3 && lastJoystickXValue <= 612) {
      currentPosition++;
      updateMenu();
    }

    // Check for button press
    if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
      executeMenuItem();
      delay(200); // Debounce delay
    }
  } else {
    // In scrolling mode
    scrollFoodNames();
  }

  // Update last joystick value
  lastJoystickXValue = joystickX;
}

// Update the displayed menu on the LCD
void updateMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> " + String(menuItems[currentPosition]));

  // If there's another item, display it on the second row
  if (currentPosition < 3) {
    lcd.setCursor(0, 1);
    lcd.print(menuItems[currentPosition + 1]);
  }
}

// Execute the action associated with the selected menu item
void executeMenuItem() {
  lcd.clear();
  String uid = waitForRFID();
  if (currentPosition == 0) { // Scan
    if (uid == -1) {
      lcd.print("ERROR: Empty tag");
      addExecute();
      currentPosition = 0;
    } else {
      getFoodName(uid);
      currentPosition = 0;
      updateMenu();   
    }
    
  } else if (currentPosition == 1) { // Add new iem
    addExecute();
  } else if (currentPosition == 2) { // Delete
    deleteExecute();
    isDelete = false;
  } else if (currentPosition == 3) { // List of Items
    scrollFoodNames();
  } else if (currentPosition == 4) { // Meal
    returnFoodNames();
    lcd.print("Please Wait...");
    String response = readSerial(); 
    scrollText(response, 300);
    delay(1000);
  }
}

void deleteExecute() {
  Serial.println("Delete");
  isDelete = true;
  scrollFoodNames();
}

void addExampleFoods() {
  // Add predefined foods for demonstration
  addFood("BD31152B", "Apple");
  addFood("A2F0C37E", "Banana");
  addFood("8EFD904D", "Orange");
  addFood("5C82B9A1", "Peach");
  addFood("3A7D812F", "Grapes");
  // Mark completion of adding example foods
  masterDone();
}

void addFood(const char* rfid, String name) {
  // Add a new food item to the array
  if (foodsCount < MAX_FOODS) {
    strncpy(foods[foodsCount].id, rfid, RFID_LENGTH - 1);
    foods[foodsCount].id[RFID_LENGTH - 1] = '\0'; // Null-terminate the string
    foods[foodsCount].name = name;
    foodsCount++;
  }
}

void printFoodArray() {
  // Print the contents of the food array to Serial
  Serial.println("Food Array:");

  for (int i = 0; i < foodsCount; i++) {
    Serial.print("RFID: ");
    Serial.print(foods[i].id);
    Serial.print(", Name: ");
    Serial.println(foods[i].name);
  }
}

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


void scrollFoodNames() {
  currentPosition = 0;
  updateDeleteList(currentPosition);
  isScrolling = true;

  while (true) {
    int joystickX = analogRead(JOYSTICK_X_PIN);

    // Check for joystick movement and update menu accordingly
    if (joystickX < 412 && currentPosition >= 0 && lastJoystickXValue >= 412) {
      currentPosition--;
      updateDeleteList(currentPosition);
    } else if (joystickX > 612 && currentPosition < foodsCount - 1 && lastJoystickXValue <= 612) {
      currentPosition++;
      updateDeleteList(currentPosition);
    }

    // Update last joystick value
    lastJoystickXValue = joystickX;
    if (isDelete) {
      if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
        if (foods[currentPosition].name != "DONE") {
          deleteFoodItem(currentPosition);
          updateDeleteList(currentPosition);
        } else {
          isScrolling = false;
          currentPosition = 0;
          break;
        }
      }
    }
    if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW && foods[currentPosition].name == "DONE") {
      isScrolling = false;
      currentPosition = 0;
      break;
    }
    delay(200);
  }
  updateMenu();
}

void updateDeleteList(int currentPosition) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> " + String(foods[currentPosition].name));

  // If there's another item, display it on the second row
  if (currentPosition != -1) {
    if (currentPosition < foodsCount - 1) {
      lcd.setCursor(0, 1);
      lcd.print(foods[currentPosition + 1

].name);
    }
  }
  if (currentPosition < 0) {
    lcd.setCursor(0, 0);
    lcd.print("> " + String(foods[0].name));
    lcd.setCursor(0, 1);
    lcd.print(foods[1].name);
  }

  delay(200);
}

void deleteFoodItem(int index) {
  // Shift elements to delete item (rewrites over it)
  for (int i = index; i < foodsCount - 1; i++) {
    foods[i] = foods[i + 1];
  }
  // Decrement the count of food items
  foodsCount--;

  // Have an empty string at the last index, clears string
  memset(foods[foodsCount].id, 0, sizeof(foods[foodsCount].id));
  foods[foodsCount].name = "";

  // Display a message indicating the deletion
  lcd.clear();
  lcd.print("Deleted");
  delay(2000);
}

void masterDone() {
  // Add a special "DONE" food item to the array for the mastercode
  addFood("MASTERKEY", "DONE");
}

// RFID tag reader
String waitForRFID() {
  lcd.print("Show the Tag...");

  // Wait until an RFID tag is detected
  while (!mfrc522.PICC_IsNewCardPresent()) {
    delay(100);
  }

  // Select the first detected tag
  if (mfrc522.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    // Halt the tag to stop further readings
    mfrc522.PICC_HaltA();
    return uid;
  }

  return "-1";  // Return "-1" if no tag is read
}


void getFoodName(String uid) {
  for (int i = 0; i < foodsCount; i++) {
    if (String(foods[i].id) == uid) {
      returnFoodsArray[returnFoodsCount] = foods[i].name;
      returnFoodsCount++;
      lcd.print("Scanned: " + foods[i].name);
      break;
    } else {
      lcd.print("Food not found");
      delay(2000);
      addExecute();
    }
  }
  delay(2000);
  updateMenu();
}


void addExecute() {
  String foodItem;
  String uid = waitForRFID();
  lcd.print("Enter Name: ");

  foodItem = serialRead();
  foodItem = foodItem.substring(0, 16);

  addFood(uid.c_str(), foodItem);

  lcd.clear();
  lcd.print("Added: " + foodItem);
  delay(2000);
  updateMenu();
}

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

void scrollText(String input, int delayTime) {
  int inputLength = input.length();
  
  // Display the text for the initial delay
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(input.substring(0, min(16, inputLength)));
  delay(delayTime);

  // Scroll the text offscreen to the left
  for (int positionCounter = 0; positionCounter < inputLength + 16; positionCounter++) {
    lcd.scrollDisplayLeft();
    lcd.clear();
    lcd.setCursor(0, 0);

    if (positionCounter < inputLength) {
      lcd.print(input.substring(positionCounter, min(positionCounter + 16, inputLength)));
    } else {
      lcd.print(' ');  // Fill empty space after scrolling offscreen
    }

    delay(delayTime);
  }

  // Additional delay after scrolling
  delay(1000);

  // Clear the LCD and return to the updateMenu
  lcd.clear();
  currentPosition = 0;
  updateMenu();
}