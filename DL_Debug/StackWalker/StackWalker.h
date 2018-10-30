/**********************************************************************
 * 
 * BaseStackWalker.h
 *
 *
 * History:
 *  2005-07-27   v1    - First public release on http://www.codeproject.com/
 *  (for additional changes see History in 'BaseStackWalker.cpp'!
 *
 **********************************************************************/
// #pragma once is supported starting with _MCS_VER 1000, 
// so we need not to check the version (because we only support _MSC_VER >= 1100)!
#pragma once

#include <windows.h>
#include <vector>

// If VC7 and later, then use the shipped 'dbghelp.h'-file
#if _MSC_VER >= 1300
#include <dbghelp.h>
#else
// inline the important dbghelp.h-declarations...
typedef enum {
	SymNone = 0,
	SymCoff,
	SymCv,
	SymPdb,
	SymExport,
	SymDeferred,
	SymSym,
	SymDia,
	SymVirtual,
	NumSymTypes
} SYM_TYPE;
typedef struct _IMAGEHLP_LINE64 {
	DWORD                       SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE64)
	PVOID                       Key;                    // internal
	DWORD                       LineNumber;             // line number in file
	PCHAR                       FileName;               // full filename
	DWORD64                     Address;                // first instruction of line
} IMAGEHLP_LINE64, *PIMAGEHLP_LINE64;
typedef struct _IMAGEHLP_MODULE64 {
	DWORD                       SizeOfStruct;           // set to sizeof(IMAGEHLP_MODULE64)
	DWORD64                     BaseOfImage;            // base load address of module
	DWORD                       ImageSize;              // virtual size of the loaded module
	DWORD                       TimeDateStamp;          // date/time stamp from pe header
	DWORD                       CheckSum;               // checksum from the pe header
	DWORD                       NumSyms;                // number of symbols in the symbol table
	SYM_TYPE                    SymType;                // type of symbols loaded
	CHAR                        ModuleName[32];         // module name
	CHAR                        ImageName[256];         // image name
	CHAR                        LoadedImageName[256];   // symbol file name
} IMAGEHLP_MODULE64, *PIMAGEHLP_MODULE64;
typedef struct _IMAGEHLP_SYMBOL64 {
	DWORD                       SizeOfStruct;           // set to sizeof(IMAGEHLP_SYMBOL64)
	DWORD64                     Address;                // virtual address including dll base address
	DWORD                       Size;                   // estimated size of symbol, can be zero
	DWORD                       Flags;                  // info about the symbols, see the SYMF defines
	DWORD                       MaxNameLength;          // maximum size of symbol name in 'Name'
	CHAR                        Name[1];                // symbol name (null terminated string)
} IMAGEHLP_SYMBOL64, *PIMAGEHLP_SYMBOL64;
typedef enum {
	AddrMode1616,
	AddrMode1632,
	AddrModeReal,
	AddrModeFlat
} ADDRESS_MODE;
typedef struct _tagADDRESS64 {
	DWORD64       Offset;
	WORD          Segment;
	ADDRESS_MODE  Mode;
} ADDRESS64, *LPADDRESS64;
typedef struct _KDHELP64 {
	DWORD64   Thread;
	DWORD   ThCallbackStack;
	DWORD   ThCallbackBStore;
	DWORD   NextCallback;
	DWORD   FramePointer;
	DWORD64   KiCallUserMode;
	DWORD64   KeUserCallbackDispatcher;
	DWORD64   SystemRangeStart;
	DWORD64  Reserved[8];
} KDHELP64, *PKDHELP64;
typedef struct _tagSTACKFRAME64 {
	ADDRESS64   AddrPC;               // program counter
	ADDRESS64   AddrReturn;           // return address
	ADDRESS64   AddrFrame;            // frame pointer
	ADDRESS64   AddrStack;            // stack pointer
	ADDRESS64   AddrBStore;           // backing store pointer
	PVOID       FuncTableEntry;       // pointer to pdata/fpo or NULL
	DWORD64     Params[4];            // possible arguments to the function
	BOOL        Far;                  // WOW far call
	BOOL        Virtual;              // is this a virtual frame?
	DWORD64     Reserved[3];
	KDHELP64    KdHelp;
} STACKFRAME64, *LPSTACKFRAME64;
typedef
BOOL
(__stdcall *PREAD_PROCESS_MEMORY_ROUTINE64)(
	HANDLE      hProcess,
	DWORD64     qwBaseAddress,
	PVOID       lpBuffer,
	DWORD       nSize,
	LPDWORD     lpNumberOfBytesRead
	);
typedef
PVOID
(__stdcall *PFUNCTION_TABLE_ACCESS_ROUTINE64)(
	HANDLE  hProcess,
	DWORD64 AddrBase
	);
typedef
DWORD64
(__stdcall *PGET_MODULE_BASE_ROUTINE64)(
	HANDLE  hProcess,
	DWORD64 Address
	);
typedef
DWORD64
(__stdcall *PTRANSLATE_ADDRESS_ROUTINE64)(
	HANDLE    hProcess,
	HANDLE    hThread,
	LPADDRESS64 lpaddr
	);
#define SYMOPT_CASE_INSENSITIVE         0x00000001
#define SYMOPT_UNDNAME                  0x00000002
#define SYMOPT_DEFERRED_LOADS           0x00000004
#define SYMOPT_NO_CPP                   0x00000008
#define SYMOPT_LOAD_LINES               0x00000010
#define SYMOPT_OMAP_FIND_NEAREST        0x00000020
#define SYMOPT_LOAD_ANYTHING            0x00000040
#define SYMOPT_IGNORE_CVREC             0x00000080
#define SYMOPT_NO_UNQUALIFIED_LOADS     0x00000100
#define SYMOPT_FAIL_CRITICAL_ERRORS     0x00000200
#define SYMOPT_EXACT_SYMBOLS            0x00000400
#define SYMOPT_ALLOW_ABSOLUTE_SYMBOLS   0x00000800
#define SYMOPT_IGNORE_NT_SYMPATH        0x00001000
#define SYMOPT_INCLUDE_32BIT_MODULES    0x00002000
#define SYMOPT_PUBLICS_ONLY             0x00004000
#define SYMOPT_NO_PUBLICS               0x00008000
#define SYMOPT_AUTO_PUBLICS             0x00010000
#define SYMOPT_NO_IMAGE_SEARCH          0x00020000
#define SYMOPT_SECURE                   0x00040000
#define SYMOPT_DEBUG                    0x80000000
#define UNDNAME_COMPLETE                 (0x0000)  // Enable full undecoration
#define UNDNAME_NAME_ONLY                (0x1000)  // Crack only the name for primary declaration;
#endif  // _MSC_VER < 1300

// Some missing defines (for VC5/6):
#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif  


// secure-CRT_functions are only available starting with VC8
#if _MSC_VER < 1400
#define strcpy_s strcpy
#define strcat_s(dst, len, src) strcat(dst, src)
#define _snprintf_s _snprintf
#define _tcscat_s _tcscat
#endif


// special defines for VC5/6 (if no actual PSDK is installed):
#if _MSC_VER < 1300
typedef unsigned __int64 DWORD64, *PDWORD64;
#if defined(_WIN64)
typedef unsigned __int64 SIZE_T, *PSIZE_T;
#else
typedef unsigned long SIZE_T, *PSIZE_T;
#endif
#endif  // _MSC_VER < 1300
class BaseStackWalker;
class StackWalkerInternal
{
public:
	StackWalkerInternal() = default;
	StackWalkerInternal(BaseStackWalker *parent, HANDLE hProcess);
	~StackWalkerInternal();
	BOOL Init(LPCSTR szSymPath);

	BaseStackWalker *m_parent;

	HMODULE m_hDbhHelp;
	HANDLE m_hProcess;
	LPSTR m_szSymPath;

	/*typedef struct IMAGEHLP_MODULE64_V3 {
	DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_MODULE64)
	DWORD64  BaseOfImage;            // base load address of module
	DWORD    ImageSize;              // virtual size of the loaded module
	DWORD    TimeDateStamp;          // date/time stamp from pe header
	DWORD    CheckSum;               // checksum from the pe header
	DWORD    NumSyms;                // number of symbols in the symbol table
	SYM_TYPE SymType;                // type of symbols loaded
	CHAR     ModuleName[32];         // module name
	CHAR     ImageName[256];         // image name
	// new elements: 07-Jun-2002
	CHAR     LoadedImageName[256];   // symbol file name
	CHAR     LoadedPdbName[256];     // pdb file name
	DWORD    CVSig;                  // Signature of the CV record in the debug directories
	CHAR         CVData[MAX_PATH * 3];   // Contents of the CV record
	DWORD    PdbSig;                 // Signature of PDB
	GUID     PdbSig70;               // Signature of PDB (VC 7 and up)
	DWORD    PdbAge;                 // DBI age of pdb
	BOOL     PdbUnmatched;           // loaded an unmatched pdb
	BOOL     DbgUnmatched;           // loaded an unmatched dbg
	BOOL     LineNumbers;            // we have line number information
	BOOL     GlobalSymbols;          // we have internal symbol information
	BOOL     TypeInfo;               // we have type information
	// new elements: 17-Dec-2003
	BOOL     SourceIndexed;          // pdb supports source server
	BOOL     Publics;                // contains public symbols
	};
	*/
	struct IMAGEHLP_MODULE64_V2 {
		DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_MODULE64)
		DWORD64  BaseOfImage;            // base load address of module
		DWORD    ImageSize;              // virtual size of the loaded module
		DWORD    TimeDateStamp;          // date/time stamp from pe header
		DWORD    CheckSum;               // checksum from the pe header
		DWORD    NumSyms;                // number of symbols in the symbol table
		SYM_TYPE SymType;                // type of symbols loaded
		CHAR     ModuleName[32];         // module name
		CHAR     ImageName[256];         // image name
		CHAR     LoadedImageName[256];   // symbol file name
	};


	// SymCleanup()
	typedef BOOL(__stdcall *tSC)(IN HANDLE hProcess);
	tSC pSC;

	// SymFunctionTableAccess64()
	typedef PVOID(__stdcall *tSFTA)(HANDLE hProcess, DWORD64 AddrBase);
	tSFTA pSFTA;

	// SymGetLineFromAddr64()
	typedef BOOL(__stdcall *tSGLFA)(IN HANDLE hProcess, IN DWORD64 dwAddr,
									OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE64 Line);
	tSGLFA pSGLFA;

	// SymGetModuleBase64()
	typedef DWORD64(__stdcall *tSGMB)(IN HANDLE hProcess, IN DWORD64 dwAddr);
	tSGMB pSGMB;

	// SymGetModuleInfo64()
	typedef BOOL(__stdcall *tSGMI)(IN HANDLE hProcess, IN DWORD64 dwAddr, OUT IMAGEHLP_MODULE64_V2 *ModuleInfo);
	tSGMI pSGMI;

	//  // SymGetModuleInfo64()
	//  typedef BOOL (__stdcall *tSGMI_V3)( IN HANDLE hProcess, IN DWORD64 dwAddr, OUT IMAGEHLP_MODULE64_V3 *ModuleInfo );
	//  tSGMI_V3 pSGMI_V3;

	// SymGetOptions()
	typedef DWORD(__stdcall *tSGO)(VOID);
	tSGO pSGO;

	// SymGetSymFromAddr64()
	typedef BOOL(__stdcall *tSGSFA)(IN HANDLE hProcess, IN DWORD64 dwAddr,
									OUT PDWORD64 pdwDisplacement, OUT PIMAGEHLP_SYMBOL64 Symbol);
	tSGSFA pSGSFA;

	// SymInitialize()
	typedef BOOL(__stdcall *tSI)(IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess);
	tSI pSI;

	// SymLoadModule64()
	typedef DWORD64(__stdcall *tSLM)(IN HANDLE hProcess, IN HANDLE hFile,
									 IN PSTR ImageName, IN PSTR ModuleName, IN DWORD64 BaseOfDll, IN DWORD SizeOfDll);
	tSLM pSLM;

	// SymSetOptions()
	typedef DWORD(__stdcall *tSSO)(IN DWORD SymOptions);
	tSSO pSSO;

	// StackWalk64()
	typedef BOOL(__stdcall *tSW)(
		DWORD MachineType,
		HANDLE hProcess,
		HANDLE hThread,
		LPSTACKFRAME64 StackFrame,
		PVOID ContextRecord,
		PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
		PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
		PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
		PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);
	tSW pSW;

	// UnDecorateSymbolName()
	typedef DWORD(__stdcall WINAPI *tUDSN)(PCSTR DecoratedName, PSTR UnDecoratedName,
										   DWORD UndecoratedLength, DWORD Flags);
	tUDSN pUDSN;

	typedef BOOL(__stdcall WINAPI *tSGSP)(HANDLE hProcess, PSTR SearchPath, DWORD SearchPathLength);
	tSGSP pSGSP;


private:
	// **************************************** ToolHelp32 ************************
#define MAX_MODULE_NAME32 255
#define TH32CS_SNAPMODULE   0x00000008
#pragma pack( push, 8 )
	typedef struct tagMODULEENTRY32
	{
		DWORD   dwSize;
		DWORD   th32ModuleID;       // This module
		DWORD   th32ProcessID;      // owning process
		DWORD   GlblcntUsage;       // Global usage count on the module
		DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
		BYTE  * modBaseAddr;        // Base address of module in th32ProcessID's context
		DWORD   modBaseSize;        // Size in bytes of module starting at modBaseAddr
		HMODULE hModule;            // The hModule of this module in th32ProcessID's context
		char    szModule[MAX_MODULE_NAME32 + 1];
		char    szExePath[MAX_PATH];
	} MODULEENTRY32;
	typedef MODULEENTRY32 *  PMODULEENTRY32;
	typedef MODULEENTRY32 *  LPMODULEENTRY32;
#pragma pack( pop )

	BOOL GetModuleListTH32(HANDLE hProcess, DWORD pid);  // GetModuleListTH32

	   // **************************************** PSAPI ************************
	typedef struct _MODULEINFO {
		LPVOID lpBaseOfDll;
		DWORD SizeOfImage;
		LPVOID EntryPoint;
	} MODULEINFO, *LPMODULEINFO;

	BOOL GetModuleListPSAPI(HANDLE hProcess);  // GetModuleListPSAPI

	DWORD LoadModule(HANDLE hProcess, LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size);
public:
	BOOL LoadModules(HANDLE hProcess, DWORD dwProcessId);


	BOOL GetModuleInfo(HANDLE hProcess, DWORD64 baseAddr, IMAGEHLP_MODULE64_V2 *pModuleInfo);
};

class StackWalkerInternal;  // forward
class BaseStackWalker
{
public:
  typedef enum StackWalkOptions
  {
    // No addition info will be retrived 
    // (only the address is available)
    RetrieveNone = 0,
    
    // Try to get the symbol-name
    RetrieveSymbol = 1,
    
    // Try to get the line for this symbol
    RetrieveLine = 2,
    
    // Try to retrieve the module-infos
    RetrieveModuleInfo = 4,
    
    // Also retrieve the version for the DLL/EXE
    RetrieveFileVersion = 8,
    
    // Contains all the abouve
    RetrieveVerbose = 0xF,
    
    // Generate a "good" symbol-search-path
    SymBuildPath = 0x10,
    
    // Also use the public Microsoft-Symbol-Server
    SymUseSymSrv = 0x20,
    
    // Contains all the abouve "Sym"-options
    SymAll = 0x30,
    
    // Contains all options (default)
    OptionsAll = 0x3F
  } StackWalkOptions;

  BaseStackWalker(
    int options = OptionsAll, // 'int' is by design, to combine the enum-flags
    LPCSTR szSymPath = NULL, 
    DWORD dwProcessId = GetCurrentProcessId(), 
    HANDLE hProcess = GetCurrentProcess()
    );
  BaseStackWalker(DWORD dwProcessId, HANDLE hProcess);
  virtual ~BaseStackWalker();

  typedef BOOL (__stdcall *PReadProcessMemoryRoutine)(
    HANDLE      hProcess,
    DWORD64     qwBaseAddress,
    PVOID       lpBuffer,
    DWORD       nSize,
    LPDWORD     lpNumberOfBytesRead,
    LPVOID      pUserData  // optional data, which was passed in "ShowCallstack"
    );

  BOOL LoadModules();

  BOOL ShowCallstack(int depth, HANDLE hThread = GetCurrentThread(), const CONTEXT *context = NULL, PReadProcessMemoryRoutine readMemoryFunction = NULL, LPVOID pUserData = NULL  /*optional to identify some data in the 'readMemoryFunction'-callback */);

  std::string GetEntry(int index) const { return m_Callstack[index]; }
#if _MSC_VER >= 1300
// due to some reasons, the "STACKWALK_MAX_NAMELEN" must be declared as "public" 
// in older compilers in order to use it... starting with VC7 we can declare it as "protected"



protected:
#endif
	enum { STACKWALK_MAX_NAMELEN = 1024 }; // max name length for found symbols

protected:
  // Entry for each Callstack-Entry
  typedef struct CallstackEntry
  {
    DWORD64 offset;  // if 0, we have no valid entry
    CHAR name[STACKWALK_MAX_NAMELEN];
    CHAR undName[STACKWALK_MAX_NAMELEN];
    CHAR undFullName[STACKWALK_MAX_NAMELEN];
    DWORD64 offsetFromSmybol;
    DWORD offsetFromLine;
    DWORD lineNumber;
    CHAR lineFileName[STACKWALK_MAX_NAMELEN];
    DWORD symType;
    LPCSTR symTypeString;
    CHAR moduleName[STACKWALK_MAX_NAMELEN];
    DWORD64 baseOfImage;
    CHAR loadedImageName[STACKWALK_MAX_NAMELEN];
  } CallstackEntry;

  enum CallstackEntryType {firstEntry, nextEntry, lastEntry};

  virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry, int depth);
  virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr);
  virtual void OnOutput(char* szText, int depth = 0);

  StackWalkerInternal m_sw;
  HANDLE m_hProcess;
  DWORD m_dwProcessId;
  BOOL m_modulesLoaded;
  LPSTR m_szSymPath;

  int m_options;

  static BOOL __stdcall myReadProcMem(HANDLE hProcess, DWORD64 qwBaseAddress, PVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead);

  std::string m_Callstack[4]; //max length 4;

  friend StackWalkerInternal;
};


// The "ugly" assembler-implementation is needed for systems before XP
// If you have a new PSDK and you only compile for XP and later, then you can use 
// the "RtlCaptureContext"
// Currently there is no define which determines the PSDK-Version... 
// So we just use the compiler-version (and assumes that the PSDK is 
// the one which was installed by the VS-IDE)

// INFO: If you want, you can use the RtlCaptureContext if you only target XP and later...
//       But I currently use it in x64/IA64 environments...
//#if defined(_M_IX86) && (_WIN32_WINNT <= 0x0500) && (_MSC_VER < 1400)

#if defined(_M_IX86)
#ifdef CURRENT_THREAD_VIA_EXCEPTION
// TODO: The following is not a "good" implementation, 
// because the callstack is only valid in the "__except" block...
#define GET_CURRENT_CONTEXT(c, contextFlags) \
  do { \
    memset(&c, 0, sizeof(CONTEXT)); \
    EXCEPTION_POINTERS *pExp = NULL; \
    __try { \
      throw 0; \
    } __except( ( (pExp = GetExceptionInformation()) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_EXECUTE_HANDLER)) {} \
    if (pExp != NULL) \
      memcpy(&c, pExp->ContextRecord, sizeof(CONTEXT)); \
      c.ContextFlags = contextFlags; \
  } while(0);
#else
// The following should be enough for walking the callstack...
#define GET_CURRENT_CONTEXT(c, contextFlags) \
  do { \
    memset(&c, 0, sizeof(CONTEXT)); \
    c.ContextFlags = contextFlags; \
    __asm    call x \
    __asm x: pop eax \
    __asm    mov c.Eip, eax \
    __asm    mov c.Ebp, ebp \
    __asm    mov c.Esp, esp \
  } while(0);
#endif

#else

// The following is defined for x86 (XP and higher), x64 and IA64:
#define GET_CURRENT_CONTEXT(c, contextFlags) \
  do { \
    memset(&c, 0, sizeof(CONTEXT)); \
    c.ContextFlags = contextFlags; \
    RtlCaptureContext(&c); \
} while(0);
#endif
