#ifndef __InputFile_H__
#define __InputFile_H__

#include <iostream>
#include <vector>

using namespace std;

class InputFile
{
public:
	InputFile();
	~InputFile();
	void openFile( const char * fileName );

    // Devuelve el contenido del archivo en forma de vector de string
    const vector<string>& getFileContent();

private:
    string mFileName;
	vector<string> mFileContent;
};

#endif // __InputFile_H__
