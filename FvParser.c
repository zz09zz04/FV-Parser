#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(__WIN32__)

#include <stdint.h>
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;

#include <windows.h>

#elif

typedef struct {
	u_int32_t	Data1;
	u_int16_t	Data2;
	u_int16_t	Data3;
	u_int8_t	Data4[8];
} GUID;

#endif

#define EFI_FIRMWARE_FILE_SYSTEM3_GUID  { 0x5473c07a, 0x3dcb, 0x4dca, { 0xbd, 0x6f, 0x1e, 0x96, 0x89, 0xe7, 0x34, 0x9a } }
#define EFI_FFS_VOLUME_TOP_FILE_GUID    { 0x1BA0062E, 0xC779, 0x4582, 0x85, 0x66, 0x33, 0x6A, 0xE8, 0xF7, 0x8F, 0x9 }



#define	Print	printf
#define	ERROR_EXIT	-1
#define SIGNATURE_16(A, B) ((A) | (B << 8))
#define SIGNATURE_32(A, B, C, D) (SIGNATURE_16 (A, B) | (SIGNATURE_16 (C, D) << 16))
#define CR(Record, TYPE, Field) \
	((TYPE *) ((u_int8_t *) (Record) - (u_int8_t *) &(((TYPE *) 0)->Field)))

#define FFS_ATTRIB_LARGE_FILE         0x01
#define FFS_ATTRIB_FIXED              0x04
#define FFS_ATTRIB_DATA_ALIGNMENT     0x38
#define FFS_ATTRIB_CHECKSUM           0x40

#define IS_FFS_FILE2(FfsFileHeaderPtr) \
    (((((EFI_FFS_FILE_HEADER *) (u_int64_t) FfsFileHeaderPtr)->Attributes) & FFS_ATTRIB_LARGE_FILE) == FFS_ATTRIB_LARGE_FILE)

#define FFS_FILE_SIZE(FfsFileHeaderPtr) \
    ((u_int32_t) (*((u_int32_t *) ((EFI_FFS_FILE_HEADER *) (u_int64_t) FfsFileHeaderPtr)->Size) & 0x00ffffff))

#define FFS_FILE2_SIZE(FfsFileHeaderPtr) \
    (((EFI_FFS_FILE_HEADER2 *) (u_int64_t) FfsFileHeaderPtr)->ExtendedSize)

#define EFI_FIRMWARE_FILE_SYSTEM2_GUID	


//#define	FILE_NAME	"FVRECOVERY.Fv"
#define	FILE_NAME	"FVOSBOOT.Fv"


typedef u_int32_t EFI_FVB_ATTRIBUTES_2;

#pragma pack(1)

typedef struct {
	u_int32_t NumBlocks;
	u_int32_t Length;
} EFI_FV_BLOCK_MAP;

typedef struct {
  u_int8_t              ZeroVector[16];
  GUID                  FileSystemGuid;
  u_int64_t             FvLength;
  u_int32_t             Signature;
  EFI_FVB_ATTRIBUTES_2  Attributes;
  u_int16_t             HeaderLength;
  u_int16_t             Checksum;
  u_int16_t             ExtHeaderOffset;
  u_int8_t              Reserved[1];
  u_int8_t              Revision;
  EFI_FV_BLOCK_MAP      BlockMap[];
} EFI_FIRMWARE_VOLUME_HEADER;

typedef union {
	struct {
		u_int8_t	Header;
		u_int8_t	File;
	}				Checksum;
	u_int16_t		Checksum16;
} EFI_FFS_INTEGRITY_CHECK;

typedef u_int8_t EFI_FV_FILETYPE;

typedef u_int8_t EFI_FFS_FILE_ATTRIBUTES;

typedef u_int8_t EFI_FFS_FILE_STATE;

typedef struct {
  GUID                    Name;
  EFI_FFS_INTEGRITY_CHECK IntegrityCheck;
  EFI_FV_FILETYPE         Type;
  EFI_FFS_FILE_ATTRIBUTES Attributes;
  u_int8_t                Size[3];
  EFI_FFS_FILE_STATE      State;
} EFI_FFS_FILE_HEADER;

typedef struct {
  GUID                    Name;
  EFI_FFS_INTEGRITY_CHECK IntegrityCheck;
  EFI_FV_FILETYPE         Type;
  EFI_FFS_FILE_ATTRIBUTES Attributes;
  u_int8_t                Size[3];
  EFI_FFS_FILE_STATE      State;
  u_int64_t               ExtendedSize;
} EFI_FFS_FILE_HEADER2;

#pragma pack()


#define BLUE_TEXT   FOREGROUND_BLUE
#define GREEN_TEXT  FOREGROUND_GREEN
#define RED_TEXT    FOREGROUND_RED
#define CYAN_TEXT   FOREGROUND_BLUE | FOREGROUND_GREEN
#define PURPLE_TEXT FOREGROUND_BLUE | FOREGROUND_RED
#define ORANGE_TEXT FOREGROUND_GREEN | FOREGROUND_RED
#define WHITE_TEXT  FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
#define GRAY_TEXT   FOREGROUND_INTENSITY
#define LIGHTBLUE_TEXT   FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define LIGHTGREEN_TEXT  FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define LIGHTRED_TEXT    FOREGROUND_RED | FOREGROUND_INTENSITY
#define LIGHTCYAN_TEXT   FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define LIGHTPURPLE_TEXT FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY
#define LIGHTORANGE_TEXT FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY


GUID FirmwareFileSystem2Guid = {0x8c8ce578, 0x8a3d, 0x4f1c, 0x99, 0x35, 0x89, 0x61, 0x85, 0xc3, 0x2d, 0xd3};
GUID FirmwareFileSystem3Guid = EFI_FIRMWARE_FILE_SYSTEM3_GUID;


int SetScreenColor (WORD TextColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    // Save current attributes 
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, TextColor);
//    printf("This is some nice COLORFUL text, isn't it? 0x%x\n", saved_attributes);

    // Restore original attributes 
//    SetConsoleTextAttribute(hConsole, saved_attributes);
//    printf("Back to normal 0x%x\n", saved_attributes);

    return 0;
}


long InternalGetFileSize (char *FileName)
{
  FILE *fp = NULL;
  long FileSize;

  FileSize = 0;

  fp = fopen(FileName, "rb");
  if (fp == NULL) {
  	return 0;
  }

  fseek (fp, 0, SEEK_END);
  FileSize = ftell (fp);
  fclose(fp);

  return FileSize;
}

void ShowRawData(u_int8_t *Buffer, u_int64_t size)
{
  int i,j;
	for (i = 0 ; i < size / 16 ; i++) {
//		Print ("%s","\033[33m");
    SetScreenColor (ORANGE_TEXT);
    Print ("%08x:",i * 16);
//		Print ("%s","\033[39m");
    SetScreenColor (WHITE_TEXT);
		for (j = 0 ; j < 16 ; j++) {
			Print (" %02x", Buffer[i*16+j]);
		}
		Print ("  ");
		for (j = 0 ; j < 16 ; j++) {
			u_int8_t data = Buffer[i*16+j];
			if (data >= 0x20 && data <= 0x7E) {
				Print ("%c",data);
			} else {
				Print (".");
			}
		}
		Print ("\n");
	}
	if (size % 16 != 0) {
		Print ("%s","\033[33m");
		Print ("%08x:",size / 16 * 16);
		Print ("%s","\033[39m");
		for (j = 0 ; j < 16 ; j++) {
			if (j > size % 16 - 1) {
				Print ("   ");
			} else {
				Print (" %02x", Buffer[size / 16 * 16 + j]);
			}
		}
		Print ("  ");
		for (j = 0 ; j < size % 16 ; j++) {
			u_int8_t data = Buffer[size / 16 * 16 + j];
			if (data >= 0x20 && data <= 0x7E) {
				Print ("%c",data);
			} else {
				Print (".");
			}
		}
		Print ("\n");
	}
}

void ShowFvRawData(u_int8_t *Buffer, u_int64_t size)
{
  int i, j;
	for (i = 0 ; i < size / 16 ; i++) {
//    Print ("%s","\033[33m");
    SetScreenColor (ORANGE_TEXT);
		Print ("%08x:",i * 16);
//    Print ("%s","\033[39m");
    SetScreenColor (WHITE_TEXT);
		for (j = 0 ; j < 16 ; j++) {
			if (i * 16 + j >= 16) {
//				Print ("\033[32m");
      SetScreenColor (BLUE_TEXT);
			}
			if (i * 16 + j >= 32) {
//				Print ("\033[34m");
      SetScreenColor (GREEN_TEXT);
			}
			if (i * 16 + j >= 40) {
//				Print ("\033[31m");
      SetScreenColor (RED_TEXT);
			}
			if (i * 16 + j >= 44) {
//				Print ("\033[35m");
      SetScreenColor (CYAN_TEXT);
			}
			if (i * 16 + j >= 48) {
//				Print ("\033[31m");
      SetScreenColor (PURPLE_TEXT);
			}
			if (i * 16 + j >= 50) {
//				Print ("\033[33m");
      SetScreenColor (LIGHTBLUE_TEXT);
			}
			if (i * 16 + j >= 52) {
//				Print ("\033[32m");
      SetScreenColor (LIGHTGREEN_TEXT);
			}
			if (i * 16 + j >= 54) {
//				Print ("\033[39m");
      SetScreenColor (LIGHTRED_TEXT);
			}
			if (i * 16 + j >= 55) {
//				Print ("\033[35m");
      SetScreenColor (LIGHTCYAN_TEXT);
			}
			if (i * 16 + j >= 56) {
//				Print ("\033[36m");
      SetScreenColor (WHITE_TEXT);
			}
			if (i * 16 + j >= 72) {
//				Print ("\033[39m");
      SetScreenColor (LIGHTPURPLE_TEXT);
			}
			Print (" %02x", Buffer[i*16+j]);
		}
		Print ("  ");
		for (j = 0 ; j < 16 ; j++) {
      if (i * 16 + j >= 16) {
//        Print ("\033[32m");
      SetScreenColor (BLUE_TEXT);
      }
      if (i * 16 + j >= 32) {
//        Print ("\033[34m");
      SetScreenColor (GREEN_TEXT);
      }
      if (i * 16 + j >= 40) {
//        Print ("\033[31m");
      SetScreenColor (RED_TEXT);
      }
      if (i * 16 + j >= 44) {
//        Print ("\033[35m");
      SetScreenColor (CYAN_TEXT);
      }
      if (i * 16 + j >= 48) {
//        Print ("\033[31m");
      SetScreenColor (PURPLE_TEXT);
      }
      if (i * 16 + j >= 50) {
//        Print ("\033[33m");
      SetScreenColor (LIGHTBLUE_TEXT);
      }
      if (i * 16 + j >= 52) {
//        Print ("\033[32m");
      SetScreenColor (LIGHTGREEN_TEXT);
      }
      if (i * 16 + j >= 54) {
//        Print ("\033[39m");
      SetScreenColor (LIGHTRED_TEXT);
      }
      if (i * 16 + j >= 55) {
//        Print ("\033[35m");
      SetScreenColor (LIGHTCYAN_TEXT);
      }
      if (i * 16 + j >= 56) {
//        Print ("\033[36m");
      SetScreenColor (WHITE_TEXT);
      }
      if (i * 16 + j >= 72) {
//        Print ("\033[39m");
      SetScreenColor (LIGHTPURPLE_TEXT);
      }
			u_int8_t data = Buffer[i*16+j];

			if (data >= 0x20 && data <= 0x7E) {
				Print ("%c",data);
			} else {
				Print (".");
			}
		}
		Print ("\n");
	}
	if (size % 16 != 0) {
//		Print ("%s","\033[33m");
    SetScreenColor (ORANGE_TEXT);
		Print ("%08x:",size / 16 * 16);
//		Print ("%s","\033[39m");
    SetScreenColor (WHITE_TEXT);
		for (j = 0 ; j < 16 ; j++) {
			if (j > size % 16 - 1) {
				Print ("   ");
			} else {
				Print (" %02x", Buffer[size / 16 * 16 + j]);
			}
		}
		Print ("  ");
		for (j = 0 ; j < size % 16 ; j++) {
			u_int8_t data = Buffer[size / 16 * 16 + j];
			if (data >= 0x20 && data <= 0x7E) {
				Print ("%c",data);
			} else {
				Print (".");
			}
		}
		Print ("\n");
	}
}

void PrintGuid (GUID *address)
{
  GUID *guid;

  guid = address;

  Print ("%08x-",guid->Data1);
  Print ("%04x-",guid->Data2);
  Print ("%04x-",guid->Data3);
  for (int i = 0 ; i < 8 ; i++)
    Print ("%02x",guid->Data4[i]);
//  Print ("\n");
}

int CompareGuid (GUID *Guid1, GUID *Guid2)
{
  if (Guid1->Data1 != Guid2->Data1) {
    return 0;
  }
  if (Guid1->Data2 != Guid2->Data2) {
    return 0;
  }
  if (Guid1->Data3 != Guid2->Data3) {
    return 0;
  }
  for (int i = 0 ; i < 8 ; i++) {
    if (Guid1->Data4[i] != Guid2->Data4[i]) {
  	  return 0;
    }
  }
  return 1;
}

long ParseFfsHeader (void* address) 
{
  EFI_FFS_FILE_HEADER *FfsFileHeader = address;

  Print ("File Name(GUID): ");
  SetScreenColor (LIGHTORANGE_TEXT);
  PrintGuid (&FfsFileHeader->Name);
  if (CompareGuid (&FfsFileHeader->Name, &(GUID)EFI_FFS_VOLUME_TOP_FILE_GUID)) {
    SetScreenColor (RED_TEXT);
    Print ("  Volume Top File Guid");
    SetScreenColor (WHITE_TEXT);
  }
  Print ("\n");
  SetScreenColor (WHITE_TEXT);

  Print ("Integrity Check                 = 0x%04x\n", FfsFileHeader->IntegrityCheck);
  Print ("Type                            = 0x%02x\n", FfsFileHeader->Type);
  Print ("Attribute                       = 0x%02x\n", FfsFileHeader->Attributes);
  Print ("FFS Size (including FFS Header) = 0x%x (%d)\n", FFS_FILE_SIZE(FfsFileHeader), FFS_FILE_SIZE(FfsFileHeader));
  Print ("State                           = 0x%02x\n", FfsFileHeader->State);
  Print ("\n");

  return FFS_FILE_SIZE(FfsFileHeader);
}

void ParseFvHeader(EFI_FIRMWARE_VOLUME_HEADER *FvHeader)
{
	int i;
  
	Print ("Start to Parse FV Header\n");

	ShowFvRawData ((u_int8_t*)FvHeader, FvHeader->HeaderLength);

  Print("Zero Vector:");
	for(i = 0 ; i < 16 ; i++)
		Print ("%02x ",FvHeader->ZeroVector[i]);
	Print("\n");
  Print ("Guid: ");
  SetScreenColor (ORANGE_TEXT);
	PrintGuid (&FvHeader->FileSystemGuid);
  if (CompareGuid (&FvHeader->FileSystemGuid, &FirmwareFileSystem2Guid)) {
    SetScreenColor (GREEN_TEXT);
    Print ("  (FirmwareFileSystem2Guid)");
  } else if (CompareGuid (&FvHeader->FileSystemGuid, &FirmwareFileSystem3Guid)) {
    SetScreenColor (BLUE_TEXT);
    Print ("  (FirmwareFileSystem3Guid)");
  }
  Print ("\n");
  SetScreenColor (WHITE_TEXT);

	Print ("FV Length            = 0x%llx (%lld)\n", FvHeader->FvLength, FvHeader->FvLength);	

	u_int8_t	*data = (u_int8_t*)&FvHeader->Signature;
	Print ("Signature            = %c%c%c%c\n", *data, *(data+1), *(data+2), *(data+3));
	
	Print ("FV Attribute         = 0x%08x\n", FvHeader->Attributes);
	Print ("Header Length        = 0x%x (%d)\n", FvHeader->HeaderLength, FvHeader->HeaderLength);
	Print ("Check Sum            = 0x%04x\n", FvHeader->Checksum);
	Print ("Extend Header Offset = 0x%04x\n",FvHeader->ExtHeaderOffset);
	Print ("Revision             = 0x%02x\n", FvHeader->Revision);
	Print ("Number of Block      = 0x%08x\n", FvHeader->BlockMap[0].NumBlocks);
	Print ("Block Length         = 0x%08x\n", FvHeader->BlockMap[0].Length);
	Print ("\n\n");

	Print ("%p\n", FvHeader);
	Print ("%p\n", (u_int8_t*)FvHeader + FvHeader->HeaderLength);


  //
  //  Start to parse FFS for this FV.
  //
	long FvSize;
	long FfsSize = 0;
	u_int8_t *DummyPointer;
	
	DummyPointer = (u_int8_t*)FvHeader + FvHeader->HeaderLength;
  FvSize = FvHeader->FvLength - FvHeader->HeaderLength;

	while ((FvSize) > 0) {
		//
		//	Debug
		//
		Print ("Address: 0x%x\n", DummyPointer - (u_int8_t*)FvHeader);

		FfsSize = ParseFfsHeader (DummyPointer);
    if (FfsSize == 0xffffff || FfsSize == 0) {
      Print ("The Final FFS.\n");
      break;
    }
		if (FfsSize % 8 != 0) {
			FfsSize += (8 - FfsSize % 8);
		}
    FvSize -= FfsSize;
		DummyPointer += FfsSize;
    Print ("remaining size = 0x%x\n",FvSize);
	}
}



void SearchSignature(u_int32_t Signature, u_int8_t *Buffer, u_int8_t Base, u_int64_t Range)
{
	int i;

//	Print ("Debug\n");

//	Print ("Base = %x\n", Base);
//	Print ("Range = %x\n", Range);

	for (i = Base ; i < Range ; i += sizeof(u_int32_t) / sizeof(u_int8_t)) {
		if (*(u_int32_t*)&Buffer[i] == Signature) {
			Print("Found \"_FVH\" at %x\n",i);
			Print ("%p\n",&Buffer[i]);
			EFI_FIRMWARE_VOLUME_HEADER *FvHeader = CR(&Buffer[i], EFI_FIRMWARE_VOLUME_HEADER, Signature);
			ParseFvHeader (FvHeader);
			
			Base = (u_int64_t) ((u_int8_t*) FvHeader - Buffer[0]);
			Range = FvHeader->FvLength;
//	Print ("Base = %x\n", Base);
//	Print ("Range = %x\n", Range);		
		}
	}
}

u_int64_t* SearchFvHeaderAddress (u_int8_t *Buffer, long BufferSize, int *NumberOfFvHeader)
{
	int i;
	int Count = 0;

	EFI_FIRMWARE_VOLUME_HEADER *FvHeader;
	
	for(i = 0 ; i < BufferSize ; i += sizeof(u_int32_t) / sizeof(u_int8_t)) {
		FvHeader = CR(&Buffer[i], EFI_FIRMWARE_VOLUME_HEADER, Signature);
		if (FvHeader->Signature == SIGNATURE_32('_','F','V','H') && 
			    (CompareGuid (&FvHeader->FileSystemGuid, &FirmwareFileSystem2Guid) ||
			    CompareGuid (&FvHeader->FileSystemGuid, &FirmwareFileSystem3Guid))) {
			Count++;
		}
	}
	Print ("Fv Header Count:%d\n",Count);
	
	u_int64_t *FvHeaderAddress;
	FvHeaderAddress = malloc (Count * sizeof (u_int64_t));

	Count = 0;
	for(i = 0 ; i < BufferSize ; i += sizeof(u_int32_t) / sizeof(u_int8_t)) {
		FvHeader = CR(&Buffer[i], EFI_FIRMWARE_VOLUME_HEADER, Signature);
		if (FvHeader->Signature == SIGNATURE_32('_','F','V','H') && 
      (CompareGuid (&FvHeader->FileSystemGuid, &FirmwareFileSystem2Guid) ||
      CompareGuid (&FvHeader->FileSystemGuid, &FirmwareFileSystem3Guid))) {
			FvHeaderAddress[Count] = (u_int64_t) FvHeader;
			Print ("Address %d: %p\n", Count, FvHeaderAddress[Count]);
			Count++;
		}
	}
	*NumberOfFvHeader = Count;
	return FvHeaderAddress;
}

int main (int argc, char **argv)
{
	FILE	*fp = NULL;
	long FileSize;

  if (argc != 2) {
    Print ("Parameter Error!\n");
    return ERROR_EXIT;
  }
  
	fp = fopen (argv[1],"rb");
	if (fp == NULL) {
		Print ("Open File %s Error!!!\n",argv[1]);
		return ERROR_EXIT;
	}
/*	
	fseek (fp, 0, SEEK_END);
	FileSize = ftell (fp);
	Print ("File size = %d\n", FileSize);
	rewind(fp);
*/
	FileSize = InternalGetFileSize (argv[1]);
	Print ("File size = %d\n", FileSize);

	u_int8_t *Buffer;
	Buffer = malloc (FileSize * sizeof(u_int8_t));
	long ReadDataSize;
	if (FileSize != fread (Buffer, sizeof(u_int8_t), FileSize, fp)) {
		Print("Read File Error!!!\n");
	}

	fclose(fp);

	Print ("size: %d\n", sizeof(u_int8_t));


/*
	for (int i = 0 ; i < 32 ; i++) {
		if (i % 16 == 0) {
			Print ("\n");
		}
		Print ("%02x ", Buffer[i]);
	}
	Print ("\n");

	PrintGuid ((GUID*)&Buffer[16]);
*/
/*
	GUID FirmwareFileSystem2Guid = {0x8c8ce578, 0x8a3d, 0x4f1c, 0x99, 0x35, 0x89, 0x61, 0x85, 0xc3, 0x2d, 0xd3};
	for (int i = 0 ; i < FileSize ; i+=4) {
//		u_int32_t *data = (u_int32_t*)&Buffer[i];
//		if (*data == SIGNATURE_32('_','F','V','H'))
		if (*(u_int32_t*)&Buffer[i] == SIGNATURE_32('_','F','V','H')) {
			Print("Found \"_FVH\" at %x\n",i);
			Print ("%p\n",&Buffer[i]);
			EFI_FIRMWARE_VOLUME_HEADER *FvHeader = CR(&Buffer[i], EFI_FIRMWARE_VOLUME_HEADER, Signature);
			ParseFvHeader (FvHeader);
			Print ("Is identity giud? %d\n",CompareGuid (&FvHeader->FileSystemGuid, &FirmwareFileSystem2Guid));
//			PrintGuid (&FvHeader->FileSystemGuid);
		}
	}
*/
	Print ("Debug\n");
//	SearchSignature (SIGNATURE_32('_','F','V','H'), Buffer, 0, (u_int64_t) FileSize);

	u_int64_t *FvHeaderAddress;
	int NumberOfFvHeader;
	FvHeaderAddress = SearchFvHeaderAddress (Buffer, FileSize, &NumberOfFvHeader);
  int i;

	for (i = 0 ; i < NumberOfFvHeader ; i++) {
		ParseFvHeader ((EFI_FIRMWARE_VOLUME_HEADER*)FvHeaderAddress[i]);
	}

//	ShowFvRawData ((u_int8_t*)FvHeaderAddress[0], 0x70000);
//	ShowFvRawData (Buffer, FileSize);
//	ParseFvHeader ((EFI_FIRMWARE_VOLUME_HEADER*)FvHeaderAddress[0]);
/*
  Print ("0x%x\n", FOREGROUND_BLUE);	
  Print ("0x%x\n", FOREGROUND_GREEN);	
  Print ("0x%x\n", FOREGROUND_RED);	
  Print ("0x%x\n", FOREGROUND_INTENSITY);	
  
  SetScreenColor (FOREGROUND_BLUE);
  SetScreenColor (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
  SetScreenColor (FOREGROUND_BLUE | FOREGROUND_GREEN);
  SetScreenColor (FOREGROUND_RED | FOREGROUND_GREEN);
  SetScreenColor (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
  SetScreenColor (FOREGROUND_BLUE | FOREGROUND_RED);
  SetScreenColor (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
  SetScreenColor (7);

  for (i = 0 ; i < 16 ; i++) {
    SetScreenColor (i);
  }
*/
  Print ("Done to Parse FV file %s\n", argv[1]);

	free(Buffer);
	free(FvHeaderAddress);
}


/*
int main(void)
{
	Print ("%s","\033[5m");
	Print ("test\n");
	
}

*/
