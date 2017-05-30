#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <vector>
using namespace std;

template <class T>
class PriorityQueue {
  private:
    vector<T> vec;

  public:
    void push(T const &item);
    void update(T const &item);
    void pop();
    T top();
    bool empty();
};

#endif
