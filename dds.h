#define 	FOURCC_DXT1   0x31545844
#define 	FOURCC_DXT3   0x33545844
#define 	FOURCC_DXT5   0x35545844
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

struct D3D_PixelFormat
{
  int dwSize;
  int dwFlags;
  int dwFourCC;
  int dwRGBBitCount;
  int dwRBitMask, dwGBitMask, dwBBitMask;
  int dwRGBAlphaBitMask;
};

struct D3D_Caps2
{
  int dwCaps1;
  int dwCaps2;
  int Reserved[2];
};
struct DDSURFACEDESC2
{
  int dwSize;
  int dwFlags;
  int dwHeight;
  int dwWidth;
  int dwLinearSize;
  int dwDepth;
  int dwMipMapCount;
  int dwReserved1[11];
  D3D_PixelFormat ddpfPixelFormat;
  D3D_Caps2 ddsCaps;
  int dwReserved2;
};
void loadCompressedTexture(const char* name){
    DDSURFACEDESC2 dd;
    GLenum format;
    GLubyte *pD;
    char filecode[4];
    FILE *pFile;
    int factor;
    int bufferSize;
    pFile = fopen( name, "rb" );
    fread( filecode, 1, 4, pFile );
    fread( &dd, sizeof(dd), 1, pFile );
    switch( dd.ddpfPixelFormat.dwFourCC ){
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            factor = 2;
        break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            factor = 4;
        break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            factor = 4;
        break;
    }
    if(dd.dwMipMapCount > 1)bufferSize = dd.dwLinearSize * factor;
    else bufferSize = dd.dwLinearSize;
    pD = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));
    fread( pD, 1, bufferSize, pFile );
    fclose( pFile );
    int nBlockSize;
    if(format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )nBlockSize = 8;
    else nBlockSize = 16;
    int nSize;
    int noffset = 0;
    for( int i = 0; i <dd.dwMipMapCount; ++i ){
         nSize = ((dd.dwWidth+3)/4) * ((dd.dwHeight+3)/4) * nBlockSize;
         glCompressedTexImage2D(GL_TEXTURE_2D,i,format,dd.dwWidth,dd.dwHeight,0,nSize,pD+noffset);
         noffset += nSize;
         dd.dwWidth = (dd.dwWidth+1)>>1;
         dd.dwHeight = (dd.dwHeight+1)>>1;
    }
    free(pD);
}

#define DDS_MAGIC 0x20534444
#define DDSD_MIPMAPCOUNT 0x00020000
union DDS_header {
  struct {
    unsigned int    dwMagic;
    unsigned int    dwSize;
    unsigned int    dwFlags;
    unsigned int    dwHeight;
    unsigned int    dwWidth;
    unsigned int    dwPitchOrLinearSize;
    unsigned int    dwDepth;
    unsigned int    dwMipMapCount;
    unsigned int    dwReserved1[ 11 ];
    struct {
      unsigned int    dwSize;
      unsigned int    dwFlags;
      unsigned int    dwFourCC;
      unsigned int    dwRGBBitCount;
      unsigned int    dwRBitMask;
      unsigned int    dwGBitMask;
      unsigned int    dwBBitMask;
      unsigned int    dwAlphaBitMask;
    }               sPixelFormat;
    struct {
      unsigned int    dwCaps1;
      unsigned int    dwCaps2;
      unsigned int    dwDDSX;
      unsigned int    dwReserved;
    }               sCaps;
    unsigned int    dwReserved2;
  };
  char data[ 128 ];
};
void loadDds( FILE * f ){
  int size;
  DDS_header hdr;
  unsigned int mipMapCount = 0;
  fread( &hdr, sizeof( hdr ), 1, f );
  glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );
  mipMapCount = (hdr.dwFlags & DDSD_MIPMAPCOUNT) ? hdr.dwMipMapCount : 1;
  size = hdr.dwWidth * hdr.dwHeight * 4;
  unsigned char * data = (unsigned char *)malloc( size );
  for( unsigned int ix = 0; ix < mipMapCount; ++ix ) {
      fread( data, 1, size, f );
      glTexImage2D( GL_TEXTURE_2D, ix, GL_RGBA, hdr.dwWidth, hdr.dwHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data );
      hdr.dwWidth = (hdr.dwWidth+1)>>1;
      hdr.dwHeight = (hdr.dwHeight+1)>>1;
      size = hdr.dwWidth * hdr.dwHeight * 4;
  }
  free( data );
}


GLuint loadTexture(bool compressed, const char* name){
       GLuint DDS;
       if(!fopen( name, "rb" )){
                  printf("cannot open DDS file %s \n", name);
                  MessageBox( NULL, name, "cannot open DDS file", MB_OK|MB_ICONEXCLAMATION );
                  return 0;
       }
       glGenTextures(1, &DDS);
       glBindTexture(GL_TEXTURE_2D, DDS);
       if(compressed)loadCompressedTexture(name);    
       else          loadDds(fopen( name, "rb" ));
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
       return DDS;
}
