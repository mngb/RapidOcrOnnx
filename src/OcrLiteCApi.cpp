#ifdef __CLIB__

#include "OcrLiteCApi.h"
#include "OcrLiteImpl.h"

extern "C"
{
typedef struct {
    OcrLiteImpl OcrObj;
    std::string strRes;
} OCR_OBJ;

_QM_OCR_API OCR_HANDLE
OcrInit(const char *szDetModel, const char *szClsModel, const char *szRecModel, const char *szKeyPath, int nThreads) {

    OCR_OBJ *pOcrObj = new OCR_OBJ;
    if (pOcrObj) {
        pOcrObj->OcrObj.setNumThread(nThreads);

        pOcrObj->OcrObj.initModels(szDetModel, szClsModel, szRecModel, szKeyPath);

        return pOcrObj;
    } else {
        return nullptr;
    }

}

_QM_OCR_API OCR_BOOL
OcrDetect(OCR_HANDLE handle, const char *imgPath, const char *imgName, OCR_PARAM *pParam) {

    OCR_OBJ *pOcrObj = (OCR_OBJ *) handle;
    if (!pOcrObj)
        return FALSE;

    OCR_PARAM Param = *pParam;
    if (Param.padding == 0)
        Param.padding = 50;

    if (Param.maxSideLen == 0)
        Param.maxSideLen = 1024;

    if (Param.boxScoreThresh == 0)
        Param.boxScoreThresh = 0.6;

    if (Param.boxThresh == 0)
        Param.boxThresh = 0.3f;

    if (Param.unClipRatio == 0)
        Param.unClipRatio = 2.0;

    if (Param.doAngle == 0)
        Param.doAngle = 1;

    if (Param.mostAngle == 0)
        Param.mostAngle = 1;

    OcrResult result = pOcrObj->OcrObj.detect(imgPath, imgName, Param.padding, Param.maxSideLen,
                                              Param.boxScoreThresh, Param.boxThresh, Param.unClipRatio,
                                              Param.doAngle != 0, Param.mostAngle != 0);
    if (result.strRes.length() > 0) {
        pOcrObj->strRes = result.strRes;
        return TRUE;
    } else
        return FALSE;
}


_QM_OCR_API OCR_BOOL
OcrFreeResult(OCR_RESULT *result) {
    if(result && result->textBlocksLength && result->textBlocks){
        for(int i = 0; i < result->textBlocksLength; i++){
            free(result->textBlocks[i].charScores);
            free(result->textBlocks[i].text);
            free(result->textBlocks[i].boxPoint);
        }
        free(result->textBlocks);
        return true;
    }
    return false;
}

_QM_OCR_API int OcrGetLen(OCR_HANDLE handle) {
    OCR_OBJ *pOcrObj = (OCR_OBJ *) handle;
    if (!pOcrObj)
        return 0;
    return pOcrObj->strRes.size() + 1;
}

_QM_OCR_API OCR_BOOL OcrGetResult(OCR_HANDLE handle, char *szBuf, int nLen) {
    OCR_OBJ *pOcrObj = (OCR_OBJ *) handle;
    if (!pOcrObj)
        return FALSE;

    if (nLen > pOcrObj->strRes.size()) {
        strncpy(szBuf, pOcrObj->strRes.c_str(), pOcrObj->strRes.size());
        szBuf[pOcrObj->strRes.size() - 1] = 0;
    }

    return pOcrObj->strRes.size();
}

_QM_OCR_API void OcrDestroy(OCR_HANDLE handle) {
    OCR_OBJ *pOcrObj = (OCR_OBJ *) handle;
    if (pOcrObj)
        delete pOcrObj;
}

};
#endif
