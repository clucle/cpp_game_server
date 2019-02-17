import pygame
from pygame.locals import *
import random

pygame.init()
screen = pygame.display.set_mode((400, 300))
done = False


def random_color():
    return [random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]


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

    def move(self, x_diff):
        self.x += x_diff


# player list
player = Player(x=40, screen=screen)
player_list = [player]

# color
white = (255, 255, 255)

while not done:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        if event.type == pygame.KEYDOWN:
            if event.key == K_RIGHT:
                player.move(Player.X_DIFF)
            elif event.key == K_LEFT:
                player.move(-Player.X_DIFF)

    # DRAW
    screen.fill(white)

    for player in player_list:
        player.draw()

    pygame.display.update()

pygame.display.flip()
