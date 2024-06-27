cmake_minimum_required(VERSION 3.1)

project (spectrometer)

# Enable pthread library
if (UNIX)
	set(CMAKE_CXX_FLAGS "-pthread -Wno-enum-compare")
endif()

include_directories(
	../KromekDriver/include 
	../SpectrometerDriver
)

add_executable( ${PROJECT_NAME}
	main.cpp
	Kromek.hpp
	Kromek.cpp
)

target_link_libraries(
	SpectrometerExample 
	SpectrometerDriver
)
