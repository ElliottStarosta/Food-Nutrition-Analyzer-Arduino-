#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <SPI.h>

#define JOYSTICK_X_PIN A0
#define JOYSTICK_BUTTON_PIN 9

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char* menuItems[] = {"Add", "Delete", "List of Items", "Done"};
int currentPosition = 0;
int initialJoystickXValue = 0;
int lastJoystickXValue = 0;

// Define RFID pins
#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Dictionary to store RFID IDs and corresponding names
const int MAX_FOODS = 20;
struct Food {
  byte id[4];
  String name;
};

Food foods[MAX_FOODS];
int foodsCount = 0;

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
}

void loop() {
  // Read current joystick position
  int joystickX = analogRead(JOYSTICK_X_PIN);

  // Check for joystick movement and update menu accordingly
  if (joystickX < 412 && currentPosition > 0 && lastJoystickXValue >= 412) {
    currentPosition--;
    updateMenu();
  } else if (joystickX > 612 && currentPosition < 3 && lastJoystickXValue <= 612) {
    currentPosition++;
    updateMenu();
  }

  // Update last joystick value
  lastJoystickXValue = joystickX;

  // Check for button press
  if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
    executeMenuItem();
    delay(200); // Debounce delay
  }
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
  if (currentPosition == 0) {
    addExecute();
    currentPosition = 0;
  } else if (currentPosition == 1) {
    deleteExecute();
    currentPosition = 0;
  } else if (currentPosition == 2) {
    viewListExecute();
    currentPosition = 0;
  } else if (currentPosition == 3) {
    doneExecute();
    currentPosition = 0;
  }
}

// Functions for executing specific menu items
void addExecute() {
  readRFID();
  updateMenu();
}

void deleteExecute() {
  foodItemsScroll();
}

void viewListExecute() {
  Serial.println("List");
}

void doneExecute() {
  Serial.println("Done");
}

void readRFID() {
  lcd.clear();
  lcd.println("Scan RFID tag...");
  
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("RFID tag detected!");

    // Get the RFID UID
    byte* uid = mfrc522.uid.uidByte;
    String rfidID = "";
    for (int i = 0; i < mfrc522.uid.size; i++) {
      rfidID += String(uid[i], HEX);
    }

    Serial.println("RFID ID: " + rfidID);

    // Check if the RFID tag is already in the dictionary
    int existingIndex = findFoodIndex(rfidID);
    
    if (existingIndex != -1) {
      lcd.clear();
      lcd.print("Tag already exists!");
      delay(2000);
      lcd.clear();
    } else {
      // Prompt user to enter a name for the food
      lcd.clear();
      lcd.print("Enter name (16 chars):");
      lcd.setCursor(0, 1);
      
      String foodName = inputText();
      
      // Save RFID ID and food name to the dictionary
      if (foodsCount < MAX_FOODS) {
        memcpy(foods[foodsCount].id, uid, mfrc522.uid.size);
        foods[foodsCount].name = foodName;
        foodsCount++;

        lcd.clear();
        lcd.print("Food added!");
        delay(2000);
        lcd.clear();
      } else {
        lcd.clear();
        lcd.print("Dictionary full!");
        delay(2000);
        lcd.clear();
      }
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

int findFoodIndex(String rfidID) {
  for (int i = 0; i < foodsCount; i++) {
    String storedID = "";
    for (int j = 0; j < mfrc522.uid.size; j++) {
      storedID += String(foods[i].id[j], HEX);
    }

    if (rfidID.equals(storedID)) {
      return i;
    }
  }
  
  return -1;
}

String inputText() {
  String newString = "";
  delay(2000);

  while (true) {
    lcd.clear();
    
    if (Serial.available()) {
      delay(300);

      newString = Serial.readStringUntil('\n'); // Read until newline character

      if (newString.length() > 16) {
        lcd.print("Invalid Name, enter a new one");
        delay(5000);
        continue;
      } else {
        break; 
      }
    }
  }

  return newString;
}

void foodItemsScroll() {
  int selectedPosition = 0;

  while (true) {
    // Display the current food item at the selected position
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("> " + foods[selectedPosition].name);

    // Display the next food item on the second row if available
    if (selectedPosition < foodsCount - 1) {
      lcd.setCursor(0, 1);
      lcd.print(foods[selectedPosition + 1].name);
    }

    // Wait for user input
    int joystickX = analogRead(JOYSTICK_X_PIN);

    if (joystickX < 412 && selectedPosition > 0) {
      selectedPosition--;
    } else if (joystickX > 612 && selectedPosition < foodsCount - 1) {
      selectedPosition++;
    }

    // Check for button press
    if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
      deleteFoodItem(selectedPosition);
      break;
    }

    delay(200);
  }
  updateMenu();
}

void deleteFoodItem(int index) {
  // Shift elements to delete item (rewrites over it)
  for (int i = index; i < foodsCount - 1; i++) {
    foods[i] = foods[i + 1];
  }
  // Decrement the count of food items
  foodsCount--;

  // Have empty string at last index, clears string
  memset(foods[foodsCount].id, 0, sizeof(foods[foodsCount].id));
  foods[foodsCount].name = "";

  // Display a message indicating the deletion
  lcd.clear();
  lcd.print("Deleted");
  delay(2000);
}
