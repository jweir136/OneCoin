build:
	g++ $(CPPFLAGS) $(CFLAGS) -I. OneCoin/*.cpp -o app

check:
	g++ $(CPPFLAGS) $(CFLAGS) -I. test/*.test.cpp -o test/testapp
	./test/testapp
	rm ./test/testapp
run:
	g++ $(CPPFLAGS) $(CFLAGS) -I. OneCoin/*.cpp -o app
	./app