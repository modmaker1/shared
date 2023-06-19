
// Copyright (c) 2009-2020, Ilya Lyutin
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <windows.h>
#include <stdio.h>

#include "utils.h"


//
// allocators
//

void* AllocMemory(size_t iSize)
{
	void* p = malloc(iSize);

	if ( p == NULL )
	{
		if ( MessageBox( NULL, "Error allocating memory. Debug?", "AllocMemory", MB_YESNOCANCEL ) == IDYES )
		{
			__debugbreak();
		}
	}

	return p;
}


void FreeMemory(void* p)
{
	free(p);
}


char* AllocString(const char* pszSrc)
{
	size_t iSize;
	char* psz;

	iSize = strlen(pszSrc) + 1;
	psz = (char*)AllocMemory(iSize);

	if (psz != NULL)
	{
		memcpy(psz, pszSrc, iSize);
	}

	return psz;
}


wchar_t* AllocStringW(const wchar_t* pszSrc)
{
	size_t iSize;
	wchar_t* psz;

	iSize = (wcslen(pszSrc) + 1) * sizeof(wchar_t);
	psz = (wchar_t*)AllocMemory(iSize);

	if (psz != NULL)
	{
		memcpy(psz, pszSrc, iSize);
	}

	return psz;
}


wchar_t* AllocStringUnicode(const char* pszSrc)
{
	wchar_t* psz;
	int iSize;

	psz = NULL;

	iSize = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	psz = (wchar_t*)AllocMemory((iSize+1)*sizeof(wchar_t));

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, psz, iSize);
	
	psz[iSize] = '\0';

	return psz;
}


void FreeStringSafe(const char* psz)
{
	if (psz != NULL)
	{
		FreeMemory((void*)psz);
	}
}


void FreeStringSafeW(const wchar_t* psz)
{
	if (psz != NULL)
	{
		FreeMemory((void*)psz);
	}
}


//
// path and filename funcs
//

#define IsPathSeparator(c) (((c) == '\\') || ((c) == '/'))
//#define IsxxxFileName(s) (((s)[0] == '.') && (((s)[1] == '\0') || (((s)[1] == '.') && ((s)[2] == '\0'))))


char* GetExtension(const char* psz)
{
	size_t i;

	i = strlen(psz);

	for (; i > 0; --i)
	{
		if (psz[i] == '.')
		{
			return (char*)&psz[i+1];
		}
		else if (IsPathSeparator(psz[i]))
		{
			break;
		}
	}

	return NULL;
}


wchar_t* GetExtensionW(const wchar_t* psz)
{
	size_t i;

	i = wcslen(psz);

	for (; i > 0; --i)
	{
		if (psz[i] == '.')
		{
			return (wchar_t*)&psz[i+1];
		}
		else if (IsPathSeparator(psz[i]))
		{
			break;
		}
	}

	return NULL;
}


char* GetFileName(const char* psz)
{
	const char* pszFileName;
	int i;

	pszFileName = psz;

	for (i = 0; psz[i] != '\0'; i++)
	{
		if (IsPathSeparator(psz[i]))
		{
			pszFileName = &psz[i+1];
		}
	}

	return (char*)pszFileName;
}


wchar_t* GetFileNameW(const wchar_t* psz)
{
	const wchar_t* pszFileName;
	int i;

	pszFileName = psz;

	for (i = 0; psz[i] != '\0'; i++)
	{
		if (IsPathSeparator(psz[i]))
		{
			pszFileName = &psz[i+1];
		}
	}

	return (wchar_t*)pszFileName;
}

/*
void ConcatPath(char* buffer, const char* psz)
{
	int i;
	
	i = (int)strlen(buffer);

	while (i > 0)
	{
		if (!IsPathSeparator(buffer[i-1]))
		{
			break;
		}
		i--;
	}

	buffer[i++] = PATH_SEPARATOR;

	strcpy(&buffer[i], psz);
}
*/

bool_t CreateDirectoryTree(const char* pszPath)
{
	char szDir[MAX_PATH];
	int i;

	strcpy(szDir, pszPath);

	for (i = 0; ; i++)
	{
		char c = szDir[i];
		
		if (IsPathSeparator(c) || (c == '\0'))
		{
			szDir[i] = '\0';

			{
				DWORD attrs = GetFileAttributesA(szDir);

				if (attrs == (DWORD)-1)
				{
					if (!CreateDirectoryA(szDir, NULL))
					{
						return false;
					}
				}
				else
				{
					if (!(attrs & FILE_ATTRIBUTE_DIRECTORY))
					{
						return false;
					}
				}
			}
			
			if (c == '\0')
			{
				break;
			}

			szDir[i] = '\\';
		}
	}

	return true;
}


bool_t CreateDirectoryTreeW(const wchar_t* pszPath)
{
	wchar_t szDir[MAX_PATH];
	int i;

	wcscpy(szDir, pszPath);

	for (i = 0; ; i++)
	{
		wchar_t c = szDir[i];
		
		if (IsPathSeparator(c) || (c == '\0'))
		{
			szDir[i] = '\0';

			{
				DWORD attrs = GetFileAttributesW(szDir);

				if (attrs == (DWORD)-1)
				{
					if (!CreateDirectoryW(szDir, NULL))
					{
						return false;
					}
				}
				else
				{
					if (!(attrs & FILE_ATTRIBUTE_DIRECTORY))
					{
						return false;
					}
				}
			}
			
			if (c == '\0')
			{
				break;
			}

			szDir[i] = '\\';
		}
	}

	return true;
}



//
// parser (ANSI only)
//


int ToInt(const char* psz)
{
	return strtol(psz, NULL, 10);
}


unsigned int ToUInt(const char* psz)
{
	return strtoul(psz, NULL, 10);
}


bool_t ToBool(const char* psz)
{
	if (FStrEq(psz, "true"))
	{
		return true;
	}
	else if (FStrEq(psz, "false"))
	{
		return false;
	}

	return (ToInt(psz) != 0);
}


int FindString(const char* psz, char** apsz, int n)
{
	int i;

	for (i = 0; i < n; i++)
	{
		if (FStrEq(psz, apsz[i]))
		{
			return i;
		}
	}

	return -1;
}


//  CutChars( pszSomeText, " \t" ); // will remove all spaces and tabs

int CutChars(char* psz, const char* pszChars)
{
	bool bQuote;
	int i;
	int j;
	int n;

	bQuote = false;
	j = 0;

	for (i = 0; psz[i] != '\0'; i++)
	{
		if (psz[i] == '\"')
		{
			bQuote = !bQuote;
		}
		else
		{
			if (!bQuote)
			{
				for (n = 0; pszChars[n] != '\0'; n++)
				{
					if (pszChars[n] == psz[i])
					{
						goto skip;
					}
				}
			}
		}

		psz[j++] = psz[i];

skip:

		;
	}

	psz[j] = '\0';

	return j;
}


//  ContractChars( pszSomeText, " \t", ' ' ); // spaces and tabs to single spaces

int ContractChars(char* psz, const char* pszChars, int cToChar)
{
	bool bQuote;
	int i;
	int j;
	int n;

	bQuote = false;
	j = 0;

	for (i = 0; psz[i] != '\0'; i++)
	{
		if (psz[i] == '\"')
		{
			bQuote = !bQuote;
		}
		else
		{
			if (!bQuote)
			{
				for (n = 0; pszChars[n] != '\0'; n++)
				{
					if (pszChars[n] == psz[i])
					{
						if ((j == 0) || (psz[j-1] == cToChar))
						{
							goto skip;
						}
						else
						{
							psz[i] = cToChar;
						}
					}
				}
			}
		}

		psz[j++] = psz[i];

skip:

		;
	}

	if (j > 0)
	{
		if (psz[j-1] == cToChar)
		{
			j--;
		}
	}

	psz[j] = '\0';

	return j;
}


//  CutComments( pszSomeText, "/*", "*/" );
//  CutComments( pszSomeText, "//", NULL );

int CutComments(char* psz, const char* pszCommentStart, const char* pszCommentEnd)
{
	bool bQuote;
	bool bComment;
	char* pszCur;
	int i;
	int j;
	int iC;

	bQuote = false;
	bComment = false;
	i = 0;
	j = 0;

	for ( ; psz[i] != '\0'; i++)
	{
		if (bComment)
		{
			if (pszCommentEnd == NULL)
			{
				if (((psz[i] == '\r') && (psz[i+1] == '\n')) || (psz[i] == '\n'))
				{
					bComment = false;
				}
				else
				{
					continue;
				}
			}
			else
			{
				pszCur = &psz[i];
				iC = 0;

				for ( ; pszCommentEnd[iC] != '\0'; iC++)
				{
					if (pszCur[iC] != pszCommentEnd[iC])
					{
						goto skip_end;
					}
				}

				i += iC;
				bComment = false;

skip_end:

				continue;
			}
		}

		if (psz[i] == '\"')
		{
			bQuote = !bQuote;
		}

		if (!bQuote)
		{
			pszCur = &psz[i];
			iC = 0;

			for ( ; pszCommentStart[iC] != '\0'; iC++)
			{
				if (pszCur[iC] != pszCommentStart[iC])
				{
					goto skip_start;
				}
			}

			i += iC;
			bComment = true;

			continue;
		}

skip_start:

		psz[j++] = psz[i];
	}

	psz[j] = '\0';

	return j;
}


int FindChar(int c, const char* psz)
{
	int i;

	for (i = 0; psz[i] != '\0'; i++)
	{
		if (psz[i] == c)
		{
			return i;
		}
	}

	return -1;
}


// strip chars from the left and the right
// StripChars( "  asd asd! ", " " ); // will give "asd asd!"

int StripChars(char* psz, const char* pszChars)
{
	int i, j;

	for (i = 0; psz[i] != '\0'; i++)
	{
		if (!IsChar(psz[i], pszChars))
		{
			break;
		}
	}

	for (j = 0; psz[i] != '\0'; i++, j++)
	{
		psz[j] = psz[i];
	}

	for ( ; j > 0; j--)
	{
		if (!IsChar(psz[j-1], pszChars))
		{
			break;
		}
	}

	psz[j] = '\0';

	return j;
}


int UnpackQuote(char* psz)
{
	int i, j;

	i = (int)strlen(psz);

	if (i >= 2) // XXX:
	{
		if ((psz[0] == '\"') && (psz[i-1] == '\"'))
		{
			i -= 2;

			for (j = i; j > 0; j--)
			{
				psz[j-1] = psz[j];
			}

			psz[i] = '\0';
		}
	}

	return i;
}


int ParseLine(int argcMax, char* argv[], char* psz, const char* pszDelimiters, char** ppszEndPtr)
{
	int argc;
	char* pszIndent;
	char* pszQuoteStart;
	char* pszQuoteEnd;
	bool bSolid;
	bool bMoreArgs;
	int nQuotes;
	int c;
	int i;
	//int n;

	argc = 0;
	pszIndent = psz;
	pszQuoteStart = NULL;
	pszQuoteEnd = NULL;
	bSolid = false;
	bMoreArgs = false;
	nQuotes = 0;

	for (i = 0; ; i++)
	{
		c = psz[i];

		if (c == '\"')
		{
			if (!bSolid)
			{
				pszQuoteStart = &psz[i];
				bSolid = TRUE;
			}
			else
			{
				pszQuoteEnd = &psz[i];
				bSolid = FALSE;
				nQuotes++;
			}

			continue;
		}

		if (!bSolid)
		{
			if ((c == '\0') || IsChar(c, pszDelimiters))
			{
				if (argc < argcMax)
				{
					psz[i] = '\0';

					// cut quotes
					if ((pszQuoteStart == pszIndent) && (pszQuoteEnd == &psz[i-1]))
					{
						if (nQuotes == 1)   // filter multiple quotation
						{
							pszIndent++;
							*pszQuoteStart = '\0';
							*pszQuoteEnd = '\0';
						}
					}

					argv[argc] = pszIndent;
					pszIndent = &psz[i+1];
				}
				else
				{
					if (argcMax != 0)
					{
						bMoreArgs = true;
						break;
					}
				}

				argc++;

				nQuotes = 0;
			}
		}

		if (c == '\0')
		{
			break;
		}
	}

	if (ppszEndPtr != NULL)
	{
		if (bMoreArgs)
		{
			(*ppszEndPtr) = pszIndent;
		}
		else
		{
			(*ppszEndPtr) = NULL;
		}
	}

	return argc;
}


// the buffer must be allocated by ReadFileToBuffer()
// or must include 0 at the [iSize] position
void ParseBuffer(char* buffer, int iSize, PFLINECALLBACK pfnLineCallback, void* param)
{
	char* pszLine;
	int i;

	pszLine = buffer;

	for (i = 0; i <= iSize; i++)
	{
		if ((buffer[i] == '\n') || (buffer[i] == '\0'))
		{
			buffer[i] = '\0';

			if (!pfnLineCallback(pszLine, param))
			{
				break;
			}

			pszLine = &buffer[i+1];
		}
		else if (buffer[i] == '\r')
		{
			buffer[i] = '\0';
		}
	}
}


long _StreamSize(FILE* stream)
{
	long iSize;
	
	fseek(stream, 0, SEEK_END);

	iSize = ftell(stream);

	fseek(stream, 0, SEEK_SET);

	return iSize;
}


char* _ReadStreamToBuffer(FILE* stream, int* piSize)
{
	char* buffer;
	int iSize;

	iSize = _StreamSize(stream);

	if (iSize != 0)
	{
		buffer = (char*)AllocMemory(iSize + 1);

		if (buffer != NULL)
		{
			if (fread(buffer, iSize, 1, stream))
			{
				buffer[iSize] = '\0';

				*piSize = iSize;

				return buffer;
			}

			FreeMemory(buffer);
		}
	}

	return NULL;
}


char* ReadFileToBuffer(const char* pszFileName, int* piSize)
{
	FILE* stream = fopen(pszFileName, "rb");

	if (stream != NULL)
	{
		char* buffer = _ReadStreamToBuffer(stream, piSize);

		if (buffer != NULL)
		{
			fclose(stream);

			return buffer;
		}

		fclose(stream);
	}

	return NULL;
}


char* ReadFileToBufferW(const wchar_t* pszFileName, int* piSize)
{
	FILE* stream = _wfopen(pszFileName, L"rb");

	if (stream != NULL)
	{
		char* buffer = _ReadStreamToBuffer(stream, piSize);

		if (buffer != NULL)
		{
			fclose(stream);

			return buffer;
		}

		fclose(stream);
	}

	return NULL;
}


bool_t SaveToFile(const char* pszFileName, void* buffer, int iSize)
{
	bool_t bSuccess;
	FILE* stream;

	bSuccess = false;

	stream = fopen(pszFileName, "wb");

	if (stream != NULL)
	{
		if (fwrite(buffer, iSize, 1, stream))
		{
			bSuccess = true;
		}

		fclose(stream);
	}

	return bSuccess;
}


bool_t ParseFile(const char* pszFileName, PFLINECALLBACK pfnLineCallback, void* param)
{
	char* buffer;
	int iSize;

	buffer = ReadFileToBuffer(pszFileName, &iSize);

	if (buffer != NULL)
	{
		ParseBuffer(buffer, iSize, pfnLineCallback, param);

		FreeMemory(buffer);

		return true;
	}

	return false;
}


bool_t ParseFileW(const wchar_t* pszFileName, PFLINECALLBACK pfnLineCallback, void* param)
{
	char* buffer;
	int iSize;

	buffer = ReadFileToBufferW(pszFileName, &iSize);

	if (buffer != NULL)
	{
		ParseBuffer(buffer, iSize, pfnLineCallback, param);

		FreeMemory(buffer);

		return true;
	}

	return false;
}


bool_t ParseDirectory(const char* pszPath, bool_t bSubDirs, PDFILECALLBACK pfnFileCallback, void* param)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	char szSearchPath[MAX_PATH];
	char szFileName[MAX_PATH];
	int i;

	sprintf(szSearchPath, "%s\\*", pszPath);

	hFind = FindFirstFile(szSearchPath, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			i = 0;
			while (fd.cFileName[i] == '.')
			{
				i++;
			}
			if (fd.cFileName[i] == '\0')
			{
				continue;
			}

			sprintf(szFileName, "%s\\%s", pszPath, fd.cFileName);

			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (bSubDirs)
				{
					ParseDirectory(szFileName, bSubDirs, pfnFileCallback, param);
				}
			}
			else
			{
				pfnFileCallback(szFileName, &fd, param);
			}
		}
		while (FindNextFile(hFind, &fd));

		FindClose(hFind);

		return true;
	}

	return false;
}


//
// bitmap support
//

/*
bool_t LoadBitmapFromFile(const char* pszFileName, bitmap_t* pbmp)
{
	FILE* stream;
	bool_t bSuccess = false;
	int i;

	memset(pbmp, 0, sizeof(bitmap_t));

	stream = fopen(pszFileName, "rb");

	if (stream != NULL)
	{
		BITMAPFILEHEADER bmf;
		BITMAPINFOHEADER bmi;

		fread(&bmf, sizeof(BITMAPFILEHEADER), 1, stream);

		if (bmf.bfType == 'MB')
		{
			fread(&bmi, sizeof(BITMAPINFOHEADER), 1, stream);
			
			if (bmi.biCompression == BI_RGB)
			{
				if ((bmi.biBitCount == 8) || (bmi.biBitCount == 24) || (bmi.biBitCount == 32))
				{
					pbmp->iWidth = bmi.biWidth;
					pbmp->iHeight = (bmi.biHeight > 0)? bmi.biHeight: -bmi.biHeight;
					pbmp->nBPP = bmi.biBitCount / 8;
					pbmp->iPitch = DWORD_ALIGNED(pbmp->iWidth * pbmp->nBPP);
					pbmp->pixels = (byte_t*)AllocMemory(pbmp->iPitch * pbmp->iHeight);

					if (pbmp->nBPP == 1)
					{
						pbmp->nColors = (bmi.biClrUsed == 0)? 256: bmi.biClrUsed;
						pbmp->pal = (RGBQUAD*)AllocMemory(pbmp->nColors * sizeof(RGBQUAD));
						fread(pbmp->pal, pbmp->nColors * sizeof(RGBQUAD), 1, stream);
					}

					fseek(stream, bmf.bfOffBits, SEEK_SET);

					if (bmi.biHeight > 0)
					{
						for (i = 0; i < pbmp->iHeight; i++)
						{
							fread(&pbmp->pixels[((pbmp->iHeight - 1) - i) * pbmp->iPitch], pbmp->iPitch, 1, stream);
						}
					}
					else
					{
						for (i = 0; i < pbmp->iHeight; i++)
						{
							fread(&pbmp->pixels[i * pbmp->iPitch], pbmp->iPitch, 1, stream);
						}
					}

					bSuccess = true;
				}
			}
		}

		fclose(stream);
	}

	return bSuccess;
}

void FreeBitmap(bitmap_t* pbmp)
{
	if (pbmp->pixels != NULL)
	{
		FreeMemory(pbmp->pixels);
	}

	if (pbmp->nBPP == 1)
	{
		if (pbmp->pal != NULL)
		{
			FreeMemory(pbmp->pal);
		}
	}
}
*/

bitmap_t* AllocBitmap( int iWidth, int iHeight, int nBPP, int nColors )
{
	bitmap_t* pbmp = (bitmap_t*)AllocMemory( sizeof(bitmap_t) );

	pbmp->iWidth = iWidth;
	pbmp->iHeight = iHeight;
	pbmp->nBPP = nBPP;
	pbmp->iPitch = DWORD_ALIGNED(pbmp->iWidth * pbmp->nBPP);
	pbmp->pixels = (byte_t*)AllocMemory(pbmp->iPitch * pbmp->iHeight);

	if ( nColors )
	{
		pbmp->nColors = nColors;
		pbmp->pal = (RGBQUAD*)AllocMemory(pbmp->nColors * sizeof(RGBQUAD));
	}
	else
	{
		pbmp->nColors = 0;
		pbmp->pal = NULL;
	}

	return pbmp;
}


bitmap_t* LoadBitmapFromFile(const char* pszFileName)
{
	FILE* stream;
	bitmap_t* pbmp = NULL;
	int i;

	//memset(pbmp, 0, sizeof(bitmap_t));

	stream = fopen(pszFileName, "rb");

	if (stream != NULL)
	{
		BITMAPFILEHEADER bmf;
		BITMAPINFOHEADER bmi;

		fread(&bmf, sizeof(BITMAPFILEHEADER), 1, stream);

		if (bmf.bfType == 'MB')
		{
			fread(&bmi, sizeof(BITMAPINFOHEADER), 1, stream);
			
			if (bmi.biCompression == BI_RGB)
			{
				if ((bmi.biBitCount == 8) || (bmi.biBitCount == 24) || (bmi.biBitCount == 32))
				{
					// XXX: i'm too lazy to change this code to use AllocBitmap currently...
					pbmp = (bitmap_t*)AllocMemory( sizeof(bitmap_t) );
					pbmp->iWidth = bmi.biWidth;
					pbmp->iHeight = (bmi.biHeight > 0)? bmi.biHeight: -bmi.biHeight;
					pbmp->nBPP = bmi.biBitCount / 8;
					pbmp->iPitch = DWORD_ALIGNED(pbmp->iWidth * pbmp->nBPP);
					pbmp->pixels = (byte_t*)AllocMemory(pbmp->iPitch * pbmp->iHeight);

					if (pbmp->nBPP == 1)
					{
						pbmp->nColors = (bmi.biClrUsed == 0)? 256: bmi.biClrUsed;
						pbmp->pal = (RGBQUAD*)AllocMemory(pbmp->nColors * sizeof(RGBQUAD));
						fread(pbmp->pal, pbmp->nColors * sizeof(RGBQUAD), 1, stream);
					}

					fseek(stream, bmf.bfOffBits, SEEK_SET);

					if (bmi.biHeight > 0)
					{
						for (i = 0; i < pbmp->iHeight; i++)
						{
							fread(&pbmp->pixels[((pbmp->iHeight - 1) - i) * pbmp->iPitch], pbmp->iPitch, 1, stream);
						}
					}
					else
					{
						for (i = 0; i < pbmp->iHeight; i++)
						{
							fread(&pbmp->pixels[i * pbmp->iPitch], pbmp->iPitch, 1, stream);
						}
					}
				}
			}
		}

		fclose(stream);
	}

	return pbmp;
}


void FreeBitmap(bitmap_t* pbmp)
{
	if (pbmp->pixels != NULL)
	{
		FreeMemory(pbmp->pixels);
	}

	if (pbmp->nBPP == 1)
	{
		if (pbmp->pal != NULL)
		{
			FreeMemory(pbmp->pal);
		}
	}

	FreeMemory( pbmp );
}

/*
void GenerateGrayscalePal(RGBQUAD* pa)
{
	int i;

	for (i = 0; i < 256; i++)
	{
		pa[i].rgbRed = pa[i].rgbGreen = pa[i].rgbRed = (unsigned)i;
		pa[i].rgbReserved = 255;
	}
}
*/

bool_t SaveBitmap(const char* pszFileName, bitmap_t* pbmp)
{
	FILE* stream;
	BITMAPFILEHEADER bmf;
	BITMAPINFOHEADER bmi;
	int iDataSize;
	int iPaletteSize;
	int iHeaderSize;
	int iFileSize;
	int iPitchDiff;
	int bInvertOrder;
	int i;

	if (pbmp->iHeight < 0)
	{
		pbmp->iHeight = -pbmp->iHeight;
		bInvertOrder = 1;
	}
	else
	{
		bInvertOrder = 0;
	}

	stream = fopen(pszFileName, "wb");

	if (stream != NULL)
	{
		iPaletteSize = (pbmp->nBPP == 1)? pbmp->nColors * sizeof(RGBQUAD): 0;
		iHeaderSize = DWORD_ALIGNED(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + iPaletteSize);
		iPitchDiff = DWORD_ALIGNED(pbmp->iPitch) - pbmp->iPitch;
		iDataSize = (pbmp->iPitch + iPitchDiff) * pbmp->iHeight;
		iFileSize = iHeaderSize + iDataSize;

		memset(&bmf, 0, sizeof(BITMAPFILEHEADER));
		bmf.bfType = 'MB';
		bmf.bfSize = iFileSize;
		bmf.bfOffBits = iHeaderSize;
		fwrite(&bmf, sizeof(BITMAPFILEHEADER), 1, stream);

		memset(&bmi, 0, sizeof(BITMAPINFOHEADER));
		bmi.biSize = sizeof(BITMAPINFOHEADER);
		bmi.biWidth = pbmp->iWidth;
		bmi.biHeight = bInvertOrder? pbmp->iHeight: -pbmp->iHeight;
		bmi.biPlanes = 1;
		bmi.biBitCount = pbmp->nBPP * 8;
		bmi.biClrUsed = (pbmp->nColors == 256)? 0: pbmp->nColors;
		fwrite(&bmi, sizeof(BITMAPINFOHEADER), 1, stream);

		if (pbmp->nBPP == 1)
		{
			// if (pbmp->pal != NULL)
			fwrite(pbmp->pal, iPaletteSize, 1, stream);
		}

		fseek(stream, iHeaderSize, SEEK_SET);

		for (i = 0; i < pbmp->iHeight; i++)
		{
			fwrite(&pbmp->pixels[i * pbmp->iPitch], pbmp->iPitch, 1, stream);
			if (iPitchDiff)
			{
				fwrite(pbmp->pixels, iPitchDiff, 1, stream);
			}
		}

		fclose(stream);

		return true;
	}

	return false;
}


bool_t SaveBitmapIndirect(const char* pszFileName, int iWidth, int iHeight, int nBPP, int iPitch, byte_t* pixels, int nColors, RGBQUAD* pal)
{
	bitmap_t bmp =
	{
		iWidth,
		iHeight,
		nBPP,
		iPitch,
		pixels,
		nColors,
		pal
	};

	return SaveBitmap(pszFileName, &bmp);
}

