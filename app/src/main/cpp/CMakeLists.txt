cmake_minimum_required(VERSION 3.22.1)

project("Fimpe")

add_library(chilkatAndroid STATIC IMPORTED)
set_property(TARGET chilkatAndroid PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/chilkatJni/${ANDROID_ABI}/libchilkatAndroid.a)
#target_link_options(chilkat PRIVATE -L/path/to/your/library -lyour-library) -->   -Wl,--gc-sections,--icf=safe

add_library( PersonalCode SHARED PersonalCode.cpp)
target_link_libraries( PersonalCode
        chilkatAndroid )

set_target_properties(PersonalCode PROPERTIES POSITION_INDEPENDENT_CODE ON)
#target_compile_options(PersonalCode PRIVATE -fpie)