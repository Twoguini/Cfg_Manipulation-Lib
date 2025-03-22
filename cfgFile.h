#ifndef CFGFILE_H
#define CFGFILE_H

/// Function: trim from
///  Description: Rempve leading and trealing spaces from string
///    example: pInData "   abcd   "
///     return "abcd"
///   Params:
///  Return:
char *trim(const char *pInData);

/// Function: isPath
///  Description: return TRUE if pPath is a existing path otherwise return FALSE
///   Params:
///  Return:
int isPath(char *pPath);

/// Function: cfgFileCreate
///  Description: Create a Config file if it do not exist
///   Params:
///  Return:
int cfgFileCreate( char *pPath, char *pCfgFileName);

/// Function: sectionExist from
///  Description: Check if session Exist
///   Params:
///  Return:
int cfgFileSectionExist(char *pPath, char *pCfgFileName,char *pSection);


/// Function: cfgFileGetStrSection
///  Description: return the pointer of trimmed section name or null if the pstrCfgFileLine has not
///               the cfgFile(INI) Section Format
///   Params:
///  Return:
int cfgFileGetStrSection(char *pstrCfgFileLine, char *pstrSectionContent);


/// Function: cfgFileKeyExist from
///  Description: Check if key in section Exist
///   Params:
///  Return:
int cfgFileKeyExist(char *pPath, char *pCfgFileName, char *pSection, char *pKey);

/// Function: addSection from
///  Description: Add a new empty session w/o any key/value
///   Params:
///  Return:
int cfgFileAddSection( char *pPath, char *pCfgFileName, char *pSection);


/// Function: cfgFileDelSection from
///  Description: Delete Session and all it key/value pairs
///   Params:
///  Return:
int cfgFileDelSection(char *pPath, char* pCfgFileName, char* pSection);

/// Function: deleteKey from
///  Description: Delete Session key /value pair
///   Params:
///  Return:
int cfgFileDelKey(char *pPath, char* pCfgFileName, char* pSection, char* pKey);

/// Function: getCfgFileValue from
///  Description: Reads a config file Session key value from current cfgFilePath
///   Params:
///  Return:
int cfgFileReadKeyValue(char *pPath,  char *pCfgFileName, char *pSection, char *pKey, char *pValue);


/// Function: cfgFileUpdtKeyValue from
///  Description: If key Exist update it, if not, create it
///   Params:
///  Return:
int cfgFileUpdtKeyValue(char *pPath, char* pCfgFileName, char* pSection, char* pKey, char* pNewValue);

#endif /* CFGFILE_H */
