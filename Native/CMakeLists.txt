CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
PROJECT(zlib) # .sln

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SUBDIRS(
    zlib-1.2.11
    RawInflate
    zip_test
    )

