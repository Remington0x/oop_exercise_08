#ifndef PUBLISH_SUBSCRIBE_HPP
#define PUBLISH_SUBSCRIBE_HPP

#include <mutex>
#include <queue>

using namespace std;

template<class T>
class Developer {

	public: 
		queue<T> messages;
		mutex m;

	void Add(T & mes) {
		m.lock();
		messages.push(mes);
		m.unlock();
	}

	bool Empty() {
		m.lock();
		bool a = messages.empty();
		m.unlock();
		return a;
	}

	T Front() {
		m.lock();
		T a = messages.front();
		m.unlock();
		return a;
	}

	void Pop() {
		m.lock();
		messages.pop();
		m.unlock();
	}

};


#endif