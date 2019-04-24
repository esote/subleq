CAREFREE := -DCAREFREE -flto
DEBUG := -g -Wall -Wextra -Wconversion -DDEBUG
PERF := -Ofast -funroll-loops -march=native

subleq: subleq.c
	gcc $(PERF) -o subleq.out subleq.c

carefree: subleq.c
	gcc $(CAREFREE) $(PERF) -o subleq.out subleq.c

debug: subleq.c
	gcc $(DEBUG) -o subleq.out subleq.c

clean:
	rm -f subleq.out
