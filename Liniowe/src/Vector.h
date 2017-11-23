#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  size_type number_of_elements;  //liczba elementow aktualnie znajdujacych sie w wektorze
  size_type max_noe;  //liczba elementow, jakie maksymalnie moga znajdowac sie w wektorze (bez nowej alokacji)
  pointer vec;  //wskaznik na poczatek wektora - zerowy element

  //rozszerzanie wektora
  void rozszerz()
  {
    Type* tmp;
    if(this->max_noe <= 2)
      throw std::runtime_error("max_noe <= 2");
    this->max_noe = (this->max_noe)*2;
    tmp = new Type[this->max_noe];
    for(size_type i = 0 ; i < this->number_of_elements ; i++)
      tmp[i] = this->vec[i];
    delete [] vec;
    this->vec = tmp;
  }

public:
  //konstruktor podstawowy
  Vector()
  {
    this->vec = new value_type[4];
    this->max_noe = 4;
    this->number_of_elements = 0;
  }

  //konstruktor z lista
  Vector(std::initializer_list<Type> l) : Vector()  {  for (auto& x : l) append(x);  }

  //konstruktor kopiujacy
  Vector(const Vector& other)
  {
    this->max_noe = other.max_noe;
    this->number_of_elements = other.number_of_elements;
    this->vec = new value_type[this->max_noe];
    for(size_type i = 0 ; i < this->number_of_elements ; i++)
      this->vec[i] =other.vec[i];
  }

  //"zastapienie" innego wektora
  Vector(Vector&& other)
  {
    this->vec = other.vec;
    this->max_noe = other.max_noe;
    this->number_of_elements = other.number_of_elements;

    other.vec = nullptr;
    other.max_noe = other.number_of_elements = 0;
  }

  //destruktor
  ~Vector()
  {
    delete [] this->vec;
  }

  //usuwamy poprzednia zawartosc wektora i kopiowanie zawartosci wektora other
  Vector& operator=(const Vector& other)
  {
    if (this != &other)
    {
      delete [] vec;
      this->number_of_elements = other.number_of_elements;
      this->max_noe = other.max_noe;
      this->vec = new value_type[this->max_noe];
      for(size_type i = 0 ; i <= other.number_of_elements ; i++)
        this->vec[i] = other.vec[i];
    }
    return *this;
  }

  //zastapienie wektora other
  Vector& operator=(Vector&& other)
  {
    delete [] vec;
    this->vec = other.vec;
    this->max_noe = other.max_noe;
    this->number_of_elements = other.number_of_elements;

    other.vec = nullptr;
    other.max_noe = other.number_of_elements = 0;
  

    return *this;
  }

  //zwraca czy wektor jest pusty na podstawie liczby jego elementow
  bool isEmpty() const
  {
    return (this->number_of_elements == 0);
  }

  //zwraca liczbe elementow wektora
  size_type getSize() const
  {
    return this->number_of_elements;
  }

  //wstawianie na pierwszym wolnym miejscu
  void append(const Type& item)
  {
    if(this->number_of_elements == this->max_noe)  //bo to oznacza, ze po dodaniu bedzie wieksze
      this->rozszerz();
    this->vec[this->number_of_elements] = item;
    (this->number_of_elements)++;
  }
  //wstawianie przed wszystkie elementy
  void prepend(const Type& item)
  {
    if(this->number_of_elements == 0)
      this->append(item);
    else
    {
      if(this->number_of_elements == this->max_noe)
        this->rozszerz();
      for(size_type i = this->number_of_elements ; i > 0 ; i--){  this->vec[i] = this->vec[i-1];  }
      this->vec[0] = item;
      (this->number_of_elements)++;
    }
  }

  //wstawia element do wektora
  void insert(const const_iterator& insertPosition, const Type& item)
  {
    Iterator iP = insertPosition;
    if(insertPosition == this->begin())
      this->prepend(item);
    else if(insertPosition == this->end())
      this->append(item);
    else
    {
      if(this->number_of_elements == this->max_noe)
        this->rozszerz();
      for(size_type i = this->number_of_elements ; i > iP.getIdx() ; i--)
        this->vec[i] = this->vec[i-1];
      this->vec[iP.getIdx()] = item;
      (this->number_of_elements)++;
    }
  }

  //"podniesienie" pierwszego elementu z wektora
  Type popFirst()
  {
    Type tmp;
    if(this->number_of_elements == 0)
      throw std::logic_error("Nie ma elementow, ktore moznaby podniesc");
    tmp = this->vec[0];
    for(size_type i = 0 ; i < number_of_elements - 1 ; i++)
      this->vec[i] = this->vec[i+1];
    (this->number_of_elements)--;
    return tmp;
  }

  //"podniesienie" ostatniego elementu z wektora
  Type popLast()
  {
    if(this->number_of_elements == 0)
      throw std::logic_error("Nie ma elementow, ktore moznaby podniesc");
    (this->number_of_elements)--;
    return this->vec[number_of_elements];
  }

  //usuniecie elementu z danej pozycji
  void erase(const const_iterator& possition)
  {
    Iterator poss = possition;
    if(possition == this->end())
      throw std::out_of_range("Koniec");
    if(this->number_of_elements == 0)
      throw std::out_of_range("Nie ma elementow, ktore mozna usunac");

    else if(poss.getIdx() < this->number_of_elements - 1)  //dla el. ost. nic nie zmieniamy - wystarczy zmniejszenie number_of_elements
    {
      for(size_type i = poss.getIdx() ; i < this->number_of_elements -1 ; i++)
        this->vec[i] = this->vec[i+1];
    }
    (this->number_of_elements)--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    Iterator fI = firstIncluded;
    Iterator lE = lastExcluded;
    int deleted = lE.getIdx() - fI.getIdx();  //liczba usunietych elementow

    while(lE != this->end())
    {
      *fI = *lE;
      lE++;
      fI++;
    }
    this->number_of_elements-=deleted;
  }

  iterator begin()  {  return Iterator(this, 0);  }
  iterator end()  {  return Iterator(this, this->number_of_elements);  }  //zwraca iterator za ostatnim elementem wektora
  const_iterator cbegin() const  {  return ConstIterator(this, 0);  }
  const_iterator cend() const  {  return ConstIterator(this, this->number_of_elements);  }
  const_iterator begin() const  {  return cbegin();  }
  const_iterator end() const  {  return cend();  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
protected:
  const Vector<Type> *collection;
  size_type idx;

public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

  ConstIterator() = delete;
  explicit ConstIterator(const Vector<Type> *vector, size_type i)
  {
    this->collection = vector;
    idx = i;
  }
  ConstIterator(const ConstIterator&) = default;

  reference operator*() const
  {
    if(idx >= this->collection->getSize())  //potem sprobuje to usunac
      throw std::out_of_range("Indeks poza wektorem przy probie wyluskania");  //to tez
    return this->collection->vec[idx];
  }

  ConstIterator& operator++()
  {
    if(this->idx >= this->collection->getSize())
      throw std::out_of_range("Iterator poza zakresem przy probie inkrementacji");
    (this->idx)++;
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
    if(idx <= 0)
      throw std::out_of_range("Iterator poza zakresem przy probie dekrementacji");
    (this->idx)--;
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
    ConstIterator iter(*this);
    if(d > 0){  while(d > 0){  ++iter;  d--;  } }
    if(d < 0){  while(d < 0){  --iter;  d++;  } }
    return iter;
  }

  ConstIterator operator-(difference_type d) const
  {
    return operator+(-d);
  }

  bool operator==(const ConstIterator& other) const
  {
    bool result = false;
    if(this->collection == other.collection && this->idx == other.idx)
      result = true;
    return result;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }

  size_type getIdx() {  return idx;  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  Iterator() = delete;
  explicit Iterator(const Vector<Type> *vector, size_type i) : ConstIterator(vector, i) {}
  Iterator(Iterator&) = default;

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

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

  size_type getIdx() 
  {
    return ConstIterator::getIdx();
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H
