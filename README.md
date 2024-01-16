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

#### Launching:
- Launch Arduino IDE and upload the main.cpp onto the Arduino.
- Launch VS Code and start the "main_food_algorithm" python script.

#### Using the Machine:
1. When starting, scan new food items you want to analyze during meal prep.
2. To scan a food item into your meal, click "Scan"; follow the provided instructions.
3. To delete a food, scroll down to "Delete"; follow the provided instructions.
4. For meal analysis, scroll down to "Analyze Meal"; wait for the analysis results.
5. To reset food list scroll down to "Reset Meal" and click on the item
6. After analyzing your meal, add suggested foods to improve its nutritional balance.
7. Repeat for multiple meals.

#### Adding New Food:
- When adding new food, scan the item using the NFC tag; then type its name in the terminal and press enter.

#### Scan:
- Place the NFC tag in front of the RFID scanner for scanning.
- If a food item cannot be scanned, it will prompt you to add the item (follow the steps provided)

#### Delete:
- Scroll down to the food item you want to delete.
- Press the joystick to delete the selected item.


### Source Code:
Check the [C++](https://github.com/ElliottStarosta/TEJ20-Summative-/blob/main/Arduino%20Based%20Code/main.cpp) and [Python](https://github.com/ElliottStarosta/TEJ20-Summative-/blob/main/Arduino%20Based%20Code/main_food_algorithm.py) source codes!

## Additional Notes
- **DISCLAIMER:** No borrowed code was used in this project, as no one has made this project before.
-Hope you enjoy it! (I spent ~40 hours on this summative...)