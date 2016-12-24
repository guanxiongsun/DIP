// jpegapi.h

#ifndef __JPEGAPI_H__
#define __JPEGAPI_H__


#ifndef DIBIMAGE_NO_JPEG

HDIB ReadJPEGFile(CFile& file);
BOOL SaveJPEGFile(HDIB hDib, CFile& file, DWORD dwQuality = 100);

#endif //DIBIMAGE_NO_JPEG


#endif //__JPEGAPI_H__
