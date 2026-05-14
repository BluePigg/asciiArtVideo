# import sys
import time
import cursor

# 터미널 제목을 강제로 설정하는 이스케이프 시퀀스 출력
# sys.stdout.write("\x1b]2;NIKO_ASCII\x07")
# sys.stdout.flush()

ASCII_PATH = (
    open("/home/bluepig/MyFoLDeR/Coding/ascii/ascii_to_play.txt", "r")
    .read()
    .split("\n")[0]
)
SPEED = int(open("/home/bluepig/MyFoLDeR/Coding/ascii/ascii_speed.txt", "r").read())

ASCII_VIDEO = open(ASCII_PATH, "r").read().split("D")

idx = 0

ASCII_VIDEO = ASCII_VIDEO[0:-1]


def step():
    global idx
    idx += 1
    if len(ASCII_VIDEO) <= idx:
        idx = 0
        return
    print("\033[H")
    print(ASCII_VIDEO[idx], end="")


cursor.hide()
while True:
    step()

    time.sleep(SPEED / 1000)
    time.sleep(0.01)
