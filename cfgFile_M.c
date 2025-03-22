#include "cfgfile.h" // TODO Fix
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// - Get File Status
#include <sys/stat.h>

// - Return Codes
#include "returnCodes.h"

//TODO Fix/Create LOG Function

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

// - R_OK is not defined by <unistd.h> or <io.h>, so we need to define it to checks the file access
#ifndef R_OK
  #define R_OK 4 // - Test the read permission
#endif

// - F_OK is not defined by <unistd.h> or <io.h>, so we need to define it to checks the file access
#ifndef F_OK
    #define F_OK 0 // - Test the existence of the file
#endif

// - Dinamic Loading Lyb test - Load this and then call it to test if the dinamic loading is working
EXPORT int exec_me_teste(int iTestIndx) {
  printf("%s - Called From %s - DINAMIC ALOCATION WORKED", __FUNCTION__, __FILE__);
}

// - Remove Leading and Trailing spaces from a string
EXPORT char *trim(const char *str) {

    char *trimmedOutput;

    // - Here we get the str pointer and loop through it until we find any character other than newlines, carriage returns, or spaces
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r')
        // - By incrementing the pointer by one, we move the memory pointer one position ahead, skipping the previous character
        str++;

    // - Gettin str Length
    int len = strlen(str);
    // - Starting off from the last character from the pointer, looping until a valid character is found on the antepenultimate case
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n' || str[len - 1] == '\r'))
        // - Here we realocate the memory pointer to the previous case
        len--;
    // - When the loop is broken, it means the case before the lenght is a valid Character, 
    // só we add the "\0" wich indicates the end of the str and tels us to ignore any character after it

    trimmedOutput = malloc(len+1);
    strncpy(trimmedOutput, str, len);
    trimmedOutput[len] = '\0';
    return trimmedOutput;
}

// - If pPath exists return TRUE, return FALSEo if is empty, null or do not exist
EXPORT int isPath( char *pPath) {

    if (pPath == NULL) { // - Is pPath Null?
        // TODO LOG ("pCfgFilePath[%s]NO - NULL",pPath)
        return RC_INVALID_PATH;
    } else if (strlen(pPath) == 0) { // - Is it Empty then? 
        // TODO LOG(" pCfgFilePath[%s]NO - EMPTY - Use Current Path",pPath)
        return RC_INVALID_PATH;
    }

    // - Create Stat structure for file data
    struct stat path_stat; 

    // - Loads pPath to path_stat struct, therefore, that path exists
    if (stat(pPath, &path_stat) == 0) {

        // Is it an Directory?
        if (S_ISDIR(path_stat.st_mode) ) {
            // - Do i have access to it?
            if (access(pPath, R_OK) == 0) {
                // TODO LOG(" pCfgFilePath[%s]YES access granted",pPath)
                return RC_PATH_IS_DIR;
            } else { // - No
                // TODO LOG(" pCfgFilePath[%s]NO access not granted",pPath)
                return RC_PATH_DIR_DECLINED_ACCESS;
            }
        // - Is it a regular File?
        } else  if (S_ISREG(path_stat.st_mode)) {
            // - Do i Have Access ?
            if (access(pPath, R_OK) == 0) {
                // TODO LOG(" pCfgFilePath[%s] file access granted",pPath)
                return RC_PATH_IS_FILE;
            // - i Don't
            } else {
                // TODO LOG(logDebug(" pCfgFilePath[%s]NO file access not granted",pPath);
                return RC_PATH_FILE_DECLINED_ACCESS;
            }
        // - it's not a path neither a file
        } else {
            // TODO logDebug(" pCfgFilePath[%s]NO",pPath);
        }
    // - It was not able to load the path into stat, so it is invalid 
    } else {
        // TODO logDebug(" pCfgFilePath[%s] Is Invalid",pPath);
    }
    return RC_INVALID_PATH;
}

// - Create a cfgfile into a path or running app path
EXPORT int cfgFileCreate( char *pPath, char *pCfgFileName) {

    // - Null File Name Received
    if (pCfgFileName == NULL) {
        //logError(" pCfgFileName = null");
        return RC_NULL_POINTER;
    }
    // - Get and check if the file path exists
    char strCfgFileFullPath[256];
    if (isPath(pPath) == RC_INVALID_PATH) {
        //cfgFilePath is invalid, empty or null, use only filename
        strcpy(strCfgFileFullPath,pCfgFileName);
    } else {
        // concatenate path with file
        sprintf(strCfgFileFullPath, "%s/%s",pPath,pCfgFileName);
    }
    // Check if a file exist
    int rc = access(strCfgFileFullPath, F_OK);
    if (rc != -1) {
        /// file already exist
        // TODO LOG(logWarning(" strCfgFileFullPath[%s] Already Exist rc[%d]",strCfgFileFullPath,rc);
        return RC_FILE_ALREADY_EXISTS;
    }
     // TODO LOG(logDebug(" strCfgFileFullPath[%s] rc[%d] Not Exist, Will be Created!!!",strCfgFileFullPath,rc);

    /// ===================================================
    /// TODO: Create Default .cfg file on current path
    /// ===================================================
    FILE *pfNovoCfgArq = fopen(strCfgFileFullPath, "w");
    if (pfNovoCfgArq == NULL) {
        // TODO LOG(logError("wat not possible to ccreate a file[%s]",strCfgFileFullPath);
        return RC_FAILED_TO_OPEN_FILE;
    }

    // TODO LOG(logDebug(" Creating default .cfg file");

    fclose(pfNovoCfgArq);
    // TODO LOG(logDebug(" strCfgFileFullPath[%s] Successfull Created!!!",strCfgFileFullPath);
    return RC_FILE_SUCCESSFULLY_CREATED; /// If all ok return it
}

// - Checks if a Section exists
EXPORT int cfgFileSectionExist(char *pPath, char *pCfgFileName,char *pSection) {
    // TODO logLogFileInitialize(WARNING, "libcfgfile", 1,1);
    if(pCfgFileName == NULL) {
        return RC_NULL_FILE_POINTER;
    } else if (pSection == NULL) {
        return RC_NULL_SECTION_KEY;
    }

    char strCfgFileFullPath[256];
    if (isPath(pPath) == RC_FAILURE) {
        //cfgFilePath is invalid, empty or null use only filename
        strcpy(strCfgFileFullPath,pCfgFileName);
    } else {
        // concatenate path with file
        sprintf(strCfgFileFullPath, "%s/%s",pPath,pCfgFileName);
    }

    FILE *fFile = fopen(strCfgFileFullPath, "r");  // Open the file for reading.
    if (fFile == NULL) {
        //logError("pPath[%s] pCfgFileName[%s] Fail to open ",pPath,pCfgFileName);
        return RC_FILE_DOES_NOT_EXIST;
    }

    char strLine[256];
    while (fgets(strLine, sizeof(strLine), fFile) != NULL) {
        if (strLine[0] == '[' && strstr(strLine, pSection) != NULL) {
            // Section exists, close the file and return true
            fclose(fFile);
            return RC_SUCCESS;
        }
    }
    fclose(fFile);
    // Section not found
    // TODO LOG(logDebug("pPath[%s] pCfgFileName[%s] pSection[%s] Not Found!!",pPath,pCfgFileName,pSection);
    return RC_CFG_SECTION_NOT_FOUND;
}

// - Format Section Name
EXPORT int cfgFileGetStrSection(char *pstrCfgSectionName, char *pstrSectionContent) {
    // TODO: logLogFileInitialize(WARNING, "libcfgfile", 1,1);
    if(pstrCfgSectionName == NULL) {
        /// Section is null
        return RC_NULL_SECTION_NAME;
    }
    char *pstrStartBracket = strstr(pstrCfgSectionName, "[");
    if(pstrStartBracket == NULL) {
        /// missing Start Bracket
        return RC_SECTION_NAME_START_BRACKET_MISSING;
    }
    char *pstrEndBracket = strstr(pstrStartBracket, "]");
    if(pstrEndBracket == NULL) {
        /// missing End Bracket
        return RC_SECTION_NAME_END_BRACKET_MISSING;
    }
    /// Line Looks  like a CfgFile section
    // - Gets the lengh of the cotent inside brackets (No brackets included)
    size_t untrimedSectionLength = (pstrEndBracket-pstrStartBracket)-1;

    char *strUntrimmedSection = malloc(untrimedSectionLength+1);

    strncpy(strUntrimmedSection,pstrStartBracket+1,untrimedSectionLength);

    strUntrimmedSection[untrimedSectionLength] = '\0';
    if(strstr(strUntrimmedSection," ") != NULL) {
        free(strUntrimmedSection);
        /// Section Must not have spaces between brackets
        return RC_SESSION_NAME_HAVE_SPACES_BETWEEN_BRACKETS;
    }
    free(strUntrimmedSection);
    pstrSectionContent = trim(strUntrimmedSection);
    return 0;
}

// - Checks if a key exists
EXPORT int cfgFileKeyExist(char *pPath, char *pCfgFileName, char *pSection, char *pKey) {
    //TODO logLogFileInitialize(WARNING, "libcfgfile", 1,1);
    if(pCfgFileName == NULL) {
        //logError("pPath[%s] pCfgFileName[%s]=NULL pSection[%s] pKey[%s]",pPath,pCfgFileName,pSection,pKey);
        return RC_NULL_FILE_POINTER;
    } else if (pSection == NULL) {
       //logError("pPath[%s] pCfgFileName[%s] pSection[%s]=NULL pKey[%s]",pPath,pCfgFileName,pSection,pKey);
        return RC_NULL_SECTION_NAME;
    } else if (pKey == NULL) {
        //logError("pPath[%s] pCfgFileName[%s] pSection[%s] pKey[%s]=NULL",pPath,pCfgFileName,pSection,pKey);
        return RC_NULL_SECTION_KEY;
    }

    char strCfgFileFullPath[256];
    if (isPath(pPath) == RC_FAILURE) {
        //cfgFilePath is invalid, empty or null use only filename
        strcpy(strCfgFileFullPath,pCfgFileName);
    } else {
        // concatenate path with file
        sprintf(strCfgFileFullPath, "%s/%s",pPath,pCfgFileName);
    }

    FILE *fFile = fopen(strCfgFileFullPath, "r");  // Open the file for reading.
    if (fFile == NULL) {
        //logError("pPath[%s] pCfgFileName[%s] Fail to open ",pPath,pCfgFileName);
        return RC_FILE_DOES_NOT_EXIST;
    }
    int iSectionFound = 0;
    char strLine[256];
    while (fgets(strLine, sizeof(strLine), fFile) != NULL) {
        if(strlen(strLine) == 0) {
            continue;
        }
        if(iSectionFound == 0) {
            if (strLine[0] == '[' && strstr(strLine, pSection) != NULL) {
                // Section exists, close the file and return true
                // TODO LOG(logDebug("pPath[%s] pCfgFileName[%s]OK pSection[%s]OK pKey[%s]..",pPath,pCfgFileName,pSection,pKey);
                iSectionFound = 1;
                while (fgets(strLine, sizeof(strLine), fFile) != NULL) {
                    if(strlen(strLine) == 0) {
                        continue;
                    }
                    else if (strLine[0] == '[') {
                        // starting a new section
                        break;
                    } else if (strstr(strLine, pKey) != NULL) {
                        // Session and Key Found
                        fclose(fFile);
                        return RC_KEY_SUCCESSFULLY_FOUND;
                    }
                }
            }
        }
    }
    fclose(fFile);
    if(iSectionFound == 0) {
        // Section not found
        // TODO LOG(logDebug("pPath[%s] pCfgFileName[%s]OK pSection[%s]NOK pKey[%s]..",pPath,pCfgFileName,pSection,pKey);
        return RC_CFG_SECTION_NOT_FOUND;
    }
    // TODO LOG(logDebug("pPath[%s] pCfgFileName[%s]OK pSection[%s]OK pKey[%s]Not Found",pPath,pCfgFileName,pSection,pKey);
    return RC_CFG_KEY_NOT_FOUND;
}

// - Function to parse an CFG file and retrieve the value associated with a key
EXPORT int cfgFileReadKeyValue(char *pPath,  char *pCfgFileName, char *pSection, char *pKey, char *pKeyValue) {
    // TODO logLogFileInitialize(WARNING, "libcfgfile", 1,1);
    int rc = RC_NULL_POINTER;
    if( pCfgFileName == NULL ) {
        //logError(" pFileName = null");
        return RC_NULL_FILE_POINTER;
    } else if ( pSection == NULL ) {
        //logError("pSessionName=null");
        return RC_NULL_SECTION_NAME;
    } else if (pKey == NULL) {
        //logError("pKeyName=null");
        return RC_NULL_SECTION_KEY;
    } else if (pKeyValue == NULL) {
        //logError("pKeyValue=null");
        return RC_NULL_POINTER;
    }
    *pKeyValue = 0;

    char strCfgFileFullPath[256];
    if (isPath(pPath) == RC_FAILURE) {
        //cfgFilePath is invalid, empty or null use only filename
        strcpy(strCfgFileFullPath,pCfgFileName);
    } else {
        // concatenate path with file
        sprintf(strCfgFileFullPath, "%s/%s",pPath,pCfgFileName);
    }

    FILE *pFileHandle = fopen(strCfgFileFullPath, "r");
    if (!pFileHandle) {
        //logError("pPath[%s] pCfgFileName[%s] Fail to open",pPath,pCfgFileName);
        return RC_FILE_DOES_NOT_EXIST;
    }
    // TODO LOG(logDebug("pPath[%s] pCfgFileName[%s] opened OK",pPath,pCfgFileName);

    char strLine[128];
    char strCurSession[128] = "";  // To keep track of the current section
    int iSectionFound = 0;
    int iKeyFound = 0;

    while (fgets(strLine, sizeof(strLine), pFileHandle)) {
        char *pTrimmedLine = trim(strLine);
        if (pTrimmedLine[0] == ';' || pTrimmedLine[0] == '#') {
            // Skip comment lines
            continue;
        } else if (pTrimmedLine[0] == '[') {
            // Check if this line is a section header
            char *pEndBracket = strchr(pTrimmedLine, ']');
            if (pEndBracket) {
                *pEndBracket = '\0';  // Null-terminate at the closing bracket
                strcpy(strCurSession, pTrimmedLine + 1);  // Set the current section
                if (strcmp(strCurSession, pSection) == 0)  {
                    iSectionFound = 1;
                    // TODO LOG(logDebug("pPath[%s] pCfgFileName[%s] strCurSession[%s] Found",pPath,pCfgFileName,strCurSession);
                } else {
                    // TODO LOG(logDebug("pPath[%s] pCfgFileName[%s] strCurSession[%s] Missmatch pSection[%s]",pPath,pCfgFileName,strCurSession,pSection);
                }
            }
        } else {
            // Check if this line has a key-value pair
            char *pEqualSign = strchr(pTrimmedLine, '=');
            if (pEqualSign && strCurSession[0] && strcmp(strCurSession, pSection) == 0) {
                *pEqualSign = '\0';  // Null-terminate at the equal sign
                char *pCurrentKey = trim(pTrimmedLine);
                char *pCurrentValue = trim(pEqualSign + 1);
                // TODO LOG(logDebug("pPath[%s] pCfgFileName[%s] pSection[%s] currentKey[%s] currentValue[%s]",pPath,pCfgFileName,pSection,pCurrentKey,pCurrentValue);

                if (strcmp(pCurrentKey, pKey) == 0) {
                    iKeyFound = 1;
                    strcpy(pKeyValue,pCurrentValue);
                }
            }
        }
    }

    if(iSectionFound == 0){
        //Section not found
        // TODO LOG(logDebug("iSectionFound[%d] Section[%s] Not Found",iSectionFound,pSection);
        rc = RC_CFG_SECTION_NOT_FOUND;
    } else if (iKeyFound == 0) {
        // Session found but key was not found
        // TODO LOG(logDebug("    iKeyFound[%d] Section[%s] pKey[%s] Not Found",iKeyFound,pSection,pKey);
        rc = RC_CFG_KEY_NOT_FOUND;
    } else if (strlen(pKeyValue)== 0) {
        // session and key were found but value is empty
        // TODO LOG(logDebug("pSection[%s] pKey[%s] pValue[%s] Key Found but value is empty",pSection,pKey,pKeyValue);
        rc = RC_SECTION_KEY_FOUND_BUT_EMPTY;
    } else {
        // Section, Key and Value found OK
        // TODO LOG(logDebug("pSection[%s] pKey[%s] pValue[%s]",pSection,pKey,pKeyValue);
        rc = RC_SUCCESS;
    }

    fclose(pFileHandle);
    return rc;
}

// - Function to maanipulate data on a cfg file - cfg Create and Uptdate
EXPORT int cfgFileUpdtKeyValue(char *pPath, char* pCfgFileName, char* pSection, char* pKey, char* pNewValue){
    // TODO logLogFileInitialize(WARNING, "libcfgfile", 1,1);
    int rc;

    // Data Validation, check if they are no null
    if( pCfgFileName == NULL ) {
        //logError(" pFileName = null");
        return RC_NULL_FILE_POINTER;
    } else if ( pSection == NULL ) {
        //logError("pSessionName=null");
        return RC_NULL_SECTION_NAME;
    } else if (pKey == NULL) {
        //logError("pKeyName=null");
        return RC_NULL_SECTION_KEY;
    } else if (pNewValue == NULL) {
        //logError("pKeyValue=null");
        return RC_NULL_POINTER;
    }
    // TODO LOG(logDebug("Path[%s] CfgFileName[%s] Section[%s] Key[%s] Value[%s]",pPath,pCfgFileName,pSection,pKey,pNewValue);

    char strCfgFileFullPath[512];
    if (isPath(pPath) == RC_FAILURE) {
        //cfgFilePath is invalid, empty or null use only filename
        strcpy(strCfgFileFullPath,pCfgFileName);
    } else {
        // concatenate path with file
        sprintf(strCfgFileFullPath, "%s/%s",pPath,pCfgFileName);
    }

    // TODO LOG(logDebug("strCfgFileFullPath[%s]", strCfgFileFullPath);

    // TODO os campos não podem ter espaços no meio - TODO
    // TODO o Campo sessão e chave só pode ter letras ouu números e _ - TODO
    // TODO secure that value has printable data - Space, simbols, letters and numbers - TODO

    // Insert the Section name inside brackets
    char *strFormattedSection = malloc(strlen(pSection)+3);
    if (strFormattedSection == NULL) {
        goto free_Memory;
        return RC_MALLOC_FAILED;
    }
    

    sprintf(strFormattedSection, "[%s]", pSection);
    char *pSectionWithBracket = strFormattedSection;

    // Open the cfg file
    FILE *fCfgTargetFile = fopen(strCfgFileFullPath, "r");
    if (fCfgTargetFile == NULL) {
        //logError("pPath[%s] pCfgFileName[%s] Fail to open",pPath,pCfgFileName);
        goto free_Memory;
        return RC_FILE_DOES_NOT_EXIST;
    }

    // Create a temporary file
    FILE *fTmpFile = fopen("temp.cfg", "w");
    if (fTmpFile == NULL) {
        fclose(fCfgTargetFile);
         // TODO LOG(logError(" fTmpFile[temp.cfg] Fail to open");
        goto free_Memory;
        return RC_FAILED_CREATION_TMP_FILE;
    }

    char strFileData[4096];
    int iActionCompleted = 0;
    int iSectionFound = 0;
    int iKeyFound = 0;

    char *strCopyOfLine = NULL;
    char *strKeyName = NULL;
    char *strKeyValue = NULL;

    while (fgets(strFileData, sizeof(strFileData), fCfgTargetFile) != NULL) {

        // if anything wasn't changed yet, the current line is a section and section wasn't found at this momment
        if (iActionCompleted == 0 && strstr(strFileData, "[") != NULL && iSectionFound == 0) {
            // if the current section is the target one
            if (strncmp(strFileData, pSectionWithBracket, (strlen(strFileData)-1)) == 0) {
                iSectionFound = 1;
                fprintf(fTmpFile, "%s", strFileData);
            } else { // if not, just save it
                fprintf(fTmpFile, "%s", strFileData);
            }
        } else if (iActionCompleted == 0 && iSectionFound == 1) { // if action wasn't comppleted at this time and section wass found
            // if another section was encountered, it means that the taarget secion is over,
            if (strstr(strFileData, "[") != NULL && iKeyFound == 0){
                fprintf(fTmpFile, "%s=%s\n\n", pKey, pNewValue);
                fprintf(fTmpFile, "%s", strFileData);
                iKeyFound = 1;
                iActionCompleted = 1;
                rc = RC_KEY_SUCCESSFULLY_CREATED;
            } else {

                // if line has nothing skip it, or it will break this application
                if (strlen(strFileData) == 1 && strFileData[0] == '\n') {
                    continue;
                }

                char *pLineData = strFileData;
                char *pTokenizedKey;
                strCopyOfLine = malloc(strlen(pLineData)+1);
                strKeyName = malloc(strlen(pLineData)+1);
                strKeyValue = malloc(strlen(pLineData)+1);

                if (strCopyOfLine == NULL || strKeyName == NULL || strKeyValue == NULL){
                    goto free_Memory;
                    return RC_MALLOC_FAILED;
                }

                // Copy the content of the line to an variable, this way strtok will not messup with the data from the file
                strcpy(strCopyOfLine, pLineData);
                // Break the line and store its values into pKeyName(Name of read key) and pKeyValue(Value of read Key)
                pTokenizedKey = strtok(strCopyOfLine, "=");
                if(pTokenizedKey != NULL) {
                    strcpy(strKeyName, pTokenizedKey);
                    pTokenizedKey = strtok(NULL, "=");
                    if(pTokenizedKey != NULL) {
                        strcpy(strKeyValue, pTokenizedKey);
                    }
                }
                // here, we  needd to check who has the largest length to use it on strncmp, this way we will be able to compare these two without the interference of the CR
                int iCompareValue;
                if (strlen(strKeyValue) > strlen(pNewValue)) {
                    iCompareValue = (strlen(strKeyValue) - 1);
                } else {
                    iCompareValue = strlen(pNewValue);
                }

                // if the key has the same name to the target
                if (strncmp(strKeyName, pKey, strlen(strKeyName)) == 0) {
                    //  if the kkey  value is the same, do nothing
                    if (strncmp(strKeyValue, pNewValue, iCompareValue) == 0) {
                        fprintf(fTmpFile, "%s", strFileData);
                        rc = RC_KEY_NOT_UPDATED;
                    } else { // if the value are different, update the key
                        fprintf(fTmpFile, "%s=%s\n", pKey, pNewValue);
                        rc = RC_KEY_VALUE_UPDATED;
                    }
                    iKeyFound = 1;
                    iActionCompleted = 1;
                } else { // if not, just print it
                    fprintf(fTmpFile, "%s", strFileData);
                }
            }
        } else { // if actions was already compleated or section was not found and this is a line with key, save this to the file
            fprintf(fTmpFile, "%s", strFileData);
        }
    }

    // if the file has ended and the section wasnt found create it at the end
    if (iSectionFound == 0 && iActionCompleted == 0) {
        fprintf(fTmpFile, "\n%s\n", pSectionWithBracket);
        fprintf(fTmpFile, "%s=%s\n", pKey, pNewValue);
        iActionCompleted = 1;
        rc = RC_SECTION_SUCCESSFULLY_CREATED;
    }

    // if key was now found and the file has ended it means that you need to create the key at the end
    if (iKeyFound == 0 && iActionCompleted == 0){
        fseek(fTmpFile, 0, SEEK_END);
        fprintf(fTmpFile, "%s=%s\n", pKey, pNewValue);
        iActionCompleted = 1;
        rc = RC_KEY_SUCCESSFULLY_CREATED;
    }

    fclose(fTmpFile);
    fclose(fCfgTargetFile);

    // ALL done, delete the old file and replace it with the temp
    if (iActionCompleted == 1){
        remove(strCfgFileFullPath);
        rename("temp.cfg", strCfgFileFullPath);
    }

    goto free_Memory;
    return rc;

free_Memory: 
    
    if (strFormattedSection != NULL) {
        free(strFormattedSection);
    }

    if (strCopyOfLine != NULL) {
        free(strCopyOfLine);
    }
    
    if (strKeyName != NULL) {
        free(strKeyName);
    }

    if (strKeyValue != NULL) {
        free(strKeyValue);
    }

    if (fCfgTargetFile) {
        fclose(fCfgTargetFile);
    }

    if (fTmpFile) {
        fclose(fTmpFile);
    }

}

// - Create Cfg Section
EXPORT int cfgFileAddSection(char *pPath, char *pCfgFileName, char *pSection) {
    // TODO logLogFileInitialize(WARNING, "libcfgfile", 1,1);
    if( pCfgFileName == NULL ) {
        //logError(" pFileName = null");
        return RC_NULL_FILE_POINTER;
    } else if ( pSection == NULL ) {
        //logError("pSessionName=null");
        return RC_NULL_SECTION_NAME;
    }

    char strCfgFileFullPath[256];
    if (isPath(pPath) == RC_FAILURE) {
        //cfgFilePath is invalid, empty or null use only filename
        strcpy(strCfgFileFullPath,pCfgFileName);
    } else {
        // concatenate path with file
        sprintf(strCfgFileFullPath, "%s/%s",pPath,pCfgFileName);
    }

    FILE *fFile = fopen(strCfgFileFullPath, "a+");  // Open the file for appending and reading.
    if (fFile == NULL) {
        // TODO LOG(logError("pPath[%s] pCfgFileName[%s] Fail to open",pPath,pCfgFileName);
        return RC_FILE_DOES_NOT_EXIST;
    }

    // Check if the section already exists
    char strLine[256];
    while (fgets(strLine, sizeof(strLine), fFile) != NULL) {
        if (strLine[0] == '[' && strstr(strLine, pSection) != NULL) {
            // Section already exists, close the file and return
            fclose(fFile);
            return RC_SECTION_ALREADY_EXISTS;
        }

    }

    // If the section doesn't exist, add it
    fprintf(fFile, "\n[%s]\n", pSection);

    fclose(fFile);
    return RC_SECTION_SUCCESSFULLY_CREATED;
}

// Function to delete a section from an INI file
EXPORT int cfgFileDelSection(char *pPath, char* pCfgFileName, char* pSection) {
    // TODO logLogFileInitialize(WARNING, "libcfgfile", 1,1);

    int rc = RC_NULL_POINTER;
    if( pCfgFileName == NULL ) {
        //logError(" pFileName = null");
        return RC_NULL_FILE_POINTER;
    } else if (strstr(pCfgFileName,".cfg") == NULL) {
        //logError("pSessionName=null");
        return RC_INVALID_FILE_NAME;
    } else if ( pSection == NULL ) {
        //logError("pSessionName=null");
        return RC_NULL_SECTION_NAME;
    } 

    // TODO LOG(logDebug("Path[%s] CfgFileName[%s] Section[%s]",pPath,pCfgFileName,pSection);

    char *strSectionWBrackets = malloc(strlen(pSection)+3);
    
    if (strSectionWBrackets == NULL) {
        return RC_MALLOC_FAILED;
    }

    sprintf(strSectionWBrackets, "[%s]", pSection);

    FILE* fFile;
    FILE* fTmpFile;
    char strLineData[256];
    int iSectionFound = 0;
    int biggerComparisonStrLength;
    int iSectionDeleted = 0;

    char strCfgFileFullPath[256];
    if (isPath(pPath) == RC_FAILURE) {
        //cfgFilePath is invalid, empty or null use only filename
        strcpy(strCfgFileFullPath,pCfgFileName);
    } else {
        // concatenate path with file
        sprintf(strCfgFileFullPath, "%s/%s",pPath,pCfgFileName);
    }

    // Open the INI file
    fFile = fopen(strCfgFileFullPath, "r");
    if (fFile == NULL) {
        //logError("pPath[%s] pCfgFileName[%s] Fail to open",pPath,pCfgFileName);
        
        // Dealocates memmory
        free(strSectionWBrackets);
        
        return RC_FILE_DOES_NOT_EXIST;
    }

    // Create a temporary file
    fTmpFile = fopen("temp.cfg", "w");
    if (fTmpFile == NULL) {
        fclose(fFile);
        // TODO LOG(logError(" pCfgFileName[temp.cfg] Fail to open");
        
        // Dealocates memmory
        free(strSectionWBrackets);
        
        return RC_FAILED_CREATION_TMP_FILE;
    }

    while (fgets(strLineData, sizeof(strLineData), fFile) != NULL){
        // Checks lines that has [
        if (strstr(strLineData, "[") != NULL && iSectionFound == 0){

            // checks witch one has the largest length andd use  that length into the strncmp
            if (strlen(strLineData) >   strlen(strSectionWBrackets)){
                biggerComparisonStrLength = strlen(strLineData)-1;
            } else {
                biggerComparisonStrLength = strlen(strSectionWBrackets);
            }
            // checks if the tarrget line its the section, if yes, skipt it and its children
            if (strncmp(strLineData, strSectionWBrackets, biggerComparisonStrLength) == 0){
                iSectionFound = 1;
                continue;
            } else { // if not save it
                fprintf(fTmpFile,  "%s", strLineData);
            }
        } else if (iSectionFound == 1) {

            // Section has ended, stop skipping lines
            if (strstr(strLineData, "[") != NULL) {
                fprintf(fTmpFile,  "%s", strLineData);
                iSectionFound = 0;
                iSectionDeleted = 1;
                continue;
            } else { // sectionn has not ended, sskkip this line
                continue;
            }
        } else {
            fprintf(fTmpFile,  "%s", strLineData);
        }
    }
    // Close the files
    fclose(fFile);
    fclose(fTmpFile);

    // Dealocates memmory
    free(strSectionWBrackets);

    if (iSectionDeleted == 1) {
        // Replace the original file with the temporary file
        remove(pCfgFileName);
        rename("temp.cfg", pCfgFileName);
        rc = RC_SUCCESSFULLY_DELETED_SECION;
    } else {
        rc = RC_CFG_SECTION_NOT_FOUND;
    }
    return rc;
}

// Function to delete a key from an INI file
EXPORT int cfgFileDelKey(char *pPath, char* pCfgFileName, char* pSection, char* pKey) {
    // TODO logLogFileInitialize(WARNING, "libcfgfile", 1,1);
    int rc = RC_NULL_FILE_POINTER;
    if( pCfgFileName == NULL ) {
        //logError(" pFileName = null rc[%d]",rc);
        return rc;// RC_NULL_FILE_POINTER;
    } else if ( pSection == NULL ) {
        //logError("pSessionName=null");
        return RC_NULL_SECTION_NAME;
    } else if (pKey == NULL) {
        //logError("pKeyName=null");
        return RC_NULL_SECTION_KEY;
    }

    char strSectionWBrackets[64];
    sprintf(strSectionWBrackets, "[%s]", pSection);

    FILE* fFile = NULL;
    FILE* fTmpFile = NULL;
    char strLineData[2038];
    char strCopyOfLine[2048];
    char strKeyName[2048];
    char strKeyValue[2048];
    char* pLineData = NULL;
    char* pTokenizedKey = NULL;
    int iSectionFound = 0;
    int iKeyFound = 0;

    char strCfgFileFullPath[256];
    if (isPath(pPath) == RC_FAILURE) {
        //cfgFilePath is invalid, empty or null use only filename
        strcpy(strCfgFileFullPath,pCfgFileName);
    } else {
        // concatenate path with file
        sprintf(strCfgFileFullPath, "%s/%s",pPath,pCfgFileName);
    }

    // Open the INI file
    fFile = fopen(strCfgFileFullPath, "r");
    if (fFile == NULL) {
            // TODO LOG(logError("pPath[%s] pCfgFileName[%s] Fail to open",pPath,pCfgFileName);
        return RC_FILE_DOES_NOT_EXIST;
    }

    // Create a temporary file
    fTmpFile = fopen("temp.cfg", "w");
    if (fTmpFile == NULL) {
        fclose(fFile);
        // TODO LOG(logError(" pCfgFileName[temp.cfg] Fail to open");
        return RC_FAILED_CREATION_TMP_FILE;
    }
    int biggerComparisonStrLength = 0;
    // Loop through each line in the file
    while (fgets(strLineData, sizeof(strLineData), fFile) != NULL) {
        if (strlen(strLineData) == 0) {
            continue;
        }
        // Checks lines that has [
nextSection:
        pLineData = strLineData;
        if (strstr(strLineData, "[") != NULL && iSectionFound == 0){
            // checks witch one has the largest length andd use  that length into the strncmp
            if (strlen(strLineData) > strlen(strSectionWBrackets)){
                biggerComparisonStrLength = strlen(strLineData)-1;
            } else {
                biggerComparisonStrLength = strlen(strSectionWBrackets);
            }
            // checks if the tarrget line its the section
            if (strncmp(strLineData, strSectionWBrackets, biggerComparisonStrLength) == 0){
                // Save this section name
                iSectionFound = 1;
                // TODO LOG(logDebug("Path[%s] CfgFileName[%s] Section[%s] Key[%s]SECTION FOUND",pPath,pCfgFileName,pSection,pKey);
                // TODO LOG(logDebug("        strLineData  %s",strLineData);
                // TODO LOG(logDebug("strSectionWBrackets  %s",strSectionWBrackets);
                if (strlen(strLineData) > 0) {
                    fprintf(fTmpFile,  "%s", strLineData);
                }
                // Section Was Found
                while (fgets(strLineData, sizeof(strLineData), fFile) != NULL) {
                    if (strstr(strLineData, "["))  {
                        // starting next section
                        // TODO LOG(logDebug("        strLineData  %s NEXT SECTTION",strLineData);
                        // TODO LOG(logDebug("strSectionWBrackets  %s",strSectionWBrackets);
                        goto nextSection;
                    }
                    strcpy(strKeyName," ");
                    if (strlen(strLineData) == 0) {
                        continue;
                    }
                    pLineData = strLineData;
                    // Searshing for Key on this section
                    // Copy the content of the line to an variable, this way strtok will not messup with the data from the file
                    strcpy(strCopyOfLine, pLineData);
                    // Break the line and store its values into pKeyName(Name of read key) and pKeyValue(Value of read Key)
                    pTokenizedKey = strtok(strCopyOfLine, "=");
                    if(pTokenizedKey != NULL) {
                        strcpy(strKeyName, pTokenizedKey);
                        pTokenizedKey = strtok(NULL, "=");
                        if(pTokenizedKey != NULL) {
                            strcpy(strKeyValue, pTokenizedKey);
                        }
                    }
                    // Check if the line is equal the target key
                    pLineData = strLineData;
                    if (strcmp(strKeyName, pKey) == 0) {
                        // key found don save it key because it will be delected
                        iKeyFound = 1;
                        // TODO LOG(logDebug("Path[%s] CfgFileName[%s] Section[%s] Key[%s]KEY_DELETED",pPath,pCfgFileName,pSection,pKey);
                        // TODO LOG(logDebug("        strLineData  %s",strLineData);
                        // TODO LOG(logDebug("strSectionWBrackets  %s",strSectionWBrackets);                        
                        break;
                    } else {
                        // Save existing key that missmatch
                        if (strlen(strLineData) > 0) {
                            fprintf(fTmpFile,  "%s", strLineData);
                        }
                    }
                }// end While searshing key on this section to be removed
            }// End of Section found
            if(iKeyFound == 0) {
                // save it line on tmpfile
                if (strlen(strLineData) > 0) {
                    fprintf(fTmpFile,  "%s", strLineData);
                }
            }
        } else {
            // save it line on tmpfile
            if (strlen(strLineData) > 0) {
                fprintf(fTmpFile,  "%s", strLineData);
            }
        }
    }// end while scanning all file

    // Close the files
     fclose(fFile);
    fclose(fTmpFile);
    if (iKeyFound == 1) {
        // Replace the original file with the temporary file
        remove(pCfgFileName);
        rename("temp.cfg", pCfgFileName);
        rc = RC_SUCCESSFULLY_DELETED_KEY;
    } else if (iSectionFound == 0) {
        // TODO LOG(logDebug("Path[%s] CfgFileName[%s] Section[%s] Key[%s]SECTION_NOT_FOUND",pPath,pCfgFileName,pSection,pKey);
        rc = RC_CFG_SECTION_NOT_FOUND;
    } else {
        // Chave não encontrada na sessão especifica
        // TODO LOG(logDebug("Path[%s] CfgFileName[%s] Section[%s] Key[%s]KEY_NOT_FOUND",pPath,pCfgFileName,pSection,pKey);
        rc = RC_CFG_KEY_NOT_FOUND;
    }
    return rc;
}