cmake_minimum_required(VERSION 3.16)
project(assembler C)

#set(CMAKE_C_STANDARD 90)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -ansi")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(assembler Assembler.c Assembler.h Iteration1.c Iteration1.h Iteration2.c Iteration2.h GlobalParams.h SymbolMngr.c SymbolMngr.h FileMngr.c FileMngr.h DataMngr.c DataMngr.h LineParser.c LineParser.h  LineParserHelperFunctions.c WordMngr.c WordMngr.h)
# BACKUP ONLY FOR COMPILATION
#add_executable(assembler Assembler.c Assembler.h Iteration1.c Iteration1.h Iteration2.c Iteration2.h GlobalParams.h SymbolMngr.c SymbolMngr.h FileMngr.c FileMngr.h DataMngr.c DataMngr.h LineParser.c LineParser.h  LineParserHelperFunctions.c)