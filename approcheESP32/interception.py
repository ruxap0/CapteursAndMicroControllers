import socket
import datetime

# Set up server
HOST = '0.0.0.0'  # Listen on all available interfaces
PORT = 5000  # Same port as in ESP32 code
file_path = 'esp32_data.csv'

# Create header in CSV file if it doesn't exist
with open(file_path, 'a') as file:
    if file.tell() == 0:  # Check if file is empty
        file.write('timestamp,value\n')

# Set up socket server
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Server listening on {HOST}:{PORT}")
    
    while True:
        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            data = conn.recv(1024).decode('utf-8')
            if data:
                # Get current timestamp
                timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                
                # Save data to file
                with open(file_path, 'a') as file:
                    file.write(f"{timestamp},{data}\n")
                
                print(f"Data received: {data}")