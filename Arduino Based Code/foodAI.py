from openai import OpenAI
import serial

# Function to send user input to GPT-3.5 Turbo and retrieve the assistant's response
def get_assistant_response(user_input):

    with open("key.txt", "r") as file:
        api_key = file.read().strip()

    client = OpenAI(api_key=api_key)

    assistant = client.beta.assistants.create(
        name="Food Meal Analyzer",
        instructions="You are a food expert and your goal is to analyze a given meal...",
        tools=[{"type": "code_interpreter"}],
        model="gpt-3.5-turbo-1106"
    )

    thread = client.beta.threads.create()

    message = client.beta.threads.messages.create(
        thread_id=thread.id,
        role="user",
        content=f"{user_input}"
    )

    run = client.beta.threads.runs.create(
        thread_id=thread.id,
        assistant_id=assistant.id
    )

    run = client.beta.threads.runs.retrieve(
        thread_id=thread.id,
        run_id=run.id
    )

    messages = client.beta.threads.messages.list(
        thread_id=thread.id
    )

    assistant_response = ""
    for message in reversed(messages.data):
        assistant_response = message.content[0].text.value + "\n"

    return assistant_response.strip()


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
                
                assistant_response = get_assistant_response(user_input) # Gives user response to the AI
                # Send the assistant response back to Arduino
                ser.write(assistant_response.encode('utf-8')) # Writes response back to the serial

    except KeyboardInterrupt:
        print("Script terminated by user.") # When code fully ends 

    finally:
        ser.close()

if __name__ == "__main__":
    main()
