# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build

# Include any dependencies generated for this target.
include deps/mb/CMakeFiles/libmb.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include deps/mb/CMakeFiles/libmb.dir/compiler_depend.make

# Include the progress variables for this target.
include deps/mb/CMakeFiles/libmb.dir/progress.make

# Include the compile flags for this target's objects.
include deps/mb/CMakeFiles/libmb.dir/flags.make

deps/mb/CMakeFiles/libmb.dir/src/error.cpp.o: deps/mb/CMakeFiles/libmb.dir/flags.make
deps/mb/CMakeFiles/libmb.dir/src/error.cpp.o: ../deps/mb/src/error.cpp
deps/mb/CMakeFiles/libmb.dir/src/error.cpp.o: deps/mb/CMakeFiles/libmb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object deps/mb/CMakeFiles/libmb.dir/src/error.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT deps/mb/CMakeFiles/libmb.dir/src/error.cpp.o -MF CMakeFiles/libmb.dir/src/error.cpp.o.d -o CMakeFiles/libmb.dir/src/error.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/mb/src/error.cpp

deps/mb/CMakeFiles/libmb.dir/src/error.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libmb.dir/src/error.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/mb/src/error.cpp > CMakeFiles/libmb.dir/src/error.cpp.i

deps/mb/CMakeFiles/libmb.dir/src/error.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libmb.dir/src/error.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/mb/src/error.cpp -o CMakeFiles/libmb.dir/src/error.cpp.s

deps/mb/CMakeFiles/libmb.dir/src/core.cpp.o: deps/mb/CMakeFiles/libmb.dir/flags.make
deps/mb/CMakeFiles/libmb.dir/src/core.cpp.o: ../deps/mb/src/core.cpp
deps/mb/CMakeFiles/libmb.dir/src/core.cpp.o: deps/mb/CMakeFiles/libmb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object deps/mb/CMakeFiles/libmb.dir/src/core.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT deps/mb/CMakeFiles/libmb.dir/src/core.cpp.o -MF CMakeFiles/libmb.dir/src/core.cpp.o.d -o CMakeFiles/libmb.dir/src/core.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/mb/src/core.cpp

deps/mb/CMakeFiles/libmb.dir/src/core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libmb.dir/src/core.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/mb/src/core.cpp > CMakeFiles/libmb.dir/src/core.cpp.i

deps/mb/CMakeFiles/libmb.dir/src/core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libmb.dir/src/core.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/mb/src/core.cpp -o CMakeFiles/libmb.dir/src/core.cpp.s

# Object files for target libmb
libmb_OBJECTS = \
"CMakeFiles/libmb.dir/src/error.cpp.o" \
"CMakeFiles/libmb.dir/src/core.cpp.o"

# External object files for target libmb
libmb_EXTERNAL_OBJECTS =

deps/mb/liblibmb.a: deps/mb/CMakeFiles/libmb.dir/src/error.cpp.o
deps/mb/liblibmb.a: deps/mb/CMakeFiles/libmb.dir/src/core.cpp.o
deps/mb/liblibmb.a: deps/mb/CMakeFiles/libmb.dir/build.make
deps/mb/liblibmb.a: deps/mb/CMakeFiles/libmb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library liblibmb.a"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb && $(CMAKE_COMMAND) -P CMakeFiles/libmb.dir/cmake_clean_target.cmake
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libmb.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
deps/mb/CMakeFiles/libmb.dir/build: deps/mb/liblibmb.a
.PHONY : deps/mb/CMakeFiles/libmb.dir/build

deps/mb/CMakeFiles/libmb.dir/clean:
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb && $(CMAKE_COMMAND) -P CMakeFiles/libmb.dir/cmake_clean.cmake
.PHONY : deps/mb/CMakeFiles/libmb.dir/clean

deps/mb/CMakeFiles/libmb.dir/depend:
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/mb /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb/CMakeFiles/libmb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : deps/mb/CMakeFiles/libmb.dir/depend

