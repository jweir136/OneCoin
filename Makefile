build:
	g++ $(CPPFLAGS) $(CFLAGS) -std=c++11 -I. OneCoin/*.cpp -o app -lcrypto

check:
	g++ $(CPPFLAGS) $(CFLAGS) -std=c++11 -I. test/*.test.cpp -o test/testapp -lcrypto
	./test/testapp
	rm ./test/testapp
run:
	g++ $(CPPFLAGS) $(CFLAGS) -std=c++11 -I. OneCoin/*.cpp -o app -lcrypto
	./app
	rm ./app
docs:
	doxygen doc/Doxyfile
