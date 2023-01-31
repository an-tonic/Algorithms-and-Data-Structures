#pragma once

using namespace std;

template <typename T>
int partition(T x[], int l, int r)
{
	T pivot = x[r];    // pivot 
	int i = (l - 1);  // Index of smaller element 

	for (int j = l; j <= r - 1; j++)
	{
		// If current element is smaller than or 
		// equal to pivot 
		if (x[j] <= pivot)
		{
			i++;    // increment index of smaller element 
			swap(x[i], x[j]);
		}
	}
	swap(x[i + 1], x[r]);
	return (i + 1);
}

template <typename T>
void QuickSort(T x[], int l, int r)
{
	if (l < r) {
		int pi = partition(x, l, r);

		QuickSort(x, l, pi - 1);
		QuickSort(x, pi + 1, r);
	}
}