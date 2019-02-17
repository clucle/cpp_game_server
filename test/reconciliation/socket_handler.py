# reference : http://parksk.tistory.com/112

import socket
import threading
from reconciliation.settings import SERVER_HOST, SERVER_PORT, SOCKET_DELIMITER

HOST = SERVER_HOST
PORT = int(SERVER_PORT)


class Socket:
    def __init__(self):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect((HOST, PORT))
        # recv thread
        self.t = None

    def close(self):
        self.s.close()
        self.t.join()

    def send(self, seq, data):
        msg = bytes(str(seq) + data + SOCKET_DELIMITER, "utf-8")
        self.s.send(msg)

    def recv(self):
        while True:
            try:
                data = self.s.recv(1024)
                data = str(data).split("b'", 1)[1].rsplit("'", 1)[0]
                # some logic
                print(data)
            except socket.timeout:
                continue
            except socket.error:
                break

    def run(self):
        self.t = threading.Thread(target=self.recv)
        self.t.start()
