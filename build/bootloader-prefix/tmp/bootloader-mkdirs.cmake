# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Santycadavid10/esp/v5.2.1/esp-idf/components/bootloader/subproject"
  "C:/Users/Santycadavid10/workspace/.metadata/luces/build/bootloader"
  "C:/Users/Santycadavid10/workspace/.metadata/luces/build/bootloader-prefix"
  "C:/Users/Santycadavid10/workspace/.metadata/luces/build/bootloader-prefix/tmp"
  "C:/Users/Santycadavid10/workspace/.metadata/luces/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/Santycadavid10/workspace/.metadata/luces/build/bootloader-prefix/src"
  "C:/Users/Santycadavid10/workspace/.metadata/luces/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Santycadavid10/workspace/.metadata/luces/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Santycadavid10/workspace/.metadata/luces/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
