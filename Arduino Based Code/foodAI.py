import openai
import time

# Your OpenAI API key
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
user_input = "chips, tuna, bagel"
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

while True:
    # Wait for 5 seconds
    time.sleep(5)  

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
            print(f"{role.capitalize()}: {content}")

       

        # Step 5: Delete the Assistant
        client.beta.assistants.delete(assistant.id)
        
        break
