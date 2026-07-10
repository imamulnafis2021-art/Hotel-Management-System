Windows (MinGW / Git Bash):
    g++ -std=c++17 -Wall -O2 -Iinclude ^
        src/main.cpp src/Utility.cpp src/Room.cpp ^
        src/Customer.cpp src/Staff.cpp ^
        src/FileManager.cpp src/HotelManagementSystem.cpp ^
        -o HotelMS.exe
    HotelMS.exe

  Clean rebuild:
    make clean && make

DEFAULT LOGINS
──────────────
  admin         admin123    (Full access)
  receptionist  recep456    (Front desk)
  manager       mgr789      (Reports & staff)

DATA FOLDER
───────────
  Created automatically on first run as: data/
  Contains all .txt storage files and invoices.
  Do NOT manually edit counters.txt.

COMPILER REQUIREMENTS
─────────────────────
  GCC 7+ with -std=c++17 flag
  Tested: GCC 11, GCC 12, GCC 13, Clang 14

PROJECT STRUCTURE
─────────────────
  HotelMS/
  ├── include/       ← All header files (.h)
  ├── src/           ← All source files (.cpp)
  ├── data/          ← Created at runtime
  ├── docs/          ← All documentation
  └── Makefile

SOURCE FILES
────────────
  include/Utility.h              Utility.cpp
  include/Person.h               (abstract base - header only)
  include/Room.h                 Room.cpp
  include/Customer.h             Customer.cpp
  include/Staff.h                Staff.cpp
  include/Booking.h              (structs - header only)
  include/FileManager.h          FileManager.cpp
  include/HotelManagementSystem.h   HotelManagementSystem.cpp
                                 main.cpp

DOCUMENTATION
─────────────
  docs/ProjectReport.txt         Full academic project report
  docs/UserManual.txt            Step-by-step user guide
  docs/VivaQA.txt                25 viva questions + answers
  docs/Synopsis.txt              Project synopsis (1 page)
  docs/PresentationScript.txt    PPT slide content (12 slides)
  docs/Diagrams.txt              Use Case, Class, ER, Flowcharts
  docs/SampleData.txt            Test data for demo

COMPILE VERIFICATION
─────────────────────
  g++ -std=c++17 -Wall -Wextra -O2 -Iinclude
  Result: 0 errors, 0 warnings ✓
