#pragma once

namespace sb
{
	template <class T>
	inline void copy(T* destination, const T* source) {
		destination = NULL;
		if (source)
			destination = new T(*source);
	}

	template <class T>
	inline void copyVector(std::vector<T*>& destination, const std::vector<T*>& source) {
		destination.clear();
		for (std::size_t i = 0; i < source.size(); i++) {
			T* element = NULL;
			copy(element, source[i]);
			destination.push_back(element);
		}

		std::copy(source.begin(), source.end(), destination.begin());
	}

	template <class T>
	void copyAll(const T& source, T& destination)
	{
		std::copy(std::begin(source), std::end(source), std::begin(destination));
	}

	template <class T>
	void removeAll(std::vector<T*>& vec, T* val) {
		vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
	}

	template <class T> 
	void deleteAll(std::vector<T*>& vec) {
		for (size_t i = 0; i < vec.size(); i++)
			delete vec[i];
	}

	template <class T>
	inline void deleteAll(std::vector<T*>& vec, bool(*predicate)(T*)) {
		std::vector<T*> toDelete;
		for (std::size_t i = 0; i < vec.size(); i++) {
			if (predicate(vec[i]))
				toDelete.push_back(vec[i]);
		}

		vec.erase(std::remove_if(vec.begin(), vec.end(), predicate), vec.end());

		for (std::size_t i = 0; i < toDelete.size(); i++)
			delete toDelete[i];
	}
	
	template <class T>
	inline void safeDelete(T* pointer) {
		if (pointer)
			delete pointer;
	}

	template<class T, class UnaryPredicate>
	bool any(const T& t, UnaryPredicate& predicate) {
		typename T::const_iterator it = std::find_if(t.begin(), t.end(), predicate);
		return it != t.end();
	}

	template <class T, class Compare>
	const typename T::const_iterator min_element(const T& t, Compare& compare) {
		return std::min_element(t.begin(), t.end(), compare);
	}

	template <class T, class Compare>
	const typename T::const_iterator max_element(const T& t, Compare& compare) {
		return std::max_element(t.begin(), t.end(), compare);
	}

	template <class TContainer>
	const typename TContainer::const_iterator last(const TContainer& container)
	{
		return --container.end();
	}
}