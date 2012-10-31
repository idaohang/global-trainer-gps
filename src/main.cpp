#include <global-trainer-gps.hpp>
#include <cstdlib>

int main() {
	GlobalTrainerGPS::GlobalTrainer gt;

	gt.init();

	gt.first();
	gt.second();
	gt.third();
	gt.fourth();
	gt.fifth();
	gt.get_first_record();
	return EXIT_SUCCESS;
}
