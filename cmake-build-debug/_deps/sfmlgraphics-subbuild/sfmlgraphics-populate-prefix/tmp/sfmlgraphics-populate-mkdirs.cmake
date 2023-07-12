# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfmlgraphics-src"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfmlgraphics-build"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfmlgraphics-subbuild/sfmlgraphics-populate-prefix"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfmlgraphics-subbuild/sfmlgraphics-populate-prefix/tmp"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfmlgraphics-subbuild/sfmlgraphics-populate-prefix/src/sfmlgraphics-populate-stamp"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfmlgraphics-subbuild/sfmlgraphics-populate-prefix/src"
  "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfmlgraphics-subbuild/sfmlgraphics-populate-prefix/src/sfmlgraphics-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfmlgraphics-subbuild/sfmlgraphics-populate-prefix/src/sfmlgraphics-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/GraphicsProjects/Junius7/cmake-build-debug/_deps/sfmlgraphics-subbuild/sfmlgraphics-populate-prefix/src/sfmlgraphics-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
