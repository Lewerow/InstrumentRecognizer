CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (InstrumentRecognitionSystem)

SET(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/../bin)
SET( IRS_INSTALL_INCLUDE_DIR include/ )
SET( IRS_INSTALL_LIB_DIR ${CMAKE_BINARY_DIR}/lib )
SET( IRS_INSTALL_CONTRIB_DIR contrib/ )
SET( IRS_INSTALL_EXAMPLE_DIR examples/ )
SET( IRS_INSTALL_DOC_DIR doc/ )

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/exe)
#SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/archive)

SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.55.0 REQUIRED COMPONENTS filesystem unit_test_framework)

IF(NOT DEFINED SHARK_INCLUDE)
    SET(SHARK_INCLUDE $ENV{SHARK_INCLUDE})
ENDIF()

IF(NOT DEFINED SHARK_INCLUDE)
  MESSAGE(FATAL_ERROR "Could not find SHARK_INCLUDE."
  "You need to set SHARK_INCLUDE environment variable to path where Shark Machine Learning Library headers are located."
  "Shark can be downloaded for free from http://image.diku.dk/shark/") 
ENDIF()

MESSAGE("Found Shark headers in: " ${SHARK_INCLUDE})

IF(NOT DEFINED SHARK_LIBRARIES)
    SET(SHARK_LIBRARIES $ENV{SHARK_LIBRARIES})
ENDIF()

IF(NOT DEFINED SHARK_LIBRARIES)
  MESSAGE(FATAL_ERROR "Could not find SHARK_LIBRARIES."
  "You need to set SHARK_LIBRARIES environment variable to path where Shark Machine Learning Library compiled binaries are located."
  "Shark can be downloaded for free from http://image.diku.dk/shark/") 
ENDIF()
MESSAGE("Found Shark libraries in: " ${SHARK_LIBRARIES})

IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
	MESSAGE(FATAL_ERROR "Trying to build in release mode. Release mode is not yet supported")
	SET(SHARK_LIB shark)
ELSE()
	SET(SHARK_LIB shark_debug)
	MESSAGE("Building in debug mode")
ENDIF()

MESSAGE("Shark library name: " ${SHARK_LIB})

ADD_SUBDIRECTORY( PugiXML)
ADD_SUBDIRECTORY( InstrumentRecognizer )
ADD_SUBDIRECTORY( Executor)
ADD_SUBDIRECTORY( InstrumentRecognizerTester )

ADD_DEPENDENCIES(InstrumentRecognizer PugiXML)
ADD_DEPENDENCIES(Tests InstrumentRecognizer)
ADD_DEPENDENCIES(Recognizer InstrumentRecognizer)
