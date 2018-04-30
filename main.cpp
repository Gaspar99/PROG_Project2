#include "Dictionary.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

int main()
{
	Dictionary d1;
	string dictionaryName;

	cout << "Insert the dictionary file name: " ;
	cin >> dictionaryName;
	d1.load(dictionaryName);



	return 0;
}