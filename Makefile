all:
	gcc -g -Wall -Wextra -fPIC -I. -shared -o dev-test-v0.0.1.so dev-test-v0.0.1.c -lm