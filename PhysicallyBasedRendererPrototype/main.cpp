// Internal includes
#include "Application.h"

int main(int argc, char** argv)
{
	TSFYP::Application* application = new TSFYP::Application();
	if (application->Initialise())
	{
		application->Run();
	}

	delete application;
	application = nullptr;

	return 0;
}