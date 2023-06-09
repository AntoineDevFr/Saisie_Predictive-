trie.o: trie.c trie.h hash.h linked_list.h
	gcc -c trie.c

hash.o: hash.c hash.h
	gcc -c hash.c -lm

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c

main.o: main.c hash.h trie.h linked_list.h
	gcc -c main.c

main: hash.o trie.o main.o linked_list.o 
	gcc hash.o trie.o linked_list.o main.o -g -o main -lm -lncurses 

clean:
	rm -f *.o main