#include <DefaultKernelSlot.h>
#include <DefaultPlayerSlot.h>
#include <Strategix.h>

#include <boost/filesystem.hpp>
#include <iostream>


#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP


using namespace Strategix;
using namespace Sample1;
using namespace std;

#if defined( _MSC_VER )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

static const WORD MAX_CONSOLE_LINES = 1000;

void RedirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;
	
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
	// point to console as well
	ios::sync_with_stdio();
}


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
	RedirectIOToConsole();
	//SetCurrentDirectory("C:\\.......");
#else

int main(int argc, char* argv[])
{
#endif
	// Logs
	el::Loggers::configureFromGlobal(Kernel::Get("log_config_file").c_str());
	el::Configurations conf;
	conf.parseFromText("*GLOBAL:\n FORMAT = %msg", el::Loggers::getLogger("default")->configurations());
	el::Loggers::getLogger("raw")->configure(conf);
	
	// Initialize graphics and make slot to it...
	auto kernelSlot = make_s<DefaultKernelSlot>();
	
	// Initialize Kernel...
	Kernel::Configure(sp_cast<KernelSlot>(kernelSlot));
	Kernel::PrintInfo();
	
	// Run a game
	try
	{
		// Initialize map and players...
		Kernel::Init("1x1");
		Kernel::AddPlayer(make_u<Player>("Inu",  HUMAN, 0, "Spher"));
		Kernel::AddPlayer(make_u<Player>("Saru", AI,    1, "Spher"));
		
		// Handle frame updates...
		// { Kernel::Tick(); }
	}
	catch (nya::exception& e)
	{
		cerr << "Strategix error occurred. \nTerminating..." << endl;
	}
	catch (exception& e)
	{
		cerr << "Unexpected error occurred [" << e.what() << "] \nTerminating..." << endl;
	}

#if defined( _MSC_VER )
	getch();
#endif
}