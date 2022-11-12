#pragma once

#include <cstdlib>
#include <utility>

template<typename T>
class Array final
{
public:
	Array();

	Array(int capacity);

	Array(const Array& other);

	~Array();

	int insert(const T& value);

	int insert(int index, const T& value);

	void remove(int index);

	const T& operator[](int index) const;

	T& operator[](int index);

	Array& operator=(Array other);

	int size() const;

	class Iterator
	{
	public:
		T& get() const;
		void set(const T& value);
		void next();
		bool hasNext() const;
		friend Array;
	private:
		Array* array_;
		int direction_;
		int index_;
	};

	class ConstIterator
	{
	public:
		const T& get() const;
		void next();
		bool hasNext() const;
		friend Array;
	private:
		const Array* array_;
		int direction_;
		int index_;
	};

	Iterator iterator();

	ConstIterator iterator() const;

	Iterator reverseIterator();

	ConstIterator reverseIterator() const;

private:
	int capacity_;
	int size_;
	T* array_;
};

constexpr int kDefaultArrayCapacity = 8;

template<typename T>
Array<T>::Array()
{
	capacity_ = kDefaultArrayCapacity;
	size_ = 0;
	array_ = (T*)std::malloc(capacity_ * sizeof(T));
}

template<typename T>
Array<T>::Array(int capacity)
{
	capacity_ = capacity;
	size_ = 0;
	array_ = (T*)std::malloc(capacity_ * sizeof(T));
}

template<typename T>
Array<T>::Array(const Array<T>& other)
{
	capacity_ = other.capacity_;
	size_ = other.size_;
	array_ = (T*)std::malloc(capacity_ * sizeof(T));
	for (int i = 0; i < size_; i++)
	{
		new(array_ + i) T(other.array_[i]);
	}
}

template<typename T>
Array<T>::~Array()
{
	for (int i = 0; i < size_; i++)
		array_[i].~T();
	std::free(array_);
}

template<typename T>
int Array<T>::insert(const T& value)
{
	if (size_ + 1 > capacity_)
	{
		capacity_ *= 2;
		T* tmp = (T*)std::malloc(capacity_ * sizeof(T));
		for (int i = 0; i < size_; i++)
		{
			new(tmp + i) T(std::move(array_[i]));
			array_[i].~T();
		}
		std::free(array_);
		array_ = tmp;
	}
	new(array_ + size_) T(value);
	size_ += 1;
	return size_ - 1;
}

template<typename T>
int Array<T>::insert(int index, const T& value)
{
	if (size_ + 1 > capacity_)
	{
		capacity_ *= 2;
		T* tmp = (T*)std::malloc(capacity_ * sizeof(T));
		for (int i = 0; i < size_; i++)
		{
			new(tmp + i) T(std::move(array_[i]));
			array_[i].~T();
		}
		std::free(array_);
		array_ = tmp;
	}
	if (size_ > index)
	{
		for (int i = size_; i > index; i--)
		{
			new(array_ + i) T(std::move(array_[i - 1]));
			array_[i - 1].~T();
		}
	}
	new(array_ + index) T(value);
	size_ += 1;
	return index;
}

template<typename T>
void Array<T>::remove(int index)
{
	for (int i = index; i < size_ - 1; i++)
	{
		array_[i].~T();
		new(array_ + i) T(std::move(array_[i + 1]));
	}
	array_[size_ - 1].~T();
	size_ -= 1;
}

template<typename T>
const T& Array<T>::operator[](int index) const
{
	return array_[index];
}

template<typename T>
T& Array<T>::operator[](int index)
{
	return array_[index];
}

template<typename T>
Array<T>& Array<T>::operator=(Array other)
{
	std::swap(array_, other.array_);
	std::swap(size_, other.size_);
	std::swap(capacity_, other.capacity_);
	return *this;
}

template<typename T>
int Array<T>::size() const
{
	return size_;
}

template<typename T>
T& Array<T>::Iterator::get() const
{
	return (*array_)[index_];
}

template<typename T>
void Array<T>::Iterator::set(const T& value)
{
	(*array_)[index_] = value;
}

template<typename T>
void Array<T>::Iterator::next()
{
	index_ += direction_;
}

template<typename T>
bool Array<T>::Iterator::hasNext() const
{
	return (index_ + direction_ >= 0) && (index_ + direction_ < array_->size_);
}

template<typename T>
const T& Array<T>::ConstIterator::get() const
{
	return (*array_)[index_];
}

template<typename T>
void Array<T>::ConstIterator::next()
{
	index_ += direction_;
}

template<typename T>
bool Array<T>::ConstIterator::hasNext() const
{
	return (index_ + direction_ >= 0) && (index_ + direction_ < array_->size_);
}

template<typename T>
typename Array<T>::Iterator Array<T>::iterator()
{
	Array<T>::Iterator it;
	it.array_ = this;
	it.index_ = 0;
	it.direction_ = +1;
	return it;
}

template<typename T>
typename Array<T>::ConstIterator Array<T>::iterator() const
{
	Array<T>::ConstIterator it;
	it.array_ = this;
	it.index_ = 0;
	it.direction_ = +1;
	return it;
}

template<typename T>
typename Array<T>::Iterator Array<T>::reverseIterator()
{
	Array<T>::Iterator it;
	it.array_ = this;
	it.index_ = size_ - 1;
	it.direction_ = -1;
	return it;
}

template<typename T>
typename Array<T>::ConstIterator Array<T>::reverseIterator() const
{
	Array<T>::ConstIterator it;
	it.array_ = this;
	it.index_ = size_ - 1;
	it.direction_ = -1;
	return it;
}
