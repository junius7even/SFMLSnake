# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfml-graphics-src"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfml-graphics-build"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfml-graphics-subbuild/sfml-graphics-populate-prefix"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfml-graphics-subbuild/sfml-graphics-populate-prefix/tmp"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfml-graphics-subbuild/sfml-graphics-populate-prefix/src/sfml-graphics-populate-stamp"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfml-graphics-subbuild/sfml-graphics-populate-prefix/src"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfml-graphics-subbuild/sfml-graphics-populate-prefix/src/sfml-graphics-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfml-graphics-subbuild/sfml-graphics-populate-prefix/src/sfml-graphics-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfml-graphics-subbuild/sfml-graphics-populate-prefix/src/sfml-graphics-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
