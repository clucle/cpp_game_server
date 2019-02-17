import pygame
from pygame.locals import *
from reconciliation.player import Player

pygame.init()
screen = pygame.display.set_mode((400, 300))
done = False


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
