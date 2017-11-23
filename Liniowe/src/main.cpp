#include <cstddef>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>

#include <initializer_list>
#include <complex>
#include <cstdint>

#include "Vector.h"
#include "LinkedList.h"

const int HIGH = 10000;
const int HIGH2 = 1000;

namespace
{

template <typename T>
using Collection1 = aisdi::LinkedList<T>;
template <typename T>
using Collection2 = aisdi::Vector<T>;

void perfomTest()
{
  {
    Collection1<int> myList;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myList.append(1);
    for(int i = 0; i < HIGH2; ++i)
      myList.prepend(10);
    t = clock() - t;
    std::cout<<"Czas dodawania elementow na poczatek listy to:  "<<t<<std::endl;
  }

  {
    Collection2<int> myVector;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myVector.append(2);
    for(int i = 0; i < HIGH2; ++i)
      myVector.prepend(20);
    t = clock() - t;
    std::cout<<"Czas dodawania elementow na poczatek wektora to:  "<<t<<std::endl;
  }
  std::cout<<std::endl;
  {
    Collection1<int> myList;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myList.append(3);
    for(int i = 0; i < HIGH2; ++i)
      myList.append(30);
    t = clock() - t;
    std::cout<<"Czas dodawanie elementow na koniec listy to:  "<<t<<std::endl;
  }

  {
    Collection2<int> myVector;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myVector.append(4);
    for(int i = 0; i < HIGH/10; ++i)
      myVector.append(40);
    t = clock() - t;
    std::cout<<"Czas dodawania elementow na koniec wektora to:  "<<t<<std::endl;
  }
  std::cout<<std::endl;
  {
    Collection1<int> myList;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myList.append(5);
    for(int i = 0; i < HIGH2; ++i)
      myList.insert(myList.begin()+(HIGH/2), 50);
    t = clock() - t;
    std::cout<<"Czas dodawania elementow w srodek listy to:  "<<t<<std::endl;
  }

  {
    Collection2<int> myVector;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myVector.append(6);
    for(int i = 0; i < HIGH2; ++i)
      myVector.insert(myVector.begin()+(HIGH/2), 60);
    t = clock() - t;
    std::cout<<"Czas dodawania elementow w srodek wektora to:  "<<t<<std::endl;
  }
  std::cout<<std::endl;
  {
    Collection1<int> myList;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myList.append(7);
    for(int i = 0; i < HIGH2; ++i)
      myList.popFirst();
    t = clock() - t;
    std::cout<<"Czas usuwania elementow z poczatku listy to:  "<<t<< std::endl;
  }

  {
    Collection2<int> myVector;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myVector.append(8);
    for(int i = 0; i < HIGH2; ++i)
      myVector.popFirst();
    t = clock() - t;
    std::cout<<"Czas usuwania elementow z poczatku wektora to:  "<<t<<std::endl;
  }
  std::cout<<std::endl;
  {
    Collection1<int> myList;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myList.append(9);
    for(int i = 0; i < HIGH2; ++i)
      myList.popLast();
    t = clock() - t;
    std::cout<<"Czas usuwawnia elementow z konca listy to:  "<<t<<std::endl;
  }

  {
    Collection2<int> myVector;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myVector.append(10);
    for(int i = 0; i < HIGH2; ++i)
      myVector.popLast();
    t = clock() - t;
    std::cout<<"Czas usuwania elementow z konca Vector to:  "<<t<< std::endl;
  }
  std::cout<<std::endl;
  {
    Collection1<int> myList;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myList.append(11);
    for(int i = 0; i < HIGH2; ++i)
      myList.erase(myList.begin()+(HIGH/2), myList.begin()+(HIGH/2)+1);
    t = clock() - t;
    std::cout<<"Czas usuwania elementow ze srodka listy to:  "<<t<<std::endl;
  }

  {
    Collection2<int> myVector;
    clock_t t = clock();
    for(int i = 0; i < HIGH; ++i)
      myVector.append(12);
    for(int i = 0; i < HIGH2; ++i)
      myVector.erase(myVector.begin()+(HIGH/2), myVector.begin()+(HIGH/2)+1);
    t = clock() - t;
    std::cout<<"Czas usuwania elementow ze srodka wektora to:  "<<t<<std::endl;
  }
  std::cout<<std::endl;
} // performTest()
} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
  return 0;
}
