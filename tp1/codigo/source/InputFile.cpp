#include "InputFile.h"
#include <fstream>

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
InputFile::InputFile() : mFileName("")
{
}
//---------------------------------------------------------------------------------------
void InputFile::openFile(const char* fName)
{
    if ( mFileName != "" )
    {
        this->mFileContent.clear();
    }

    fstream file(fName, fstream::in);

    if (file.fail())
	{
        string expression("Couldn't open file ");
        expression += fName;

        throw expression;
    }
    else
	{
        mFileName = fName;
        while( !file.eof() )
        {
            std::string strLine;
            getline(file, strLine);
            string line(strLine);

            if (!line.empty())
			{
                this->mFileContent.push_back( line );
            }
        }
        file.close();
    }
}
//---------------------------------------------------------------------------------------
InputFile::~InputFile()
{
}

//---------------------------------------------------------------------------------------
const vector< string >& InputFile::getFileContent()
{
    return this->mFileContent;
}
//---------------------------------------------------------------------------------------
