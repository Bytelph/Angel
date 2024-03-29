# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bytecat/桌面/Angel

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bytecat/桌面/Angel

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/bytecat/桌面/Angel/CMakeFiles /home/bytecat/桌面/Angel/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/bytecat/桌面/Angel/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named config_test

# Build rule for target.
config_test: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 config_test
.PHONY : config_test

# fast build rule for target.
config_test/fast:
	$(MAKE) -f CMakeFiles/config_test.dir/build.make CMakeFiles/config_test.dir/build
.PHONY : config_test/fast

#=============================================================================
# Target rules for targets named test

# Build rule for target.
test: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 test
.PHONY : test

# fast build rule for target.
test/fast:
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/build
.PHONY : test/fast

#=============================================================================
# Target rules for targets named angel

# Build rule for target.
angel: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 angel
.PHONY : angel

# fast build rule for target.
angel/fast:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/build
.PHONY : angel/fast

angel/config.o: angel/config.cpp.o

.PHONY : angel/config.o

# target to build an object file
angel/config.cpp.o:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/angel/config.cpp.o
.PHONY : angel/config.cpp.o

angel/config.i: angel/config.cpp.i

.PHONY : angel/config.i

# target to preprocess a source file
angel/config.cpp.i:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/angel/config.cpp.i
.PHONY : angel/config.cpp.i

angel/config.s: angel/config.cpp.s

.PHONY : angel/config.s

# target to generate assembly for a file
angel/config.cpp.s:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/angel/config.cpp.s
.PHONY : angel/config.cpp.s

angel/log.o: angel/log.cpp.o

.PHONY : angel/log.o

# target to build an object file
angel/log.cpp.o:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/angel/log.cpp.o
.PHONY : angel/log.cpp.o

angel/log.i: angel/log.cpp.i

.PHONY : angel/log.i

# target to preprocess a source file
angel/log.cpp.i:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/angel/log.cpp.i
.PHONY : angel/log.cpp.i

angel/log.s: angel/log.cpp.s

.PHONY : angel/log.s

# target to generate assembly for a file
angel/log.cpp.s:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/angel/log.cpp.s
.PHONY : angel/log.cpp.s

angel/util.o: angel/util.cpp.o

.PHONY : angel/util.o

# target to build an object file
angel/util.cpp.o:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/angel/util.cpp.o
.PHONY : angel/util.cpp.o

angel/util.i: angel/util.cpp.i

.PHONY : angel/util.i

# target to preprocess a source file
angel/util.cpp.i:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/angel/util.cpp.i
.PHONY : angel/util.cpp.i

angel/util.s: angel/util.cpp.s

.PHONY : angel/util.s

# target to generate assembly for a file
angel/util.cpp.s:
	$(MAKE) -f CMakeFiles/angel.dir/build.make CMakeFiles/angel.dir/angel/util.cpp.s
.PHONY : angel/util.cpp.s

tests/config_test.o: tests/config_test.cpp.o

.PHONY : tests/config_test.o

# target to build an object file
tests/config_test.cpp.o:
	$(MAKE) -f CMakeFiles/config_test.dir/build.make CMakeFiles/config_test.dir/tests/config_test.cpp.o
.PHONY : tests/config_test.cpp.o

tests/config_test.i: tests/config_test.cpp.i

.PHONY : tests/config_test.i

# target to preprocess a source file
tests/config_test.cpp.i:
	$(MAKE) -f CMakeFiles/config_test.dir/build.make CMakeFiles/config_test.dir/tests/config_test.cpp.i
.PHONY : tests/config_test.cpp.i

tests/config_test.s: tests/config_test.cpp.s

.PHONY : tests/config_test.s

# target to generate assembly for a file
tests/config_test.cpp.s:
	$(MAKE) -f CMakeFiles/config_test.dir/build.make CMakeFiles/config_test.dir/tests/config_test.cpp.s
.PHONY : tests/config_test.cpp.s

tests/log_test.o: tests/log_test.cpp.o

.PHONY : tests/log_test.o

# target to build an object file
tests/log_test.cpp.o:
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/tests/log_test.cpp.o
.PHONY : tests/log_test.cpp.o

tests/log_test.i: tests/log_test.cpp.i

.PHONY : tests/log_test.i

# target to preprocess a source file
tests/log_test.cpp.i:
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/tests/log_test.cpp.i
.PHONY : tests/log_test.cpp.i

tests/log_test.s: tests/log_test.cpp.s

.PHONY : tests/log_test.s

# target to generate assembly for a file
tests/log_test.cpp.s:
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/tests/log_test.cpp.s
.PHONY : tests/log_test.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... config_test"
	@echo "... edit_cache"
	@echo "... test"
	@echo "... angel"
	@echo "... angel/config.o"
	@echo "... angel/config.i"
	@echo "... angel/config.s"
	@echo "... angel/log.o"
	@echo "... angel/log.i"
	@echo "... angel/log.s"
	@echo "... angel/util.o"
	@echo "... angel/util.i"
	@echo "... angel/util.s"
	@echo "... tests/config_test.o"
	@echo "... tests/config_test.i"
	@echo "... tests/config_test.s"
	@echo "... tests/log_test.o"
	@echo "... tests/log_test.i"
	@echo "... tests/log_test.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

