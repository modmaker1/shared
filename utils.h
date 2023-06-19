
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


#ifndef _UTIL_H
#define _UTIL_H

#include <windows.h>

// new here
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define TO_DEG(x) ((x) * (180/M_PI))
#define TO_RAD(x) ((x) * (M_PI/180))


#define DWORD_ALIGNED(v) ((((unsigned int)(v)) + 3) & (-4))
#define ALIGNED(v, a) ((((v) + ((a) - 1)) / (a)) * (a))


#define FStrEq(a, b) (_stricmp((a),(b)) == 0)
#define FWStrEq(a, b) (wcsicmp((a),(b)) == 0)
#define FStrEqW FWStrEq

// rarely used...
#define IsInRange(v, l, u) (((v) >= (l)) && ((v) < (u)))
#define IsValidIndex(i, r) ((unsigned int)(i) < (r))

// still need these?
//typedef unsigned int uint_t;
//typedef unsigned short word_t;
typedef unsigned char byte_t;
typedef unsigned char bool_t;


#ifdef __cplusplus
extern "C" {
#endif

// do not mix such code with c++ (mixed c/c++ projects), use bool_t instead
#ifndef __cplusplus
typedef enum { false, true } bool;
#endif



//
// custom allocators
//


void* AllocMemory(size_t iSize);
void FreeMemory(void* p);

char* AllocString(const char* psz);
wchar_t* AllocStringW(const wchar_t* psz);
wchar_t* AllocStringUnicode(const char* pszSrc);
void FreeStringSafe(const char* psz);
#define FreeString FreeStringSafe
void FreeStringSafeW(const wchar_t* psz);
#define FreeStringW FreeStringSafeW

//
// file name and path functions
//

char* GetExtension(const char* psz);
wchar_t* GetExtensionW(const wchar_t* psz);
char* GetFileName(const char* psz);
wchar_t* GetFileNameW(const wchar_t* psz);

//void ConcatPath(char* buffer, const char* psz);

bool_t CreateDirectoryTree(const char* pszPath);
bool_t CreateDirectoryTreeW(const wchar_t* pszPath);


//
// parser (ANSI only)
//

int ToInt(const char* psz);
unsigned int ToUInt(const char* psz);
bool_t ToBool(const char* psz);

int FindString(const char* psz, char** apsz, int nItems);

int CutChars(char* psz, const char* pszChars);
int ContractChars(char* psz, const char* pszChars, int cToChar);
int CutComments(char* psz, const char* pszCommentStart, const char* pszCommentEnd);
int FindChar(int c, const char* pszChars);
#define IsChar(c,psz) (FindChar((c),(psz))!=-1)
int StripChars(char* psz, const char* pszChars);
int UnpackQuote(char* psz);

int ParseLine(int argcMax, char* argv[], char* psz, const char* pszDelimiters, char** ppszEndPtr);

char* ReadFileToBuffer(const char* pszFileName, int* piSize);
char* ReadFileToBufferW(const wchar_t* pszFileName, int* piSize);

bool_t SaveToFile(const char* pszFileName, void* buffer, int iSize);
//void SaveToFileW(const char* pszFileName, void* buffer int iSize);

typedef int (*PFLINECALLBACK)(char* pszLine, void* param); // return 0 to stop, 1 to continue
void ParseBuffer(char* buffer, int iSize, PFLINECALLBACK pfnLineCallback, void* param);
bool_t ParseFile(const char* pszFileName, PFLINECALLBACK pfnLineCallback, void* param);
bool_t ParseFileW(const wchar_t* pszFileName, PFLINECALLBACK pfnLineCallback, void* param);

typedef void (*PDFILECALLBACK)(char* pszFileName, WIN32_FIND_DATA* pfd, void* param);
bool_t ParseDirectory(const char* pszPath, bool_t bSubDirs, PDFILECALLBACK pfnFileCallback, void* param);
bool_t ParseDirectoryW(const wchar_t* pszPath, bool_t bSubDirs, PDFILECALLBACK pfnFileCallback, void* param);


//
// bitmap support
//

typedef struct bitmap_s
{
	int iWidth;
	int iHeight;
	int nBPP; // bytes per pixel
	int iPitch;
	byte_t* pixels;
	int nColors;
	RGBQUAD* pal;
} bitmap_t;

bitmap_t* AllocBitmap( int iWidth, int iHeight, int nBPP, int nColors ); // need testing
bitmap_t* LoadBitmapFromFile(const char* pszFileName);
void FreeBitmap(bitmap_t* pbmp);

bool_t SaveBitmap(const char* pszFileName, bitmap_t* pbmp);
bool_t SaveBitmapIndirect(const char* pszFileName, int iWidth, int iHeight, int nBPP, int iPitch, byte_t* pixels, int nColors, RGBQUAD* pal);


#ifdef __cplusplus
}
#endif

#endif // UTIL_H


