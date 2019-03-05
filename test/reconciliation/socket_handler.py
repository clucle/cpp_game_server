# reference : http://parksk.tistory.com/112

import socket
import threading
from reconciliation.settings import SERVER_HOST, SERVER_PORT, SOCKET_DELIMITER
from reconciliation.user_move_protocol import UserMoveProtocol
from reconciliation.player import set_player_pos

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

    def send(self, data):
        msg = bytearray("", "utf-8")
        msg.extend(data['id'].to_bytes(4, byteorder='little'))
        msg.extend(data['op'].to_bytes(4, byteorder='little'))
        msg.extend(data['seq'].to_bytes(4, byteorder='little'))
        msg.extend(str.encode(SOCKET_DELIMITER))
        self.s.send(msg)

    def recv(self):
        while True:
            try:
                data = self.s.recv(1024)
                buf = bytearray(data)
                s = UserMoveProtocol.from_buffer(buf)
                set_player_pos(s.id, s.seq, s.pos)

            except socket.timeout:
                continue
            except socket.error:
                break

    def run(self):
        self.t = threading.Thread(target=self.recv)
        self.t.start()
