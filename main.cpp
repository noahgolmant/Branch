#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stack>
#include <list>

using namespace std;

stack<TCHAR*> dirs; //Stack item'

/* Branch function: takes dpeth, directory, and whether it is a subdirectory of the root. */
void branch(int tDepth, TCHAR* rootDir, bool isSub) {
	dirs.push(rootDir); //Put new directory into stack

	TCHAR dir[MAX_PATH]; //New directory with proper Win32 format
	StringCchCopy(dir, MAX_PATH, rootDir);
	StringCchCat(dir, MAX_PATH, TEXT("\\*"));

	//Win32 Structures to contain file data
	HANDLE hFind;
	WIN32_FIND_DATA findNext;

	//Get rid of standard '...'
	hFind = FindFirstFile(dir, &findNext);
	FindNextFile(hFind, &findNext);

	int currentDepth = dirs.size();
	while(FindNextFile(hFind, &findNext) != 0) { //Iterate through files
		if(isSub && currentDepth != 1) {	
			for(int a = 1; a < currentDepth; a++) {
				_tprintf(TEXT("  "));
			}
		}
		_tprintf(TEXT("%c%c"), 124, 45); //Append visual 'tree' structure
		_tprintf(findNext.cFileName);
		printf("\n");

		if(findNext.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { //If is folder
			TCHAR nDir[MAX_PATH]; //New Directory to branch into
			StringCchCopy(nDir, sizeof(nDir), rootDir);
			StringCchCat(nDir, sizeof(nDir), TEXT("\\"));
			StringCchCat(nDir, sizeof(nDir), findNext.cFileName);

			int nDepth = tDepth - 1;
			if(nDepth != 0)
				branch(nDepth, nDir, true);
		}

	}

	//After done in directory, go back up
	if(dirs.size() > 1) {
		dirs.pop();
	}
}

void main(int argc, char* argv[]) {
	TCHAR rootDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, rootDir); //Get dir


	if(argc > 2) {
		printf("Incorrect use of command.");
		return;
	}

	if(argc == 1) { //Default is one level
		branch(1, rootDir, false);
		
	}

	if(argc == 2) {
		int size = atoi(argv[1]);
		branch(size, rootDir, false);
	}
}