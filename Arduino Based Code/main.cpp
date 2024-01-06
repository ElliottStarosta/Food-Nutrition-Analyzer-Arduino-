#include <LiquidCrystal.h>

// Define pin assignments
#define JOYSTICK_X_PIN A0
#define JOYSTICK_BUTTON_PIN 9

// Define LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Menu items
const char* menuItems[] = {"Add", "Delete", "List of Items", "Done"};

// Current position in the menu
int currentPosition = 0;

// Joystick variables
int initialJoystickXValue = 0;
int lastJoystickXValue = 0;

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
  } else if (currentPosition == 1) {
    deleteExecute();
  } else if (currentPosition == 2) {
    viewListExecute();
  } else if (currentPosition == 3) {
    doneExecute();
  }
}

// Functions for executing specific menu items
void addExecute() {
  Serial.println("Add");
}

void deleteExecute() {
  Serial.println("Delete");
}

void viewListExecute() {
  Serial.println("List");
}

void doneExecute() {
  Serial.println("Done");
}