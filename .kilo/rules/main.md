# Tests

- Before running tests, make sure that they are built with CMake (CMake subdirectory tests)
- Tests are run by launching test executable (CMake target widget_tests)
- When running tests, run them from project root, not build/ folder
- Use `--no-crt-dialog` argument to run test executable so that Windows CRT dialogs will not be shown
