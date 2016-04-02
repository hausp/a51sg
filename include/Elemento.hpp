#ifndef ELEMENTO_HPP
#define ELEMENTO_HPP

template<typename T>
class Elemento {
private:
	T *info;
	Elemento<T>* _next;

public:
	Elemento(const T& info, Elemento<T>* next) : info(new T(info)), _next(next) {}

	Elemento() : info(NULL), _next(NULL) { }

	~Elemento()
	{
		delete info;
	}

	Elemento<T>* getProximo() const
	{
		return _next;
	}

	T getInfo() const
	{
		return *info;
	}

	void setProximo(Elemento<T>* next)
	{
		_next = next;
	}

	Elemento<T>& operator++() const {
		return *getProximo();
	}

	Elemento<T>& operator++(int) {
		
	}
};

template<typename T>
struct lista_iterator {
	Elemento<T>* pointer;

	lista_iterator(Elemento<T>* pointer) :
	pointer(pointer) { }

	lista_iterator& operator++() {
		pointer = pointer->getProximo();
		return *this;
	}

	lista_iterator& operator++(int) {
		lista_iterator temp;
		temp.pointer = this->pointer;
		operator++();
		return temp;
	}

	T operator*() {
		return pointer->getInfo();
	}

	bool operator==(lista_iterator<T>& it) const { return this->pointer == it.pointer; }
	bool operator!=(lista_iterator<T>& it) const { return !(*this == it); }	
};

#endif

