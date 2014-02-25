#include "irrlicht.h"
#include <XnCppWrapper.h>
#include <iostream>
#include <vector>

#include "Skeleton.h"

using namespace irr;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	const unsigned int x[] = {1,2,3,4,5,6};

	try{

	}
	catch(std::exception &e){
		printf("%s\n", e.what());
	}
	getchar();
	return 0;
}