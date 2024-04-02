# Food Analyzer Arduino Project

## Overview

This repository contains the code and documentation for Elliott Starosta's TEJ20 Summative Arduino project. The project involves modifying an Arduino-based idea and demonstrating the skills and knowledge acquired during the course.

## Project Details

- **Title:** Arduino Summative 
- **Student:** Elliott Starosta
- **Course:** TEJ20
- **Due Date:** January 24th, 2024

## Project Description Introduction

The inspiration for my project originated from popular food information logging apps like "My Fitness Pal" and "My Net Diary." While these applications efficiently track food consumption, I noticed a gap in the market for a comprehensive food management system that combines RFID sensors and NFC tags on food items. Taking a unique approach, I decided to develop a food tracker/analyzer using Arduino, incorporating features that set it apart from existing solutions.

My project is a sophisticated food analyzer designed to log meals and offer personalized suggestions for healthier choices. Utilizing an RFID sensor, users can effortlessly scan food items into the database. When ready to analyze their meal, a simple press of the analyze button sends the data to the OpenAI API, retrieving nutritional information and providing tailored suggestions for improving the meal's health quotient. The system allows users to add new items to the database, scan them into their meals, delete scanned items, analyze the overall meal, and even reset the meal if desired. Unlike conventional apps, my project presents information on an LCD screen, enhancing the user experience and making meal preparation more dynamic and user-friendly. This innovative combination of RFID technology and AI-driven analysis sets my food analyzer apart, offering a novel solution for those seeking a more interactive and insightful approach to managing their nutritional intake.

## Journal

Check the [Journal](https://docs.google.com/document/d/1gp0loAV-oS-ApLvzDotrjCvRYIWIoJ3BhBvraUlo2Vc/edit) for daily reflections on project progress, encountered problems, and solutions.

## Hardware Design Documentation

### Functional Block Diagram

![FinalBlock drawio (1)](https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/5bc544c5-b863-467f-9801-8506d67558c9)

### User Manual 

*Getting Started:*

1. **Launch Arduino IDE:**
   - Open the Arduino IDE.
   - Upload the "main.cpp" file onto the Arduino (hit the button surrounding the red box).
   <img width="1279" alt="Screenshot 2024-01-17 131139" src="https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/6e5779e3-75ee-4d4f-b7f0-81b4612ff296">

2. **Launch VS Code:**
   - Open Visual Studio Code, and clone the repository by doing the following command in terminal:
   ```bash 
   git clone https://github.com/ElliottStarosta/Food-Nutrition-Analyzer-Arduino-.git
   ```
   - Have the dependencies installed to run the code:
   ```bash 
   python -m pip install pyserial
   ```
   ```bash 
   python -m pip install openai
   ```
   - Start the "main_food_algorithm" Python script by clicking on the arrow in the top right (boxed in red) or run this command in terminal:
   ``` bash 
   py Food-Nutrition-Analyzer-Arduino-\Arduino Based Code\main_food_algorithm.py
   ```
   <img width="1064" alt="Screenshot 2024-01-17 124628" src="https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/b29c6219-89b5-4491-8731-82e952f3bcae">


*Using the Machine:*

3. **Scan New Food Items:**
   - During meal prep, scan new food items.
   - Click "Scan" and follow the provided instructions.

   ![IMG_4869](https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/edb915f1-5f30-4286-b7bf-fed9a91876f7)


4. **Delete Food:**
   - Scroll down to "Delete" and follow the instructions to remove a food item.

   ![IMG-4876](https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/cb4a6816-36ec-49c5-9645-0620cb8a5468)


5. **Meal Analysis:**
   - Scroll down to "Analyze Meal" and wait for the analysis results.
   - Add suggested foods to improve nutritional balance.

    ![IMG_4874 (1)](https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/379e97bf-b038-4fec-be19-b943ee9fb64c)

6. **Reset Food List:**
   - Scroll down to "Reset Meal" and click on the item to reset the food list.

   ![IMG_4875](https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/3e37b92e-8693-4516-9f6c-59ddfa0283ed)


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

   ![IMG-4877](https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/d22f3dcd-3be4-4673-befb-2ada8a224206)



*Deleting:*

9. **Deleting a Food Item:**
   - Scroll down to the food item you want to delete.
   - Press the joystick to delete the selected item.
   - Hit "Done" at the bottom of the page to go back to the main menu

   ![IMG_4872](https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/72d5ad8c-3ce7-4836-bdeb-4a341f52765a)
   ![IMG_4873](https://github.com/ElliottStarosta/TEJ20-Summative-/assets/147102252/59e4717c-910b-4d40-bcd7-88b3da35ce3c)





### Source Code:
Check the [C++](https://github.com/ElliottStarosta/TEJ20-Summative-/blob/main/Arduino%20Based%20Code/main.cpp) and [Python](https://github.com/ElliottStarosta/TEJ20-Summative-/blob/main/Arduino%20Based%20Code/main_food_algorithm.py) source codes!

## Additional Notes
- **DISCLAIMER:** No borrowed code was used in this project, as no one has made this project before.
- Hope you enjoy it! (I spent ~50 hours on this summative...)
