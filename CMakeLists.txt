cmake_minimum_required(VERSION 3.5)

project (Spectrometer)

# Set Kromek Driver directory
set(DRIVER "~/Projects/Kromek/Driver")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-pthread -Wno-enum-compare")

include_directories(
	${DRIVER}/KromekDriver
	${DRIVER}/SpectrometerDriver
)

add_executable( ${PROJECT_NAME}
	main.cpp
	Kromek.hpp
	Kromek.cpp
	Helpers.hpp
	Helpers.cpp
)

target_link_libraries( ${PROJECT_NAME}
	${DRIVER}/output/linux64/release/libSpectrometerDriver.so
)
