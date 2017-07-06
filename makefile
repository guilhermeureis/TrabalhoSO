inicio: clean

trabalho: passo1
	g++ -o main *.o	
passo1:
	g++ -c *.cpp 
clean: trabalho
	rm -rf *.o
