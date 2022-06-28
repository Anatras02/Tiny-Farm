#! env/lib/python3.10 python3
import sys
import struct
import socket
import threading

# host e porta di default
HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 65432  # Port to listen on (non-privileged ports are > 1023)


# codice da eseguire nei singoli thread
class ClientThread(threading.Thread):
    def __init__(self, conn, addr):
        threading.Thread.__init__(self)
        self.conn = conn
        self.addr = addr

    def run(self):
        gestisci_connessione(self.conn, self.addr)


def main(host=HOST, port=PORT):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        while True:
            conn, addr = s.accept()

            t = ClientThread(conn, addr)
            t.start()

    s.shutdown(socket.SHUT_RDWR)


def gestisci_connessione(conn, addr):
    with conn:
        data = recv_all(conn, 4)
        dimensione = struct.unpack("!i", data[:4])[0]

        data = recv_all(conn, dimensione)

        print(data)

        somma = struct.unpack("i", data[:4])[0]

        print(somma)


def recv_all(conn, n):
    chunks = b''
    bytes_recd = 0
    while bytes_recd < n:
        chunk = conn.recv(min(n - bytes_recd, 1024))
        if len(chunk) == 0:
            raise RuntimeError("socket connection broken")
        chunks += chunk
        bytes_recd = bytes_recd + len(chunk)
    return chunks


if len(sys.argv) == 1:
    main()
else:
    print(f"Uso:\n\t {sys.argv[0]}")
