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
include src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/compiler_depend.make

# Include the progress variables for this target.
include src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/progress.make

# Include the compile flags for this target's objects.
include src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/flags.make

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/flags.make
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.o: ../src/Carcassonne/Frontend/GameView.cpp
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.o -MF CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.o.d -o CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/GameView.cpp

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/GameView.cpp > CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.i

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/GameView.cpp -o CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.s

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/flags.make
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.o: ../src/Carcassonne/Frontend/Resource.cpp
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.o -MF CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.o.d -o CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/Resource.cpp

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/Resource.cpp > CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.i

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/Resource.cpp -o CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.s

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/flags.make
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.o: ../src/Carcassonne/Frontend/BoardView.cpp
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.o -MF CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.o.d -o CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/BoardView.cpp

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/BoardView.cpp > CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.i

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/BoardView.cpp -o CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.s

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/flags.make
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.o: ../src/Carcassonne/Frontend/Object.cpp
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.o -MF CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.o.d -o CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/Object.cpp

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/Object.cpp > CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.i

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/Object.cpp -o CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.s

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/flags.make
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.o: ../src/Carcassonne/Frontend/PlaceTileView.cpp
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.o -MF CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.o.d -o CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/PlaceTileView.cpp

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/PlaceTileView.cpp > CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.i

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/PlaceTileView.cpp -o CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.s

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/flags.make
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.o: ../src/Carcassonne/Frontend/PlaceFigureView.cpp
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.o -MF CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.o.d -o CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/PlaceFigureView.cpp

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/PlaceFigureView.cpp > CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.i

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/PlaceFigureView.cpp -o CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.s

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/flags.make
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.o: ../src/Carcassonne/Frontend/FiguresView.cpp
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.o -MF CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.o.d -o CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/FiguresView.cpp

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/FiguresView.cpp > CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.i

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/FiguresView.cpp -o CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.s

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/flags.make
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.o: ../src/Carcassonne/Frontend/ScoreBoardView.cpp
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.o: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.o"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.o -MF CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.o.d -o CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.o -c /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/ScoreBoardView.cpp

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.i"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/ScoreBoardView.cpp > CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.i

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.s"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend/ScoreBoardView.cpp -o CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.s

# Object files for target msi_carcassonne_frontend
msi_carcassonne_frontend_OBJECTS = \
"CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.o" \
"CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.o" \
"CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.o" \
"CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.o" \
"CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.o" \
"CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.o" \
"CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.o" \
"CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.o"

# External object files for target msi_carcassonne_frontend
msi_carcassonne_frontend_EXTERNAL_OBJECTS =

src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/GameView.cpp.o
src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Resource.cpp.o
src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/BoardView.cpp.o
src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/Object.cpp.o
src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceTileView.cpp.o
src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/PlaceFigureView.cpp.o
src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/FiguresView.cpp.o
src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/ScoreBoardView.cpp.o
src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/build.make
src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a: src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX static library libmsi_carcassonne_frontend.a"
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && $(CMAKE_COMMAND) -P CMakeFiles/msi_carcassonne_frontend.dir/cmake_clean_target.cmake
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/msi_carcassonne_frontend.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/build: src/Carcassonne/Frontend/libmsi_carcassonne_frontend.a
.PHONY : src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/build

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/clean:
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend && $(CMAKE_COMMAND) -P CMakeFiles/msi_carcassonne_frontend.dir/cmake_clean.cmake
.PHONY : src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/clean

src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/depend:
	cd /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/src/Carcassonne/Frontend /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/Carcassonne/Frontend/CMakeFiles/msi_carcassonne_frontend.dir/depend

