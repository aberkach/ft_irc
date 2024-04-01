import socket
import threading

# Function to handle client connections
def connect_to_server(client_id):
    # Server configuration
    HOST = '127.0.0.1'  # Server IP address
    PORT = 3500  # Server port

    # Create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the server
        client_socket.connect((HOST, PORT))
        print(f"Client {client_id} connected to server")

        # Send data to the server
        client_socket.sendall(f"Hello from client {client_id}".encode())

        # Receive data from the server
        data = client_socket.recv(1024)
        print(f"Client {client_id} received: {data.decode()}")

    except Exception as e:
        print(f"Error in client {client_id}: {e}")
    finally:
        # Close the client socket
        client_socket.close()
        print(f"Client {client_id} disconnected")

# Number of clients to simulate
NUM_CLIENTS = 10000

# Create and start threads for each client
threads = []
for i in range(NUM_CLIENTS):
    thread = threading.Thread(target=connect_to_server, args=(i+1,))
    threads.append(thread)
    thread.start()

# Wait for all threads to complete
for thread in threads:
    thread.join()
