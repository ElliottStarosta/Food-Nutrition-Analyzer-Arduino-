# Elliott Starosta TEJ20 Summative - Arduino Project

## Overview

This repository contains the code and documentation for Elliott Starosta's TEJ20 Summative Arduino project. The project involves modifying an Arduino-based idea, demonstrating the skills and knowledge acquired during the course.

## Project Details

- **Title:** Arduino Summative 
- **Student:** Elliott Starosta
- **Course:** TEJ20
- **Due Date:** January 24th, 2024

## Project Description Introduction

The inspiration for my project stemmed from popular food information logging apps like "My Fitness Pal" and "My Net Diary." While these applications efficiently track food consumption, I noticed a gap in the market for a comprehensive food management system that combines RFID sensors and NFC tags on food items. Taking a unique approach, I decided to develop a food tracker/analyzer using Arduino, incorporating features that set it apart from existing solutions. Notably, I integrated an RFID sensor to scan food items and employed the OpenAI API to analyze nutritional content, providing users with personalized feedback on creating balanced and well-constructed meals. Unlike traditional apps, my project displays information on an LCD screen for a more dynamic and user-friendly experience during meal preparation.

## Journal

Check the [Journal](https://docs.google.com/document/d/1gp0loAV-oS-ApLvzDotrjCvRYIWIoJ3BhBvraUlo2Vc/edit) for daily reflections on project progress, encountered problems, and solutions.

## Hardware Design Documentation

### Functional Block Diagram

![FinalBlock drawio](https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/ae4c0c9d-94b1-4098-9133-1fdef4dadc0e)

### User Manual 

*Getting Started:*

1. **Launch Arduino IDE:**
   - Open the Arduino IDE.
   - Upload the "main.cpp" file onto the Arduino (hit the button surronding the red box).
   <img width="1279" alt="Screenshot 2024-01-17 131139" src="https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/6e5779e3-75ee-4d4f-b7f0-81b4612ff296">

2. **Launch VS Code:**
   - Open Visual Studio Code.
   - Start the "main_food_algorithm" Python script by clicking on the arrow in the top right (boxed in red)
   <img width="1064" alt="Screenshot 2024-01-17 124628" src="https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/b29c6219-89b5-4491-8731-82e952f3bcae">


*Using the Machine:*

3. **Scan New Food Items:**
   - During meal prep, scan new food items.
   - Click "Scan" and follow the provided instructions.

   ![Scan Food](images/scan_food.png)  
   *(Placeholder: Image of food scanning process)*

4. **Delete Food:**
   - Scroll down to "Delete" and follow the instructions to remove a food item.

   ![Delete Food](images/delete_food.png)  
   *(Placeholder: Image of deleting a food item)*

5. **Meal Analysis:**
   - Scroll down to "Analyze Meal" and wait for the analysis results.
   - Add suggested foods to improve nutritional balance.

   ![Analyze Meal](images/analyze_meal.png)  
   *(Placeholder: Screenshot of meal analysis)*

6. **Reset Food List:**
   - Scroll down to "Reset Meal" and click on the item to reset the food list.

   ![Reset Meal](images/reset_meal.png)  
   *(Placeholder: Image of resetting the meal)*

*Adding New Food:*

7. **Adding New Food:**
   - Scan the item using the NFC tag.
   - Type its name in the terminal and press enter.

<img width="207" alt="Screenshot 2024-01-17 130850" src="https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/2e692d22-154c-4ae1-843e-04cd9c8e0ff3">

**NOTE**: If an item is already in the database, you would not be able to add it again and a message of "Already exists" will pop up


*Scanning:*

8. **Scanning a Food Item:**
   - Place the NFC tag in front of the RFID scanner for scanning.
   - If a food item cannot be scanned, follow the prompts to add the item.

   ![Scanning Process](images/scanning_process.png)  
   *(Placeholder: Image of scanning a food item)*

*Deleting:*

9. **Deleting a Food Item:**
   - Scroll down to the food item you want to delete.
   - Press the joystick to delete the selected item.

   ![Deleting Food](images/deleting_food.png)  
   *(Placeholder: Image of deleting a food item)*



### Source Code:
Check the [C++](https://github.com/ElliottStarosta/TEJ20-Summative-/blob/main/Arduino%20Based%20Code/main.cpp) and [Python](https://github.com/ElliottStarosta/TEJ20-Summative-/blob/main/Arduino%20Based%20Code/main_food_algorithm.py) source codes!

## Additional Notes
- **DISCLAIMER:** No borrowed code was used in this project, as no one has made this project before.
- Hope you enjoy it! (I spent ~50 hours on this summative...)