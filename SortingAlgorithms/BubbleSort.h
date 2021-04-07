#pragma once
template<class T>
class BubbleSort
{
public:
	void sortArray(T *arr, size_t size);
private:
	void swap(T *first, T *second);
};

template<class T>
inline void BubbleSort<T>::sortArray(T * arr, size_t size)
{
	for (size_t i = 0; i < size-1; i++) {
		bool swapped = false;
		for (size_t j = 0; j < size - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(&arr[j], &arr[j + 1]);
				swapped = true;
			}
		}
		if (swapped == false) {
			break; //break on swapped == false is one way of optimization
		}
	}
}

template<class T>
inline void BubbleSort<T>::swap(T * first_elem, T * second_elem)
{
	T temp_elem = *first_elem;
	*first_elem = *second_elem;
	*second_elem = temp_elem;
}

