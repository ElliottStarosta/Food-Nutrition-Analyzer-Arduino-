# Import necessary libraries
import openai
import time
import serial

# Establish a serial communication connection
serialcomm = serial.Serial('COM3', 9600)
serialcomm.timeout = 1

# Function to read the OpenAI API key from a file
def read_api_key_from_file(file_path):
    try:
        with open(file_path, 'r') as file:
            api_key = file.read().strip()
        return api_key
    except FileNotFoundError:
        print("API key file not found.")
        return None

# Define the file path for the OpenAI API key
api_key_file_path = 'api_key.txt'

# Function to get the response from the OpenAI Assistant
def get_assistant_response(user_input):
    # Read the API key from the file
    api_key = read_api_key_from_file(api_key_file_path)

    if api_key:
        # Initialize the OpenAI client
        client = openai.OpenAI(api_key=api_key)
    else:
        print("Failed to read the API key.")
        return -1  # Return -1 to indicate failure and exit the function

    # Steps 1-4: Create an Assistant, Create a Thread, Add a Message, and Run the Assistant
    assistant = client.beta.assistants.create(
        name="Food Expert",
        instructions="You're a food expert. Suggest healthier additions for a meal. Response: 'Add (foods)' for a better balance because (8 words or less). If balanced, say 'Well-balanced with nutritious elements due to... 8 words or less.",
        tools=[{"type": "code_interpreter"}],
        model="gpt-4-1106-preview"
    )
    thread = client.beta.threads.create()
    message = client.beta.threads.messages.create(
        thread_id=thread.id,
        role="user",
        content=f"{user_input}"
    )
    run = client.beta.threads.runs.create(
        thread_id=thread.id,
        assistant_id=assistant.id,
    )

    assistant_content = ""

    while True:
        # Wait for 2 seconds for bug issues
        time.sleep(2)

        # Retrieve the run status
        run_status = client.beta.threads.runs.retrieve(
            thread_id=thread.id,
            run_id=run.id
        )

        # If run is completed, get messages and tokens used
        if run_status.status == 'completed':
            messages = client.beta.threads.messages.list(
                thread_id=thread.id
            )

            # Loop through messages and print content based on role
            for msg in messages.data:
                role = msg.role
                content = msg.content[0].text.value

                if role.lower() == "assistant":
                    assistant_content = content

            # Step 5: Delete the Assistant
            client.beta.assistants.delete(assistant.id)

            return assistant_content

# Infinite loop to continuously read data from serial communication
while True:
    time.sleep(0.5)
    received_line = serialcomm.readline().decode('ascii').strip()

    if received_line:  # Check if the line is not empty
        print(received_line)

    # Handle different cases based on received data
    if received_line == "Food in Meal:":
        error_message = "1"
        serialcomm.write(error_message.encode('utf-8') + b'\n')

    elif 'Food in Meal:' in received_line:
        # Get response from OpenAI Assistant based on received line
        api_request = get_assistant_response(received_line)
        print(api_request)
        serialcomm.write(api_request.encode('utf-8') + b'\n')

    elif received_line == "Scan":
        # Prompt user to enter the name of a food item
        while True:
            user_input = input("Enter name of food item (max 16 chars, one word): ")
            if len(user_input) <= 16 and len(user_input.split()) == 1:
                # Send user input over serial communication
                serialcomm.write(user_input.encode('utf-8') + b'\n')
                break
            else:
                print("Input must be 16 characters or less and one word. Please try again.")
