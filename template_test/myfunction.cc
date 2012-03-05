#include "myfunction.hh"

//myfunction<int> myfunction_int;
//myfunction_int = myfunction<int>;
//myfunction_double = myfunction<double>;


void dummy(void) {
	myfunction<int>(3, 5);
	myfunction<double>(3., 5.);
}
