/*
 * Copyright (c) 2018 ViralTaco.
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * <http://www.gnu.org/licenses/agpl.txt>
 */

#include "toast.hpp"
#include <cstring>

int main(int argc, char *argv[]) {
  static const char* kUser = std::getenv("USER");
  if (strcmp(kUser, "root") == 0) {
    std::cout << "Toast: who dis?\n";
    std::cerr << "Toast: error: cannot run as root\n";
    std::exit(EXIT_FAILURE);
  }

  if (argc == 3) {
    vt::toast the_toast(argv[1], argv[2]);
    the_toast.print();
  } else if (argc == 2) {
    vt::toast the_clear_toast(argv[1]);
  } else if (argc == 1) {
    vt::toast the_toast;
    the_toast.print();
  }
}
