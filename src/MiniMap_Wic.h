#pragma once
namespace MiniMapWic {
  typedef GUID WICPixelFormatGUID;
  enum { WICBitmapEncoderNoCache = 2 };
  #define MMWIC_SLOT( n ) virtual HRESULT STDMETHODCALLTYPE _slot##n() = 0
  struct __declspec( uuid( "00000105-a8f2-4877-ba0a-fd2b6645fb94" ) ) W_IFrameEncode : public IUnknown {
    virtual HRESULT STDMETHODCALLTYPE Initialize( IPropertyBag2* pIEncoderOptions ) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetSize( UINT uiWidth, UINT uiHeight ) = 0;
    MMWIC_SLOT( 1 );
    virtual HRESULT STDMETHODCALLTYPE SetPixelFormat( WICPixelFormatGUID* pPixelFormat ) = 0;
    MMWIC_SLOT( 2 );
    MMWIC_SLOT( 3 );
    MMWIC_SLOT( 4 );
    virtual HRESULT STDMETHODCALLTYPE WritePixels( UINT lineCount, UINT cbStride, UINT cbBufferSize, BYTE* pbPixels ) = 0;
    MMWIC_SLOT( 5 );
    virtual HRESULT STDMETHODCALLTYPE Commit() = 0;
  };
  struct __declspec( uuid( "00000103-a8f2-4877-ba0a-fd2b6645fb94" ) ) W_IEncoder : public IUnknown {
    virtual HRESULT STDMETHODCALLTYPE Initialize( IStream* pIStream, int cacheOption ) = 0;
    MMWIC_SLOT( 1 );
    MMWIC_SLOT( 2 );
    MMWIC_SLOT( 3 );
    MMWIC_SLOT( 4 );
    MMWIC_SLOT( 5 );
    MMWIC_SLOT( 6 );
    virtual HRESULT STDMETHODCALLTYPE CreateNewFrame( W_IFrameEncode** ppIFrameEncode, IPropertyBag2** ppIEncoderOptions ) = 0;
    virtual HRESULT STDMETHODCALLTYPE Commit() = 0;
  };
  struct __declspec( uuid( "135FF860-22B7-4ddf-B0F6-218F4F299A43" ) ) W_IStreamW : public IStream {
    MMWIC_SLOT( 1 );
    virtual HRESULT STDMETHODCALLTYPE InitializeFromFilename( LPCWSTR wzFileName, DWORD dwDesiredAccess ) = 0;
  };
  struct __declspec( uuid( "ec5ec8a9-c395-4314-9c77-54d7a935ff70" ) ) W_IFactory : public IUnknown {
    MMWIC_SLOT( 1 );
    MMWIC_SLOT( 2 );
    MMWIC_SLOT( 3 );
    MMWIC_SLOT( 4 );
    MMWIC_SLOT( 5 );
    virtual HRESULT STDMETHODCALLTYPE CreateEncoder( REFGUID guidContainerFormat, const GUID* pguidVendor, W_IEncoder** ppIEncoder ) = 0;
    MMWIC_SLOT( 6 );
    MMWIC_SLOT( 7 );
    MMWIC_SLOT( 8 );
    MMWIC_SLOT( 9 );
    MMWIC_SLOT( 10 );
    virtual HRESULT STDMETHODCALLTYPE CreateStream( W_IStreamW** ppIWICStream ) = 0;
  };
  #undef MMWIC_SLOT
}
