# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gregory/Projects/DecisionTreeClassifier

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gregory/Projects/DecisionTreeClassifier/build

# Utility rule file for aws-lambda-package-decisiontreeclassifier.

# Include the progress variables for this target.
include CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/progress.make

CMakeFiles/aws-lambda-package-decisiontreeclassifier: decisiontreeclassifier
	/home/gregory/out/lib/aws-lambda-runtime/cmake/packager /home/gregory/Projects/DecisionTreeClassifier/build/decisiontreeclassifier

aws-lambda-package-decisiontreeclassifier: CMakeFiles/aws-lambda-package-decisiontreeclassifier
aws-lambda-package-decisiontreeclassifier: CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/build.make

.PHONY : aws-lambda-package-decisiontreeclassifier

# Rule to build all files generated by this target.
CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/build: aws-lambda-package-decisiontreeclassifier

.PHONY : CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/build

CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/cmake_clean.cmake
.PHONY : CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/clean

CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/depend:
	cd /home/gregory/Projects/DecisionTreeClassifier/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gregory/Projects/DecisionTreeClassifier /home/gregory/Projects/DecisionTreeClassifier /home/gregory/Projects/DecisionTreeClassifier/build /home/gregory/Projects/DecisionTreeClassifier/build /home/gregory/Projects/DecisionTreeClassifier/build/CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/aws-lambda-package-decisiontreeclassifier.dir/depend
