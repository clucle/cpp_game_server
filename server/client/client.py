import sys
import socket
import pygame
import threading
from pygame.locals import *


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("yout server", port))

while True:
    t = input()
    if t == "exit":
        break
    else:
        s.send("U".encode())
