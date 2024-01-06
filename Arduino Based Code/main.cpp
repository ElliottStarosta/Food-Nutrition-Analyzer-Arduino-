#include <LiquidCrystal.h>

#define JOYSTICK_X_PIN A0
#define JOYSTICK_BUTTON_PIN 9

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char* menuItems[] = {"Add", "Delete", "List of Items", "Done"};
int currentOption = 0;
int displayStartIndex = 0;
int initialJoystickXValue = 0;
int lastJoystickXValue = 0;
bool initializationPhase = true;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
  initialJoystickXValue = analogRead(JOYSTICK_X_PIN);
  lastJoystickXValue = initialJoystickXValue;
}

void loop() {
  if (initializationPhase) {
    // Display "Add" during the initialization phase
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("> Add");
    lcd.setCursor(0, 1);
    lcd.print("Delete");

    // Wait for the joystick value to change
    while (analogRead(JOYSTICK_X_PIN) == lastJoystickXValue) {
      if (!digitalRead(JOYSTICK_BUTTON_PIN)){
        executeMenuOption(currentOption);
      }
      delay(100);
    }

    lcd.clear();
    lastJoystickXValue = analogRead(JOYSTICK_X_PIN);
    initializationPhase = false;  // Move to the regular menu behavior
  }

  // Main loop
  int joystickXValue = analogRead(JOYSTICK_X_PIN);
  int joystickButtonState = digitalRead(JOYSTICK_BUTTON_PIN);

  if (abs(joystickXValue - initialJoystickXValue) > 10) {
    // Calculate the offset from the initial position
    int offset = joystickXValue - initialJoystickXValue;
    currentOption = map(offset, -512, 512, 0, sizeof(menuItems) / sizeof(menuItems[0]));

    if (currentOption < 0) {
      currentOption = 0;
    } else if (currentOption >= sizeof(menuItems) / sizeof(menuItems[0])) {
      currentOption = sizeof(menuItems) / sizeof(menuItems[0]) - 1;
    }

    if (currentOption < displayStartIndex) {
      displayStartIndex = currentOption;
    } else if (currentOption >= displayStartIndex + 2) {
      displayStartIndex = currentOption - 1;
    }

    displayMenu();
  }

  if (joystickButtonState == LOW) {
    executeMenuOption(currentOption);
    delay(500);  // Debounce
  }

  delay(100);  // Small delay for stability
}

void displayMenu() {
  lcd.clear();

  for (int i = 0; i < 2; i++) {
    int displayIndex = displayStartIndex + i;

    if (displayIndex >= 0 && displayIndex < sizeof(menuItems) / sizeof(menuItems[0])) {
      lcd.setCursor(0, i);

      if (displayIndex == currentOption) {
        lcd.print("> ");
      }

      lcd.print(menuItems[displayIndex]);
    }
  }
}

void executeMenuOption(int option) {
  if (currentOption == 0) {
    addExecute();
  } else if (currentOption == 1) {
    deleteExecute();
  } else if (currentOption == 2) {
    viewListExecute();
  } else if (currentOption == 3) {
    doneExecute();
  }
}

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
