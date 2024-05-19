#include <cstdio>
#include <ncurses.h>
#include <filesystem>
#include <string>
#include <vector>

#define KEY_ENTER 10

namespace fs = std::filesystem;
using std::vector;
using std::string;

vector<fs::path> fetch_files(fs::path path) {
  vector<fs::path> result;

  for (const auto &entry : fs::directory_iterator(path)) {
    result.push_back(entry.path());
  }

  return result;
}

int main(int argc, char *argv[]) {
  // init screen with its properties
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  if (!has_colors()) {
    endwin();
    printf("Colors not supported. Exit...");
    exit(1);
  }

  start_color();

  // init color pairs
  init_pair(1, COLOR_BLACK, COLOR_WHITE);

  // pad window beginning
  const int min_row = 1, min_col = 1;

  int selected = 0;

  for(;;) {
    clear();
    int max_row, max_col;
    getmaxyx(stdscr, max_row, max_col);
    max_row--; max_col--;

    int row = min_row;
    int col = min_col;

    fs::path path = fs::current_path();
    vector<fs::path> files = fetch_files(path);

    move(0, 0);
    printw(path.c_str());

    int i = 0;
    for (const auto entry : files) {
      if (i++ == selected) {
        attron(COLOR_PAIR(1));
      } 
      move(row++, col);
      printw(entry.filename().c_str());
      attroff(COLOR_PAIR(1));
      refresh();
    }

    int k = getch();

    switch(k) {
      case (KEY_UP):
        if (selected > 0)
          selected--;
        break;
      case (KEY_DOWN):
        if (selected < files.size() - 1)
          selected++;
        break;
      case (KEY_LEFT):
        fs::current_path(path.parent_path());
        selected = 0;
        break;
      case (KEY_RIGHT):
      case (KEY_ENTER):
        if (fs::is_directory(files[selected])) {
          fs::current_path(files[selected]);
          selected = 0;
        }
        break;
    }

    if (k == 'q') {
      break;
    }

  }

  endwin();
}
