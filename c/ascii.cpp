#include <cstdio>
#include <math.h>
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;
using namespace cv;

void print_img(Mat img);
void reverse_array(char *p, size_t size_p);

String buffer = "";
const int yRatio = 3;
int imgSize = 0;

uint64_t GetTickCount64_Linux() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

int main() {
  char inputSize[5];
  printf("4자리 숫자를 입력해주세요. (예: 0200) > ");
  fgets(inputSize, 5, stdin);

  char *is_ptr = strchr(inputSize, '\n');
  while (!is_ptr && getchar() != '\n')
    ;

  reverse_array(inputSize, 4);
  for (int i = 0; i < 4; i++) {
    char chr_num = inputSize[i] - 48;
    imgSize += ((int)chr_num) * (int)pow(10, i);
  }

  printf("\033[H\033[J");

  char videoName[50];
  printf("재생할 영상의 이름을 입력하세요 (.mp4 제외) > ");
  fgets(videoName, 50, stdin);

  char *vn_ptr = strchr(videoName, '\n');
  if (vn_ptr)
    *vn_ptr = '\0';

  char for_mp4[60];

  snprintf(for_mp4, sizeof(for_mp4), "%s.mp4", videoName);

  printf("\033[H\033[J");

  VideoCapture cap(for_mp4);
  double fps = cap.get(CAP_PROP_FPS);
  uint64_t startTime = GetTickCount64_Linux();

  if (!cap.isOpened())
    return -1;
  Mat frame;
  while (1) {
    uint64_t time_elapsed = GetTickCount64_Linux() - startTime;

    // AI 형님 고맙습니다
    int targetFrame = (int)((time_elapsed / 1000.0) * fps);
    cap.set(CAP_PROP_POS_FRAMES, targetFrame);

    printf("\033[3J\033[H");
    cap.read(frame);
    if (frame.empty())
      break;
    print_img(frame);
    usleep(1);
  }

  return 0;
}

void print_img(Mat img) {
  const char asciiTable[10] = {' ', ',', ':', ';', '*',
                               '?', '%', 'S', '#', '@'};
  cvtColor(img, img, COLOR_BGR2GRAY);
  resize(img, img, Size(imgSize, imgSize / yRatio));
  unsigned char *pPtr = img.data;

  buffer.clear();
  buffer.reserve(imgSize * (imgSize / yRatio + 1));
  for (int i = 0; i < imgSize * imgSize / yRatio; i++) {
    buffer += asciiTable[(int)(*(pPtr + i) / 26)];
    if ((i + 1) % imgSize == 0)
      buffer += "\n";
  }
  printf("%s", buffer.c_str());
}

void reverse_array(char *p, size_t size_p) {
  if (p == nullptr || size_p <= 1)
    return;
  for (int i = 0; i * 2 < size_p; i++) {
    char temp = p[i];
    p[i] = p[size_p - 1 - i];
    p[size_p - 1 - i] = temp;
  }
}
