#include "StdAfx.h"
#include "..\stdafx.h"
#include <fstream>
#include <commoncontrols.h>
#include "FileIcon.h"

//////////////////////////////////////////////////////////////////////////
///
CFileIcon *CFileIcon::s_pInstance = NULL;

CFileIcon *CFileIcon::getInstance()
{
	if (NULL == s_pInstance)
	{
		s_pInstance = new CFileIcon();
	}
	return s_pInstance;
}

CFileIcon::CFileIcon(void)
{
}

CFileIcon::~CFileIcon(void)
{
}


int CFileIcon::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0, size = 0;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	bool found = false;
	for (UINT ix = 0; !found && ix < num; ++ix)
	{
		if (_wcsicmp(pImageCodecInfo[ix].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[ix].Clsid;
			found = true;
			break;
		}
	}

	free(pImageCodecInfo);
	return found;
}

//根据图标HICON保存图片为Png图片
bool CFileIcon::SaveHIconToPngFile(HICON hIcon, LPCTSTR lpszPicFileName)
{
	if (hIcon == NULL)
	{
		return false;
	}

	ICONINFO icInfo = { 0 };
	if (!::GetIconInfo(hIcon, &icInfo))
	{
		return false;
	}

	BITMAP bitmap;
	GetObject(icInfo.hbmColor, sizeof(BITMAP), &bitmap);

	Gdiplus::Bitmap* pBitmap = NULL;
	Gdiplus::Bitmap* pWrapBitmap = NULL;

	do
	{
		if (bitmap.bmBitsPixel != 32)
		{
			pBitmap = Gdiplus::Bitmap::FromHICON(hIcon);
		}
		else
		{
			pWrapBitmap = Gdiplus::Bitmap::FromHBITMAP(icInfo.hbmColor, NULL);
			if (!pWrapBitmap)
				break;

			Gdiplus::BitmapData bitmapData;
			Gdiplus::Rect rcImage(0, 0, pWrapBitmap->GetWidth(), pWrapBitmap->GetHeight());

			pWrapBitmap->LockBits(&rcImage, Gdiplus::ImageLockModeRead, pWrapBitmap->GetPixelFormat(), &bitmapData);
			pBitmap = new (Gdiplus::Bitmap)(bitmapData.Width, bitmapData.Height, bitmapData.Stride, PixelFormat32bppARGB, (BYTE*)bitmapData.Scan0);
			pWrapBitmap->UnlockBits(&bitmapData);
		}

		CLSID encoderCLSID;
		GetEncoderClsid(_T("image/png"), &encoderCLSID);
		Gdiplus::Status st = pBitmap->Save(lpszPicFileName, &encoderCLSID, NULL);
		if (st != Gdiplus::Ok)
			break;

	} while (false);

	delete pBitmap;
	if (pWrapBitmap)
		delete pWrapBitmap;
	DeleteObject(icInfo.hbmColor);
	DeleteObject(icInfo.hbmMask);

	return true;
}

LPCTSTR CFileIcon::GetIconFile(LPCTSTR fileName, BOOL isFolder, BYTE iconSize)
{
	CDuiString imgName = GetIconPath(fileName, isFolder, iconSize);
	fstream file;
	file.open(imgName);
	if (file)
		return imgName;
	BOOL ret;
	DWORD fileType;
	SHFILEINFO shFileInfo = { 0 };
	fileName = ::PathFindExtension(fileName);
	if (isFolder)
		fileType = FILE_ATTRIBUTE_DIRECTORY;
	else 
		fileType = FILE_ATTRIBUTE_NORMAL;
	if (iconSize == IconSize::MEDIUM_ICON){
		SHGetFileInfo(fileName, fileType, &shFileInfo, sizeof(shFileInfo), SHGFI_ICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);
		HIMAGELIST* imageList = NULL;
		//SHIL_EXTRALARGE获取48 * 48的图标， SHIL_JUMBO 获取256 * 256的图标。
		HRESULT hResult = ::SHGetImageList(SHIL_EXTRALARGE, IID_IImageList, (void**)&imageList);
		HICON icon_handle = NULL;
		if (hResult == S_OK)
		{
			hResult = ((IImageList*)imageList)->GetIcon(shFileInfo.iIcon, ILD_NORMAL, &icon_handle);
		}
		ret = SaveHIconToPngFile(icon_handle, imgName);
		DestroyIcon(icon_handle);
	}
	else if (iconSize == IconSize::SMALL_ICON){
		SHGetFileInfo(fileName, fileType, &shFileInfo, sizeof(shFileInfo), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);
		ret = SaveHIconToPngFile(shFileInfo.hIcon, imgName);
	}

	//销毁一个图标并且释放该图标所占用的内存
	DestroyIcon(shFileInfo.hIcon);
	if (!ret)
		imgName = _T("");
	return imgName;
}

CDuiString CFileIcon::GetIconPath(LPCTSTR fileName, bool isFolder, byte iconSize)
{
	CDuiString iconPath = _T("D:\\fileIcon\\");
	if (iconSize == IconSize::MEDIUM_ICON)
		iconPath += _T("mediumIcon\\");
	else if (iconSize==IconSize::SMALL_ICON)
		iconPath += _T("smallIcon\\");

	//判断路径是否存在，如果没有则创建一个
	if (!SetCurrentDirectory(iconPath))
		CreateDirectory(iconPath, NULL);

	if (isFolder){
		iconPath += _T("folder.png");
	}
	else{
		CDuiString ext = ::PathFindExtension(fileName);
		iconPath += ext.Right(ext.GetLength() - 1) + _T(".png");
	}
	return iconPath;
}