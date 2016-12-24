#include "GlobalApi.h"
#include "stdafx.h"
#include "cdib.h"
#include "math.h"
/*************************************************************************
 *
 * \�������ƣ�
 *   GetFileName()
 *
 * \�������:
 *   CString		strFilePathName		- ͼ����ļ��� 
 *   int		nCurFrameNum		- ��ǰ֡��ͼ���ļ���
 *
 * \����ֵ:
 *   CString			- ���ظ���֡����ͼ���ļ���
 *
 * \˵��:
 *   �ú�������ָ���ļ�·�����͵�ǰͼ�����е�֡����ȡͼ���ļ���
 *   �ú�������Ҫע����ǣ�ֻ�ܶ�ȡ0-999֡ͼ��ͼ��Ϊbmp��ʽ���Ұ���
 *   ֡�����ֽ��д洢�������һ֡ͼ���ļ���Ϊ������001.bmp����33֡ͼ��
 *   ���ļ���Ϊ������033.bmp���������bmp�ļ����򷵻�"NULL"��
 *
 *************************************************************************
 */
CString GetFileName(CString strFilePathName, int nCurFrameNum)
{
	//�ļ���·����
	CString strTempFileName;

	int nNumPos=strFilePathName.Find(".");
	if(nNumPos==-1){
		AfxMessageBox("Please choose a bmp file");
		return "NULL";
	}

	//��ʾȥ������չ�������ֱ�ŵ�·������������޶�֡��Ϊ0��999�����Բ�����λ����ʾ
	CString strFileNameNoExtNoNum=strFilePathName.Left(nNumPos-3);

	//��ʾ��ŵ��ַ���
	CString strTempNum;

	if(nCurFrameNum<10){
		strTempNum.Format("00%d",nCurFrameNum);
	}
	else {
		if(nCurFrameNum<100 &&nCurFrameNum>=10){
			strTempNum.Format("0%d",nCurFrameNum);
		}
		else{
			strTempNum.Format("%d",nCurFrameNum);
		}
	}
	
	// �õ�ͼ���ļ���
	strTempFileName=strFileNameNoExtNoNum+strTempNum+".bmp";
	
	// ����
	return strTempFileName;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   LoadDibSeq()
 *
 * \�������:
 *   CString		strFilePath		- ��һ֡ͼ����ļ��� 
 *   int		nCurFrameNum		- ��ǰ֡��ͼ���ļ���
 *   int		nTotalFrameNum		- ���м���ͼ��֡��
 *   CDib*		pDib			- ָ�򷵻�CDib���ָ��
 *
 * \����ֵ:
 *   BOOL			- �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 * \˵��:
 *   �ú�������ָ���ļ�·�����͵�ǰͼ�����е�֡����ȡͼ�����ݵ�pDib��
 *   �ú�������Ҫע����ǣ�ֻ�ܶ�ȡ0-999֡ͼ��ͼ��Ϊbmp��ʽ���Ұ���
 *   ֡�����ֽ��д洢�������һ֡ͼ���ļ���Ϊ������001.bmp����33֡ͼ��
 *   ���ļ���Ϊ������033.bmp��
 *
 *************************************************************************
 */
BOOL LoadDibSeq(CString strFilePath, int nCurFrameNum, int nTotalFrameNum, CDib* pDib)
{
	//һ������,�����ڴ���Ĺ�������Ҫװ�ص�֡��Ӧ���������ָ����
	//��ָ����֡�Ų��Ϸ�ʱ,��װ�ص�ǰ֡��ΪĬ��ֵ
	if (nCurFrameNum<1 || nCurFrameNum>nTotalFrameNum)
	{
		AfxMessageBox("Invalidate file frame number");
		return FALSE;
	}

	// ��õ�ǰ֡��ͼ���ļ���
	CString strTempFileName;
	strTempFileName=GetFileName(strFilePath,nCurFrameNum);
	
	CFile fileOpen=NULL;

	// ���ļ�����ȡ
	fileOpen.Open(strTempFileName,CFile::modeRead);
	if(pDib->Read(&fileOpen)==FALSE){
		AfxMessageBox("can not open the file "+strTempFileName);
		return FALSE;
	}
	return TRUE;
}


/*************************************************************************
 *
 * \�������ƣ�
 *   BinaFrameDiff()
 *
 * \�������:
 *   unsigned char*	pUnchImg1		- ͼ����ļ��� 
 *   unsigned char*	pUnchImg2		- ��ǰ֡��ͼ���ļ���
 *   int		nWidth
 *   int		nHeight
 *   unsigned char*	pUnchResult
 *   int		nThreshold
 *
 * \����ֵ:
 *   CString			- ���ظ���֡����ͼ���ļ���
 *
 * \˵��:
 *   �ú����Ƚ�pUnchImg1��pUnchImg2���������е�����,�������������     
 *�ݵĲ�ֵ�ľ���ֵ��Threshold��,��pUnchResult��Ӧ��Ԫ������Ϊ�߼�ֵ1,
 *�ûҶ�255��ʾ������Ϊ0�����ûҶ�0��ʾ
 *
 *************************************************************************
 */
void BinaFrameDiff(unsigned char *pUnchImg1, unsigned char *pUnchImg2, 
		  int nWidth, int nHeight, unsigned char * pUnchResult,
		  int nThreshold)
{
	int nTemp=0;
	for (int i=0;i<nHeight*nWidth;i++) {
			nTemp = abs(pUnchImg1[i] - pUnchImg2[i]);			
			pUnchResult[i] = nTemp > nThreshold ? 255:0;
	} 
	return ;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   ErodeFrameDiff()
 *
 * \�������:
 *   unsigned char*	pUnchImg1		- ͼ������ָ��
 *   int		nWidth			- ͼ����
 *   int		nHeight			- ͼ��߶�
 *   int		nErodeHalfWin		- ��ʴ���ڴ�С��һ��
 *   unsigned char*	pUnchResult		- ���������ֹ
 *   int		nThreshold		- ��ֵ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú������и�ʴ����,��̬ѧ������pUnchImg�е�ÿһ��,������һ���Ӧ��
 *�����ڵ�һЩ����,Ȼ����ݲ�������������������Ӧ��ֵ. �������൱�ڹ����˲�	
 *
 *************************************************************************
 */
void ErodeFrameDiff(unsigned char *pUnchImg, int nWidth, int nHeight, int nErodeHalfWin, 
	  int nErodeThreshold, unsigned char *pUnchResult)
{
	// ��������ͼ�󣬶�ͼ����и�ʴ����
	for (int i=nErodeHalfWin;i<nHeight-nErodeHalfWin;i++) {
		for (int j=nErodeHalfWin;j<nWidth-nErodeHalfWin;j++) {
			// ���֡��ĲΪ0�Ž��д���
			if (pUnchImg[i*nWidth+j] != 0) 
			{
				int iPointCount = 0;

				// ���ݴ˵�������жϴ˵��Ƿ���Ҫɾ��
				for (int r=-nErodeHalfWin;r<=nErodeHalfWin;r++) {
					for (int c=-nErodeHalfWin;c<=nErodeHalfWin;c++) {
						if (pUnchImg[(i+r)*nWidth+j+c] != 0) {
							iPointCount++;
						}
					}
				}

				// ��������в�Ϊ0�ĸ���С���趨����ֵ����ǿ������Ϊ0
				if (iPointCount < nErodeThreshold) {
					pUnchResult[i*nWidth+j] = 0;
				}
				else {
					pUnchResult[i*nWidth+j] = 255;
				}
			}
			else 
			{
				pUnchResult[i*nWidth+j] = 0;
			}
		}
	}
	return ;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   GetBackground()
 *
 * \�������:
 *   CString		strFilePath		- ��һ֡ͼ����ļ��� 
 *   int		nTotalFrameNum		- ���м���ͼ��֡��
 *   int		nImageWidth		- ͼ����
 *   int		nImageHeight		- ͼ��߶�
 *   unsigned char *	pUnchBackGround		- ָ�򷵻ر������ݵ�ָ��
 *
 * \����ֵ:
 *   BOOL			- �ɹ��򷵻�TRUE�����򷵻�FALSE
 *
 * \˵��:
 *   �ú�������ָ���ļ�����ͼ��������ȡ��ֹ����
 *
 *************************************************************************
 */
BOOL GetBackground(CString strFilePath, int nTotalFrameNum, int nImageWidth, 
		   int nImageHeight, unsigned char* pUnchBackGround)
{
	// �����ʱ�����Ѿ�����,��������,����Ҫ��һ�μ���
	/*if (pUnchBackGround!=NULL){		
		return TRUE;
	}*/

	
	// pUnchTemp1��pUnchTemp2��������������֮֡���֡��
	// ÿ��ֻҪ����һ֡����,��:����ոձȽ�k-1��k֡,��ô���ڱȽ�k��
	// k+1֡,��ôk֡�ǲ���Ҫ���¶����
	unsigned char* pUnchTemp1;
	unsigned char* pUnchTemp2;

	pUnchTemp1 = new unsigned char[nImageWidth * nImageHeight * sizeof(unsigned char)];
	pUnchTemp2 = new unsigned char[nImageWidth * nImageHeight * sizeof(unsigned char)];
	
	// ��ʱ���ͼ�����ݵ�CDibָ��
	CDib* pDibTemp;
	pDibTemp = new CDib;

	// ������һ֡���ݲ�����pDibTemp
	pDibTemp->Empty();
	if(!LoadDibSeq(strFilePath,1,nTotalFrameNum,pDibTemp)){
		return FALSE;
	}

	// Ȼ������ȡ���������pUnchTemp1��
	memcpy(pUnchTemp2,pDibTemp->m_lpImage,nImageWidth*nImageHeight*sizeof(unsigned char));	

	// pChResultAfterMor ��������¼֡��仯���ڴ�����
	unsigned char * pUnchTrackBox = new unsigned char[(nTotalFrameNum)*
		nImageWidth*nImageHeight*sizeof(unsigned char)];

	unsigned int index = 0;

	// ֡��������,������
	unsigned char *pUnchTemp3=new unsigned char[nImageWidth*nImageHeight*sizeof(unsigned char)];

	// ��ʴ֮�������,������
	unsigned char * pUnchResultAfterMor = new unsigned char[nImageWidth*nImageHeight*sizeof(unsigned char)];

	// ��ÿһ֡���бȽ�
	for (int i = 2; i<nTotalFrameNum-1; i++)	{	
		// �򿪵�i֡ͼ���ļ�������ͼ������CDib����pDibTemp��
		pDibTemp->Empty();
		if(!LoadDibSeq(strFilePath , i , nTotalFrameNum , pDibTemp)){
			return FALSE;
		}

		// Ȼ������ȡ���������pUnchTemp2��
		memcpy(pUnchTemp2,pDibTemp->m_lpImage,nImageWidth*nImageHeight);	

		// ��ͼ��֡����ж�ֵ������������ֵ�����ͼ������pUnchTemp3��
		BinaFrameDiff(pUnchTemp1,pUnchTemp2 ,nImageWidth,nImageHeight,pUnchTemp3,10);

		// �Զ�ֵ�����ͼ����и�ʴ����������Ը�ʴ���ڵĴ�С����Ϊ2����ֵΪ7
		ErodeFrameDiff(pUnchTemp3,nImageWidth,nImageHeight,2,7,pUnchResultAfterMor);

		// ���˶�ֵ����ĳ������pUnchTrackBox����Ӧλ��		
		memcpy(pUnchTrackBox+index,pUnchResultAfterMor,sizeof(unsigned char)*nImageWidth*nImageHeight);

		// ����ͼ��������pUnchTrackBox�е�ƫ����
		index = index + nImageWidth*nImageHeight*sizeof(unsigned char);
		
		// ÿ������֮֡��ıȽ�,��ʹ֡������һ��,pUnchTemp1���Ǵ�k֡����,pUnchTemp2֡�Ǵ�k+1
		// ֡����,����,ÿ��ֻҪ��pUnchTemp2�е����ݸ�pTemp1,��pTemp2���¶���ȿ�����.
		unsigned char* pUnchTag = NULL;

		pUnchTag   = pUnchTemp1;
		pUnchTemp1 = pUnchTemp2;
		pUnchTemp2 = pUnchTag;
	}
	
	// �ͷ��ѷ����ڴ�
	delete []pUnchTemp1;	
	pUnchTemp1 = NULL;
	delete []pUnchTemp2;	
	pUnchTemp2 = NULL;	
	delete []pUnchTemp3;	
	pUnchTemp3 = NULL;
	delete []pUnchResultAfterMor;	
	pUnchResultAfterMor=NULL;
	
	// ÿһ֡�Ĵ�С
	int nFrameSize = nImageWidth * nImageHeight * sizeof(unsigned char);
	
	// ��¼��󳤶�	
	int * pnTrackSegLen = new int [nImageWidth*nImageHeight];

	// ��¼��󳤶��е��м�֡���
	int * pnTrackSegFrame = new int [nImageWidth*nImageHeight];

	// ��ÿһ�����ص�������Ϊ0�ĳ��ȣ�������󳤶��е��м�֡��ż�¼����
	for (int y = 0; y<nImageHeight; y++)	{
		for (int x = 0; x<nImageWidth; x++)	{
			// ��������һ֡������ڸ�֡��һ��Ԫ�ص�ƫ����
			int offset = y * nImageWidth + x;

			// ��ʼ����󳤶�
			int largeLen = 0;
			int t = 1;

			// ����Ϊ0�ĶεĿ�ʼ�ͽ���֡���
			int segStart,segEnd;

			// ���ٳ���
			int segLen;

			// ��ǰ֡��
			int frameNum;
			
			segLen = 0;
			frameNum = 1;

			// �����������У����ٲ���¼�˵�����Ϊ0����󳤶�
			while (t < nTotalFrameNum - 1)	{
				// �����û�е������н������Ҵ˵�Ĳ�Ϊ0�����������һ֡����
				while ((t < nTotalFrameNum -1) && (pUnchTrackBox[t*nFrameSize+offset] != 0))	
					t++;

					//�����ʱ��t>= nTotalFrameNum - 1,��˵��,�Ѿ����������һ֡��
					if (t >= nTotalFrameNum - 1)	
						break;

					// ��ʱӦΪ�˳��ȵĿ�ʼ
					segStart = t;

					while ((t < nTotalFrameNum - 1) && (pUnchTrackBox[t*nFrameSize+offset] == 0))	
						t++;						

					// �˳��ȵĽ���֡���
					segEnd = t - 1;

					// ��ô�����Ϊ0��֡�ĳ���
					segLen = segEnd +1 -segStart;	

					// �ж��Ƿ�Ϊ��󳤶ȣ���������滻
					if (segLen > largeLen)	{
						largeLen = segLen;
						frameNum = (segEnd + segStart)/2;				
					}
				} 
				
				pnTrackSegLen[offset] = largeLen;
				pnTrackSegFrame[offset] = frameNum;
		}
	}

	delete []pUnchTrackBox;	pUnchTrackBox=NULL;
	
	// ��Ϊ��ÿ�����ض���,�������ܳ����ڲ�ͬ֡��,��ʱ��Ҫ������֡�����ڴ�
	unsigned char* pBuffer = new unsigned char[nTotalFrameNum*(nImageWidth*nImageHeight)];

	for (int k=1; k<nTotalFrameNum; k++)	{
		pDibTemp->Empty();
		LoadDibSeq(strFilePath , k , nTotalFrameNum , pDibTemp);

		// Ȼ������ȡ���������pBuffer����Ӧλ����
		memcpy(pBuffer+k*nFrameSize,pDibTemp->m_lpImage,nImageWidth*nImageHeight);	
	}

	// ��������ͼ�����ñ�������
	for (y=0; y<nImageHeight; y++)	{
		for (int x=0; x<nImageWidth; x++)	{
			// ��ô����ص���һ֡�����е�ƫ����
			int k = y * nImageWidth + x;	

			// ��ô����ص�����Ϊ0����󳤶ȵ��м�֡���
			int nFrameMax = pnTrackSegFrame [k];

			// ���þ�����󳤶ȵ��м�֡������Ϊ��������
			pUnchBackGround[k] = pBuffer[(nFrameMax*nFrameSize)+k];
		}
	}		

	delete []pnTrackSegLen; 
	pnTrackSegLen=NULL;
	delete []pnTrackSegFrame;	
	pnTrackSegFrame=NULL;
	delete []pBuffer; 
	pBuffer=NULL;
	
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	
	return TRUE;
}

