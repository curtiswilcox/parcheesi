# Parcheesi - CS205 Final Project (Spring 2019)
- Ben Beaudin
- Bennett Cazayoux
- Curtis Wilcox

## Technical Requirements
- C++ 17
- Qt Graphics Library, version 5.12.1 (https://www.qt.io)
## Important Notes
- If you are utilizing a non-Mac machine, you will have to slightly update the `CMakeLists.txt` file if you wish to run the program. Replace the `DIRECTORY_HERE` (see the code shown below) note in the file with the location. Keep the quotation marks.
```
if (WIN32)
  # come back to this if it becomes necessary
  # set(CMAKE_PREFIX_PATH "DIRECTORY_HERE")
...
```

- If you cannot run `C++ 17` on your machine, change the `CMAKE_CXX_STANDARD` from `17` to `14`. You must then go into `Tile.h` and delete the line at the top that says `#include <optional>`, and replace it with `#include <experimental/optional>`. After the last `#include` line, type `using std::experimental::optional;` and `using std::experimental::nullopt;`.
