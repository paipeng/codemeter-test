
#include "test_codemeter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CodeMeter.h>

#define FIRM_CODE 10

void ErrorHandler(char *line, int ExitCode, HCMSysEntry hcmEntry);



HCMSysEntry openCMSysEntry() {
    HCMSysEntry hcmse;
    CMACCESS2 cmAcc;
    memset(&cmAcc, 0, sizeof(CMACCESS2));
    cmAcc.mflCtrl |= CM_ACCESS_NOUSERLIMIT;
    cmAcc.mulFirmCode = FIRM_CODE;
    hcmse = CmAccess2(CM_ACCESS_LOCAL, &cmAcc);
    return hcmse;
}

HCMSysEntry openCodeMeterEntry(int firm_code, int product_code) {                                                                     
    HCMSysEntry hcmse;
    CMACCESS2 cmAcc;
    memset(&cmAcc, 0, sizeof(CMACCESS2));
    strncpy(cmAcc.mcmCredential.mszUsername, "paipeng", sizeof(cmAcc.mcmCredential.mszUsername));
    cmAcc.mflCtrl = CM_ACCESS_NOUSERLIMIT;
    cmAcc.mulFirmCode = firm_code;
    cmAcc.mulProductCode = product_code;
    hcmse = CmAccess2(CM_ACCESS_LOCAL, &cmAcc);
    return hcmse;
}

unsigned int get_codemeter_sn() {
    HCMSysEntry hcmse = openCMSysEntry();
    CMBOXINFO cmBoxInfo;
    int res;
    char sn[11];
    int serial_number = 0;

    memset(&cmBoxInfo, 0, sizeof(CMBOXINFO));
    res = CmGetInfo(hcmse, CM_GEI_BOXINFO, &cmBoxInfo, sizeof(cmBoxInfo));
    
    if(0 != res) {
        if (cmBoxInfo.musBoxMask < 10) {
            snprintf(sn, sizeof(char)*11, "%d%d", cmBoxInfo.musBoxMask, cmBoxInfo.mulSerialNumber);
            serial_number = atoi(sn);
        } else {
            serial_number = cmBoxInfo.mulSerialNumber;
        }
    }
    CmRelease(hcmse);
    return serial_number;
}

long read_codemeter_counter(int product_code) {
    HCMSysEntry hcmse;
    CMBOXENTRY cmboxentry;
    CMACCESS2 cmacc;
    long ulUnitCounter = -3;
    printf("open entry product_code: %d\n", product_code);

    hcmse = openCodeMeterEntry(FIRM_CODE, product_code);  
    
    if (hcmse) {
        memset(&cmboxentry, 0, sizeof(CMBOXENTRY));
        if (0 == CmGetInfo(hcmse, CM_GEI_ENTRYINFO, &cmboxentry, sizeof(CMBOXENTRY))) {
            printf("Error %d in CmGetInfo(CM_GEI_ENTRYINFO)", CmGetLastErrorCode());
            ulUnitCounter = -1;
        } else {
            if ((cmboxentry.mflSetPios & CM_GF_UNITCOUNTER) == CM_GF_UNITCOUNTER) {
                ulUnitCounter = cmboxentry.mulUnitCounter;
            }
        }
        CmRelease(hcmse);
    } else {
        printf("Error %d in CmAccess().\n", CmGetLastErrorCode());
        ulUnitCounter = -2;
    }
    return ulUnitCounter;
}

void set_codemeter_led(int state) {
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

