
import pygame
import random
from reconciliation.utils import random_color
from reconciliation.settings import PRESS_KEY_LEFT, PRESS_KEY_RIGHT

player_dict = {}
player_screen = None


def generate_player():
    player = Player(x=40, screen=player_screen)
    player_dict[player.uid] = player
    return player


def set_player_screen(screen):
    global player_screen
    player_screen = screen


def set_player_pos(uid, seq, pos):

    if uid not in player_dict:
        player = Player(x=40, uid=uid, screen=player_screen)
        player_dict[player.uid] = player

    if player_dict[uid].seq >= seq:
        return

    player_dict[uid].x = pos + 40


class Player:
    X_DIFF = 40

    def __init__(self, x, screen, uid=random.randint(0, 210000000)):
        self.x = x
        self.y = 40
        self.uid = uid
        self.seq = 0
        self.rad = 40
        self.color = random_color()
        self.screen = screen

    def draw(self):
        pygame.draw.circle(self.screen, self.color, [self.x, self.y], self.rad)

    def send(self, socket, data):
        self.seq += 1
        send_data = {'seq': self.seq, 'id': self.uid}
        if data == PRESS_KEY_LEFT:
            send_data['op'] = 1
            self.x += 10
        elif data == PRESS_KEY_RIGHT:
            send_data['op'] = 2
            self.x -= 10
        set_player_pos(self.uid, self.seq, self.x)
        socket.send(send_data)

    def move(self, x_diff):
        self.x += x_diff
