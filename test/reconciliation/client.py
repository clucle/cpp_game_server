import pygame
import random
from pygame.locals import *
from reconciliation.player import player_dict, set_player_screen, generate_player
from reconciliation.socket_handler import Socket
from reconciliation.settings import PRESS_KEY_LEFT, PRESS_KEY_RIGHT


pygame.init()
screen = pygame.display.set_mode((400, 300))
done = False


# player list
player_screen = screen
set_player_screen(screen)


player_id = random.randint(0, 210000000)
player = generate_player(player_id)

# color
white = (255, 255, 255)

# socket
socket = Socket()
socket.run()


while not done:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
            socket.close()
        if event.type == pygame.KEYDOWN:
            if event.key == K_RIGHT:
                print(player_id)
                player_dict[player_id].send(socket, PRESS_KEY_LEFT)
            elif event.key == K_LEFT:
                print(player_id)
                player_dict[player_id].send(socket, PRESS_KEY_RIGHT)

    # DRAW
    screen.fill(white)

    for uid, player in player_dict.items():
        player.draw()

    pygame.display.update()

pygame.display.flip()
