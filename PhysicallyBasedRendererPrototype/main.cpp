#include "Application.h"

int main(int argc, char** argv)
{
	TSFYP::Application* application = new TSFYP::Application();
	if (!application->Initialise())
	{
		delete application;
		application = nullptr;

		return 0;
	}

	application->Run();

	delete application;
	application = nullptr;

	return 0;
}