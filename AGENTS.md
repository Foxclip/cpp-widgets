# Tests

- Before running tests, make sure that they are built with CMake (CMake subdirectory tests)
- Tests are run by launching test executable (CMake target widget_tests)
- When running tests, run them from project root, not build/ folder
- When running tests executable, launch it minimized with `--minimized` flag
- When running tests executable, launch it in foreground unless absolutely necessary by the nature of the task, or explicitly requested by the user
- Use `--no-crt-dialog` argument to run test executable so that Windows CRT dialogs will not be shown
- If you want to take a screenshot of a test, you can use glvx::Window::saveScreenshot() method by injecting it into test

# Sandbox

- When running sandbox executable, launch it minimized with `--minimized` and `--no-crt-dialog` flags
- When running sandbox executable, launch it in foreground unless absolutely necessary by the nature of the task, or explicitly requested by the user
- If you want to take a screenshot, use `--screenshot` flag

# Debugging

- Run debug builds (tests, sandbox) under cdb (`C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe`) so crashes leave a symbolized stack: `cdb.exe -c "g; kv 40; q" <exe> <args> > log.txt 2>&1` — breaks only on fatal exceptions, dumps a 40-frame stack, then exits. No special build needed (cdb picks up the PDB next to the exe)
- Tests: run in the foreground as usual. Sandbox: run in the background with output to a log file (it runs until the window is closed)
- Skip cdb for timing/performance measurements, VTune profiling, and ASAN runs
- Use ASAN when chasing a suspected memory bug (use-after-free, buffer overflow, intermittent AV where cdb shows a bad pointer but not where the memory was allocated/freed), or when verifying a fix for a memory bug
- ASAN: MSVC has native `/fsanitize=address` support (x64) — configure a separate build directory with `-DCMAKE_CXX_FLAGS="/fsanitize=address"` (applies to compile + link) and run the exe like any normal build; the ASAN report in the output is the diagnostic (no cdb needed). If the runtime DLL is not found, add `VC\Tools\MSVC\<version>\bin\Hostx64\x64` to PATH
