#include <iostream>
#include <exception>

#include "CGFapplication.h"
#include "ANFScene.h"
#include "ANFInterface.h"

using namespace std;

int main(int argc, char* argv[]) {

	if (argc != 2) {
		cout << "Wrong number of arguments. Expected \"CGFexample <filename>\""
				<< endl;
		return -1;
	}

	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		app.setScene(new ANFScene(argv[1]));
		app.setInterface(new ANFInterface());

		app.run();
	} catch (GLexception& ex) {
		cout << "Error: " << ex.what();
		return -1;
	} catch (exception& ex) {
		cout << "Unexpected error: " << ex.what();
		return -1;
	}

	return 0;
}
