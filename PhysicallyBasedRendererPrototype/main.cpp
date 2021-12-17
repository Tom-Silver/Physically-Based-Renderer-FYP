#include "Application.h"

using namespace TSFYP;

int main(int argc, char** argv)
{
	Application* application = new Application();
	application->Run();

	delete application;

	return 0;
}