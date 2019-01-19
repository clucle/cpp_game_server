# reference : http://parksk.tistory.com/112

import socket
import _thread

HOST = "your server"
PORT = "port"
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))


def sending_msg():
    while True:
        data = input()
        data = bytes(data, "utf-8")
        s.send(data)
    s.close()


def getting_msg():
    while True:
        data = s.recv(1024)
        data = str(data).split("b'", 1)[1].rsplit("'",1)[0]
        print(data)
    s.close()


_thread.start_new_thread(sending_msg, ())
_thread.start_new_thread(getting_msg, ())


while True:
    pass
