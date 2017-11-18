L#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
public:
  using difference_type = std::ptrdiff_t; //moze byc ujemny lub dodatni
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;
  using node_type = struct Node {T data; Node *next; Node *previous;}   //wezel: zawiera dane oraz odpowiednie wskazania
  using node_pointer = *Node;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  size_type number_of_nodes;  //zliczac wartownika czy nie - oto jest pytanie
  node_pointer head;
  node_pointer tail;  //wartownik

public:
  LinkedList()
  {
    number_of_nodes = 0;
    this->head = this->tail = new Node;  //straznik
    this->tail->next = nullptr;
    this->tail->prev = nullptr;
  }

  LinkedList(std::initializer_list<Type> l) : LinkedList()
  {
    for (auto& x : l) append(x);
  }

  //konstruktor kopiujacy
  LinkedList(const LinkedList& other) : LinkedList()
  {
    node_pointer tmp_o = other.head;  //wskaznik tymczasowy - czyta po other
    node_pointer tmp = this->head;    //potrzebny przy dodawaniu elementow do nowej listy
    while(tmp_o != nullptr)
    {
      this->head = new Node;
      this->head->previous = nullptr;
      this->head->next = tmp;
      this->head->data = tmp_o->data; //tu moga pojawic sie problemy
      tmp->previous = this->head;
      tmp_o = tmp_o->next;
      this->number_of_nodes++;        //UWAGA: nie liczymy wartownika
    }
  }

  LinkedList(LinkedList&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  //destruktor 
  ~LinkedList()
  {
    node_pointer tmp;
    while(this->head != nullptr)  // nie uzywamy number_of_elements, bo usuwamy tez straznika
    {
      tmp = this->head->next;
      delete(this->head);
      this->head = tmp;
    }  
  }

  LinkedList& operator=(const LinkedList& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  LinkedList& operator=(LinkedList&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  //sprawdza, czy lista jest pusta - nie liczac wartownika - na podstawie liczby elementow w liscie
  bool isEmpty() const
  {
    bool result = false;
    if(this->number_of_elements == 0)
      result = true;
    return result;
  }

  //zwraca liczbe elementow listy - ale bez straznika
  size_type getSize() const
  {
    return this->number_of_elements;
  }

  //wstawianie za ostatnim elementem - ale przed straznikiem
  void append(const Type& item)
  {
    if(number_of_elements > 0)  //rozrozniamy 2 przypadki, bo nastepuje odwolanie do "elementu nastepnego elementu poprzedzajacego wartownika"
    {
      node_pointer tmp = new Node;
      tmp->data = item;
      tmp->next = this->tail;
      tmp->previous = this->tail->previous;
      this->tail->previous->next = tmp; //tu byloby nullptr->next przy liczbie elementow rownej 0, dodatkowo brak przestawienia head
      this->tail->previous = tmp;

      (this->number_of_elements)++;
    }
    else
      prepend(item);
  }

  //wstawianie na poczatek listy
  void prepend(const Type& item)  //funkcja dostaje referencje na "dane"
  {
    node_pointer tmp = new Node;
    tmp->previous = nullptr;
    tmp->next = this->head;
    this->head->previous = tmp;
    this->head = tmp;

    tmp->data = item;

    (this->number_of_elements)++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    (void)insertPosition;
    (void)item;
    throw std::runtime_error("TODO");
  }

  //zdejmuje pierwszy element z listy
  Type popFirst()
  {
    throw std::runtime_error("TODO");
  }

  //zdejmuje ostatni element z listy - ale nie straznika
  Type popLast()
  {
    throw std::runtime_error("TODO");
  }

  //wymazuje z listy zadany element
  void erase(const const_iterator& possition)
  {
    (void)possition;
    throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    (void)firstIncluded;
    (void)lastExcluded;
    throw std::runtime_error("TODO");
  }

  iterator begin()
  {
    return Iterator(head);
  }

  iterator end()
  {
    return Iterator(tail);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(head);
  }

  const_iterator cend() const
  {
    return ConstIterator(tail);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

private:
  node_pointer curr;            //wskaznik na "aktualny"

public:

  ConstIterator() = delete;
  //konstruktor jawny => inicjalizacja tylko za pomoca nawiasow () {}
  explicit ConstIterator(node_pointer new_position) : curr(new_position) {}
  ConstIterator(ConstIterator &other) = default;

  //operator wyluskania - zwraca wartosc elementu na jaki wskazuje
  reference operator*() const
  {
    if(curr->next == nullptr) //iterator wskazuje na straznika
      throw std::out_of_range("Iterator poza zakresem");
    return curr->data;
  }

  ConstIterator& operator++()
  {
    if(curr->next == nullptr)  //iterator jest na strazniku => nie mozemy zwiekszyc iteratora
      throw std::out_of_range("Iterator poza zakresem");
    curr = curr->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  ConstIterator& operator--()
  {
    if(curr->previous == nullptr)
      throw out_of_range("Iterator poza zakresem");
    curr = curr->previous;
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  ConstIterator operator+(difference_type d) const
  {
    ConstIterator tmp = *this;
    if(d > 0)
    {
      for(int i = 0 ; i < d ; i++)
        ++tmp;
    }
    else
    {
      for(int i = d ; i < 0 ; i++)
        --tmp;
    }
    return *this;
  }

  ConstIterator operator-(difference_type d) const
  {
    return operator+(-d);
  }

  bool operator==(const ConstIterator& other) const
  {
    return (*this->curr == other->curr);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);  //jesli nie sa rowne, to nawias zwraca falsz. Za pomoca ! mieniamy go na prawde. Nie sa rowne => prawda
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  Iterator() = delete;
  Iterator(Node *in) : ConstIterator(in) {}
  Iterator(const ConstIterator& other) : ConstIterator(other) {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
