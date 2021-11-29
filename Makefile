clean:
	rm simdb

simdb:
	g++ simdb.cpp dataStorage.cpp -o simdb

test:
	g++ test.cpp dataStorage.cpp buffer.cpp -o test