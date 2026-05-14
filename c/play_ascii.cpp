#include <H5PLpublic.h>
#include <chrono>
#include <fstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

string ascii_to_play;
string ascii_to_play_url;
string speed_str;

int speed = 100;
int idx = -1; // Start at -1 to make the first frame index 0

vector<string> split(const string &input, const string &delimiter) {
  vector<string> ret;
  size_t start = 0;
  size_t end = input.find(delimiter);
  while (end != string::npos) {
    ret.push_back(input.substr(start, end - start));
    start = end + delimiter.length();
    end = input.find(delimiter, start);
  }
  ret.push_back(input.substr(start));
  return ret;
}

int main() {
  ifstream atp_file("/path/to/ascii_to_play.txt");
  ifstream as_file("/path/to/ascii_speed.txt");
  if (!atp_file.is_open() || !as_file.is_open()) {
    fprintf(stderr, "Error: Could not open config files.\n");
    return 1;
  }
  getline(atp_file, ascii_to_play_url);
  getline(as_file, speed_str);
  atp_file.close();
  as_file.close();

  ifstream video_file(ascii_to_play_url);
  if (!video_file.is_open()) {
    fprintf(stderr, "Error: Could not open video file: %s\n",
            ascii_to_play_url.c_str());
    return 1;
  }
  ascii_to_play.assign((istreambuf_iterator<char>(video_file)),
                       istreambuf_iterator<char>());
  video_file.close();

  try {
    speed = stoi(speed_str);
  } catch (...) {
    speed = 100;
  }

  vector<string> ascii_videos = split(ascii_to_play, "D");
  while (!ascii_videos.empty() && ascii_videos.back().empty()) {
    ascii_videos.pop_back();
  }

  if (ascii_videos.empty()) {
    fprintf(stderr, "Error: No frames found in %s\n",
            ascii_to_play_url.c_str());
    return 1;
  }

  // \033[?1049h: Alternate Screen Buffer
  // \033[?25l: Hide Cursor
  // \033[2J: Clear Screen
  // \033[H: Home
  printf("\033[?1049h\033[?25l\033[2J\033[H");
  fflush(stdout);

  auto next_frame_time = chrono::steady_clock::now();
  int frame_count = (int)ascii_videos.size();
  idx = 0;

  setvbuf(stdout, NULL, _IOFBF, 2048 * 2048);

  while (1) {
    // Move cursor to home (0,0) instead of clearing screen to prevent flicker
    printf("\033[H");

    const string &frame = ascii_videos.at(idx);
    // Print the frame directly
    fwrite(frame.c_str(), 1, frame.size(), stdout);

    fflush(stdout);

    idx = (idx + 1) % frame_count;

    next_frame_time += chrono::milliseconds(speed);
    this_thread::sleep_until(next_frame_time);

    if (chrono::steady_clock::now() >
        next_frame_time + chrono::milliseconds(speed)) {
      next_frame_time = chrono::steady_clock::now();
    }
  }

  return 0;
}
