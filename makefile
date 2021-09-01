myProg: Assembler.o FileMngr.o Iteration1.o Iteration2.o SymbolMngr.o DataMngr.o LineParser.o LineParserHelperFunctions.o WordMngr.o
	gcc -g -Wall -pedantic ./program_files/Assembler.o ./program_files/FileMngr.o ./program_files/Iteration1.o ./program_files/Iteration2.o ./program_files/SymbolMngr.o ./program_files/DataMngr.o ./program_files/LineParser.o ./program_files/LineParserHelperFunctions.o ./program_files/WordMngr.o -o ./myProg

Assembler.o: Assembler.c Assembler.h FileMngr.o GlobalParams.h
	gcc -c -Wall -pedantic Assembler.c -o ./program_files/Assembler.o -lm

FileMngr.o: FileMngr.c FileMngr.h GlobalParams.h
	gcc -c -Wall -pedantic FileMngr.c -o ./program_files/FileMngr.o -lm

Iteration1.o: Iteration1.c Iteration1.h GlobalParams.h
	gcc -c -Wall -pedantic Iteration1.c -o ./program_files/Iteration1.o -lm

Iteration2.o: Iteration2.c Iteration2.h GlobalParams.h
	gcc -c -Wall -pedantic Iteration2.c -o ./program_files/Iteration2.o -lm

SymbolMngr.o: SymbolMngr.c SymbolMngr.h GlobalParams.h
	gcc -c -Wall -pedantic SymbolMngr.c -o ./program_files/SymbolMngr.o -lm

DataMngr.o: DataMngr.c DataMngr.h GlobalParams.h
	gcc -c -Wall -pedantic DataMngr.c -o ./program_files/DataMngr.o -lm

LineParser.o: LineParser.c LineParser.h GlobalParams.h
	gcc -c -Wall -pedantic LineParser.c -o ./program_files/LineParser.o -lm

LineParserHelperFunctions.o: LineParserHelperFunctions.c LineParser.h GlobalParams.h
	gcc -c -Wall -pedantic LineParserHelperFunctions.c -o ./program_files/LineParserHelperFunctions.o -lm

WordMngr.o: WordMngr.c WordMngr.h GlobalParams.h
	gcc -c -Wall -pedantic WordMngr.c -o ./program_files/WordMngr.o -lm