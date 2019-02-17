
import os
from dotenv import load_dotenv
load_dotenv()

SERVER_HOST = os.getenv("SERVER_HOST")
SERVER_PORT = os.getenv("SERVER_PORT")
SOCKET_DELIMITER = os.getenv("SOCKET_DELIMITER")

PRESS_KEY_RIGHT = os.getenv("PRESS_KEY_RIGHT")
PRESS_KEY_LEFT = os.getenv("PRESS_KEY_LEFT")
