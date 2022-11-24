#include <stdio.h>
#include <stdlib.h>

#include "SetScreenColor.c"

#include "FvRelatedHeader.h"


//#define	FILE_NAME	"FVRECOVERY.Fv"
#define	FILE_NAME	"FVOSBOOT.Fv"

#define	Print	printf
#define	ERROR_EXIT	-1


GUID FirmwareFileSystem2Guid = EFI_FIRMWARE_FILE_SYSTEM2_GUID;
GUID FirmwareFileSystem3Guid = EFI_FIRMWARE_FILE_SYSTEM3_GUID;


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
    SetScreenColor (ORANGE_TEXT);
    Print ("%08x:",i * 16);
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
    SetScreenColor (ORANGE_TEXT);
  	Print ("%08x:", (int)size / 16 * 16);
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

void ShowFvRawData(u_int8_t *Buffer, u_int64_t size)
{
  int i, j;
  for (i = 0 ; i < size / 16 ; i++) {
    SetScreenColor (ORANGE_TEXT);
    Print ("%08x:",i * 16);
    SetScreenColor (WHITE_TEXT);
    for (j = 0 ; j < 16 ; j++) {
      if (i * 16 + j >= 16) {
        SetScreenColor (BLUE_TEXT);
      }
      if (i * 16 + j >= 32) {
        SetScreenColor (GREEN_TEXT);
      }
      if (i * 16 + j >= 40) {
        SetScreenColor (RED_TEXT);
      }
      if (i * 16 + j >= 44) {
        SetScreenColor (CYAN_TEXT);
      }
      if (i * 16 + j >= 48) {
        SetScreenColor (PURPLE_TEXT);
      }
      if (i * 16 + j >= 50) {
        SetScreenColor (LIGHTBLUE_TEXT);
      }
      if (i * 16 + j >= 52) {
        SetScreenColor (LIGHTGREEN_TEXT);
      }
      if (i * 16 + j >= 54) {
        SetScreenColor (LIGHTRED_TEXT);
      }
      if (i * 16 + j >= 55) {
        SetScreenColor (LIGHTCYAN_TEXT);
      }
      if (i * 16 + j >= 56) {
        SetScreenColor (WHITE_TEXT);
      }
      if (i * 16 + j >= 72) {
        SetScreenColor (LIGHTPURPLE_TEXT);
      }
      Print (" %02x", Buffer[i*16+j]);
    }
    Print ("  ");
    for (j = 0 ; j < 16 ; j++) {
      if (i * 16 + j >= 16) {
        SetScreenColor (BLUE_TEXT);
      }
      if (i * 16 + j >= 32) {
        SetScreenColor (GREEN_TEXT);
      }
      if (i * 16 + j >= 40) {
        SetScreenColor (RED_TEXT);
      }
      if (i * 16 + j >= 44) {
        SetScreenColor (CYAN_TEXT);
      }
      if (i * 16 + j >= 48) {
        SetScreenColor (PURPLE_TEXT);
      }
      if (i * 16 + j >= 50) {
        SetScreenColor (LIGHTBLUE_TEXT);
      }
      if (i * 16 + j >= 52) {
        SetScreenColor (LIGHTGREEN_TEXT);
      }
      if (i * 16 + j >= 54) {
        SetScreenColor (LIGHTRED_TEXT);
      }
      if (i * 16 + j >= 55) {
        SetScreenColor (LIGHTCYAN_TEXT);
      }
      if (i * 16 + j >= 56) {
        SetScreenColor (WHITE_TEXT);
      }
      if (i * 16 + j >= 72) {
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
    SetScreenColor (ORANGE_TEXT);
    Print ("%08x:", (int)size / 16 * 16);
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

const char* ParseFvFileTypeWorker (EFI_FV_FILETYPE Type) {
  const char* FvFileType[0x10] = { 
                        NULL,
                        "Raw",
                        "FreeForm",
                        "SEC Core",
                        "PEI Core",
                        "DXE Core",
                        "PEIM",
                        "Driver",
                        "Combined PEIM Driver",
                        "Application",
                        "MM",
                        "FV Image",
                        "Combined MM DXE",
                        "MM Core",
                        "MM Stand alone",
                        "MM Core Stand alone"
                        };
  if (Type == 0xF0) {
    return "Pad File";
  }
  if (Type >= 0x10) {
    return NULL;
  }
  return FvFileType[Type];
}

long ParseFfsHeader (void* address) 
{
  EFI_FFS_FILE_HEADER *FfsFileHeader = address;

  if (FFS_FILE_SIZE(FfsFileHeader) == 0xffffff) {
    return FFS_FILE_SIZE(FfsFileHeader);
  }

  Print ("File Name(GUID): ");
  SetScreenColor (LIGHTORANGE_TEXT);
  PrintGuid (&FfsFileHeader->Name);
  if (CompareGuid (&FfsFileHeader->Name, &(GUID)EFI_FFS_VOLUME_TOP_FILE_GUID)) {
    SetScreenColor (RED_TEXT);
    Print ("  Volume Top File Guid");
    SetScreenColor (WHITE_TEXT);
  }
  if (CompareGuid (&FfsFileHeader->Name, &(GUID)FSP_FFS_INFORMATION_FILE_GUID)) {
    SetScreenColor (CYAN_TEXT);
    Print ("  Firmware Support Package FFS Guid");
    SetScreenColor (WHITE_TEXT);
    Print ("\n");
  }
  SetScreenColor (WHITE_TEXT);
  Print ("\n");

  Print ("Integrity Check                 = 0x%04x\n", FfsFileHeader->IntegrityCheck.Checksum16);
  Print ("Type                            = 0x%02x (%s)\n", FfsFileHeader->Type, ParseFvFileTypeWorker(FfsFileHeader->Type));
  Print ("Attribute                       = 0x%02x\n", FfsFileHeader->Attributes);
  Print ("FFS Size (including FFS Header) = 0x%x (%d)\n", FFS_FILE_SIZE(FfsFileHeader), FFS_FILE_SIZE(FfsFileHeader));
  Print ("State                           = 0x%02x\n", FfsFileHeader->State);
  Print ("\n");

  if (CompareGuid (&FfsFileHeader->Name, &(GUID)FSP_FFS_INFORMATION_FILE_GUID)) {
    FSP_INFO_HEADER *FspInfoHeader;
    FspInfoHeader = (FSP_INFO_HEADER*)((u_int8_t*)FfsFileHeader + sizeof (EFI_FFS_FILE_HEADER) + 4);
    ShowRawData ((u_int8_t*)FspInfoHeader, FFS_FILE_SIZE(FfsFileHeader) - sizeof (EFI_FFS_FILE_HEADER) - 4);
    Print ("Signature                       = 0x%x\n", FspInfoHeader->Signature);
    Print ("HeaderLength                    = 0x%x\n", FspInfoHeader->HeaderLength);
    Print ("SpecVersion                     = 0x%x\n", FspInfoHeader->SpecVersion);
    Print ("HeaderRevision                  = 0x%x\n", FspInfoHeader->HeaderRevision);
    Print ("ImageRevision                   = 0x%x\n", FspInfoHeader->ImageRevision);
    Print ("ImageId                         = \n");
    ShowRawData ((u_int8_t*)&FspInfoHeader->ImageId, 8);
    Print ("ImageSize                       = 0x%x\n", FspInfoHeader->ImageSize);
    Print ("ImageBase                       = 0x%x\n", FspInfoHeader->ImageBase);
    Print ("ImageAttribute                  = 0x%x\n", FspInfoHeader->ImageAttribute);
    Print ("ComponentAttribute              = 0x%x\n", FspInfoHeader->ComponentAttribute);
    Print ("CfgRegionOffset                 = 0x%x\n", FspInfoHeader->CfgRegionOffset);
    Print ("CfgRegionSize                   = 0x%x\n", FspInfoHeader->CfgRegionSize);
    Print ("TempRamInitEntryOffset          = 0x%x\n", FspInfoHeader->TempRamInitEntryOffset);
    Print ("NotifyPhaseEntryOffset          = 0x%x\n", FspInfoHeader->NotifyPhaseEntryOffset);
    Print ("FspMemoryInitEntryOffset        = 0x%x\n", FspInfoHeader->FspMemoryInitEntryOffset);
    Print ("TempRamExitEntryOffset          = 0x%x\n", FspInfoHeader->TempRamExitEntryOffset);
    Print ("FspSiliconInitEntryOffset       = 0x%x\n", FspInfoHeader->FspSiliconInitEntryOffset);
    Print ("FspMultiPhaseSiInitEntryOffset  = 0x%x\n", FspInfoHeader->FspMultiPhaseSiInitEntryOffset);

    Print ("ExtendedImageRevision           = 0x%x\n", FspInfoHeader->ExtendedImageRevision);
//    Print ("Signature = 0x%x\n", FspInfoHeader->);

  }

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

  for (i = Base ; i < Range ; i += sizeof(u_int32_t) / sizeof(u_int8_t)) {
    if (*(u_int32_t*)&Buffer[i] == Signature) {
      Print("Found \"_FVH\" at %x\n",i);
      Print ("%p\n",&Buffer[i]);
      EFI_FIRMWARE_VOLUME_HEADER *FvHeader = CR(&Buffer[i], EFI_FIRMWARE_VOLUME_HEADER, Signature);
      ParseFvHeader (FvHeader);

      Base = (u_int64_t) ((u_int8_t*) FvHeader - Buffer[0]);
      Range = FvHeader->FvLength;
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
      Print ("[Buffer Address %d: 0x%08x][ROM Address %d: 0x%x]\n", Count, (int)(FvHeader), Count, (int)((u_int64_t)FvHeader - (u_int64_t)Buffer));
      Count++;
    }
  }
  *NumberOfFvHeader = Count;
  return FvHeaderAddress;
}

int main (int argc, char **argv)
{
  FILE  *fp = NULL;
  long  FileSize;

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
//	SearchSignature (SIGNATURE_32('_','F','V','H'), Buffer, 0, (u_int64_t) FileSize);

  u_int64_t *FvHeaderAddress;
  int NumberOfFvHeader;
  FvHeaderAddress = SearchFvHeaderAddress (Buffer, FileSize, &NumberOfFvHeader);
  int i;

  for (i = 0 ; i < NumberOfFvHeader ; i++) {
    Print ("[0x%x]\n",(int)(FvHeaderAddress[i] - FvHeaderAddress[0]));
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
