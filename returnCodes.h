#ifndef RETURN_CODES_H
#define RETURN_CODES_H

// ======================================
//             Return Codes
// ======================================

// ----------------------------------------------------------- Successes ----------------------------------------------------------

const int RC_SUCCESS                                        =     1;   // - Success or True Value

// --- Successfull Checks ---

// - Path -
const int RC_PATH_IS_DIR                                    =     2;   // - Received Path is a DIR and i have access to it
const int RC_PATH_IS_FILE                                   =     3;   // - Received Path is a File and i have access to it

// - Key and Section -
const int RC_KEY_SUCCESSFULLY_FOUND                         =     4;   // - Key and Section were Found
const int RC_SECTION_KEY_FOUND_BUT_EMPTY                    =     5;   // - Section and Key were found, but key is empty

// --- Successfull Operations ---

// - File -
const int RC_FILE_SUCCESSFULLY_CREATED                      =     6;   // - File was Successfully Created 

// - Key and Section -
const int RC_KEY_SUCCESSFULLY_CREATED                       =     7;   // - Key was successfully added to the end of the section
const int RC_SECTION_SUCCESSFULLY_CREATED                   =     8;   // - Section was created

// --- Successfull Update ---
const int RC_KEY_VALUE_UPDATED                              =     9;   // - Key Vallue was successfully updated

// --- Successfull Delete ---
const int RC_SUCCESSFULLY_DELETED_SECION                    =    10;   // - Section was successfully deleted
const int RC_SUCCESSFULLY_DELETED_KEY                       =    11;   // - Key was successfully deleted

// ----------------------------------------------------------- Failures ---------------------------------------------------------- 

const int RC_FAILURE                                        =     0;   // - General Failure or False Value

// --- Failure Checks ---

// - Path - 
const int RC_INVALID_PATH                                   =    -1;   // - Invalid Path Received
const int RC_PATH_DIR_DECLINED_ACCESS                       =    -2;   // - Access to Path was Declined and it is a DIR
const int RC_PATH_FILE_DECLINED_ACCESS                      =    -3;   // - Access to Path was Declined and it is a File

// - File - 
const int RC_NULL_FILE_POINTER                              =    -4;   // - Received a Null File Pointer
const int RC_INVALID_FILE_NAME                              =    -5;   // - Received a Invalid file name
const int RC_FILE_DOES_NOT_EXIST                            =    -6;   // - Target File doesn't Exists
const int RC_FILE_ALREADY_EXISTS                            =    -7;   // - Tried to Create a File that Already Exists

// - Sections and Keys -
const int RC_NULL_SECTION_KEY                               =    -8;   // - Received a Null Section Key
const int RC_NULL_SECTION_NAME                              =    -9;   // - Received Section Name is Null
const int RC_SECTION_NAME_START_BRACKET_MISSING             =   -10;   // - Received Section Name is Missing the Start Bracket 
const int RC_SECTION_NAME_END_BRACKET_MISSING               =   -11;   // - Received Section Name is Missing the End Bracket
const int RC_SESSION_NAME_HAVE_SPACES_BETWEEN_BRACKETS      =   -12;   // - Received Section Name have spaces between brackets 
const int RC_SECTION_ALREADY_EXISTS                         =   -13;   // - Targed section already  exists

// - Oter Parameters - 
const int RC_NULL_POINTER                                   =   -14;   // - Received a NULL pointer

// --- Open Failures ---
const int RC_FAILED_TO_OPEN_FILE                            =   -15;   // - Failed to Open the File

// --- Not Found ---
const int RC_CFG_SECTION_NOT_FOUND                          =   -16;  // - Target Cfg Section Was not Found
const int RC_CFG_KEY_NOT_FOUND                              =   -17;  // - Targed Cfg Key was not found

// --- Operation Failure ---
const int RC_FAILED_CREATION_TMP_FILE                       =   -18;  // - Failed to Create a Temp File
const int RC_KEY_NOT_UPDATED                                =   -19;  // - Failed to Update the Key

// --- Memmory Failure ---
const int RC_MALLOC_FAILED                                  =   -20;  // - Memmory Alocation (Malloc) went wrong

// --- Testing Failure ---
const int RC_NOT_A_NUMBER                                   =   -21;  // - Imput is not a number

#endif