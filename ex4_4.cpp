#include <assert.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>

/*
 *    4_4. Скользящий максимум.
 *    Дан массив натуральных чисел A[0..n), n не превосходит 10^8.
 *    Так же задан размер некоторого окна (последовательно расположенных
 *    элементов массива) в этом массиве k, k<=n. Требуется для каждого
 *    положения окна (от 0 и до n-k) вывести значение максимума в окне.
 *
 *      Требования: Скорость работы O(n log n), память O(n).
 *
 *    Формат входных данных. Вначале вводится n - количество элементов массива.
 *    Затем вводится n строк со значением каждого элемента. Затем вводится k  -
 * размер окна. Формат выходных данных. Разделенные пробелом значения максимумов
 * для каждого положения окна.
 */

class Element {
 public:
  Element();
  Element(const int& _value, const int& _order);
  Element(const Element&);
  Element(const Element&&);
  Element& operator=(const Element&);
  Element& operator=(const Element&&);
  int getValue() const;
  int getOrder() const;

 private:
  int order;
  int value;
};

template <class T>
class Heap {
 public:
  T extractMax(bool (*isLess)(const T&, const T&));
  void addElem(const T&);
  void insert(const T&, bool(isLess)(const T&, const T&));
  const T& getMax() const;
  Heap();
  explicit Heap(const int& _size);
  Heap(const Heap&) = delete;
  Heap(const Heap&&) = delete;
  Heap operator=(const Heap&) = delete;
  Heap operator=(const Heap&&) = delete;
  ~Heap();

 private:
  void reSize();
  bool isFull() const;
  void siftDown(const int&, bool(isLess)(const T&, const T&));
  void siftUp(int, bool(isLess)(const T&, const T&));
  T* buffer;
  int size;
  int cardinality;
};

bool isLessForElement(const Element& f, const Element& s) {
  return f.getValue() > s.getValue();
}

void run(std::istream& input, std::ostream& output) {
  int count;
  input >> count;
  Heap<Element> myHeap(count);
  int* arr = new int[count];
  for (int i = 0; i < count; ++i) {
    input >> arr[i];
  }
  int window;
  input >> window;
  int lifeTime = 0 - window;
  for (int j = 0; j < count; ++j) {
    Element tmp(arr[j], j);
    myHeap.insert(tmp, isLessForElement);
    lifeTime++;
    if (lifeTime >= 0) {
      Element tmp = myHeap.getMax();
      while (tmp.getOrder() < lifeTime)
        tmp = myHeap.extractMax(isLessForElement);
      output << tmp.getValue();
      output << " ";
    }
  }

  delete[] arr;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "10 1 2 5 4 0 18 7 5 20 7 1";
    run(input, output);
    assert(output.str() == "1 2 5 4 0 18 7 5 20 7 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "20 1 2 5 4 0 18 7 5 20 7 1 2 5 4 0 18 7 5 20 7 1";
    run(input, output);
    assert(output.str() == "1 2 5 4 0 18 7 5 20 7 1 2 5 4 0 18 7 5 20 7 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "10 1 2 5 4 0 18 7 5 20 7 4";
    run(input, output);
    assert(output.str() == "5 5 18 18 18 20 20 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 1 2 3 2";
    run(input, output);  // 9 0 7 3 8 4 5 10 4 6 4
    assert(output.str() == "2 3 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "6 5 4 3 2 1 2 3";
    run(input, output);  // 9 0 7 3 8 4 5 10 4 6 4
    assert(output.str() == "5 4 3 2 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "6 5 3 4 2 1 2 3";
    run(input, output);  // 9 0 7 3 8 4 5 10 4 6 4
    assert(output.str() == "5 4 4 2 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "6 10 9 8 7 6 5 3";
    run(input, output);  // 9 0 7 3 8 4 5 10 4 6 4
    assert(output.str() == "10 9 8 7 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "9 0 7 3 8 4 5 10 4 6 4";
    run(input, output);
    assert(output.str() == "8 8 8 10 10 10 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "6 10 9 8 8 9 10 3";
    run(input, output);
    assert(output.str() == "10 9 9 10 ");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "6 10 9 8 8 9 10 6";
    run(input, output);
    assert(output.str() == "10 ");
  }
}

int main() {
  run(std::cin, std::cout);
  // test();
  return 0;
}

template <class T>
Heap<T>::Heap(const int& _size) {
  cardinality = 0;
  size = _size;
  buffer = new T[size];
}

template <class T>
void Heap<T>::siftDown(const int& i, bool(isLess)(const T&, const T&)) {
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  int largest = i;
  if (left < cardinality &&
      isLess(buffer[left], buffer[i]))  // возварщает F > s
    largest = left;
  if (right < cardinality && isLess(buffer[right], buffer[largest]))
    largest = right;
  if (largest != i) {
    std::swap(buffer[i], buffer[largest]);
    siftDown(largest, isLess);
  }
}

template <class T>
void Heap<T>::siftUp(int index, bool(isLess)(const T&, const T&)) {
  while (index > 0) {
    int parent = (index - 1) / 2;
    if (isLess(buffer[parent],
               buffer[index]))  //( arr[index] <= arr[parent] ) возварщает F > s
      return;
    std::swap(buffer[index], buffer[parent]);
    index = parent;
  }
}

template <class T>
void Heap<T>::addElem(const T& elem) {
  if (isFull()) reSize();
  buffer[cardinality++] = elem;
}

template <class T>
Heap<T>::Heap() {
  cardinality = 0;
  size = 3;
  buffer = new T[size];
}

template <class T>
void Heap<T>::reSize() {
  T* tmpBuffer = new T[size * 2];
  memcpy(tmpBuffer, buffer, sizeof(T) * size);
  delete[] buffer;
  buffer = tmpBuffer;
  size *= 2;
}

template <class T>
bool Heap<T>::isFull() const {
  return size == cardinality;
}

template <class T>
const T& Heap<T>::getMax() const {
  return buffer[0];
}

template <class T>
Heap<T>::~Heap() {
  delete[] buffer;
}

template <class T>
void Heap<T>::insert(const T& element, bool (*isLess)(const T&, const T&)) {
  {
    addElem(element);
    siftUp(cardinality - 1, isLess);
  }
}

template <class T>
T Heap<T>::extractMax(bool (*isLess)(const T&, const T&)) {
  std::swap(buffer[0], buffer[--cardinality]);
  siftDown(0, isLess);
  return buffer[cardinality];
}

Element::Element(const Element& el) : order(el.order), value(el.value) {}

Element::Element(const Element&& el) : order(el.order), value(el.value) {}

Element& Element::operator=(const Element& el) {
  this->order = el.getOrder();
  this->value = el.getValue();
  return *this;
}

Element& Element::operator=(const Element&& el) {
  this->order = el.getOrder();
  this->value = el.getValue();
  return *this;
}

int Element::getValue() const { return value; }

int Element::getOrder() const { return order; }

Element::Element() : order(-1), value(0) {}

Element::Element(const int& _value, const int& _order)
    : value(_value), order(_order){};
