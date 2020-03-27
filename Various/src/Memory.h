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
	inline void deleteFromVector(std::vector<T*>& vec, bool(*predicate)(T*)) {
		std::vector<T*> toDelete;
		for (std::size_t i = 0; i < vec.size(); i++) {
			if (predicate(vec[i]))
				toDelete.push_back(vec[i]);
		}

		vec.erase(std::remove_if(vec.begin(), vec.end(), predicate), vec.end());

		for (std::size_t i = 0; i < toDelete.size(); i++)
			delete toDelete[i];
	}
}