#include <iostream>
//#include "myfunction.hh"

extern myfunction<int>;

int main() {
	//myfunction<int>(3,5)
	int result = myfunction<int>(3, 5);
	std::cout << "int: " << result << std::endl;
	return 0;
}
