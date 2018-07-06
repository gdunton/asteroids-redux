#ifndef WINWAV_H
#define WINWAV_H

#include <Windows.h>
#include <mmsystem.h>
#include <string>

//-----------------------------------------------------------------------------
// Typing macros 
//-----------------------------------------------------------------------------
#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2


class WinWav
{
	WAVEFORMATEX* m_pwfx;
	HMMIO m_hmmio; // MM I/O handle for the WAVE
	MMCKINFO m_ck; // Multimedia RIFF chunk
	MMCKINFO m_ckRiff; // Use in opening a WAVE file
	DWORD m_dwSize; // The size of the wave file
	MMIOINFO m_mmioinfoOut;
	DWORD m_dwFlags;
	bool m_bIsReadingFromMemory;
	BYTE* m_pbData;
	BYTE* m_pbDataCur;
	ULONG m_ulDataSize;
	char* m_pResourceBuffer;

protected:
	HRESULT ReadMMIO();
	HRESULT WriteMMIO(WAVEFORMATEX* pwfxDest);

public:
	WinWav();
	~WinWav();

	HRESULT Open(LPWSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags);
	HRESULT OpenFromMemory(BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags);
	HRESULT Close();

	HRESULT Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead);
	HRESULT Write(UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote);

	DWORD GetSize();
	HRESULT ResetFile();

	WAVEFORMATEX* GetFormat()
	{
		return m_pwfx;
	};
};

#endif
