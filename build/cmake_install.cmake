# Install script for directory: /home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/bin/x86_64-linux-gnu-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/gtest/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/mb/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/deps/fmt/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Graphics/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Input/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Game/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/AI/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Carcassonne/Frontend/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/bin/carcassonne/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/bin/evolution/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Evolution/cmake_install.cmake")
  include("/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/src/Test/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/kmro/Documents/pw_mini/S1/msi/msi-carcassonne/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
