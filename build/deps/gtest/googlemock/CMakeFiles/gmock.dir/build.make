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
include deps/gtest/googlemock/CMakeFiles/gmock.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include deps/gtest/googlemock/CMakeFiles/gmock.dir/compiler_depend.make

# Include the progress variables for this target.
include deps/gtest/googlemock/CMakeFiles/gmock.dir/progress.make

# Include the compile flags for this target's objects.
include deps/gtest/googlemock/CMakeFiles/gmock.dir/flags.make

deps/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o: deps/gtest/googlemock/CMakeFiles/gmock.dir/flags.make
deps/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o: ../deps/gtest/googlemock/src/gmock-all.cc
deps/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o: deps/gtest/googlemock/CMakeFiles/gmock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object deps/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/gtest/googlemock && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT deps/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o -MF CMakeFiles/gmock.dir/src/gmock-all.cc.o.d -o CMakeFiles/gmock.dir/src/gmock-all.cc.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/gtest/googlemock/src/gmock-all.cc

deps/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gmock.dir/src/gmock-all.cc.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/gtest/googlemock && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/gtest/googlemock/src/gmock-all.cc > CMakeFiles/gmock.dir/src/gmock-all.cc.i

deps/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gmock.dir/src/gmock-all.cc.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/gtest/googlemock && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/gtest/googlemock/src/gmock-all.cc -o CMakeFiles/gmock.dir/src/gmock-all.cc.s

# Object files for target gmock
gmock_OBJECTS = \
"CMakeFiles/gmock.dir/src/gmock-all.cc.o"

# External object files for target gmock
gmock_EXTERNAL_OBJECTS =

lib/libgmockd.a: deps/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
lib/libgmockd.a: deps/gtest/googlemock/CMakeFiles/gmock.dir/build.make
lib/libgmockd.a: deps/gtest/googlemock/CMakeFiles/gmock.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../lib/libgmockd.a"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/gtest/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock.dir/cmake_clean_target.cmake
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/gtest/googlemock && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gmock.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
deps/gtest/googlemock/CMakeFiles/gmock.dir/build: lib/libgmockd.a
.PHONY : deps/gtest/googlemock/CMakeFiles/gmock.dir/build

deps/gtest/googlemock/CMakeFiles/gmock.dir/clean:
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/gtest/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock.dir/cmake_clean.cmake
.PHONY : deps/gtest/googlemock/CMakeFiles/gmock.dir/clean

deps/gtest/googlemock/CMakeFiles/gmock.dir/depend:
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/deps/gtest/googlemock /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/gtest/googlemock /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/gtest/googlemock/CMakeFiles/gmock.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : deps/gtest/googlemock/CMakeFiles/gmock.dir/depend

