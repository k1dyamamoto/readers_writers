

#include <thread>
#include <mutex>
#include <stdio.h>
#include <io.h>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <fcntl.h>


#define READER_NUM 5
#define WRITER_NUM 3
#define RDELAY 2
#define WDELAY RDELAY * 2


std::mutex data_mutex;
int readers = 0;
std::mutex readers_mutex;
int counter = 0;



void writer(int num)
{
	while (1) {
		std::this_thread::sleep_for(std::chrono::seconds(WDELAY));
		data_mutex.lock();
		counter++;
		std::cout << "I'm writer " + std::to_string(num) + " and i changed counter to " + std::to_string(counter) + '\n';
		data_mutex.unlock();
	}
}

void reader(int num)
{
	while (1) {
		std::this_thread::sleep_for(std::chrono::seconds(RDELAY));
		readers_mutex.lock();
		readers++;
		if (readers == 1)
			data_mutex.lock();

		readers_mutex.unlock();

		std::cout << "I'm reader " + std::to_string(num) + " and i the counter is " + std::to_string(counter) + '\n';


		readers_mutex.lock();

		readers--;
		if (readers == 0)
			data_mutex.unlock();
		readers_mutex.unlock();
	}
}


int main()
{

	std::vector<std::thread> read_vec;
	for (int i = 1; i <= READER_NUM; ++i)
		read_vec.push_back(std::thread(reader, i));

	std::vector<std::thread> write_vec;
	for (int i = 1; i <= WRITER_NUM; ++i)
		write_vec.push_back(std::thread(writer, i));



	for (int i = 0; i < READER_NUM; ++i)
		read_vec[i].join();

	for (int i = 0; i < WRITER_NUM; ++i)
		write_vec[i].join();

}

