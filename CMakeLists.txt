###########################################
#Overview
###########################################
cmake_minimum_required (VERSION 2.6)

project (StopNow CXX)
set (exeName ${PROJECT_NAME})

#Messages
if (APPLE)
	message(STATUS "Mac Found!")
elseif (CYGWIN)
	message(STATUS "Cygwin Found!")
elseif (MINGW)
	message(STATUS "Mingw Found!")
elseif (UNIX)
  message(STATUS "Linux Found!")
endif (APPLE)

message("Build Type: " ${CMAKE_BUILD_TYPE})

message("Debug Build Flags: " ${CMAKE_C_FLAGS_DEBUG})
message("Release Build Flags: " ${CMAKE_C_FLAGS_RELEASE})

###########################################
#Packages
#See http://www.itk.org/Wiki/CMake_2.6.1_Docs
###########################################
set(boostComponents
	date_time
	python
	regex
	thread
)
find_package(Boost 1.38 REQUIRED ${boostComponents})
if(Boost_FOUND)
	link_directories(${Boost_LIBRARY_DIRS})
	include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

find_package(PythonLibs REQUIRED)
if(PYTHONLIBS_FOUND)
	link_directories(${PYTHON_LIBRARIES})
	include_directories(${PYTHON_INCLUDE_PATH})
endif(PYTHONLIBS_FOUND)

###########################################
#Main project details
###########################################
#Project include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib)

#Take all srcs in directory, but must run cmake manually for this to work (refreshes cache)
aux_source_directory(. currentDirSrc)
add_executable(${exeName} ${currentDirSrc})

target_link_libraries (${exeName} ${Boost_LIBRARIES})
target_link_libraries (${exeName} ${PYTHON_LIBRARIES})

###########################################
#Doxygen
###########################################
#Generate doxygen doc as part of "all" target
option(doxygenAll "Doxygen documentation part of all target" OFF)
set(doxygenTargetName doc)
set(doxygenCommand doxygen Doxyfile)
set(doxygenCommandDir ./doc)
set(doxygenLogFile doxygen.log)
if (doxygenAll)
	#Target Doc, which calls doxygen command, depends on project to run last
	add_custom_target(${doxygenTargetName} ALL
		COMMAND echo "-----------------Doxygen generation output-----------------" >> ${doxygenLogFile}
		COMMAND date >> ${doxygenLogFile}
		COMMAND ${doxygenCommand} >> ${doxygenLogFile} 2>&1
		COMMAND echo "Doxygen: Standard output and error redirected to ./doc/doxygen.log."
		WORKING_DIRECTORY ${doxygenCommandDir}
		VERBATIM
		)
else (doxygenAll)
	#Not part all "all" target
	add_custom_target(${doxygenTargetName} 
		COMMAND ${doxygenCommand}
		WORKING_DIRECTORY ${doxygenCommandDir}
		VERBATIM
	)
endif (doxygenAll)

###########################################
#Unit tests
###########################################
#Add unit tests
option(enableUnitTests "Enable unit test building" ON)
if (enableUnitTests)
	add_subdirectory(tests)
	
endif (enableUnitTests)
