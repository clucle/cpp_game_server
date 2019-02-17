
import pygame
from reconciliation.utils import random_color
from reconciliation.settings import PRESS_KEY_LEFT, PRESS_KEY_RIGHT


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
        if data == PRESS_KEY_LEFT:
            socket.send(self.seq, 0)
        elif data == PRESS_KEY_RIGHT:
            socket.send(self.seq, 1)

    def move(self, x_diff):
        self.x += x_diff
