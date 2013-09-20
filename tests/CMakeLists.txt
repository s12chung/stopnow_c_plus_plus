#For unit testing, based on http://svn.mech.kuleuven.be/websvn/orocos/trunk/kdl/

#CppUnit library and include dir
find_library(cppunitLib cppunit)
find_path(cppunitInclude cppunit/TestRunner.h)
if (NOT (cppunitLib AND cppunitInclude))	
	message(STATUS "Cppunit not found. Unit test building stopped.")
	return()
endif(NOT (cppunitLib AND cppunitInclude))

enable_testing()

#Structs.cpp
set (testName StructsTest)
set ( testSrc
	StructsTest.cpp
	../Structs.cpp
	TestRunner.cpp
)
add_executable(${testName} ${testSrc})
target_link_libraries (${testName} ${cppunitLib})
target_link_libraries (${testName} ${Boost_LIBRARIES})
add_test(${testName} ${testName})

