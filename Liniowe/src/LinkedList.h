#ifndef AISDI_LINEAR_LINKEDLIST_H
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
  using node_type = struct Node {Type data; Node *next; Node *previous;};   //wezel: zawiera dane oraz odpowiednie wskazania
  using node_pointer = Node*;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  size_type number_of_nodes;  //zliczac wartownika czy nie - oto jest pytanie
  node_pointer head;
  node_pointer tail;  //wartownik

public:
  //konstruktor podstawowy
  LinkedList() : number_of_nodes(0)
  {
    this->head = this->tail = new Node;  //straznik
    this->tail->next = nullptr;
    this->tail->previous = nullptr;
  }

  //konstruktor z lista
  LinkedList(std::initializer_list<Type> l) : LinkedList()
  {
    for (auto& x : l) append(x);
  }

  //konstruktor kopiujacy
  LinkedList(const LinkedList& other) : LinkedList()  //tworzenie wlasnego wartownika
  {
    Iterator iter = other.begin();
    while(iter != other.end())  //nie kopiujemy straznika
    {
      append(*iter); //append dostaje referencje na dane i na ich podstawie tworzy nowy wezel
      iter++;
    }
  }

  //"zastapienie" innej listy
  LinkedList(LinkedList&& other)
  {
    this->tail = other.tail;
    this->head = other.head;
    this->number_of_nodes = other.number_of_nodes;

    other.tail = other.head = nullptr;
    other.number_of_nodes = 0;
  }

  //destruktor 
  ~LinkedList()
  {
    node_pointer tmp;
    while(this->head != nullptr)  // nie uzywamy number_of_nodes, bo usuwamy tez straznika
    {
      tmp = this->head;
      this->head = this->head->next;
      delete(tmp);
    }  
  }

  //jesli listy sa rozne: usuwa elementy z listy pierwotnej i kopiuje do niej nowe elementy z other
  LinkedList& operator=(const LinkedList& other)
  {
    Iterator iter = other.begin();

    if(this != &other)
    {
      erase(begin(), end());    //usuwamy elementy, ktore juz znajduja sie na naszej liscie
      while(iter != other.end())  //kopiujemy elementy z other
      {
        append(*iter);
        iter++;
      }
    }
    return *this;
  }

  //"zastapienie" innej listy nasza lista
  LinkedList& operator=(LinkedList&& other)
  {
    erase(begin(), end());
    this->head = other.head;
    this->tail = other.tail;
    this->number_of_nodes = other.number_of_nodes;

    other.head = nullptr;
    other.tail = nullptr;

    return *this;
  }

  //sprawdza, czy lista jest pusta - nie liczac wartownika - na podstawie liczby elementow w liscie
  bool isEmpty() const
  {
    bool result = false;
    if(this->number_of_nodes == 0)
      result = true;
    return result;
  }

  //zwraca liczbe elementow listy - ale bez straznika
  size_type getSize() const
  {
    return this->number_of_nodes;
  }

  //wstawianie za ostatnim elementem - ale przed straznikiem
  void append(const Type& item)
  {
    if(number_of_nodes > 0)  //rozrozniamy 2 przypadki, bo nastepuje odwolanie do "elementu nastepnego elementu poprzedzajacego wartownika"
    {
      node_pointer tmp = new Node;
      tmp->data = item;
      tmp->next = this->tail;
      tmp->previous = this->tail->previous;
      this->tail->previous->next = tmp; //tu byloby nullptr->next przy liczbie elementow rownej 0, dodatkowo brak przestawienia head
      this->tail->previous = tmp;

      (this->number_of_nodes)++;
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

    (this->number_of_nodes)++;
  }

  //wstawia nowy wezel za wezel wskazany przez iterator
  void insert(const const_iterator& insertPosition, const Type& item)
  {
    Iterator iter = insertPosition;
    Node *curr, *new_node;

    if(insertPosition == this->begin())
      prepend(item);
    else if(insertPosition == this->end())
      append(item);
    else
    {
        curr = reinterpret_cast<Node*>(&*iter);
        new_node = new Node();
        new_node->next = curr;
        new_node->previous = curr->previous;
        curr->previous->next = new_node;
        curr->previous = new_node;
        new_node->data = item;
        (this->number_of_nodes)++;
    }
  }

  //zdejmuje pierwszy element z listy - zwraca wartosc, usuwa wezel
  Type popFirst()
  {
    node_pointer tmp = this->head;
    Type data;

    //lista jest pusta
    if(this->isEmpty() == true)
      throw std::logic_error("Lista jest pusta");

    //lista ma co najmniej jeden element
    data = this->head->data;
    this->head->next->previous = nullptr;  //dziala nawet gdy na liscie ma pozostac sam wartownik
    this->head = this->head->next;
    (this->number_of_nodes)--;

    delete tmp;
    return data;
  }

  //zdejmuje ostatni element z listy - ale nie straznika
  Type popLast()
  {
    node_pointer tmp = this->tail->previous;
    Type data;

    //lista pusta lub 1 element - rozwiazanie z popFirst
    if(this->number_of_nodes <= 1)
      return this->popFirst();
    //lista ma wiecej niz jeden element
    data = tmp->data;
    tmp->previous->next = tmp->next;  //istnieje previous (bo jest wiecej niz jeden element)
    tmp->next->previous = tmp->previous;
    (this->number_of_nodes)--;

    delete tmp;
    return data;
  }

  //wymazuje z listy zadany element
  void erase(const const_iterator& position)
  {
    if(position == this->end())
      throw std::out_of_range("Nie mozna usunac straznika");

    Iterator iter = position;
    Node* curr_node = reinterpret_cast<Node*>(&*iter);

    //usuwany jest pierwszy element => musimy zmienic head
    if (curr_node->previous == nullptr)
        this->head = curr_node->next;
    //istnieje element poprzedzajacy => musimy zmienic jego wskazanie na el. nastepny
    else
        curr_node->previous->next = curr_node->next;

    curr_node->next->previous = curr_node->previous;
    (this->number_of_nodes)--;
    delete curr_node;
  }

  //usuwa z listy elementy w zadanym przedziale
  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    for (Iterator tmp = firstIncluded; tmp != lastExcluded;)
    {
        Iterator tmp_next = tmp + 1;
        erase(tmp);
        tmp = tmp_next;
    }
  }

  //zwraca iterator na poczatek
  iterator begin()
  {
    return Iterator(this->head);
  }
  
  //zwraca iterator na koniec
  iterator end()
  {
    return Iterator(this->tail);
  }

  //zwraca ConctIterator na poczatek
  const_iterator cbegin() const
  {
    return ConstIterator(this->head);
  }

  //zwraca ConstIterator na koniec
  const_iterator cend() const
  {
    return ConstIterator(this->tail);
  }

  //to samo co cbegin
  const_iterator begin() const
  {
    return cbegin();
  }

  //to samo co cend
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

protected:
  node_pointer curr;            //wskaznik na "aktualny" wezel

public:
  ConstIterator() = delete;
  explicit ConstIterator(node_pointer new_position) {curr = new_position;}//konstruktor jawny => inicjalizacja tylko za pomoca nawiasow () {}
  ConstIterator(const ConstIterator&) = default;

  //operator wyluskania - zwraca wartosc elementu na jaki wskazuje
  reference operator*() const
  {
    if(this->curr->next == nullptr)  //iterator wskazuje na straznika
      throw std::out_of_range("Iterator poza zakresem");
    return this->curr->data;
  }

  ConstIterator& operator++()
  {
    if(this->curr->next == nullptr)  //iterator jest na strazniku => nie mozemy zwiekszyc iteratora
      throw std::out_of_range("Iterator poza zakresem");
    this->curr = this->curr->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto tmp = *this;
    operator++();
    return tmp;
  }

  ConstIterator& operator--()
  {
    if(this->curr->previous == nullptr)
      throw std::out_of_range("Iterator poza zakresem");
    this->curr = this->curr->previous;
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto tmp = *this;
    operator--();
    return tmp;
  }

  ConstIterator operator+(difference_type d) const
  {
    ConstIterator tmp(this->curr);
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
    return tmp;
  }

  ConstIterator operator-(difference_type d) const
  {
    return operator+(-d);
  }

  bool operator==(const ConstIterator& other) const
  {
    return (this->curr == other.curr);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);  //jesli nie sa rowne, to nawias zwraca falsz. Za pomoca ! zmieniamy go na prawde
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  Iterator() = delete;
  Iterator(Node *new_position) : ConstIterator(new_position) {}
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
