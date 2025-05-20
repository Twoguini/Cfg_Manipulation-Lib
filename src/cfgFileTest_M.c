#include "cfgFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "returnCodes.h"
#include "logMessages.h"

// - Defining Export according on the patflorm used
#ifdef _WIN32
  // - If windows, export as dll
  #define EXPORT __declspec(dllexport)
  // - If windows, export
  #include <io.h>
  // - If windows, Use _access
  #define access _access
#else 
  // - If Linux, export as .so
  #define EXPORT __attribute__((visibility("default")))
  // - If Linux, export 
  #include <unistd.h>
#endif

// "Every testing routine has to create it's own needed resorces"
//
// - Index - Tested Funcion         - Testing Routine
// -  000  - Running all functions  - 
// -  001  - trim                   - Success           |
// -  002  - isPath                 - Null Path         | Empty Path             | Path is a Dir W/ Granted Access | Path is a Dir W/ out Granted Access | Path is a File W/ Granted Access | Path is a File W/ out Granted Access
// -  003  - cfgFileCreate          - Null Name         | No Path / Invalid Path | File Already Exists             | Failed to Create File               | File Succesfully Created         |
// -  004  - cfgFileSectionExist    - Null Arguments    | File Does not Exist    | Section Exists                  | Section Not Found                   | 
// -  005  - cfgFileGetStrSection   - Null Section Name | No Start Bracket       | No End Bracket                  | Spaces Beetwen Brackets             | Trimmed Section                  |
// -  006  - cfgFileKeyExist        - Null Arguments    | File Does not Exist    | Section not Found               | Key Not Found                       | Key succesfully found            |
// -  007  - cfgFileReadKeyValue    - Null Arguments    | File Does not Exist    | Section not Found               | Key Not Found                       | RC_SECTION_KEY_FOUND_BUT_EMPTY   | Key read
// -  008  - cfgFileUpdtKeyValue    - Null Arguments    | File Does not Exist    | Key created                     | Key not Updated                     | Key Updated                      | Section Created
// -  009  - cfgFileAddSection      - Null Arguments    | File Does not Exist    | Section Alreay Exists           | Section Successfully created        |                                     
// -  010  - cfgFileDelSection      - Null Arguments    | File Does not Exist    | Section Not Found               | Section Successfully Deleted        | 
// -  011  - cfgFileDelKey          - Null Arguments    | File Does not Exist    | Section Not Found               | Key not Found                       | Key Successfully Deleted         |

int test_Trim() {
  printf("=============================== Trim Tests ==============================\n\n");

  // - Testing with Raw str parameter
  printf("================== Str Direct Param =================\n");
  char *pOutData = trim("  Teste    ");
  if (strcmp(pOutData, "Teste") != 0) { 
    printf("FAIL: Entered Data: '   Teste     ' - Expected: 'Teste' - Received: '%s'\n\n", pOutData);
    return 1;
  } else {
    printf("Passed: Entered Data: '   Teste     ' - Expected: 'Teste' - Received: '%s'\n\n", pOutData);
  }

  // - Testing with the text inside a variable - CORRECT WAY OF USING - 
  printf("====================== Var Param ====================\n");
  char strInData[14];
  strcpy(strInData,"  Teste    ");
  pOutData = trim(strInData);
  if (strcmp(pOutData, "Teste") != 0) { 
    printf("FAIL: Entered Data: '%s' - Expected: 'Teste' - Received: '%s'\n\n",strInData , pOutData);
    return 1;
  } else {
    printf("Passed: Entered Data: '%s' - Expected: 'Teste' - Received: '%s'\n\n", strInData, pOutData);
  }

  printf("=============================== Trim Tests ==============================\n\n");
  return 0; 
}

/*int test_IsPath() {}
int test_FileCreate() {}
int test_FileSectionExist() {}
int test_FileGetStrSection() {}
int test_FileKeyExist() {}
int test_FileReadKeyValue() {}
int test_FileUpdtKeyValue() {}
int test_FileAddSection() {} 
int test_FileDelSection() {}
int test_FileDelKey() {}*/

int main() {

  int testingIndex;
  printf("||================== Initializing Testing Operations ==================||\n"
       "\n||---------------------------------------------------------------------||\n"
         "||              Index               |          Tested Funcion          ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               000                |      Running all functions       ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               001                |               trim               ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               002                |              Is Path             ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               003                |           Create File            ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               004                |          Section Exists          ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               005                |        Format Section Name       ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               006                |            Key Exists            ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               007                |             Key Value            ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               008                |          Updt Key Value          ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               009                |           Add Section            ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               010                |          Delete Section          ||\n"
         "||---------------------------------------------------------------------||\n"
         "||               011                |            Delete Key            ||\n"
         "||---------------------------------------------------------------------||\n\n");
         
  if(scanf("%d", &testingIndex) != 1) {
    printf("Invalid Input Received");
    return RC_NOT_A_NUMBER; 
  }

  printf("Chosen Index Was: %03d\n", testingIndex);

  printf("========================= Dinamic Alocation Test ========================\n");
  //TODO - Dynamic allocation test
  printf("========================= Dinamic Alocation Test ========================\n");

  switch (testingIndex) {
  // - Run All Tests
  case 0:
    test_Trim();
    /*test_IsPath();
    test_FileCreate();
    test_FileSectionExist();
    test_FileGetStrSection();
    test_FileKeyExist();
    test_FileReadKeyValue();
    test_FileUpdtKeyValue();
    test_FileAddSection(); 
    test_FileDelSection();
    test_FileDelKey();*/
    break;

  // - Trim Function
  case 1:
    test_Trim();
    break;

  // - Is Path Function
/*case 2:
    test_IsPath();
    break;

  // - Create File Function
  case 3:
    test_FileCreate();
    break;

  // - Section Exists Function
  case 4:
    test_FileSectionExist();
    break;

  // - Get Section Function
  case 5:
    test_FileGetStrSection();
    break;

  // - Key Exists Function
  case 6:
    test_FileKeyExist();
    break;

  // - Key Value Function
  case 7:
    test_FileReadKeyValue();
    break;

  // - Updt Key Function
  case 8:
    test_FileUpdtKeyValue();
    break;

  // - Add Section Function
  case 9:
    test_FileAddSection();
    break;

  // - Delete Section Function
  case 10:
    test_FileDelSection();
    break;

  // - Delete Key Function
  case 11:
    test_FileDelKey();
    break;*/      

  // - Then this test doesn't exist
  default:
    printf("Test of Index: %03d - Doesn't Exist", testingIndex); 
    break;
  }
  return 0;
}