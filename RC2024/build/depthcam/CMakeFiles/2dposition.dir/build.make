# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/wheeltec/RC2024new/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wheeltec/RC2024new/build

# Include any dependencies generated for this target.
include depthcam/CMakeFiles/2dposition.dir/depend.make

# Include the progress variables for this target.
include depthcam/CMakeFiles/2dposition.dir/progress.make

# Include the compile flags for this target's objects.
include depthcam/CMakeFiles/2dposition.dir/flags.make

depthcam/CMakeFiles/2dposition.dir/src/2dposition.cpp.o: depthcam/CMakeFiles/2dposition.dir/flags.make
depthcam/CMakeFiles/2dposition.dir/src/2dposition.cpp.o: /home/wheeltec/RC2024new/src/depthcam/src/2dposition.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wheeltec/RC2024new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object depthcam/CMakeFiles/2dposition.dir/src/2dposition.cpp.o"
	cd /home/wheeltec/RC2024new/build/depthcam && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/2dposition.dir/src/2dposition.cpp.o -c /home/wheeltec/RC2024new/src/depthcam/src/2dposition.cpp

depthcam/CMakeFiles/2dposition.dir/src/2dposition.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/2dposition.dir/src/2dposition.cpp.i"
	cd /home/wheeltec/RC2024new/build/depthcam && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wheeltec/RC2024new/src/depthcam/src/2dposition.cpp > CMakeFiles/2dposition.dir/src/2dposition.cpp.i

depthcam/CMakeFiles/2dposition.dir/src/2dposition.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/2dposition.dir/src/2dposition.cpp.s"
	cd /home/wheeltec/RC2024new/build/depthcam && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wheeltec/RC2024new/src/depthcam/src/2dposition.cpp -o CMakeFiles/2dposition.dir/src/2dposition.cpp.s

# Object files for target 2dposition
2dposition_OBJECTS = \
"CMakeFiles/2dposition.dir/src/2dposition.cpp.o"

# External object files for target 2dposition
2dposition_EXTERNAL_OBJECTS =

/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: depthcam/CMakeFiles/2dposition.dir/src/2dposition.cpp.o
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: depthcam/CMakeFiles/2dposition.dir/build.make
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /home/wheeltec/wheeltec_robot/devel/lib/libcv_bridge.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_calib3d.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_dnn.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_features2d.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_flann.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_gapi.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_highgui.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_ml.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_objdetect.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_photo.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_stitching.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_video.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_videoio.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_core.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_imgproc.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libopencv_imgcodecs.so.4.5.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/libimage_transport.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/libmessage_filters.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/libclass_loader.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libPocoFoundation.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libdl.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/libroscpp.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libpthread.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libboost_chrono.so.1.71.0
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/librosconsole.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/librosconsole_log4cxx.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/librosconsole_backend_interface.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/liblog4cxx.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libboost_regex.so.1.71.0
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/libxmlrpcpp.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/libroslib.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/librospack.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libpython3.8.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libboost_filesystem.so.1.71.0
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libboost_program_options.so.1.71.0
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libtinyxml2.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/libroscpp_serialization.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/librostime.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libboost_date_time.so.1.71.0
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /opt/ros/noetic/lib/libcpp_common.so
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libboost_system.so.1.71.0
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libboost_thread.so.1.71.0
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: /usr/lib/aarch64-linux-gnu/libconsole_bridge.so.0.4
/home/wheeltec/RC2024new/devel/lib/depthcam/2dposition: depthcam/CMakeFiles/2dposition.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wheeltec/RC2024new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/wheeltec/RC2024new/devel/lib/depthcam/2dposition"
	cd /home/wheeltec/RC2024new/build/depthcam && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/2dposition.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
depthcam/CMakeFiles/2dposition.dir/build: /home/wheeltec/RC2024new/devel/lib/depthcam/2dposition

.PHONY : depthcam/CMakeFiles/2dposition.dir/build

depthcam/CMakeFiles/2dposition.dir/clean:
	cd /home/wheeltec/RC2024new/build/depthcam && $(CMAKE_COMMAND) -P CMakeFiles/2dposition.dir/cmake_clean.cmake
.PHONY : depthcam/CMakeFiles/2dposition.dir/clean

depthcam/CMakeFiles/2dposition.dir/depend:
	cd /home/wheeltec/RC2024new/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wheeltec/RC2024new/src /home/wheeltec/RC2024new/src/depthcam /home/wheeltec/RC2024new/build /home/wheeltec/RC2024new/build/depthcam /home/wheeltec/RC2024new/build/depthcam/CMakeFiles/2dposition.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : depthcam/CMakeFiles/2dposition.dir/depend

