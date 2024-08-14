import socket
import signal
import sys

def signal_handler(sig, frame):
    print('Вы нажали Ctrl+C!')
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind(('0.0.0.0', 12345))

    while True:
        message, address = server_socket.recvfrom(1024)
        print(f"Received message from {address}: {message.decode()}")
        if message.decode() == "stop":
            print("Stopping server.")
            break
        server_socket.sendto("Pong".encode(), address)

def start_client():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    while True:
        message = input("Enter your message: ")
        client_socket.sendto(message.encode(), ('localhost', 12345))
        if message == "stop":
            break
        response, _ = client_socket.recvfrom(1024)
        print(f"Received response: {response.decode()}")

start_server()