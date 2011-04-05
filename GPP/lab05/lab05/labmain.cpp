#include <boost\thread\thread.hpp>

#include <iostream>
#include <string>

using namespace std;

void printToScreen()
{
	cout << "This is printed from a thread" << endl;
}

boost::mutex mutex1;

void printToScreen2(string str)
{
	mutex1.lock();
	{
		cout << "This is printed from a thread: " << str << endl;
	}
	mutex1.unlock();
}

void printToScreen3(string str, int id)
{
	mutex1.lock();
	{
		cout << "This is printed from thread" << id << ": " << str << endl;
	}
	mutex1.unlock();
}

struct alarm
{
	alarm(int secs) : m_secs(secs) {}
	void operator()()
	{
		boost::system_time timeout = boost::get_system_time();
		timeout += boost::posix_time::seconds(m_secs);
		boost::thread::sleep(timeout);
		cout << "Alarm!!" << endl;
	}
	int m_secs;
};

int labmain()
{
	alarm Alarm5S(5);
	boost::thread threadAlarm(Alarm5S);
	
	boost::thread thread1(printToScreen2, "thread1");
	
	string strIn = "thread2";
	boost::thread thread2(printToScreen2, strIn);

	int numThreads = boost::thread::hardware_concurrency();
	boost::thread_group threads;
	for(int i = 0; i < numThreads; i++)
	{
		threads.create_thread(boost::bind(printToScreen3, "hello!", i));
	}
	threads.join_all();

	threadAlarm.join();
	thread1.join();
	thread2.join();

	getchar();
	return 0;
}