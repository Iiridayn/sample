libfat.so:	functions.cpp  #include all of your cpp and h files here as dependencies
	g++ -fPIC -shared -o libfat.so functions.cpp #include all of your cpp files here

test: functions.cpp
	g++ -g -o test functions.cpp -DTEST -Wall
