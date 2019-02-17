import pygame
import random

pygame.init()
screen = pygame.display.set_mode((400, 300))
done = False


def random_color():
    return [random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]


class Player:
    def __init__(self, x, screen):
        self.x = x
        self.y = 40
        self.seq = 0
        self.rad = 40
        self.color = random_color()
        self.screen = screen

    def draw(self):
        pygame.draw.circle(self.screen, self.color, [self.x, self.y], self.rad)

    def move(self, x):
        self.x = x


# player list
player_list = [Player(x=40, screen=screen)]

# color
white = (255, 255, 255)

while not done:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True

    screen.fill(white)

    for player in player_list:
        player.draw()

    pygame.display.update()

pygame.display.flip()
