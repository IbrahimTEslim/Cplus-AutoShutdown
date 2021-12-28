#include <string>
#include <Windows.h>
#include <iostream>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")


using namespace std;

string delay, copied_delay;

int FullDelay, circle = 0, length;

BOOL MySystemShutdown()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process. 

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return(FALSE);

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	// Shut down the system and force all applications to close. 

	if (!ExitWindowsEx(EWX_SHUTDOWN ,
		SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
		SHTDN_REASON_MINOR_UPGRADE |
		SHTDN_REASON_FLAG_PLANNED))
		return FALSE;

	//shutdown was successful
	return TRUE;
}

void main()
{
	system("title Shutdown.exe");
	system("cls");

	cout << "Welcome" << endl;

	cout << "After how many H,M,s to shutdown *format*(H.M.S): ";
	getline(cin, delay);

	for (int i = 0; i < delay.length(); i++)
	{


		if (delay[i] == '.')
		{
			for (int j = 0; j < i; j++)
			{
				copied_delay += delay[j];
			}
			if (circle == 0)
				FullDelay += stoi(copied_delay) * 3600;

			if (circle == 1)
				FullDelay += stoi(copied_delay) * 60;

			if (circle == 2)
				FullDelay += stoi(copied_delay);

			delay.erase(0, i + 1);
			circle++;
			copied_delay.erase();
			i = 0;
		}

	}

	FullDelay += stoi(delay);

	int h = FullDelay / 3600;
	FullDelay = FullDelay - (h * 3600);

	int m = FullDelay / 60;
	FullDelay = FullDelay - (m * 60);

	int s = FullDelay;

	cout << endl << endl << "Shutdown After: " << h << "," << m << "," << FullDelay;

	for (;;)
	{


		if (s == 0)
		{
			//if (h == 0 && m == 0)
			//	s = 59;

			if (m != 0)
			{
				m = m - 1;
				s = 59;
			}

			if (m == 0)
			{
				if (h != 0)
				{
					m = 59;
					s = 60;
					h = h - 1;
				}
				else
				{
					h = 0;
				}
			}


		}

		if (s > 10)
			length += 2;
		else
			length += 1;


		if (m > 10)
			length += 2;
		else
			length += 1;

		if (h > 10)
			length += 2;
		else
			length += 1;


		for (int i = 0; i < (length + 2); i++)
			cout << "\r";

		length = 0;

		Sleep(1000);

		if (s == -1)
		{
			s = 0;
			cout << "Shutdown After: " << h << "," << m << "," << s << " ";
			s = -1;
		}
		else
			cout << "Shutdown After: " << h << "," << m << "," << s << " ";

		s--;

		if (s == -1)
			break;


	}

	cout << "\n\n\nDone......";
	Sleep(500);
	MySystemShutdown();
	Sleep(500);
}