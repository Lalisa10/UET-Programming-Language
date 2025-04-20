flex Flex/upl.l
g++ main.cpp lex.yy.c src/*.cpp src/symbol/*.cpp src/parsingTable/*.cpp src/parseTree/*.cpp  -Iinclude -o upl -ll
./upl < input.txt > output.txt