#pragma once

#include <functional> // std::less
#include <iterator> // std::iterator_traits

namespace sort {

	// This is C++ magic which will allows our function
	// to default to using a < b if the comparator arg
	// is unspecified. It uses defines std::less<T>
	// for the iterator's value_type.
	//
	// For example: if you have a vector<float>, the 
	// iterator's value type will be float. std::less 
	// will select the < for sorting floats as the 
	// default comparator.

	

	template<typename RandomIter>
	using less_for_iter = std::less<typename std::iterator_traits<RandomIter>::value_type>;

	/* Efficiently swap two items - use this to implement your sorts */
	template<typename T>
	void swap(T & a, T & b) noexcept { 
		T temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	 }

	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void bubble(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings
		using _it             = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		using value_type      = typename _it::value_type;
		using reference       = typename _it::reference;
		using pointer         = typename _it::pointer;

		difference_type n = std::distance(begin, end);
		bool swapped = true;
		
		while (swapped) {
        swapped = false;
        RandomIter current = begin;
        RandomIter next = std::next(begin);
        
        for (typename _it::difference_type i = 0; i < n - 1; ++i) {
            if (comp(*next, *current)) { // Use comp for comparisons
                swap(*current, *next); 
                swapped = true;
            }
            
            ++current;
            ++next;
        }
        
        if (!swapped) {
            // If no swaps were made during this pass, the array is already sorted
            break;
        }
        
        --n;
    	}
	}




	template<typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void insertion(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		using _it = std::iterator_traits<RandomIter>;

		if (begin == end) {
			return;  // Nothing to sort
		}

		for (RandomIter current = begin + 1; current != end; ++current) {
			typename _it::value_type key = std::move(*current);
			RandomIter j = current;

			while (j != begin && comp(key, *(j - 1))) {
				*j = std::move(*(j - 1)); // Shift elements to the right
				--j;
			}

			*j = std::move(key); // Place the stored element in its correct position
		}
	}



	template <typename RandomIter, typename Comparator = std::less<typename std::iterator_traits<RandomIter>::value_type>>
	void selection(RandomIter begin, RandomIter end, Comparator comp = Comparator{}) {
		using _it = std::iterator_traits<RandomIter>;
		
		size_t comparisons = 0; // Initialize a counter for comparisons
		
		for (RandomIter i = begin; i != end; ++i) {
			RandomIter min = i;
			
			// Find the minimum element in the unsorted portion
			for (RandomIter j = i + 1; j != end; ++j) {
				if (comp(*j, *min)) {
					min = j;
				}
				comparisons++; // Count the comparison
			}
			
			// Swap the found minimum element with the current element
			if (min != i) {
				swap(*i, *min);
			}
		}
	}
}
