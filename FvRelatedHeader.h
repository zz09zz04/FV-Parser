#ifndef __FV_RELATED_HEADER__
#define __FV_RELATED_HEADER__


#if defined(_WIN32) || defined(__WIN32__)
#include <stdint.h>
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;

#else

typedef struct {
  u_int32_t  Data1;
  u_int16_t  Data2;
  u_int16_t  Data3;
  u_int8_t   Data4[8];
} GUID;

#endif  ///#if defined(_WIN32) || defined(__WIN32__)

#define EFI_FIRMWARE_FILE_SYSTEM2_GUID	{ 0x8c8ce578, 0x8a3d, 0x4f1c, { 0x99, 0x35, 0x89, 0x61, 0x85, 0xc3, 0x2d, 0xd3 } }
#define EFI_FIRMWARE_FILE_SYSTEM3_GUID  { 0x5473c07a, 0x3dcb, 0x4dca, { 0xbd, 0x6f, 0x1e, 0x96, 0x89, 0xe7, 0x34, 0x9a } }
#define EFI_FFS_VOLUME_TOP_FILE_GUID    { 0x1BA0062E, 0xC779, 0x4582, { 0x85, 0x66, 0x33, 0x6A, 0xE8, 0xF7, 0x8F, 0x09 } }
#define FSP_FFS_INFORMATION_FILE_GUID   { 0x912740be, 0x2284, 0x4734, { 0xb9, 0x71, 0x84, 0xb0, 0x27, 0x35, 0x3f, 0x0c } }


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
  u_int8_t Header;
  u_int8_t File;
  }            Checksum;
  u_int16_t    Checksum16;
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


typedef struct {
  ///
  /// Byte 0x00: Signature ('FSPH') for the FSP Information Header.
  ///
  u_int32_t  Signature;
  ///
  /// Byte 0x04: Length of the FSP Information Header.
  ///
  u_int32_t  HeaderLength;
  ///
  /// Byte 0x08: Reserved.
  ///
  u_int8_t   Reserved1[2];
  ///
  /// Byte 0x0A: Indicates compliance with a revision of this specification in the BCD format.
  ///            For revision v2.3 the value will be 0x23.
  ///
  u_int8_t   SpecVersion;
  ///
  /// Byte 0x0B: Revision of the FSP Information Header.
  ///            The Current value for this field is 0x6.
  ///
  u_int8_t   HeaderRevision;
  ///
  /// Byte 0x0C: Revision of the FSP binary.
  ///            Major.Minor.Revision.Build
  ///            If FSP HeaderRevision is <= 5, the ImageRevision can be decoded as follows:
  ///               7 : 0  - Build Number
  ///              15 : 8  - Revision
  ///              23 : 16 - Minor Version
  ///              31 : 24 - Major Version
  ///            If FSP HeaderRevision is >= 6, ImageRevision specifies the low-order bytes of the build number and revision
  ///            while ExtendedImageRevision specifies the high-order bytes of the build number and revision.
  ///               7 : 0  - Low Byte of Build Number
  ///              15 : 8  - Low Byte of Revision
  ///              23 : 16 - Minor Version
  ///              31 : 24 - Major Version
  ///
  u_int32_t  ImageRevision;
  ///
  /// Byte 0x10: Signature string that will help match the FSP Binary to a supported HW configuration.
  ///
  u_int8_t   ImageId[8];
  ///
  /// Byte 0x18: Size of the entire FSP binary.
  ///
  u_int32_t  ImageSize;
  ///
  /// Byte 0x1C: FSP binary preferred base address.
  ///
  u_int32_t  ImageBase;
  ///
  /// Byte 0x20: Attribute for the FSP binary.
  ///
  u_int16_t  ImageAttribute;
  ///
  /// Byte 0x22: Attributes of the FSP Component.
  ///
  u_int16_t  ComponentAttribute;
  ///
  /// Byte 0x24: Offset of the FSP configuration region.
  ///
  u_int32_t  CfgRegionOffset;
  ///
  /// Byte 0x28: Size of the FSP configuration region.
  ///
  u_int32_t  CfgRegionSize;
  ///
  /// Byte 0x2C: Reserved2.
  ///
  u_int32_t  Reserved2;
  ///
  /// Byte 0x30: The offset for the API to setup a temporary stack till the memory is initialized.
  ///
  u_int32_t  TempRamInitEntryOffset;
  ///
  /// Byte 0x34: Reserved3.
  ///
  u_int32_t  Reserved3;
  ///
  /// Byte 0x38: The offset for the API to inform the FSP about the different stages in the boot process.
  ///
  u_int32_t  NotifyPhaseEntryOffset;
  ///
  /// Byte 0x3C: The offset for the API to initialize the memory.
  ///
  u_int32_t  FspMemoryInitEntryOffset;
  ///
  /// Byte 0x40: The offset for the API to tear down temporary RAM.
  ///
  u_int32_t  TempRamExitEntryOffset;
  ///
  /// Byte 0x44: The offset for the API to initialize the CPU and chipset.
  ///
  u_int32_t  FspSiliconInitEntryOffset;
  ///
  /// Byte 0x48: Offset for the API for the optional Multi-Phase processor and chipset initialization.
  ///            This value is only valid if FSP HeaderRevision is >= 5.
  ///            If the value is set to 0x00000000, then this API is not available in this component.
  ///
  u_int32_t  FspMultiPhaseSiInitEntryOffset;
  ///
  /// Byte 0x4C: Extended revision of the FSP binary.
  ///            This value is only valid if FSP HeaderRevision is >= 6.
  ///            ExtendedImageRevision specifies the high-order byte of the revision and build number in the FSP binary revision.
  ///               7 : 0 - High Byte of Build Number
  ///              15 : 8 - High Byte of Revision
  ///            The FSP binary build number can be decoded as follows:
  ///            Build Number = (ExtendedImageRevision[7:0] << 8) | ImageRevision[7:0]
  ///            Revision = (ExtendedImageRevision[15:8] << 8) | ImageRevision[15:8]
  ///            Minor Version = ImageRevision[23:16]
  ///            Major Version = ImageRevision[31:24]
  ///
  u_int16_t  ExtendedImageRevision;
  ///
  /// Byte 0x4E: Reserved4.
  ///
  u_int16_t  Reserved4;
} FSP_INFO_HEADER;

  
#pragma pack()

#endif  ///#ifndef __FV_RELATED_HEADER__

