#include "Application.h"

int main(int argc, char** argv)
{
	TSFYP::Application* application = new TSFYP::Application();
	application->Run();

	delete application;
	application = nullptr;

	return 0;
}