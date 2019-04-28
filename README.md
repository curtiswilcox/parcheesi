# Parcheesi - CS205 Final Project (Spring 2019)
- Ben Beaudin
- Bennett Cazayoux
- Curtis Wilcox: Curtis.Wilcox@uvm.edu

## Technical Requirements
- C++ 17
- Qt Graphics Library, version 5.12.1 (https://www.qt.io)
  - It is free. You should only need to download the code specific to your machine (in the installer helper, you can choose which parts to download). It should only be one checkbox, if I remember correctly for when I downloaded it. If you download a newer version, you'll have to adjust the `CMAKE_PREFIX_PATH` slightly (in `CMakeLists.txt`).
  - If you have a Mac, please download the files to your `/Users/USERNAME/` directory (if you put it elsewhere, you will have to edit the `CMAKE_PREFIX_PATH` in the aforementioned file to deal with that difference as well).
## Important Notes
- If you are utilizing a non-Mac machine, you will have to slightly update the `CMakeLists.txt` file if you wish to run the program. Replace the `DIRECTORY_HERE` (see the code shown below) note in the file with the location. Keep the quotation marks.
```
if (WIN32)
  # come back to this if it becomes necessary
  # set(CMAKE_PREFIX_PATH "DIRECTORY_HERE")
...
```

- If you cannot run `C++ 17` on your machine, change the `CMAKE_CXX_STANDARD` from `17` to `14` (in `CMakeLists.txt`). You must then go into `Tile.h` and delete the line at the top that says `#include <optional>`, and replace it with `#include <experimental/optional>`. After the last `#include` line in the file, type `using std::experimental::optional;` and `using std::experimental::nullopt;`.

- If the program will still not run, we apologize for the inconvenience it is causing. Jason might be able to help (unsure about his C++ knowledge), and you are certainly welcome to email Curtis if nothing is going your way.
