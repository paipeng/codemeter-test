
#include "test_codemeter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CodeMeter.h>

#define FIRM_CODE 10

void ErrorHandler(char *line, int ExitCode, HCMSysEntry hcmEntry);
void dg_set_codemeter_led(int state) {
    unsigned int iFirmCode, iProductCode, res, flags;
    HCMSysEntry hcmEntry;
    CMACCESS cmAcc;
    CMBOXCONTROL hcmBoxCtrl;
    CMPROGRAM_BOXCONTROL hcmPgmBoxCtrl;
    
    // Set the variables identifying the CmStick
    // and the action to be carried out.
    // Firm Code = Product Code = 0 will select the first found CmStick
    
    iFirmCode = FIRM_CODE;
    iProductCode = 0;
    flags = state;
    
    memset(&cmAcc, 0, sizeof(CMACCESS));
    cmAcc.mflCtrl = CM_ACCESS_NOUSERLIMIT;
    
    cmAcc.mulFirmCode = iFirmCode;
    cmAcc.mulProductCode = iProductCode;
    
    // Access the CmStick only locally
    hcmEntry = CmAccess(CM_ACCESS_LOCAL , &cmAcc);
    
    // Handle any errors.
    // Most probably no CmStick with the appropriate Firm Code
    // and Product Code was found.
    if(NULL == hcmEntry){
        ErrorHandler("CmAccess", 1, hcmEntry);
    }
    
    // Now retrieve the the old data from the CmStick.
    // We only change the LED settings.
    res = CmGetInfo(hcmEntry, CM_GEI_BOXCONTROL, &hcmBoxCtrl, sizeof(hcmBoxCtrl));
    
    // Handle any errors.
    // As the handle was just being opened, there should not be any.
    if(0 == res){
        ErrorHandler("CmGetInfo", 2, hcmEntry);
    }
    
    // Now set the LED information to the requested values.
    hcmPgmBoxCtrl.mflCtrl = CM_BC_ABSOLUTE;
    hcmPgmBoxCtrl.musIndicatorFlags = ((hcmBoxCtrl.musIndicatorFlags & 0x0fffffffc) | flags);
    hcmPgmBoxCtrl.musReserve = 0;
    res = CmProgram(hcmEntry, CM_GF_SET_BOXCONTROL, &hcmPgmBoxCtrl, sizeof(hcmPgmBoxCtrl), NULL, 0);
    
    // Handle any errors.
    // As the handle was just being opened, there should not be any.
    if(0 == res){
        ErrorHandler("CmProgram", 3, hcmEntry);
    }
    
    // Clean up anything.
    CmRelease(hcmEntry);
}


void ErrorHandler(char *line, int ExitCode, HCMSysEntry hcmEntry)
{
    char acErrText[256];
    switch(CmGetLastErrorCode())
    {
        case CMERROR_NO_ERROR:
            return;
        case CMERROR_BOX_NOT_FOUND:
            fprintf(stderr, "%s: Appropriate CmContainer not found.\n", line);
            break;
        case CMERROR_ENTRY_NOT_FOUND:
            fprintf(stderr, "%s: Appropriate entry not found.\n", line);
            break;
        case CMERROR_INVALID_HANDLE:
            fprintf(stderr, "%s: Handle invalid! CmDongle removed?\n", line);
            break;
        default:
            CmGetLastErrorText(CM_GLET_ERRORTEXT, acErrText, sizeof(acErrText));
            fprintf(stderr, "%s: Other error occurred: \"%s\"\n", line, acErrText);
            break;
    }
    
    // Despite the error try to close the handle.
    if (NULL != hcmEntry)
        CmRelease(hcmEntry);
}

