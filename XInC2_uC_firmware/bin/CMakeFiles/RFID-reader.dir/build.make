# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.1

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\ProgramData\chocolatey\lib\cmake.3.1.3\content\cmake-3.1.3-win32-x86\bin\cmake.exe

# The command to remove a file.
RM = C:\ProgramData\chocolatey\lib\cmake.3.1.3\content\cmake-3.1.3-win32-x86\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Andrew\Documents\GitHub\RFID-reader\XInC2_uC_firmware

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Andrew\Documents\GitHub\RFID-reader\XInC2_uC_firmware\bin

# Include any dependencies generated for this target.
include CMakeFiles/RFID-reader.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/RFID-reader.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RFID-reader.dir/flags.make

CMakeFiles/RFID-reader.dir/main.c.obj: CMakeFiles/RFID-reader.dir/flags.make
CMakeFiles/RFID-reader.dir/main.c.obj: ../main.c
	$(CMAKE_COMMAND) -E cmake_progress_report C:\Users\Andrew\Documents\GitHub\RFID-reader\XInC2_uC_firmware\bin\CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/RFID-reader.dir/main.c.obj"
	C:\PROGRA~2\SXC\bin\SXC7F3~1.EXE  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles\RFID-reader.dir\main.c.obj   -c C:\Users\Andrew\Documents\GitHub\RFID-reader\XInC2_uC_firmware\main.c

CMakeFiles/RFID-reader.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RFID-reader.dir/main.c.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_C_CREATE_PREPROCESSED_SOURCE

CMakeFiles/RFID-reader.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RFID-reader.dir/main.c.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_C_CREATE_ASSEMBLY_SOURCE

CMakeFiles/RFID-reader.dir/main.c.obj.requires:
.PHONY : CMakeFiles/RFID-reader.dir/main.c.obj.requires

CMakeFiles/RFID-reader.dir/main.c.obj.provides: CMakeFiles/RFID-reader.dir/main.c.obj.requires
	$(MAKE) -f CMakeFiles\RFID-reader.dir\build.make CMakeFiles/RFID-reader.dir/main.c.obj.provides.build
.PHONY : CMakeFiles/RFID-reader.dir/main.c.obj.provides

CMakeFiles/RFID-reader.dir/main.c.obj.provides.build: CMakeFiles/RFID-reader.dir/main.c.obj

# Object files for target RFID-reader
RFID__reader_OBJECTS = \
"CMakeFiles/RFID-reader.dir/main.c.obj"

# External object files for target RFID-reader
RFID__reader_EXTERNAL_OBJECTS =

RFID-reader.a: CMakeFiles/RFID-reader.dir/main.c.obj
RFID-reader.a: CMakeFiles/RFID-reader.dir/build.make
RFID-reader.a: CMakeFiles/RFID-reader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable RFID-reader.a"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\RFID-reader.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RFID-reader.dir/build: RFID-reader.a
.PHONY : CMakeFiles/RFID-reader.dir/build

CMakeFiles/RFID-reader.dir/requires: CMakeFiles/RFID-reader.dir/main.c.obj.requires
.PHONY : CMakeFiles/RFID-reader.dir/requires

CMakeFiles/RFID-reader.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\RFID-reader.dir\cmake_clean.cmake
.PHONY : CMakeFiles/RFID-reader.dir/clean

CMakeFiles/RFID-reader.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Andrew\Documents\GitHub\RFID-reader\XInC2_uC_firmware C:\Users\Andrew\Documents\GitHub\RFID-reader\XInC2_uC_firmware C:\Users\Andrew\Documents\GitHub\RFID-reader\XInC2_uC_firmware\bin C:\Users\Andrew\Documents\GitHub\RFID-reader\XInC2_uC_firmware\bin C:\Users\Andrew\Documents\GitHub\RFID-reader\XInC2_uC_firmware\bin\CMakeFiles\RFID-reader.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RFID-reader.dir/depend

