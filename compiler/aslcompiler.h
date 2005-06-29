#ifndef AslCompiler_C_INTERFACE
#define AslCompiler_C_INTERFACE

#include <stdio.h>
#include "acpi.h"

#define ASL_PARSE_OPCODE_BASE       ACCESSAS        /* First Lex type */   


/* TBD: define new (2.0) opcodes  - move to amlcode.h */

#define AML_BREAKPOINT_OP           (UINT16) 0x00FE
#define AML_BUFF_OP                 (UINT16) 0x00FE
#define AML_CASE_OP                 (UINT16) 0x00FE
#define AML_CONCAT_TPL_OP           (UINT16) 0x00FE
#define AML_CONTINUE_OP             (UINT16) 0x00FE
#define AML_COPY_OP                 (UINT16) 0x00FE
#define AML_QWORD_FIELD_OP          (UINT16) 0x00FE
#define AML_DATA_REGION_OP          (UINT16) 0x00FE
#define AML_DEC_STR_OP              (UINT16) 0x00FE
#define AML_DEFAULT_OP              (UINT16) 0x00FE
#define AML_ELSEIF_OP               (UINT16) 0x00FE
#define AML_HEXSTR_OP               (UINT16) 0x00FE
#define AML_INT_OP                  (UINT16) 0x00FE
#define AML_LOAD_TABLE_OP           (UINT16) 0x00FE
#define AML_MID_OP                  (UINT16) 0x00FE
#define AML_MOD_OP                  (UINT16) 0x00FE
#define AML_SWITCH_OP               (UINT16) 0x00FE
#define AML_COPY_OP                 (UINT16) 0x00FE
#define AML_COPY_OP                 (UINT16) 0x00FE

#define AML_RAW_DATA_BYTE           (UINT16) 0xAA01
#define AML_RAW_DATA_WORD           (UINT16) 0xAA02
#define AML_RAW_DATA_DWORD          (UINT16) 0xAA04
#define AML_RAW_DATA_QWORD          (UINT16) 0xAA08
#define AML_PACKAGE_LENGTH          (UINT16) 0xAA10

#define AML_UNASSIGNED_OPCODE       (UINT16) 0xEEEE
#define AML_DEFAULT_ARG_OP          (UINT16) 0xDDDD

#define NODE_VISITED                0x01
#define NODE_AML_PACKAGE            0x02

#define ASL_WALK_VISIT_DOWNWARD     0x01
#define ASL_WALK_VISIT_UPWARD       0x02
#define ASL_WALK_VISIT_TWICE        0x03

typedef enum
{
    REGION_MEMORY,
    REGION_IO,
    REGION_PCI_CONFIG,
    REGION_EC,
    REGION_SMBUS,
    REGION_CMOS,
    REGION_PCI_BAR

} AML_REGION_TYPES;


#define OP_TABLE_ENTRY(a,b,c)        {b,a,c}

typedef struct asl_mapping_entry
{
    UINT32                  Value;
    UINT16                  AmlOpcode;
    UINT8                   Flags;

} ASL_MAPPING_ENTRY;

typedef union asl_node_value
{
    UINT32                  Integer;
    UINT32                  Integer32;
    UINT16                  Integer16;
    UINT8                   Integer8;
    void                    *Pointer;
    char                    *String;

} ASL_NODE_VALUE;

typedef struct asl_parse_node
{
    struct asl_parse_node   *Parent;
    struct asl_parse_node   *Peer;
    struct asl_parse_node   *Child;
    union asl_node_value    Value;
    void                    *Valuex;
    UINT16                  AmlOpcode;
    UINT16                  ParseOpcode;
    UINT32                  AmlLength;
    UINT32                  AmlSubtreeLength;
    UINT8                   AmlOpcodeLength;
    UINT8                   AmlPkgLenBytes;
    UINT8                   Flags;

} ASL_PARSE_NODE;


typedef struct asl_walk_info
{
    ASL_PARSE_NODE          **NodePtr;
    UINT32                  *LevelPtr;

} ASL_WALK_INFO;


typedef
void (*ASL_WALK_CALLBACK) (
    ASL_PARSE_NODE          *Node,
    UINT32                  Level,
    void                    *Context);



/*
 * Global variables.  Declared in aslmain.c only
 */

#ifdef _DECLARE_GLOBALS
#define EXTERN
#define INIT_GLOBAL(a,b)        a=b
#else
#define EXTERN                  extern
#define INIT_GLOBAL(a,b)        a
#endif

extern int                      optind;
extern char                     *optarg;
extern FILE                     *AslCompilerin;
extern int                      AslCompilerparse(void);
extern int                      AslCompilerdebug;
extern ASL_MAPPING_ENTRY        AslKeywordMapping[];
extern int                      yydebug;



EXTERN int                      INIT_GLOBAL (Gbl_CurrentColumn, 0);
EXTERN int                      INIT_GLOBAL (Gbl_CurrentLineNumber, 1);
EXTERN char                     Gbl_CurrentLineBuffer[256];
EXTERN char                     INIT_GLOBAL (*Gbl_LineBufPtr, Gbl_CurrentLineBuffer);

/* Option flags */

EXTERN BOOLEAN                  INIT_GLOBAL (Gbl_DebugFlag, FALSE);
EXTERN BOOLEAN                  INIT_GLOBAL (Gbl_ListingFlag, FALSE);

/* Files */

EXTERN char                     INIT_GLOBAL (*Gbl_InputFilename, NULL);
EXTERN char                     INIT_GLOBAL (*Gbl_OutputFilename, NULL);
EXTERN char                     INIT_GLOBAL (*Gbl_ListingFilename, NULL);
EXTERN char                     INIT_GLOBAL (*Gbl_DebugFilename, NULL);
EXTERN FILE                     *Gbl_AslInputFile;
EXTERN FILE                     *Gbl_OutputAmlFile;
EXTERN FILE                     *Gbl_DebugFile;
EXTERN FILE                     *Gbl_ListingFile;

/* Statistics */

EXTERN UINT32                   INIT_GLOBAL (InputChars, 0);
EXTERN UINT32                   INIT_GLOBAL (TotalKeywords, 0);
EXTERN UINT32                   INIT_GLOBAL (TotalNamedObjects, 0);
EXTERN UINT32                   INIT_GLOBAL (TotalExecutableOpcodes, 0);
EXTERN UINT32                   INIT_GLOBAL (ErrorCount, 0);
EXTERN UINT32                   INIT_GLOBAL (WarningCount, 0);

/* Misc */


EXTERN ASL_PARSE_NODE           INIT_GLOBAL (*RootNode, NULL);
EXTERN ACPI_TABLE_HEADER        TableHeader;
EXTERN UINT32                   INIT_GLOBAL (TableLength, 0);



#define ASL_ERROR_PREFIX        1000
#define ASL_WARNING_PREFIX      2000

typedef enum 
{
    ASL_WARNING_BUFFER_LENGTH = 0,
    ASL_WARNING_PACKAGE_LENGTH,

} ASL_WARNING_IDS;

typedef enum 
{
    ASL_ERROR_INPUT_FILE_OPEN = 0,
    ASL_ERROR_OUTPUT_FILENAME,
    ASL_ERROR_OUTPUT_FILE_OPEN,
    ASL_ERROR_LISTING_FILENAME,
    ASL_ERROR_LISTING_FILE_OPEN,
    ASL_ERROR_DEBUG_FILENAME,
    ASL_ERROR_DEBUG_FILE_OPEN,
    ASL_ERROR_ENCODING_LENGTH,

} ASL_ERROR_IDS;


int
AslCompilererror(char *s);

int
DbgPrint (
    char                    *Format,
    ...);

int
getopt (
    int                     argc,
    char                    **argv,
    char                    *opts);

void
ErrorContext (void);


void
AslWarning (
    UINT32                  WarningId);

void
AslError (
    UINT32                  ErrorId);


void
CgSetOptimalIntegerSize (
    ASL_PARSE_NODE          *Node);

void
CgGenerateAmlLengths (
    ASL_PARSE_NODE          *Node);

void
CgGenerateAmlOpcode (
    ASL_PARSE_NODE          *Node);


ACPI_STATUS
CgOpenOutputFile (
    char                    *InputFilename);


void
CgAmlOpcodeWalk (
    ASL_PARSE_NODE          *Node,
    UINT32                  Level,
    void                    *Context);

void
CgAmlPackageLengthWalk (
    ASL_PARSE_NODE          *Node,
    UINT32                  Level,
    void                    *Context);

void
CgAmlWriteWalk (
    ASL_PARSE_NODE          *Node,
    UINT32                  Level,
    void                    *Context);


void
CgGenerateAmlOperands (
    ASL_PARSE_NODE          *Node);


void
CgGenerateOutput(
    void);

void
CgCloseTable (void);




void
TgWalkParseTree (
    UINT32                  Visitation,
    ASL_WALK_CALLBACK       Callback,
    void                    *Context);


char *
TgAddNode (
    void                    *Thing);

char *
_TgUpdateNode (
    UINT32                  ParseOpcode,
    ASL_PARSE_NODE          *Node);

char *
TgCreateNode (
    UINT32                  ParseOpcode,
    UINT32                  NumChildren,
    ...);

char *
TgCreateLeafNode (  
    UINT32                  ParseOpcode,
    void                    *Value);

void
TgWalkTree (void);


#define TgLinkPeerNode(a,b)      (char *)_TgLinkPeerNode ((ASL_PARSE_NODE *)(a),(ASL_PARSE_NODE *)(b))
#define TgLinkChildNode(a,b)     (char *)_TgLinkChildNode ((ASL_PARSE_NODE *)(a),(ASL_PARSE_NODE *)(b))
#define TgUpdateNode(a,b)        (char *)_TgUpdateNode (a,(ASL_PARSE_NODE *)(b))

ASL_PARSE_NODE *
_TgLinkPeerNode (
    ASL_PARSE_NODE          *Node1,
    ASL_PARSE_NODE          *Node2);

ASL_PARSE_NODE *
_TgLinkChildNode (
    ASL_PARSE_NODE          *Node1,
    ASL_PARSE_NODE          *Node2);


#endif

