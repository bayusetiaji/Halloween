#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
	cout << "playsound\n";

	PlaySound(L"halloween.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	system("pause");

	return 0;
}