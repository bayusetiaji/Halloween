#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

using namespace std;

static bool even(int i)
{
	return i % 2 == 0;
}

int main()
{
	vector<int> nums;
	random_device rd;
	for (int i = 0; i < 20; ++i)
		nums.push_back(rd() % 10 + 1);

	for (int n : nums)
		cout << n << ' ';

	cout << endl;
	system("pause");

	nums.erase(remove_if(begin(nums), end(nums), even), end(nums));
	for (int n : nums)
		cout << n << ' ';

	cout << endl;
	system("pause");

	return 0;
}