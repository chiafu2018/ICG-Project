# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/edkoz/Documents/CG/ICG_2024_HW3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build

# Include any dependencies generated for this target.
include extern/glad/CMakeFiles/glad.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include extern/glad/CMakeFiles/glad.dir/compiler_depend.make

# Include the progress variables for this target.
include extern/glad/CMakeFiles/glad.dir/progress.make

# Include the compile flags for this target's objects.
include extern/glad/CMakeFiles/glad.dir/flags.make

extern/glad/CMakeFiles/glad.dir/src/glad.c.obj: extern/glad/CMakeFiles/glad.dir/flags.make
extern/glad/CMakeFiles/glad.dir/src/glad.c.obj: extern/glad/CMakeFiles/glad.dir/includes_C.rsp
extern/glad/CMakeFiles/glad.dir/src/glad.c.obj: C:/Users/edkoz/Documents/CG/ICG_2024_HW3/extern/glad/src/glad.c
extern/glad/CMakeFiles/glad.dir/src/glad.c.obj: extern/glad/CMakeFiles/glad.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object extern/glad/CMakeFiles/glad.dir/src/glad.c.obj"
	cd C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/extern/glad && "C:/Program Files/w64devkit/bin/gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT extern/glad/CMakeFiles/glad.dir/src/glad.c.obj -MF CMakeFiles/glad.dir/src/glad.c.obj.d -o CMakeFiles/glad.dir/src/glad.c.obj -c C:/Users/edkoz/Documents/CG/ICG_2024_HW3/extern/glad/src/glad.c

extern/glad/CMakeFiles/glad.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/glad.dir/src/glad.c.i"
	cd C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/extern/glad && "C:/Program Files/w64devkit/bin/gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:/Users/edkoz/Documents/CG/ICG_2024_HW3/extern/glad/src/glad.c > CMakeFiles/glad.dir/src/glad.c.i

extern/glad/CMakeFiles/glad.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/glad.dir/src/glad.c.s"
	cd C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/extern/glad && "C:/Program Files/w64devkit/bin/gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:/Users/edkoz/Documents/CG/ICG_2024_HW3/extern/glad/src/glad.c -o CMakeFiles/glad.dir/src/glad.c.s

# Object files for target glad
glad_OBJECTS = \
"CMakeFiles/glad.dir/src/glad.c.obj"

# External object files for target glad
glad_EXTERNAL_OBJECTS =

extern/glad/libglad.a: extern/glad/CMakeFiles/glad.dir/src/glad.c.obj
extern/glad/libglad.a: extern/glad/CMakeFiles/glad.dir/build.make
extern/glad/libglad.a: extern/glad/CMakeFiles/glad.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libglad.a"
	cd C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/extern/glad && $(CMAKE_COMMAND) -P CMakeFiles/glad.dir/cmake_clean_target.cmake
	cd C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/extern/glad && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glad.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
extern/glad/CMakeFiles/glad.dir/build: extern/glad/libglad.a
.PHONY : extern/glad/CMakeFiles/glad.dir/build

extern/glad/CMakeFiles/glad.dir/clean:
	cd C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/extern/glad && $(CMAKE_COMMAND) -P CMakeFiles/glad.dir/cmake_clean.cmake
.PHONY : extern/glad/CMakeFiles/glad.dir/clean

extern/glad/CMakeFiles/glad.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/edkoz/Documents/CG/ICG_2024_HW3 C:/Users/edkoz/Documents/CG/ICG_2024_HW3/extern/glad C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/extern/glad C:/Users/edkoz/Documents/CG/ICG_2024_HW3/build/extern/glad/CMakeFiles/glad.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : extern/glad/CMakeFiles/glad.dir/depend

