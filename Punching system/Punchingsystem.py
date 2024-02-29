import serial
import pandas as pd
from datetime import datetime
from time import sleep

# Set up the serial connection
ser = serial.Serial('COM5', 9600, timeout=1)
ser.flush()

# Initialize DataFrame and other variables
columns = ['Date', 'Day', 'Time', 'Person', 'Status']
data = pd.DataFrame(columns=columns)
last_status = {}  # Dictionary to track the last status of each person
entry_number = 1  # Initialize entry number

print('Place your card...')
sleep(0.5)
# Print column headers
print('       '.join(columns))

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()

        if line.startswith("Timestamp"):
            timestamp, person = line.split(" - ")
            _, timestamp = timestamp.split(": ")
            person = person.split(": ")[1]

            timestamp = datetime.now()
            date = timestamp.strftime("%d-%m-%Y")
            time = timestamp.strftime("%I:%M %p")
            day = timestamp.strftime("%A")

            # Determine the status
            if person in last_status and last_status[person] == 'Entered':
                status = 'Exited'
            else:
                status = 'Entered'
            last_status[person] = status  # Update the last status

            # Create new entry
            new_entry = {'Date': date, 'Day': day, 'Time': time, 'Person': person, 'Status': status}
            data = pd.concat([data, pd.DataFrame([new_entry])], ignore_index=True)

            # Print the new entry
            print(f"{entry_number:<2} {date}  {day}  {time}  {person:<8}  {status}")
            entry_number += 1

        data.to_excel('rfid_data.xlsx', index=False)
