#include <windows.h>
#include <stdio.h>
#include <psapi.h>

int main( int argc, char *argv[ ] ) {
	if( argc < 2 ) {
		printf( "Usage: serve start | stop\n" );
		return -1;
	} else {
		if( strcmp( argv[1], "start" ) != 0 && strcmp( argv[1], "stop" ) != 0 ) {
			printf( "Usage: serve start | stop\n" );
			return -1;
		}
	}
	
    DWORD process[1024], bytesReturned, processes;
	HANDLE hProcess;
	char processName[ MAX_PATH ] = "";
    int result;

    result = EnumProcesses( process, sizeof process, &bytesReturned );
    if ( result == 0 ) {
        printf( "EnumProcesses failed with error: %d\n", result );
        return -1;
    }
    processes = bytesReturned / sizeof (DWORD);
	printf( "Processes: %d\n", processes );

	int i;
	int found = 0;
    for ( i = 0; i < processes; i++ ) {
        if( process[i] != 0 ) {
			hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process[i] );

			if( hProcess != NULL ) {
				HMODULE hModule;
				DWORD bytesReturned;

				if( EnumProcessModules( hProcess, &hModule, sizeof hModule, &bytesReturned) ) {
					GetModuleBaseName( hProcess, hModule, processName, sizeof processName / sizeof (TCHAR) );
				}
			}
			if( strcmp( processName, "server.exe" ) == 0 ) { found = 1; break; }			
        }
    }
	if( found == 1 && strcmp( argv[1], "stop" ) == 0 ) {
		printf( "pid: %u, name: %s\n", process[i], processName );
		hProcess = OpenProcess( PROCESS_TERMINATE, FALSE, process[i] );
		result = TerminateProcess( hProcess, 0 );
		if ( result == 0 ) {
			printf( "TerminateProcesses failed with error: %d\n", result );
			return -1;
		}
		printf( "Server stopped.\n" );
	}
	if( found == 0 && strcmp( argv[1], "start" ) == 0 ) {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof si );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof pi );

		result = CreateProcess(
			NULL,
			"server",
			NULL,
			NULL,
			FALSE,
			NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
			NULL,
			NULL,
			&si,
			&pi
		);
		if ( result == 0 ) {
			printf( "CreateProcesses failed with error: %d\n", result );
			return -1;
		}
		printf( "Server started.\n" );
	}
	
    return 0;
}
