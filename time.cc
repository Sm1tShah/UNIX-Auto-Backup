//Smit Shah sss160030@utdallas.edu CS3376
#include <time.h>
#include "Prog5.h"
using namespace std;
string currentDateTime() {
    time_t curr = time(0);
    struct tm  s;
    char b[80];
    s = *localtime(&curr);
    strftime(b, sizeof(b), "%Y.%m.%d-%X", &s);
	return b;
}