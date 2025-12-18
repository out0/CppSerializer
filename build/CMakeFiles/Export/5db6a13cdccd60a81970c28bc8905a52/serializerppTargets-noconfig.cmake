#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "serializerpp::serializerpp" for configuration ""
set_property(TARGET serializerpp::serializerpp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(serializerpp::serializerpp PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libserializerpp.so.1.0.0"
  IMPORTED_SONAME_NOCONFIG "libserializerpp.so.1"
  )

list(APPEND _cmake_import_check_targets serializerpp::serializerpp )
list(APPEND _cmake_import_check_files_for_serializerpp::serializerpp "${_IMPORT_PREFIX}/lib/libserializerpp.so.1.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
