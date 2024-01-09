import openai
import time
import serial

# Your OpenAI API key
def get_assistant_response(user_input):

    api_key = ""

    # Initialize the client
    client = openai.OpenAI(api_key=api_key)

    # Step 1: Create an Assistant
    assistant = client.beta.assistants.create(
        name="Food Expert",
        instructions="You're a food expert. Suggest healthier additions for a meal. Response: 'Add (foods)' for a better balance because (8 words or less). If balanced, say 'Well-balanced with nutritious elements due to... 8 words or less.",
        tools=[{"type": "code_interpreter"}],
        model="gpt-4-1106-preview"
    )

    # Step 2: Create a Thread
    thread = client.beta.threads.create()

    # Step 3: Add a Message to a Thread
    message = client.beta.threads.messages.create(
        thread_id=thread.id,
        role="user",
        content=f"{user_input}"
    )

    # Step 4: Run the Assistant
    run = client.beta.threads.runs.create(
        thread_id=thread.id,
        assistant_id=assistant.id,
    )

    assistant_content = ""

    while True:
        # Wait for 5 seconds
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
            break



# Main loop to continuously read input from Arduino and interact with the assistant
def main():

    arduino_port = 'COM8'  # Replace 'COMx' with your Arduino's serial port
    baud_rate = 9600

    ser = serial.Serial(arduino_port, baud_rate, timeout=1)
    ser.readline()  # Wait for Arduino to initialize

    try:
        while True:
            user_input = ser.readline().decode('utf-8').strip()

            if user_input:

                assistant_response = user_input

                #assistant_response = get_assistant_response(user_input) # Gives user response to the AI
                # Send the assistant response back to Arduino
                ser.write(assistant_response.encode('utf-8')) # Writes response back to the serial

    except KeyboardInterrupt:
        print("Script terminated by user.") # When code fully ends 

    finally:
        ser.close()

if __name__ == "__main__":
    main()