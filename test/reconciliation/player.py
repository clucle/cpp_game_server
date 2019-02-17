
import pygame
from reconciliation.utils import random_color


class Player:
    X_DIFF = 40

    def __init__(self, x, screen):
        self.x = x
        self.y = 40
        self.seq = 0
        self.rad = 40
        self.color = random_color()
        self.screen = screen

    def draw(self):
        pygame.draw.circle(self.screen, self.color, [self.x, self.y], self.rad)

    def send(self, socket, data):
        self.seq += 1
        socket.send(self.seq, data)

    def move(self, x_diff):
        self.x += x_diff
