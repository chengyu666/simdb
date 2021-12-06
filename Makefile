.PHONY:simdb test
clean:
	rm simdb

cleant:
	rm test

simdb:
	g++ simdb.cpp dataStorage.cpp buffer.cpp -o simdb

test:
	g++ test.cpp dataStorage.cpp buffer.cpp -o test