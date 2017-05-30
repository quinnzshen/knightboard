#include "priority_queue.h"
#include "move.h"
using namespace std;

template <class T>
void PriorityQueue<T>::push(T const &item) {
  vec.push_back(item);
  push_heap(vec.begin(), vec.end());
}

template <class T>
void PriorityQueue<T>::update(T const &item) {
  typename vector<T>::iterator itr = find(vec.begin(), vec.end(), item);

  if (itr != vec.end()) {
    vec.erase(itr);
    make_heap(vec.begin(), vec.end());
  }

  vec.push_back(item);
  push_heap(vec.begin(), vec.end());
}

template <class T>
void PriorityQueue<T>::pop() {
  pop_heap(vec.begin(), vec.end());
  vec.pop_back();
}

template <class T>
T PriorityQueue<T>::top() {
  return vec.front();
}

template <class T>
bool PriorityQueue<T>::empty() {
  return vec.empty();
}

template class PriorityQueue<Move>;
