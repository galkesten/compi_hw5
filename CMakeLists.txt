cmake_minimum_required(VERSION 3.10)
project(compi_hw5) # <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< project name

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -pedantic)

add_executable(${CMAKE_PROJECT_NAME} parser.tab.cpp lex.yy.c Instructions.cpp bp.cpp codeGen.cpp
        hw3_output.cpp Instructions.cpp semanticAnalyzer.cpp symbolTables.cpp)
add_custom_command(
        # Generates Flex file
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/lex.yy.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND flex scanner.lex
        DEPENDS scanner.lex
)

add_custom_command(
        # Generates Bison files
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/parser.tab.cpp
        BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/parser.tab.hpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND bison -d parser.ypp
        DEPENDS parser.ypp
)

add_custom_target(
        # Build this target to clean flex and bison output (only build don't run)
        CleanFlexBisonFiles
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND rm -f lex.yy.c parser.tab.*pp
)

# compile lex.yy.c as c++
set_source_files_properties(lex.yy.c PROPERTIES LANGUAGE CXX )
