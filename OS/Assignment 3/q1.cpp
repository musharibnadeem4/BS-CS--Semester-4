/*Musharib
20i-1764*/
#include <iostream>
#include <atomic>
#include <thread>

using namespace std;

class BakeryLock {
public:
    BakeryLock(int numThreads) : numThreads(numThreads) {
        isEntering = new atomic<bool>[numThreads];
        ticketNumber = new atomic<int>[numThreads];

        for (int i = 0; i < numThreads; ++i) {
            isEntering[i] = false;
            ticketNumber[i] = 0;
        }
    }

    ~BakeryLock() {
        delete[] isEntering;
        delete[] ticketNumber;
    }

    void lock(int threadId) {
        isEntering[threadId] = true;
        ticketNumber[threadId] = 1 + getMaxTicket();
        isEntering[threadId] = false;

        for (int i = 0; i < numThreads; ++i) {
            while (isEntering[i].load()) {
                this_thread::yield(); 
            }

            while ((ticketNumber[i].load() != 0) &&
                   ((ticketNumber[i].load() < ticketNumber[threadId].load()) ||
                    ((ticketNumber[i].load() == ticketNumber[threadId].load()) && (i < threadId)))) {
                this_thread::yield(); 
            }
        }
    }

    void unlock(int threadId) {
        ticketNumber[threadId].store(0);
    }

private:
    int numThreads;
    atomic<bool>* isEntering;
    atomic<int>* ticketNumber;

    int getMaxTicket() {
        int max = ticketNumber[0].load(); 
        for (int i = 1; i < numThreads; ++i) {
            int currentTicket = ticketNumber[i].load(); 
            max = (currentTicket > max) ? currentTicket : max;
        }
        return max;
    }
};

BakeryLock lock(5); 

void criticalSection(int threadId) {
    lock.lock(threadId);
    // Critical Section
    cout << "Thread " << threadId << " in critical section." << std::endl;
    for (int i = 0; i < 1000000; ++i) {
    }
    lock.unlock(threadId);
}

int main() {
    const int numThreads = 5;
    thread threads[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        threads[i] = thread(criticalSection, i);
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    return 0;
}
