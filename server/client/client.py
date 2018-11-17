import sys
import socket
import pygame
import threading
from pygame.locals import *

pygame.init()

screen = pygame.display.set_mode((480, 320), DOUBLEBUF)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('server', 34343))


def receive_msg():
    while True:
        data = s.recv(1024)
        if not data:
            print("exit")
            break
        else:
            print(data)


t = threading.Thread(target=receive_msg)
t.start()

while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
        if event.type == KEYDOWN:
            if event.key == K_RIGHT:
                s.send("key Down right".encode())
            elif event.key == K_LEFT:
                s.send("key Down left".encode())

        if event.type == KEYUP:
            if event.key == K_RIGHT:
                s.send("key UP right".encode())
            elif event.key == K_LEFT:
                s.send("key UP left".encode())

    screen.fill((0, 0, 0))
    pygame.draw.rect(screen, (255, 255, 255), [150, 10, 50, 20])
    pygame.display.flip()