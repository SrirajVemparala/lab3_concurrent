all: mysort counter

mysort: sort_thread.o bucket_sort.o mysort.o all_locks.o
	g++-11 -g sort_thread.o bucket_sort.o mysort.o all_locks.o -o mysort

counter: counter_main.o counter.o all_locks.o
	g++-11 counter_main.o counter.o all_locks.o -o counter

counter_main.o: counter_main.cpp
	g++-11 -g -Wall -Werror -c counter_main.cpp

counter.o: counter.cpp
	g++-11 -g -c -pthread -O3 -std=c++2a counter.cpp

bucket_sort.o: bucket_sort.cpp
	g++-11 -g -Wall -Werror -c bucket_sort.cpp

mysort.o: mysort.cpp
	g++-11 -g -Wall -Werror -c mysort.cpp

sort_thread.o: sort_thread.cpp
	g++-11 -g -c -pthread -O3 -std=c++2a sort_thread.cpp

all_locks.o: all_locks.cpp
	g++-11 -g -Wall -Werror -c all_locks.cpp

.PHONY : clean
clean:
	rm -f sort_thread bucket_sort mysort all_locks counter counter_main *.o
