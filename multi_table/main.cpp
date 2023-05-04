#include <iostream>
#include <iomanip>
using namespace std;


int main(int argc, char **argv) {
	const int lbreak_criteria_num = 5; 		// the number of the sub-tables
	
	int max_num = atoi(argv[1]);			// get the max number of sub-tables
	
	int st_num, end_num;				// it helps to control the 3rd loop
	int i, j, k;					// for the loop
	int loop_time = max_num / lbreak_criteria_num;
	if (max_num % lbreak_criteria_num != 0)
		loop_time++;				// it needs when the max_num is not multiple of lbreak_criteria_num(=5)

	for (i = 0; i < loop_time; i++) {
		st_num = i * lbreak_criteria_num;

		if (i == loop_time - 1) 
			end_num = max_num;		// it needs when the max_num is not multiple of lbreak_criteria_num(=5)
		else 
			end_num = (i + 1) * lbreak_criteria_num;	// general case


		for (j = 1; j <= end_num; j++) {
			for (k = 1 + st_num; k <= end_num; k++) {
				// only print out when the left operand is bigger than the right operand
				if (j <= k)
					cout << setw(4) << k << " x " << setw(4) << j << " = " << setw(4) << k * j << "\t"; 
				else
					cout << setw(18) << " " << "\t";
			}
			cout << endl;
		}
		cout << endl;
	}

	return 0;
}
