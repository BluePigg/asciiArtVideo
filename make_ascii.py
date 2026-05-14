import cv2
import os


def clear():
    print("\033[2J\033[H")


clear()
INPUT_PATH = input("변환할 파일 경로를 입력하세요 > ")
clear()
OUTPUT_PATH = input("출력파일의 경로를 입력하세요 > ")
clear()

width_proto = input("가로 길이를 입력하세요 (세로는 가로의 1/3) > ")
clear()
WIDTH = 0
try:
    if width_proto.isdigit() and 0 < int(width_proto):
        WIDTH = int(width_proto)
    else:
        raise Exception("길이가 올바르게 입력되지 않았습니다.")
except Exception as exception:
    raise Exception(exception)

HEIGHT = WIDTH // 3

INPUT_PATH = os.path.expanduser(INPUT_PATH)
OUTPUT_PATH = os.path.expanduser(OUTPUT_PATH)

ASCII_TABLE = ["@", "#", "$", "%", "?", "*", "+", ";", ":", ",", " "]

if input("뒤집을건가요 (y/N) > ") == "y":
    ASCII_TABLE.reverse()


def ImageToAscii(img_proto):
    img = cv2.cvtColor(img_proto, cv2.COLOR_BGR2GRAY)

    img = cv2.resize(img, (WIDTH, HEIGHT))
    img = img.flatten()

    result = "".join([ASCII_TABLE[pixel // 25] for pixel in img])

    result = "\n".join(result[i : i + WIDTH] for i in range(0, len(result), WIDTH))
    return result


video = cv2.VideoCapture(INPUT_PATH)

buffer = ""
while video.isOpened():
    success, image = video.read()
    if success:
        buffer = buffer + ImageToAscii(image) + "D"
    else:
        break
with open(OUTPUT_PATH, "w") as txt:
    txt.write(buffer)
