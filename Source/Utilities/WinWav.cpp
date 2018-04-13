#include "STD.h"
#include "WinWav.h"

//-----------------------------------------------------------------------------
// File: Wave.cpp
//
// Desc: Classes for reading and writing wav files. Feel free to use this class
//       as a starting point for adding extra functionality.
//
// XNA Developer Connection
//
// Copyright (c) Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------

#include "WinWav.h"
#undef min // use __min instead
#undef max // use __max instead

//--------------------------------------------------------------------------------------
// Helper macros
//--------------------------------------------------------------------------------------
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		{if(p) { delete (p); (p)=NULL; } }
#endif

//-----------------------------------------------------------------------------
// Name: CWaveFile::CWaveFile()
// Desc: Constructs the class.  Call Open() to open a wave file for reading.
//       Then call Read() as needed.  Calling the destructor or Close()
//       will close the file.
//-----------------------------------------------------------------------------
WinWav::WinWav()
{
    m_pwfx = nullptr;
    m_hmmio = nullptr;
    m_pResourceBuffer = nullptr;
    m_dwSize = 0;
    m_bIsReadingFromMemory = FALSE;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::~CWaveFile()
// Desc: Destructs the class
//-----------------------------------------------------------------------------
WinWav::~WinWav()
{
    Close();

    if( !m_bIsReadingFromMemory )
        SAFE_DELETE_ARRAY( m_pwfx );
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::Open()
// Desc: Opens a wave file for reading
//-----------------------------------------------------------------------------
HRESULT WinWav::Open( LPWSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags )
{
    HRESULT hr;

    m_dwFlags = dwFlags;
    m_bIsReadingFromMemory = FALSE;

    if( m_dwFlags == WAVEFILE_READ )
    {
        if( strFileName == nullptr )
            return E_INVALIDARG;
        SAFE_DELETE_ARRAY( m_pwfx );

        m_hmmio = mmioOpen( strFileName, nullptr, MMIO_ALLOCBUF | MMIO_READ );

        if(nullptr == m_hmmio )
        {
            HRSRC hResInfo;
            HGLOBAL hResData;
            DWORD dwSize;
            VOID* pvRes;

            // Loading it as a file failed, so try it as a resource
            if(nullptr == ( hResInfo = FindResource(nullptr, strFileName, L"WAVE" ) ) )
            {
                if(nullptr == ( hResInfo = FindResource(nullptr, strFileName, L"WAV" ) ) )
                    return E_FAIL;
            }

            if(nullptr == ( hResData = LoadResource( GetModuleHandle(nullptr ), hResInfo ) ) )
                return E_FAIL;

            if( 0 == ( dwSize = SizeofResource( GetModuleHandle(nullptr ), hResInfo ) ) )
                return E_FAIL;

            if(nullptr == ( pvRes = LockResource( hResData ) ) )
                return E_FAIL;

            m_pResourceBuffer = new CHAR[ dwSize ];
            if( m_pResourceBuffer == nullptr )
                return E_OUTOFMEMORY;
            memcpy( m_pResourceBuffer, pvRes, dwSize );

            MMIOINFO mmioInfo;
            ZeroMemory( &mmioInfo, sizeof( mmioInfo ) );
            mmioInfo.fccIOProc = FOURCC_MEM;
            mmioInfo.cchBuffer = dwSize;
            mmioInfo.pchBuffer = ( CHAR* )m_pResourceBuffer;

            m_hmmio = mmioOpen(nullptr, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );
        }

        if( FAILED( hr = ReadMMIO() ) )
        {
            // ReadMMIO will fail if its an not a wave file
            mmioClose( m_hmmio, 0 );
            return hr;
        }

        if( FAILED( hr = ResetFile() ) )
            return hr;

        // After the reset, the size of the wav file is m_ck.cksize so store it now
        m_dwSize = m_ck.cksize;
    }
    else
    {
        m_hmmio = mmioOpen( strFileName, nullptr, MMIO_ALLOCBUF |
                            MMIO_READWRITE |
                            MMIO_CREATE );
        if(nullptr == m_hmmio )
            return E_FAIL;

        if( FAILED( hr = WriteMMIO( pwfx ) ) )
        {
            mmioClose( m_hmmio, 0 );
            return hr;
        }

        if( FAILED( hr = ResetFile() ) )
            return hr;
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::OpenFromMemory()
// Desc: copy data to CWaveFile member variable from memory
//-----------------------------------------------------------------------------
HRESULT WinWav::OpenFromMemory( BYTE* pbData, ULONG ulDataSize,
                                   WAVEFORMATEX* pwfx, DWORD dwFlags )
{
    m_pwfx = pwfx;
    m_ulDataSize = ulDataSize;
    m_pbData = pbData;
    m_pbDataCur = m_pbData;
    m_bIsReadingFromMemory = TRUE;

    if( dwFlags != WAVEFILE_READ )
        return E_NOTIMPL;

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::ReadMMIO()
// Desc: Support function for reading from a multimedia I/O stream.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_pwfx.
//-----------------------------------------------------------------------------
HRESULT WinWav::ReadMMIO()
{
    MMCKINFO ckIn;           // chunk info. for general use.
    PCMWAVEFORMAT pcmWaveFormat;  // Temp PCM structure to load in.

    memset( &ckIn, 0, sizeof(ckIn) );

    m_pwfx = nullptr;

    if( ( 0 != mmioDescend( m_hmmio, &m_ckRiff, nullptr, 0 ) ) )
        return E_FAIL;

    // Check to make sure this is a valid wave file
    if( ( m_ckRiff.ckid != FOURCC_RIFF ) ||
        ( m_ckRiff.fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) )
        return E_FAIL;

    // Search the input file for for the 'fmt ' chunk.
    ckIn.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
    if( 0 != mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK ) )
        return E_FAIL;

    // Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
    // if there are extra parameters at the end, we'll ignore them
    if( ckIn.cksize < ( LONG )sizeof( PCMWAVEFORMAT ) )
        return E_FAIL;

    // Read the 'fmt ' chunk into <pcmWaveFormat>.
    if( mmioRead( m_hmmio, ( HPSTR )&pcmWaveFormat,
                  sizeof( pcmWaveFormat ) ) != sizeof( pcmWaveFormat ) )
        return E_FAIL;

    // Allocate the waveformatex, but if its not pcm format, read the next
    // word, and thats how many extra bytes to allocate.
    if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
    {
        m_pwfx = ( WAVEFORMATEX* )new CHAR[ sizeof( WAVEFORMATEX ) ];
        if(nullptr == m_pwfx )
            return E_FAIL;

        // Copy the bytes from the pcm structure to the waveformatex structure
        memcpy( m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
        m_pwfx->cbSize = 0;
    }
    else
    {
        // Read in length of extra bytes.
        WORD cbExtraBytes = 0L;
        if( mmioRead( m_hmmio, ( CHAR* )&cbExtraBytes, sizeof( WORD ) ) != sizeof( WORD ) )
            return E_FAIL;

        m_pwfx = ( WAVEFORMATEX* )new CHAR[ sizeof( WAVEFORMATEX ) + cbExtraBytes ];
        if(nullptr == m_pwfx )
            return E_FAIL;

        // Copy the bytes from the pcm structure to the waveformatex structure
        memcpy( m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
        m_pwfx->cbSize = cbExtraBytes;

        // Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
        if( mmioRead( m_hmmio, ( CHAR* )( ( ( BYTE* )&( m_pwfx->cbSize ) ) + sizeof( WORD ) ),
                      cbExtraBytes ) != cbExtraBytes )
        {
            SAFE_DELETE( m_pwfx );
            return E_FAIL;
        }
    }

    // Ascend the input file out of the 'fmt ' chunk.
    if( 0 != mmioAscend( m_hmmio, &ckIn, 0 ) )
    {
        SAFE_DELETE( m_pwfx );
        return E_FAIL;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::GetSize()
// Desc: Retuns the size of the read access wave file
//-----------------------------------------------------------------------------
DWORD WinWav::GetSize()
{
    return m_dwSize;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::ResetFile()
// Desc: Resets the internal m_ck pointer so reading starts from the
//       beginning of the file again
//-----------------------------------------------------------------------------
HRESULT WinWav::ResetFile()
{
    if( m_bIsReadingFromMemory )
    {
        m_pbDataCur = m_pbData;
    }
    else
    {
        if( m_hmmio == nullptr )
            return CO_E_NOTINITIALIZED;

        if( m_dwFlags == WAVEFILE_READ )
        {
            // Seek to the data
            if( -1 == mmioSeek( m_hmmio, m_ckRiff.dwDataOffset + sizeof( FOURCC ),
                                SEEK_SET ) )
                return E_FAIL;

            // Search the input file for the 'data' chunk.
            m_ck.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
            if( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
                return E_FAIL;
        }
        else
        {
            // Create the 'data' chunk that holds the waveform samples.
            m_ck.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
            m_ck.cksize = 0;

            if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
                return E_FAIL;

            if( 0 != mmioGetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
                return E_FAIL;
        }
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::Read()
// Desc: Reads section of data from a wave file into pBuffer and returns
//       how much read in pdwSizeRead, reading not more than dwSizeToRead.
//       This uses m_ck to determine where to start reading from.  So
//       subsequent calls will be continue where the last left off unless
//       Reset() is called.
//-----------------------------------------------------------------------------
HRESULT WinWav::Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
    if( m_bIsReadingFromMemory )
    {
        if( m_pbDataCur == nullptr )
            return CO_E_NOTINITIALIZED;
        if( pdwSizeRead != nullptr )
            *pdwSizeRead = 0;

        if( ( BYTE* )( m_pbDataCur + dwSizeToRead ) >
            ( BYTE* )( m_pbData + m_ulDataSize ) )
        {
            dwSizeToRead = m_ulDataSize - ( DWORD )( m_pbDataCur - m_pbData );
        }

#pragma warning( disable: 4616 )    // disable warning about warning number '22104' being out of range
#pragma warning( disable: 22104 )   // disable PREfast warning during static code analysis
        CopyMemory( pBuffer, m_pbDataCur, dwSizeToRead );
#pragma warning( default: 22104 )
#pragma warning( default: 4616 )

        if( pdwSizeRead != nullptr )
            *pdwSizeRead = dwSizeToRead;

        return S_OK;
    }
    else
    {
        MMIOINFO mmioinfoIn; // current status of m_hmmio

        if( m_hmmio == nullptr )
            return CO_E_NOTINITIALIZED;
        if( pBuffer == nullptr || pdwSizeRead == nullptr )
            return E_INVALIDARG;

        *pdwSizeRead = 0;

        if( 0 != mmioGetInfo( m_hmmio, &mmioinfoIn, 0 ) )
            return E_FAIL;

        UINT cbDataIn = dwSizeToRead;
        if( cbDataIn > m_ck.cksize )
            cbDataIn = m_ck.cksize;

        m_ck.cksize -= cbDataIn;

        for( DWORD cT = 0; cT < cbDataIn; cT++ )
        {
            // Copy the bytes from the io to the buffer.
            if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
            {
                if( 0 != mmioAdvance( m_hmmio, &mmioinfoIn, MMIO_READ ) )
                    return E_FAIL;

                if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
                    return E_FAIL;
            }

            // Actual copy.
            *( ( BYTE* )pBuffer + cT ) = *( ( BYTE* )mmioinfoIn.pchNext );
            mmioinfoIn.pchNext++;
        }

        if( 0 != mmioSetInfo( m_hmmio, &mmioinfoIn, 0 ) )
            return E_FAIL;

        *pdwSizeRead = cbDataIn;

        return S_OK;
    }
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::Close()
// Desc: Closes the wave file
//-----------------------------------------------------------------------------
HRESULT WinWav::Close()
{
    if( m_dwFlags == WAVEFILE_READ )
    {
        if ( m_hmmio != nullptr )
        {
            mmioClose( m_hmmio, 0 );
            m_hmmio = nullptr;
        }
        SAFE_DELETE_ARRAY( m_pResourceBuffer );
    }
    else
    {
        m_mmioinfoOut.dwFlags |= MMIO_DIRTY;

        if( m_hmmio == nullptr )
            return CO_E_NOTINITIALIZED;

        if( 0 != mmioSetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
            return E_FAIL;

        // Ascend the output file out of the 'data' chunk -- this will cause
        // the chunk size of the 'data' chunk to be written.
        if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
            return E_FAIL;

        // Do this here instead...
        if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
            return E_FAIL;

        mmioSeek( m_hmmio, 0, SEEK_SET );

        if( 0 != ( INT )mmioDescend( m_hmmio, &m_ckRiff, nullptr, 0 ) )
            return E_FAIL;

        m_ck.ckid = mmioFOURCC( 'f', 'a', 'c', 't' );

        if( 0 == mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
        {
            DWORD dwSamples = 0;
            mmioWrite( m_hmmio, ( HPSTR )&dwSamples, sizeof( DWORD ) );
            mmioAscend( m_hmmio, &m_ck, 0 );
        }

        // Ascend the output file out of the 'RIFF' chunk -- this will cause
        // the chunk size of the 'RIFF' chunk to be written.
        if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
            return E_FAIL;

        mmioClose( m_hmmio, 0 );
        m_hmmio = nullptr;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::WriteMMIO()
// Desc: Support function for reading from a multimedia I/O stream
//       pwfxDest is the WAVEFORMATEX for this new wave file.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_ck.
//-----------------------------------------------------------------------------
HRESULT WinWav::WriteMMIO( WAVEFORMATEX* pwfxDest )
{
    DWORD dwFactChunk; // Contains the actual fact chunk. Garbage until WaveCloseWriteFile.
    MMCKINFO ckOut1;

    memset( &ckOut1, 0, sizeof(ckOut1) );

    dwFactChunk = ( DWORD )-1;

    // Create the output file RIFF chunk of form type 'WAVE'.
    m_ckRiff.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );
    m_ckRiff.cksize = 0;

    if( 0 != mmioCreateChunk( m_hmmio, &m_ckRiff, MMIO_CREATERIFF ) )
        return E_FAIL;

    // We are now descended into the 'RIFF' chunk we just created.
    // Now create the 'fmt ' chunk. Since we know the size of this chunk,
    // specify it in the MMCKINFO structure so MMIO doesn't have to seek
    // back and set the chunk size after ascending from the chunk.
    m_ck.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
    m_ck.cksize = sizeof( PCMWAVEFORMAT );

    if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
        return E_FAIL;

    // Write the PCMWAVEFORMAT structure to the 'fmt ' chunk if its that type.
    if( pwfxDest->wFormatTag == WAVE_FORMAT_PCM )
    {
        if( mmioWrite( m_hmmio, ( HPSTR )pwfxDest,
                       sizeof( PCMWAVEFORMAT ) ) != sizeof( PCMWAVEFORMAT ) )
            return E_FAIL;
    }
    else
    {
        // Write the variable length size.
        if( ( UINT )mmioWrite( m_hmmio, ( HPSTR )pwfxDest,
                               sizeof( *pwfxDest ) + pwfxDest->cbSize ) !=
            ( sizeof( *pwfxDest ) + pwfxDest->cbSize ) )
            return E_FAIL;
    }

    // Ascend out of the 'fmt ' chunk, back into the 'RIFF' chunk.
    if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
        return E_FAIL;

    // Now create the fact chunk, not required for PCM but nice to have.  This is filled
    // in when the close routine is called.
    ckOut1.ckid = mmioFOURCC( 'f', 'a', 'c', 't' );
    ckOut1.cksize = 0;

    if( 0 != mmioCreateChunk( m_hmmio, &ckOut1, 0 ) )
        return E_FAIL;

    if( mmioWrite( m_hmmio, ( HPSTR )&dwFactChunk, sizeof( dwFactChunk ) ) !=
        sizeof( dwFactChunk ) )
        return E_FAIL;

    // Now ascend out of the fact chunk...
    if( 0 != mmioAscend( m_hmmio, &ckOut1, 0 ) )
        return E_FAIL;

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::Write()
// Desc: Writes data to the open wave file
//-----------------------------------------------------------------------------
HRESULT WinWav::Write( UINT nSizeToWrite, BYTE* pbSrcData, UINT* pnSizeWrote )
{
    UINT cT;

    if( m_bIsReadingFromMemory )
        return E_NOTIMPL;
    if( m_hmmio == nullptr )
        return CO_E_NOTINITIALIZED;
    if( pnSizeWrote == nullptr || pbSrcData == nullptr )
        return E_INVALIDARG;

    *pnSizeWrote = 0;

    for( cT = 0; cT < nSizeToWrite; cT++ )
    {
        if( m_mmioinfoOut.pchNext == m_mmioinfoOut.pchEndWrite )
        {
            m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
            if( 0 != mmioAdvance( m_hmmio, &m_mmioinfoOut, MMIO_WRITE ) )
                return E_FAIL;
        }

        *( ( BYTE* )m_mmioinfoOut.pchNext ) = *( ( BYTE* )pbSrcData + cT );
        ( BYTE* )m_mmioinfoOut.pchNext++;

        ( *pnSizeWrote )++;
    }

    return S_OK;
}
