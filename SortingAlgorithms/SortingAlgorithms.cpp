// SortingAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <array>
#include "BubbleSort.h"

int main()
{
	int ex_array[] = { 1, 5, 7, 4, 0, 2, 3 };
	char ch_array[] = {'a', 'g', '7', 'x', 'j'};
	size_t arr_size = sizeof(ch_array) / sizeof(ch_array[0]);
	BubbleSort<char>::sortArray(ch_array, arr_size);

	for (size_t i = 0; i < arr_size; ++i) {
		std::cout << ch_array[i] << " ";
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
