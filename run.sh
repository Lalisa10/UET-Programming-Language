flex upl.l
gcc lex.yy.c -o upl -ll
./upl < input.txt > output.txt