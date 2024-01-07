#include <LiquidCrystal.h>

// Define pin assignments
#define JOYSTICK_X_PIN A0
#define JOYSTICK_BUTTON_PIN 9

// Define LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Menu items
const char* menuItems[] = {"Add", "Delete", "List of Items", "Analyze Meal"};

// Define RFID related constants
const int RFID_LENGTH = 9;
const int MAX_FOODS = 20;

struct Food {
  char id[RFID_LENGTH];
  String name;
};

Food foods[MAX_FOODS];
int foodsCount = 0;

// Current position in the menu
int currentPosition = 0;
int currentPos = 0;

// Joystick variables
int initialJoystickXValue = 0;
int lastJoystickXValue = 0;

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

  addExampleFoods();

  // Print the food array
  printFoodArray();
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
  if (currentPosition == 0) {
    // addExecute();
  } else if (currentPosition == 1) {
    deleteExecute();
    isDelete = false;
  } else if (currentPosition == 2) {
    scrollFoodNames();
  } else if (currentPosition == 3) {
    // doneExecute();
  }
}

void deleteExecute() {
  Serial.println("Delete");
  isDelete = true;
  scrollFoodNames();
}

void addExampleFoods() {
  addFood("BD31152B", "Apple");
  addFood("A2F0C37E", "Banana");
  addFood("8EFD904D", "Orange");
  addFood("5C82B9A1", "Peach");
  addFood("3A7D812F", "Grapes");
  masterDone();
}

void addFood(const char* rfid, String name) {
  if (foodsCount < MAX_FOODS) {
    strncpy(foods[foodsCount].id, rfid, RFID_LENGTH - 1);
    foods[foodsCount].id[RFID_LENGTH - 1] = '\0'; // Null-terminate the string
    foods[foodsCount].name = name;
    foodsCount++;
  }
}

void printFoodArray() {
  Serial.println("Food Array:");

  for (int i = 0; i < foodsCount; i++) {
    Serial.print("RFID: ");
    Serial.print(foods[i].id);
    Serial.print(", Name: ");
    Serial.println(foods[i].name);
  }
}

void scrollFoodNames() {
  int currentPos = 0;
  updateDeleteList(currentPos);
  isScrolling = true;

  while (true) {
    int joystickX = analogRead(JOYSTICK_X_PIN);
    Serial.println(joystickX);

    // Check for joystick movement and update menu accordingly
    if (joystickX < 412 && currentPos >= 0 && lastJoystickXValue >= 412) {
      currentPos--;
      updateDeleteList(currentPos);
    } else if (joystickX > 612 && currentPos < foodsCount - 1 && lastJoystickXValue <= 612) {
      currentPos++;
      updateDeleteList(currentPos);
    }

    // Update last joystick value
    lastJoystickXValue = joystickX;
    if (isDelete) {
      if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
        if (foods[currentPos].name != "DONE") {
          deleteFoodItem(currentPos);
          updateDeleteList(currentPos);
        } else {
          isScrolling = false;
          currentPosition = 0;
          break;
        }
      }
    }
    if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW && foods[currentPos].name == "DONE" ) {
      isScrolling = false;
      currentPosition = 0;
      break;
    }
    delay(200);
  }
  updateMenu();
}

void updateDeleteList(int currentPos) {
  int selectedPosition = currentPos;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> " + String(foods[selectedPosition].name));

  // If there's another item, display it on the second row
  if (selectedPosition != -1) {
    if (selectedPosition < foodsCount - 1) {
      lcd.setCursor(0, 1);
      lcd.print(foods[selectedPosition + 1].name);
    }
  }
  if (selectedPosition < 0) {
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
  addFood("MASTERKEY", "DONE");
}