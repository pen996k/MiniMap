#pragma once
namespace MiniMap {
  inline void DetachViewTexture( zCViewDraw* view, zCTexture* own ) {
    if( !view || !view->Texture ) return;
    zCTexture* const t = view->Texture;
    view->Texture = 0;
    if( t != own )
      t->Release();
  }
  inline void DetachViewTexture() {
    DetachViewTexture( g_blit, g_genTex );
  }
  inline void ApplyTexture() {
    g_texReady = false;
    g_texTried = true;
    if( g_mapIdx < 0 ) return;
    DetachViewTexture();
    if( !g_genTex || !g_genWorld[0] || !StrEqUp( g_genWorld, g_world ) ) return;
    EnsureBlitter();
    if( !g_blit ) return;
    g_blit->Texture = g_genTex;
    g_texReady = true;
    MM_LOG( zSTRING( "terrain map in use, world " ) + zSTRING( g_world ) );
  }
  const float MINIMAP_Z = 1.0f;
  inline zCTexture* CurTex() {
    if( g_curTex ) return g_curTex;
    return g_blit ? g_blit->Texture : 0;
  }
  inline const TMapInfo& CurRect() {
    return g_curRect ? *g_curRect : g_maps[g_mapIdx];
  }
  inline unsigned char CurAlpha() {
    return (unsigned char)( ( g_curAlpha >= 0 ) ? g_curAlpha : g_alpha );
  }
  inline void BlitTexTile( float px, float py, float pw, float ph,
                           float u0, float v0, float u1, float v1 ) {
    if( !zrenderer || !CurTex() ) return;
    const unsigned char b = (unsigned char)g_bright;
    const unsigned char a = CurAlpha();
    zrenderer->DrawTile( CurTex(),
                         zVEC2( px, py ),
                         zVEC2( px + pw, py + ph ),
                         MINIMAP_Z,
                         zVEC2( u0, v0 ),
                         zVEC2( u1, v1 ),
                         zCOLOR( b, b, b, a ) );
  }
  inline void BlitTexF( float px, float py, float pw, float ph,
                        float u0, float v0, float u1, float v1 ) {
    if( !CurTex() || pw <= 0.0f || ph <= 0.0f ) return;
    static bool logged = false;
    if( !logged && g_blit && g_blit->Texture ) {
      logged = true;
      MM_LOG( zSTRING( "first slice: px " ) + zSTRING( (int)px ) + " " + zSTRING( (int)py ) + " " + zSTRING( (int)pw ) + "x" + zSTRING( (int)ph ) + "  uv " + zSTRING( u0, 4 ) + " " + zSTRING( v0, 4 ) + " " + zSTRING( u1, 4 ) + " " + zSTRING( v1, 4 ) );
      int tw = 0, th = 0;
      g_blit->Texture->GetPixelSize( tw, th );
      MM_LOG( zSTRING( "map texture: " ) + zSTRING( tw ) + "x" + zSTRING( th ) + ", frames " + zSTRING( g_blit->Texture->numAniFrames[0] ) + "x" + zSTRING( g_blit->Texture->numAniFrames[1] ) + ", tile " + zSTRING( (int)g_blit->Texture->isTextureTile ) + ", alpha " + zSTRING( (int)g_blit->Texture->hasAlpha ) );
    }
    BlitTexTile( px, py, pw, ph, u0, v0, u1, v1 );
  }
  inline void BlitTex( int px, int py, int pw, int ph,
                       float u0, float v0, float u1, float v1 ) {
    BlitTexF( (float)px, (float)py, (float)pw, (float)ph, u0, v0, u1, v1 );
  }
  inline bool ClipParam( float a, float b, float& t0, float& t1 ) {
    t0 = 0.0f;
    t1 = 1.0f;
    const float d = b - a;
    if( d > -0.000001f && d < 0.000001f ) return a >= 0.0f && a <= 1.0f;
    float ta = ( 0.0f - a ) / d;
    float tb = ( 1.0f - a ) / d;
    if( ta > tb ) {
      std::swap( ta, tb );
    }
    if( ta > t0 ) t0 = ta;
    if( tb < t1 ) t1 = tb;
    return t1 > t0;
  }
  static float g_cx = 0.0f, g_cy = 0.0f;
  static float g_ppuX = 0.1f;
  static float g_ppuY = 0.1f;
  static float g_px = 0.0f, g_pz = 0.0f;
  static float g_hx = 0.0f, g_hz = 1.0f;
  inline int AlignRaw() {
    if( g_full && g_fullAlign >= 0 ) return g_fullAlign & 7;
    return ( ( g_align >= 0 ) ? g_align : g_alignAuto ) & 7;
  }
  inline int AlignNow() {
    return AlignRaw() & 3;
  }
  inline void UpdateHeading() {
    if( !g_rotate || g_full ) {
      g_mirror = ( AlignRaw() & 4 ) != 0;
      const int a = AlignNow();
      switch( a & 3 ) {
        case 1:  g_hx =  1.0f;  g_hz =  0.0f;  break;
        case 2:  g_hx =  0.0f;  g_hz = -1.0f;  break;
        case 3:  g_hx = -1.0f;  g_hz =  0.0f;  break;
        default: g_hx =  0.0f;  g_hz =  1.0f;  break;
      }
      return;
    }
    g_mirror = ( AlignRaw() & 4 ) != 0;
    const zVEC3 at = player->GetAtVectorWorld();
    float tx = at[0];
    float tz = at[2];
    float len = sqrtf( tx * tx + tz * tz );
    if( len < 0.0001f ) return;
    tx /= len;
    tz /= len;
    float k = (float)g_rotSmooth * 0.01f;
    if( k <= 0.0f || k > 1.0f ) k = 1.0f;
    g_hx += ( tx - g_hx ) * k;
    g_hz += ( tz - g_hz ) * k;
    len = sqrtf( g_hx * g_hx + g_hz * g_hz );
    if( len < 0.0001f ) {
      g_hx = tx;
      g_hz = tz;
    }
    else {
      g_hx /= len;
      g_hz /= len;
    }
  }
  inline void VisibleHalf( int w, int h, float& halfX, float& halfZ ) {
    if( g_full && g_mapIdx >= 0 && g_maps[g_mapIdx].hasCoords ) {
      const TMapInfo& m = g_maps[g_mapIdx];
      float dx = m.x2 - m.x1;
      float dz = m.y1 - m.y2;
      if( dx < 0.0f ) dx = -dx;
      if( dz < 0.0f ) dz = -dz;
      {
        float ax1, az1, ax2, az2;
        if( FullArea( m, ax1, az1, ax2, az2 ) ) {
          float adx = ax2 - ax1;
          float adz = az2 - az1;
          if( adx < 0.0f ) adx = -adx;
          if( adz < 0.0f ) adz = -adz;
          if( adx > 1.0f && adz > 1.0f ) {
            dx = adx;
            dz = adz;
          }
        }
      }
      if( dx > 1.0f && dz > 1.0f ) {
        const float k = (float)g_fullPct * 0.01f;
        if( AlignNow() & 1 ) {
          std::swap( dx, dz );
        }
        const float needX = dx * 0.5f;
        const float needZ = ( h > 0 ) ? ( dz * 0.5f * (float)w / (float)h )
                                      : ( dz * 0.5f );
        const float big = ( needX > needZ ) ? needX * 2.0f : needZ * 2.0f;
        halfX = big * 0.5f * k;
        halfZ = ( w > 0 ) ? ( halfX * (float)h / (float)w ) : halfX;
        return;
      }
    }
    if( g_mapCells > 0 && g_mapIdx >= 0 && g_maps[g_mapIdx].hasCoords ) {
      const TMapInfo& m = g_maps[g_mapIdx];
      float dx = m.x2 - m.x1;
      float dz = m.y1 - m.y2;
      if( dx < 0.0f ) dx = -dx;
      if( dz < 0.0f ) dz = -dz;
      if( dx > 1.0f && dz > 1.0f ) {
        halfX = dx / (float)( 2 * g_mapCells );
        halfZ = dz / (float)( 2 * g_mapCells );
        if( g_shape && w > 0 ) halfZ = halfX * (float)h / (float)w;
        return;
      }
    }
    halfX = (float)g_zoom * 0.5f;
    halfZ = ( w > 0 ) ? ( halfX * (float)h / (float)w ) : halfX;
  }
  inline void ClampToWorld( const TMapInfo& m, float halfX, float halfZ ) {
    float hx = halfX, hz = halfZ;
    if( AlignNow() & 1 ) {
      hx = halfZ;
      hz = halfX;
    }
    const float xLo = ( m.x1 < m.x2 ) ? m.x1 : m.x2;
    const float xHi = ( m.x1 < m.x2 ) ? m.x2 : m.x1;
    const float zLo = ( m.y1 < m.y2 ) ? m.y1 : m.y2;
    const float zHi = ( m.y1 < m.y2 ) ? m.y2 : m.y1;
    if( xHi - xLo > 2.0f * hx ) {
      if( g_px < xLo + hx ) g_px = xLo + hx; else if( g_px > xHi - hx ) g_px = xHi - hx;
    }
    else
      g_px = ( xLo + xHi ) * 0.5f;
    if( zHi - zLo > 2.0f * hz ) {
      if( g_pz < zLo + hz ) g_pz = zLo + hz; else if( g_pz > zHi - hz ) g_pz = zHi - hz;
    }
    else
      g_pz = ( zLo + zHi ) * 0.5f;
  }
  inline bool FullArea( const TMapInfo& m,
                        float& x1, float& z1, float& x2, float& z2 ) {
    if( !g_fullCenter ) return false;
    float ax1, az1, ax2, az2;
    if( g_landReady ) {
      ax1 = g_landX1;
      az1 = g_landZ1;
      ax2 = g_landX2;
      az2 = g_landZ2;
    }
    else if( !WalkArea( ax1, az1, ax2, az2 ) )
      return false;
    const float xLo = ( m.x1 < m.x2 ) ? m.x1 : m.x2;
    const float xHi = ( m.x1 < m.x2 ) ? m.x2 : m.x1;
    const float zLo = ( m.y1 < m.y2 ) ? m.y1 : m.y2;
    const float zHi = ( m.y1 < m.y2 ) ? m.y2 : m.y1;
    if( ax1 < xLo ) ax1 = xLo;
    if( ax2 > xHi ) ax2 = xHi;
    if( az1 < zLo ) az1 = zLo;
    if( az2 > zHi ) az2 = zHi;
    if( ax2 - ax1 < 1000.0f || az2 - az1 < 1000.0f ) return false;
    float padX = ( ax2 - ax1 ) * 0.06f;
    if( padX > ax1 - xLo ) padX = ax1 - xLo;
    if( padX > xHi - ax2 ) padX = xHi - ax2;
    if( padX < 0.0f )      padX = 0.0f;
    float padZ = ( az2 - az1 ) * 0.06f;
    if( padZ > az1 - zLo ) padZ = az1 - zLo;
    if( padZ > zHi - az2 ) padZ = zHi - az2;
    if( padZ < 0.0f )      padZ = 0.0f;
    ax1 -= padX;
    ax2 += padX;
    az1 -= padZ;
    az2 += padZ;
    x1 = ax1;
    z1 = az1;
    x2 = ax2;
    z2 = az2;
    return true;
  }
  inline void SetupMapping( int x, int y, int w, int h ) {
    g_cx  = (float)x + (float)w * 0.5f;
    g_cy  = (float)y + (float)h * 0.5f;
    float halfX, halfZ;
    VisibleHalf( w, h, halfX, halfZ );
    const zVEC3 p = player->GetPositionWorld();
    g_px = p[0];
    g_pz = p[2];
    if( g_full && g_mapIdx >= 0 && g_maps[g_mapIdx].hasCoords ) {
      const TMapInfo& m = g_maps[g_mapIdx];
      float midX = ( m.x1 + m.x2 ) * 0.5f;
      float midZ = ( m.y1 + m.y2 ) * 0.5f;
      {
        float ax1, az1, ax2, az2;
        if( FullArea( m, ax1, az1, ax2, az2 ) ) {
          midX = ( ax1 + ax2 ) * 0.5f;
          midZ = ( az1 + az2 ) * 0.5f;
        }
      }
      if( g_fullPct >= 100 ) {
        g_px = midX;
        g_pz = midZ;
      }
      else {
        if( !g_fullCamSet ) {
          g_fullCamX   = g_px;
          g_fullCamZ   = g_pz;
          g_fullCamSet = true;
        }
        g_px = g_fullCamX;
        g_pz = g_fullCamZ;
        ClampToWorld( m, halfX, halfZ );
        g_fullCamX = g_px;
        g_fullCamZ = g_pz;
      }
    }
    g_fullMapped = g_full;
    g_ppuX = ( halfX > 0.0001f ) ? ( (float)w * 0.5f / halfX ) : 0.1f;
    g_ppuY = ( halfZ > 0.0001f ) ? ( (float)h * 0.5f / halfZ ) : 0.1f;
    UpdateHeading();
  }
  inline void WorldToPx( float wx, float wz, float& px, float& py ) {
    const float dx = wx - g_px;
    const float dz = wz - g_pz;
    const float s  = g_mirror ? -1.0f : 1.0f;
    px = g_cx + s * ( dx * g_hz - dz * g_hx ) * g_ppuX;
    py = g_cy -     ( dx * g_hx + dz * g_hz ) * g_ppuY;
  }
  inline void PxToWorld( float px, float py, float& wx, float& wz ) {
    const float s = g_mirror ? -1.0f : 1.0f;
    const float u = s * ( px - g_cx ) / g_ppuX;
    const float v =     ( g_cy - py ) / g_ppuY;
    wx = g_px + u * g_hz + v * g_hx;
    wz = g_pz - u * g_hx + v * g_hz;
  }
  inline void DirToPx( float wx, float wz, float& sx, float& sy ) {
    const float s = g_mirror ? -1.0f : 1.0f;
    sx =  s * ( wx * g_hz - wz * g_hx );
    sy = -    ( wx * g_hx + wz * g_hz );
  }
  inline float DiscR() {
    const float rx = ( g_clipX1 - g_clipX0 ) * 0.5f;
    const float ry = ( g_clipY1 - g_clipY0 ) * 0.5f;
    return ( rx < ry ) ? rx : ry;
  }
  inline bool InDisc( float px, float py ) {
    if( !g_shape ) return true;
    const float r  = DiscR();
    const float dx = px - g_cx;
    const float dy = py - g_cy;
    return dx * dx + dy * dy <= r * r;
  }
  inline bool ClipDisc( float& x0, float& y0, float& x1, float& y1 ) {
    const float r  = DiscR();
    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const float fx = x0 - g_cx;
    const float fy = y0 - g_cy;
    const float a  = dx * dx + dy * dy;
    const float b  = 2.0f * ( fx * dx + fy * dy );
    const float c  = fx * fx + fy * fy - r * r;
    if( a < 0.000001f ) return c <= 0.0f;
    const float d = b * b - 4.0f * a * c;
    if( d < 0.0f ) return false;
    const float sq = sqrtf( d );
    float t0 = ( -b - sq ) / ( 2.0f * a );
    float t1 = ( -b + sq ) / ( 2.0f * a );
    if( t0 < 0.0f ) t0 = 0.0f;
    if( t1 > 1.0f ) t1 = 1.0f;
    if( t1 <= t0 ) return false;
    const float ax = x0 + t0 * dx;
    const float ay = y0 + t0 * dy;
    const float bx = x0 + t1 * dx;
    const float by = y0 + t1 * dy;
    x0 = ax; y0 = ay; x1 = bx; y1 = by;
    return true;
  }
  inline bool ScriptFloat( const char* name, float& out ) {
    if( !parser || !name ) return false;
    zCPar_Symbol* const s = parser->GetSymbol( zSTRING( name ) );
    if( !s ) return false;
    int raw = 0;
    s->GetValue( raw, 0 );
    out = *(float*)&raw;
    return true;
  }
  inline bool ScriptRotate90() {
    zCPar_Symbol* const s = parser ? parser->GetSymbol( zSTRING( "SPRITEMAP_ROTATE90" ) ) : 0;
    if( !s ) return false;
    int raw = 0;
    s->GetValue( raw, 0 );
    return raw != 0;
  }
  enum { CROP_BINS = 1024 };
  inline void CropRectToLand( oCWorld* world, TMapInfo& m ) {
    const zCMesh* const mesh = world ? world->bspTree.mesh : 0;
    if( !mesh || mesh->numPoly < 1000 || !mesh->polyList ) return;
    const float bx1 = m.x1, bx2 = m.x2;
    const float bz1 = m.y2, bz2 = m.y1;
    if( bx2 - bx1 < 1000.0f || bz2 - bz1 < 1000.0f ) return;
    static int hx[CROP_BINS], hz[CROP_BINS];
    for( int i = 0; i < CROP_BINS; i++ ) { hx[i] = 0; hz[i] = 0; }
    const float kx = (float)CROP_BINS / ( bx2 - bx1 );
    const float kz = (float)CROP_BINS / ( bz2 - bz1 );
    int cnt = 0;
    for( int p = 0; p < mesh->numPoly; p++ ) {
      const zCPolygon* const poly = mesh->polyList[p];
      if( !poly || !poly->vertex || poly->polyNumVert < 3 ) continue;
      float sx = 0.0f, sz = 0.0f;
      for( int v = 0; v < poly->polyNumVert; v++ ) {
        const zCVertex* const vx = poly->vertex[v];
        if( !vx ) { sx = -1.0e30f; break; }
        sx += vx->position[0];
        sz += vx->position[2];
      }
      if( sx < -1.0e29f ) continue;
      sx /= (float)poly->polyNumVert;
      sz /= (float)poly->polyNumVert;
      int ix = (int)( ( sx - bx1 ) * kx ), iz = (int)( ( sz - bz1 ) * kz );
      if( ix < 0 ) ix = 0;
      if( ix >= CROP_BINS ) ix = CROP_BINS - 1;
      if( iz < 0 ) iz = 0;
      if( iz >= CROP_BINS ) iz = CROP_BINS - 1;
      hx[ix]++;
      hz[iz]++;
      cnt++;
    }
    if( cnt < 1000 ) return;
    const int cut = cnt / 1000;
    int lx, rx, lz, rz;
    TrimHistRange( hx, CROP_BINS, cut, lx, rx );
    TrimHistRange( hz, CROP_BINS, cut, lz, rz );
    if( rx <= lx || rz <= lz ) return;
    float x1 = bx1 + (float)lx / kx, x2 = bx1 + (float)( rx + 1 ) / kx;
    float z1 = bz1 + (float)lz / kz, z2 = bz1 + (float)( rz + 1 ) / kz;
    if( world->wayNet )
      for( zCListSort<zCWaypoint>* node = &world->wayNet->wplist; node; node = node->next ) {
        zCWaypoint* const wp = node->data;
        if( !wp ) continue;
        const float wx = wp->pos[0], wz = wp->pos[2];
        if( wx < x1 ) x1 = wx;
        if( wx > x2 ) x2 = wx;
        if( wz < z1 ) z1 = wz;
        if( wz > z2 ) z2 = wz;
      }
    float mx = ( x2 - x1 ) * 0.03f, mz = ( z2 - z1 ) * 0.03f;
    if( mx < 5000.0f ) mx = 5000.0f;
    if( mz < 5000.0f ) mz = 5000.0f;
    float cx1 = x1 - mx, cx2 = x2 + mx, cz1 = z1 - mz, cz2 = z2 + mz;
    if( cx1 < bx1 ) cx1 = bx1;
    if( cx2 > bx2 ) cx2 = bx2;
    if( cz1 < bz1 ) cz1 = bz1;
    if( cz2 > bz2 ) cz2 = bz2;
    if( cx2 - cx1 < 1000.0f || cz2 - cz1 < 1000.0f ) return;
    const double bsp = (double)( bx2 - bx1 ) * (double)( bz2 - bz1 );
    const double lnd = (double)( cx2 - cx1 ) * (double)( cz2 - cz1 );
    if( bsp < lnd * 4.0 ) return;
    m.x1 = cx1;
    m.y1 = cz2;
    m.x2 = cx2;
    m.y2 = cz1;
    MM_LOG( zSTRING( "world rectangle cropped to land (BSP area " ) + zSTRING( (int)( bsp / lnd ) ) + "x larger): " + zSTRING( (int)cx1 ) + " " + zSTRING( (int)cz2 ) + " " + zSTRING( (int)cx2 ) + " " + zSTRING( (int)cz1 ) );
  }
  inline bool DeriveCoords( oCWorld* world, TMapInfo& m ) {
    if( !world || !world->bspTree.bspRoot ) return false;
    const zTBBox3D& bb = world->bspTree.bspRoot->bbox3D;
    const float minX = bb.mins[0], maxX = bb.maxs[0];
    const float minZ = bb.mins[2], maxZ = bb.maxs[2];
    if( maxX - minX < 100.0f || maxZ - minZ < 100.0f ) return false;
    m.x1 = minX;
    m.y1 = maxZ;
    m.x2 = maxX;
    m.y2 = minZ;
    m.hasCoords = true;
    MM_LOG( zSTRING( "world rectangle from BSP: " ) + zSTRING( (int)minX ) + " " + zSTRING( (int)maxZ ) + " " + zSTRING( (int)maxX ) + " " + zSTRING( (int)minZ ) );
    CropRectToLand( world, m );
    return true;
  }
  inline unsigned int WorldFingerprint( oCWorld* world ) {
    if( !world || !world->bspTree.bspRoot ) return 0;
    const zTBBox3D& bb = world->bspTree.bspRoot->bbox3D;
    const zCMesh*   me = world->bspTree.mesh;
    int vals[9];
    vals[0] = me ? me->numPoly : 0;
    vals[1] = me ? me->numVert : 0;
    vals[2] = world->bspTree.numNodes;
    vals[3] = world->bspTree.numLeafs;
    vals[4] = world->bspTree.numPolys;
    vals[5] = (int)bb.mins[0];
    vals[6] = (int)bb.mins[2];
    vals[7] = (int)bb.maxs[0];
    vals[8] = (int)bb.maxs[2];
    const unsigned int h = FnvHashInts( vals, 9 );
    return h ? h : 1u;
  }
  enum {
    SURF_TERRAIN = 0,
    SURF_WATER,
    SURF_SNOW,
    SURF_BUILDING,
    SURF_PATH,
    SURF_GRASS,
    SURF_FOREST,
    SURF_ROCK,
    SURF_PLANT,
    SURF_ROOF,
    SURF_STONEWORK,
    SURF_FIELD,
    SURF_SWAMP,
    SURF_BEACH,
    SURF_CAVE
  };
  static const unsigned char kTxSurf[15] = {
    SURF_WATER, SURF_SNOW, SURF_ROOF, SURF_STONEWORK, SURF_BUILDING, SURF_FIELD,
    SURF_PATH, SURF_GRASS, SURF_FOREST, SURF_ROCK, SURF_PLANT, SURF_SWAMP,
    SURF_BEACH, SURF_CAVE, SURF_TERRAIN
  };
  inline int TxSurfOf( const char* name ) {
    const int k = TextureClassTxt( name );
    return ( k >= 0 && k < 15 ) ? (int)kTxSurf[k] : -1;
  }
  const unsigned char SURF_SKIP = 255;
  inline bool GenStartsUp( const char* s, const char* w ) { return MiniMapCfg::StartsNoCase( s, w ); }
  inline bool GenG1PathName( const char* s ) {
    if( !s || !s[0] ) return false;
    const char* b = SkipG1Prefix( s );
    for( int z = 2; z <= 3; z++ ) {
      for( int k = 0; k < z; k++ )
        if( !IsAsciiAlpha( b[k] ) ) return false;
      const char* c = b + z;
      if( !c[0] || !c[1] || !c[2] || !c[3] ) return false;
      const bool od = ( c[0] == 'O' || c[0] == 'I' || c[0] == 'o' || c[0] == 'i' )
                   && ( c[1] == 'D' || c[1] == 'd' );
      const bool pa = ( c[2] == 'P' || c[2] == 'p' ) && ( c[3] == 'A' || c[3] == 'a' );
      if( od && pa ) return GenHasWord( c + 4, "GRAS" ) && !GenHasWord( c + 4, "ENDE" );
    }
    return false;
  }
  inline bool GenClassNameG1( const char* s, unsigned char& cls ) {
    if( !s || !s[0] ) return false;
    if( ( s[0] == 'P' || s[0] == 'p' ) && ( s[1] == ':' || ( s[1] && s[2] == ':' ) ) ) return false;
    const char* b = SkipG1Prefix( s );
    const bool pOM  = ( b[0] == 'O' && b[1] == 'M' );
    const bool pOT  = ( b[0] == 'O' && b[1] == 'T' );
    const bool pDT  = ( b[0] == 'D' && b[1] == 'T' );
    const bool pFMC = ( b[0] == 'F' && b[1] == 'M' && b[2] == 'C' );
    {
      static const char* const kZone[] = {
        "OW", "OC", "NC", "PC", "OM", "OT", "ORK", "DT", "FMC"
      };
      bool zone = GenStartsUp( b, "PSI_" ) || GenStartsUp( b, "OLD_PSI_" )
               || GenStartsUp( b, "TEMPLE_OT" );
      for( int z = 0; z < 9 && !zone; z++ ) {
        if( !GenStartsUp( b, kZone[z] ) ) continue;
        int k = (int)strlen( kZone[z] );
        const char c = b[k];
        zone = ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' );
      }
      if( !zone ) return false;
    }
    if( GenHasAnyWord( b, "WATFAST|WATSTAND|WATSTOP|FLWAT|WFALL" ) ) {
      cls = SURF_WATER;  return true;
    }
    if( GenHasWord( b, "LAKESIDE" ) ) {
      cls = SURF_BEACH;  return true;
    }
    if( GenHasAnyWord( b, "BAUM|EFEU|RANKEN|TREE" ) ) {
      cls = SURF_PLANT;  return true;
    }
    if( pOM || GenHasAnyWord( b, "WAGREY|FLGREY|WABROWN|WAGREEN|SKULL|MOUNTAI" ) ) {
      cls = SURF_ROCK;  return true;
    }
    if( pOT || pDT || GenHasAnyWord( b, "TILE|KACHEL|MARBEL|SITAL|RELIEF|ORNAMENT|SYMBOL|TEMPLE|TEMPEL|SAEULE|WATOR|GATE|STAIR|FLOO" ) ) {
      cls = SURF_STONEWORK;  return true;
    }
    if( pFMC || GenHasAnyWord( b, "WAND|WOOD|LATTEN|BALKEN|RING|WEHRGANG|HUT|PANEL|STEG|WACHPOSTEN|MATTE|FELL|TABLE|RAHMEN|SEIL|TUCH|TEXTIL|DOOR" ) ) {
      cls = SURF_BUILDING;  return true;
    }
    if( GenHasWord( b, "SIFFGRA" ) ) {
      cls = SURF_SWAMP;  return true;
    }
    if( GenHasWord( b, "DARKDIR" ) ) {
      cls = SURF_PATH;  return true;
    }
    if( GenHasWord( b, "ENDEGR" ) ) {
      cls = SURF_GRASS;  return true;
    }
    return false;
  }
  inline unsigned char GenClassName( const char* s ) {
    if( !s || !s[0] ) return SURF_TERRAIN;
    if( g_genTexColors ) {
      if( GenG1PathName( s ) ) return SURF_PATH;
      if( GenHasAnyWord( s, "SURROUNDING|CRYSTAL|KRISTALL|BOULDER" ) ) return SURF_ROCK;
      if( GenHasWord( s, "ICE", true, true ) || GenHasAnyWord( s, "FROST|GLACIER" ) ) return SURF_SNOW;
      if( GenHasWord( s, "SCHLAMM" ) || GenHasWord( s, "BOG", true, true )
          || GenHasAnyWord( s, "DUCKWEED|SEEROSE" ) )
        return SURF_SWAMP;
      if( GenHasAnyWord( s, "STRAW|STROH|BAMBOO|BAMBUS|THATCH" ) ) return SURF_ROOF;
      if( GenHasWord( s, "PUTZ" ) || GenHasWord( s, "TILE", true )
          || GenHasAnyWord( s, "FLIESE|PLASTER" ) )
        return SURF_STONEWORK;
      if( GenHasWord( s, "TREE", true ) || GenHasAnyWord( s, "TREETOP|BRANCH" )
          || GenHasAnyWord( s, "BUSH|FARN|FERN|PALM|PINE" , true ) || GenHasWord( s, "LIANA" )
          || GenHasWord( s, "IVY", true, true ) )
        return SURF_PLANT;
      if( GenHasAnyWord( s, "CLAY|LEHM" ) ) return SURF_PATH;
      if( GenHasWord( s, "WOODGROUND" ) ) return SURF_FOREST;
      if( GenHasWord( s, "DARKWOOD_BODEN" ) ) return SURF_FOREST;
      if( GenHasAnyWord( s, "SEQ_GROUND_01|SEQ_GROUND_02" ) ) return SURF_GRASS;
      if( GenHasAnyWord( s, "MAYA_GROUND_03|MAYA_GROUND_GREEN" ) ) return SURF_STONEWORK;
      if( GenHasAnyWord( s, "DRAGON_GROUND|VOLCANO_GROUND" ) ) return SURF_ROCK;
    }
    if( g_genTexColors && GenHasWord( s, "UNDERWATER" ) ) {
      char rest[64];
      int n = 0;
      for( int i = 0; s[i] && n < 63; ) {
        if( GenStartsUp( s + i, "UNDERWATER" ) ) { i += 10; continue; }
        rest[n++] = s[i++];
      }
      rest[n] = 0;
      const unsigned char r = GenClassName( rest );
      return ( r == SURF_WATER ) ? SURF_TERRAIN : r;
    }
    if( GenHasAnyWord( s, "WATER|WASSER" ) ) return SURF_WATER;
    if( GenHasAnyWord( s, "SNOW|SCHNEE" ) ) return SURF_SNOW;
    if( GenHasAnyWord( s, "SWAMP|SUMPF" )
        || GenHasAnyWord( s, "MOOR|MARSH" , true ) )
      return SURF_SWAMP;
    if( GenHasAnyWord( s, "BEACH|STRAND|SHORE" ) ) return SURF_BEACH;
    if( GenHasAnyWord( s, "CAVE|HOEHLE|DUNGEON" )
        || GenHasWord( s, "MINE", true ) || GenHasWord( s, "CRYPT" ) )
      return SURF_CAVE;
    if( GenHasAnyWord( s, "ROOF|DACH|ZIEGEL|SHINGLE|SCHINDEL" ) ) return SURF_ROOF;
    if( GenHasAnyWord( s, "COBBLE|PFLASTER|STAIRS|TREPPE|HARBOUR|HARBOR|HAFEN|PIER|BRIDGE|BRUECKE|MARBLE|GRANIT|IRON|EISEN|WALL|MAUER" ) ) return SURF_STONEWORK;
    if( GenHasAnyWord( s, "HOUSE|HAUS|CASTLE|MONASTERY|CHURCH|TOWER|TURM|BRICK|PLANK|HOLZ|TIMBER|FACHWERK" ) || GenHasWord( s, "WOOD", true )
        || GenHasAnyWord( s, "BRETT|ROPEWAY|BRIDGE" )
        || GenHasAnyWord( s, "HUT|CITY|TOWN" , true ) )
      return SURF_BUILDING;
    if( GenHasAnyWord( s, "ACKER|FIELD|FARM|CROP|GETREIDE" ) ) return SURF_FIELD;
    if( GenHasAnyWord( s, "FOREST|WALD|WBODEN|LEAV|LAUB|NEEDLE|BARK|RINDE" ) ) return SURF_FOREST;
    if( GenHasAnyWord( s, "MEADOW|GRASS|WIESE|MOSS|GRAS" ) ) return SURF_GRASS;
    if( GenHasAnyWord( s, "ROAD|PATH|TRAIL|PFAD|STEIG|TRAMPEL|LIGHTGROUND|MUD|DIRT|SAND|GRAVEL|KIES|SCHOTTER|GROUND|BODEN|WASTELAND|ERDE" )
        || GenHasWord( s, "WEG", true ) )
      return SURF_PATH;
    if( GenHasAnyWord( s, "ROCK|FELS|STONE|STEIN|CLIFF|GRANIT|MARBLE|MOUNTAIN|VOLCANO|LAVA" ) ) return SURF_ROCK;
    if( g_genTexColors ) {
      unsigned char g1 = SURF_TERRAIN;
      if( GenClassNameG1( s, g1 ) ) return g1;
    }
    return SURF_TERRAIN;
  }
  inline unsigned char GenClassVisual( const char* s ) {
    if( !s || !s[0] ) return SURF_BUILDING;
    if( g_genTexColors && GenHasWord( s, "FIRE", true ) ) return SURF_BUILDING;
    if( g_genTreeShapes != 0
        && ( GenHasAnyWord( s, "TREE|TREES" , false, true )
             || GenHasAnyWord( s, "FARN|LIANA|LIANE|WHEAT|VINEYARD" ) ) )
      return SURF_PLANT;
    if( GenHasAnyWord( s, "BAUM|BUSH|BUSCH|PLANT|FERN|STRAUCH|GRASS|FLOWER|BLUME|SHRUB|HECKE|HEDGE|BIRCH|BIRKE|TANNE|EICHE|WEED|REED|SCHILF|MUSHROOM|PILZ" )
        || GenHasAnyWord( s, "TREE|IVY|FIR|OAK|PINE|PALM" , true ) )
      return SURF_PLANT;
    if( GenHasAnyWord( s, "STONE|ROCK|FELS|CLIFF|MOUNTAIN" ) ) return SURF_ROCK;
    return SURF_BUILDING;
  }
  inline float GenNoise( int x, int y ) {
    unsigned int h = (unsigned int)x * 374761393u + (unsigned int)y * 668265263u;
    h = ( h ^ ( h >> 13 ) ) * 1274126177u;
    h ^= h >> 16;
    return (float)( h & 255 ) / 255.0f - 0.5f;
  }
  static const void*   g_surfKey[4096] = { 0 };
  static unsigned char g_surfVal[4096] = { 0 };
  static int           g_texPalCount   = 1;
  static char g_surfUnk[16][40];
  static int  g_surfUnkCount = 0;
  inline bool SurfUnkLogOnce( const char* name ) {
    static char seen[64][40];
    static int  n = 0;
    for( int i = 0; i < n; i++ )
      if( StrEqUp( seen[i], name ) ) return false;
    if( n >= 64 ) return false;
    StrCopy( seen[n++], name, 40 );
    return true;
  }
  const int SURF_COUNT = 15;
  static char g_surfFirst[15][40];
  static int  g_surfHits[15];
  inline bool GenWaterGroupIsLand( const char* name ) {
    {
      const int t = TxSurfOf( name );
      if( t >= 0 ) return t != SURF_WATER;
    }
    if( !g_genTexColors || !name || !name[0] ) return false;
    if( GenHasAnyWord( name, "WAT|WASS|SEA|SWAMP|SUMPF|FALL" )
        || !GenHasWord( name, "GRAS" ) )
      return false;
    const char* b = SkipG1Prefix( name );
    for( int z = 2; z <= 3; z++ ) {
      for( int k = 0; k < z; k++ )
        if( !IsAsciiAlpha( b[k] ) ) return false;
      const char* c = b + z;
      if( !c[0] || !c[1] || !c[2] || !c[3] ) return false;
      const bool od = ( c[0] == 'O' || c[0] == 'I' || c[0] == 'o' || c[0] == 'i' )
                   && ( c[1] == 'D' || c[1] == 'd' );
      if( od ) return GenStartsUp( c + 2, "FL" ) || GenStartsUp( c + 2, "WA" );
    }
    return false;
  }
  inline unsigned char GenSurfCached( const void* key, const char* name,
                                      bool isVob ) {
    if( !key ) return isVob ? SURF_BUILDING : SURF_TERRAIN;
    static int s_tex = -1;
    static unsigned int s_txs = 0;
    const int texKey = g_genTexColors | ( ( g_genTreeShapes != 0 ) ? 16 : 0 );
    if( s_tex != texKey || s_txs != g_txSig ) {
      s_tex = texKey;
      s_txs = g_txSig;
      memset( g_surfKey, 0, (size_t)( 4096 ) * sizeof( g_surfKey[0] ) );
    }
    const int slot = (int)( ( ( (unsigned int)key ) >> 4 ) & 4095 );
    if( g_surfKey[slot] == key ) return g_surfVal[slot];
    const int txc = isVob ? -1 : TxSurfOf( name );
    const unsigned char cls = ( txc >= 0 ) ? (unsigned char)txc
                            : isVob ? GenClassVisual( name ) : GenClassName( name );
    g_surfKey[slot] = key;
    g_surfVal[slot] = cls;
    if( cls < SURF_COUNT && name && name[0] && !g_surfFirst[cls][0] )
      StrCopy( g_surfFirst[cls], name, 40 );
    if( !isVob && cls == SURF_TERRAIN && name && name[0]
        && g_surfUnkCount < 16 ) {
      bool seen = false;
      for( int u = 0; u < g_surfUnkCount && !seen; u++ )
        seen = StrEqUp( g_surfUnk[u], name );
      if( !seen )
        StrCopy( g_surfUnk[g_surfUnkCount++], name, 40 );
    }
    return cls;
  }
  inline void GenSurfReset() {
    memset( g_surfKey, 0, (size_t)( 4096 ) * sizeof( g_surfKey[0] ) );
    memset( g_surfVal, 0, (size_t)( 4096 ) * sizeof( g_surfVal[0] ) );
    g_surfUnkCount = 0;
    for( int c = 0; c < SURF_COUNT; c++ ) {
      g_surfFirst[c][0] = 0;
      g_surfHits[c]     = 0;
    }
    g_texPalCount = 1;
  }
  struct TTexPal {
    unsigned char cls, key[3];
    float nr, ng, nb;
  };
  static TTexPal g_texPal[256];
  inline unsigned char GenTexPalIndex( unsigned char cls, int r, int g, int b ) {
    const int lum = ( r * 3 + g * 6 + b ) / 10;
    if( lum < 12 || ( r >= 245 && g >= 245 && b >= 245 ) || cls >= SURF_COUNT ) return 0;
    const unsigned char kr = (unsigned char)( r >> 3 ), kg = (unsigned char)( g >> 3 ),
                        kb = (unsigned char)( b >> 3 );
    for( int i = 1; i < g_texPalCount; i++ ) {
      const TTexPal& e = g_texPal[i];
      if( e.cls == cls && e.key[0] == kr && e.key[1] == kg && e.key[2] == kb ) return (unsigned char)i;
    }
    if( g_texPalCount < 256 ) {
      TTexPal& e = g_texPal[g_texPalCount];
      e.cls    = cls;
      e.key[0] = kr;  e.key[1] = kg;  e.key[2] = kb;
      const float L  = 0.299f * (float)r + 0.587f * (float)g + 0.114f * (float)b;
      float       f  = (float)pow( (double)( L / 78.0f ), 0.35 );
      if( f < 0.78f ) f = 0.78f; else if( f > 1.25f ) f = 1.25f;
      e.nr = (float)r / L * f;
      e.ng = (float)g / L * f;
      e.nb = (float)b / L * f;
      return (unsigned char)( g_texPalCount++ );
    }
    int best = 0, bestD = 0x7FFFFFFF;
    for( int i = 1; i < g_texPalCount; i++ ) {
      const TTexPal& e = g_texPal[i];
      if( e.cls != cls ) continue;
      const int dr = e.key[0] - kr, dg = e.key[1] - kg, db = e.key[2] - kb;
      const int d  = dr * dr + dg * dg + db * db;
      if( d < bestD ) { bestD = d; best = i; }
    }
    return (unsigned char)best;
  }
  inline unsigned char GenTexOfMaterial( const zCMaterial* mtl, unsigned char cls ) {
    if( !g_genTexColors || !mtl ) return 0;
    const zCOLOR c = mtl->color;
    return GenTexPalIndex( cls, (int)c.r, (int)c.g, (int)c.b );
  }
  inline void GenRamp( float t, float& r, float& g, float& b ) {
    static const float stop[6][3] = {
      {  62.0f,  84.0f,  56.0f },
      {  92.0f, 114.0f,  68.0f },
      { 132.0f, 138.0f,  86.0f },
      { 146.0f, 124.0f,  92.0f },
      { 158.0f, 152.0f, 144.0f },
      { 236.0f, 236.0f, 234.0f }
    };
    if( t < 0.0f ) t = 0.0f; else if( t > 1.0f ) t = 1.0f;
    const float f = t * 5.0f;
    int i = (int)f;
    if( i > 4 ) i = 4;
    const float k = f - (float)i;
    r = stop[i][0] + ( stop[i + 1][0] - stop[i][0] ) * k;
    g = stop[i][1] + ( stop[i + 1][1] - stop[i][1] ) * k;
    b = stop[i][2] + ( stop[i + 1][2] - stop[i][2] ) * k;
  }
  inline void GenVividPx( float& r, float& g, float& b ) {
    const float sat   = (float)g_genVivid  / 100.0f;
    const float bri   = (float)g_genBright / 100.0f;
    const float lum   = 0.299f * r + 0.587f * g + 0.114f * b;
    r = ( lum + ( r - lum ) * sat ) * bri;
    g = ( lum + ( g - lum ) * sat ) * bri;
    b = ( lum + ( b - lum ) * sat ) * bri;
    if( r < 0.0f ) r = 0.0f;  if( r > 255.0f ) r = 255.0f;
    if( g < 0.0f ) g = 0.0f;  if( g > 255.0f ) g = 255.0f;
    if( b < 0.0f ) b = 0.0f;  if( b > 255.0f ) b = 255.0f;
  }
  inline float GenDither( int x, int y ) {
    static const int m[4][4] = {
      {  0,  8,  2, 10 },
      { 12,  4, 14,  6 },
      {  3, 11,  1,  9 },
      { 15,  7, 13,  5 }
    };
    return ( (float)m[y & 3][x & 3] - 7.5f ) / 16.0f;
  }
  inline unsigned short GenPack16( unsigned long c, int fmt, float dither ) {
    const unsigned int a = ( c >> 24 ) & 0xFF;
    const float step = ( fmt == (int)zRND_TEX_FORMAT_ARGB_4444 ) ? 16.0f : 8.0f;
    const float d    = dither * step;
    float rf = (float)( ( c >> 16 ) & 0xFF ) + d;
    float gf = (float)( ( c >>  8 ) & 0xFF ) + d;
    float bf = (float)(   c         & 0xFF ) + d;
    if( rf < 0.0f ) rf = 0.0f;  if( rf > 255.0f ) rf = 255.0f;
    if( gf < 0.0f ) gf = 0.0f;  if( gf > 255.0f ) gf = 255.0f;
    if( bf < 0.0f ) bf = 0.0f;  if( bf > 255.0f ) bf = 255.0f;
    const unsigned int r = (unsigned int)rf;
    const unsigned int g = (unsigned int)gf;
    const unsigned int b = (unsigned int)bf;
    if( fmt == (int)zRND_TEX_FORMAT_ARGB_1555 )
      return (unsigned short)( ( a >= 128 ? 0x8000 : 0 )
                             | ( ( r >> 3 ) << 10 )
                             | ( ( g >> 3 ) << 5 )
                             |   ( b >> 3 ) );
    if( fmt == (int)zRND_TEX_FORMAT_RGB_565 )
      return (unsigned short)( ( ( r >> 3 ) << 11 )
                             | ( ( g >> 2 ) << 5 )
                             |   ( b >> 3 ) );
    return (unsigned short)( ( ( a >> 4 ) << 12 )
                           | ( ( r >> 4 ) << 8 )
                           | ( ( g >> 4 ) << 4 )
                           |   ( b >> 4 ) );
  }
  inline void FreeGenTexture() {
    if( !g_genTex ) return;
    if( g_blit && g_blit->Texture == g_genTex ) g_blit->Texture = 0;
    g_genTex->Release();
    g_genTex      = 0;
    g_genTexSize  = 0;
    g_genSizeUsed = 0;
    g_genFmt      = -1;
    g_genBpp      = 0;
    g_genWorld[0] = 0;
    g_texReady    = false;
    g_texTried    = false;
  }
  inline void DeferGenTexture() {
    if( !g_genTex ) return;
    const int keepUsed = g_genSizeUsed, keepFmt = g_genFmt, keepBpp = g_genBpp;
    FreeGenTexture();
    g_genSizeUsed = keepUsed;
    g_genFmt      = keepFmt;
    g_genBpp      = keepBpp;
  }
  inline int MaxTexSize() {
    if( !zrenderer ) return 0;
    const int mx = zrenderer->GetMaxTextureSize();
    return ( mx > 0 ) ? mx : 0;
  }
  inline bool ProcLowMem() {
    static int s_low = -1;
    if( s_low < 0 ) {
      MEMORYSTATUSEX ms = { sizeof( ms ) };
      const BOOL memOk = GlobalMemoryStatusEx( &ms );
      s_low = ( memOk && ms.ullTotalVirtual <= ( (unsigned __int64)2100 << 20 ) ) ? 1 : 0;
      const int mb = memOk ? (int)( ms.ullTotalVirtual >> 20 ) : 0;
      MM_LOG( zSTRING( "process memory: " ) + zSTRING( mb ) + zSTRING( s_low ? " MB (no LAA) - detail tiles up to 2048" : " MB" ) );
      SYSTEM_INFO si;
      GetSystemInfo( &si );
      DISPLAY_DEVICEA dd;
      dd.cb = sizeof( dd );
      char gpu[256] = "";
      char seen[4][128];
      int  nSeen = 0;
      for( DWORD i = 0; i < 16 && EnumDisplayDevicesA( 0, i, &dd, 0 ); i++ ) {
        if( !dd.DeviceString[0] || ( dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER ) ) continue;
        bool dup = false;
        for( int k = 0; k < nSeen && !dup; k++ )
          dup = StrEqUp( seen[k], dd.DeviceString );
        if( dup || nSeen >= 4 ) continue;
        StrCopy( seen[nSeen++], dd.DeviceString, 128 );
        const size_t at = strlen( gpu );
        snprintf( gpu + at, sizeof( gpu ) - at, "%s%s%s", at ? "; " : "",
                  ( dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE ) ? "*" : "", dd.DeviceString );
      }
      if( !gpu[0] )
        StrCopy( gpu, "unknown", 256 );
      const int ramMb = memOk ? (int)( ms.ullTotalPhys >> 20 ) : 0;
      MM_LOG( zSTRING( "hardware: graphics " ) + zSTRING( gpu ) + ", max texture " + zSTRING( MaxTexSize() ) + ", RAM " + zSTRING( ramMb ) + " MB, CPU threads " + zSTRING( (int)si.dwNumberOfProcessors ) );
    }
    return s_low == 1;
  }
  inline bool PickTexFormat( bool alphaOnly, zTRnd_TextureFormat& fmt, int& bpp ) {
    if( !zrenderer ) return false;
    static const zTRnd_TextureFormat kAlpha[] = { zRND_TEX_FORMAT_ARGB_8888, zRND_TEX_FORMAT_ARGB_4444, zRND_TEX_FORMAT_ARGB_1555 };
    static const zTRnd_TextureFormat kMap[]   = { zRND_TEX_FORMAT_ARGB_1555, zRND_TEX_FORMAT_ARGB_4444, zRND_TEX_FORMAT_ARGB_8888, zRND_TEX_FORMAT_BGRA_8888, zRND_TEX_FORMAT_RGB_565 };
    const zTRnd_TextureFormat* const list = alphaOnly ? kAlpha : kMap;
    const int cnt = alphaOnly ? 3 : 5;
    for( int t = 0; t < cnt; t++ ) {
      if( !zrenderer->SupportsTextureFormat( list[t] ) ) continue;
      fmt = list[t];
      bpp = ( fmt == zRND_TEX_FORMAT_ARGB_8888 || fmt == zRND_TEX_FORMAT_BGRA_8888 ) ? 4 : 2;
      return true;
    }
    return false;
  }
  inline zCTexture* CreateTex2D( int w, int h, zTRnd_TextureFormat fmt, const zCOLOR& avg, bool hasAlpha ) {
    if( !zrenderer ) return 0;
    zCTexture* tex = zrenderer->CreateTexture();
    if( !tex ) return 0;
    zCTextureInfo info;
    info.texFormat    = fmt;
    info.sizeX        = w;
    info.sizeY        = h;
    info.numMipMap    = 1;
    info.refSizeX     = w;
    info.refSizeY     = h;
    info.averageColor = avg;
    tex->SetTextureInfo( info );
    if( hasAlpha ) tex->hasAlpha = 1;
    return tex;
  }
  inline zCTexture* GenEnsureTexture( int n ) {
    const int mx = MaxTexSize();
    if( mx > 0 && n > mx ) {
      MM_LOG( zSTRING( "terrain map: graphics card supports at most " ) + zSTRING( mx ) + "x" + zSTRING( mx ) + ", reduced from " + zSTRING( n ) );
      n = mx;
    }
    if( g_genTex && g_genTexSize == n ) return g_genTex;
    if( g_genTex ) FreeGenTexture();
    zTRnd_TextureFormat fmt = zRND_TEX_FORMAT_ARGB_4444;
    int bpp = 0;
    if( !PickTexFormat( false, fmt, bpp ) ) {
      if( zrenderer ) MM_LOG( "terrain map: renderer accepts no known texture format" );
      return 0;
    }
    zCTexture* tex = CreateTex2D( n, n, fmt, zCOLOR( 120, 128, 100, 255 ), false );
    if( !tex ) {
      MM_LOG( "terrain map: renderer gave no texture" );
      return 0;
    }
    g_genTex = tex; g_genTexSize = n; g_genFmt = (int)fmt; g_genBpp = bpp;
    MM_LOG( zSTRING( "terrain map: texture " ) + zSTRING( n ) + "x" + zSTRING( n ) + ", format " + zSTRING( (int)fmt ) + ", " + zSTRING( bpp * 8 ) + " bit" );
    return g_genTex;
  }
  const int GEN_CACHE_MAGIC = 0x4D454743;
  const int GEN_ALGO_VERSION = 2;
  const int GEN_ALGO_VERSION_TEX = 7;
  const int GEN_TREE_VERSION = 3;
  const int GEN_NAT_VERSION = 1;
  const int GEN_PATH_VERSION = 2;
  inline int GenAlgoVersion() {
    if( g_genTexColors == 2 ) return ( GEN_ALGO_VERSION_TEX << 8 ) | GEN_NAT_VERSION;
    return g_genTexColors ? GEN_ALGO_VERSION_TEX : GEN_ALGO_VERSION;
  }
  inline bool GenMatSkip( const char* name ) {
    return g_genTreeShapes != 0 && name && StrEqUp( name, "Z_PORTALMAT" );
  }
  const float TREE_GIANT_W = 3000.0f;
  const float TREE_FLAT_W  = 1500.0f;
  inline bool GenCrownSkip( unsigned char cls ) {
    return g_genTreeShapes == 3 && cls == SURF_PLANT;
  }
  inline bool GenCrownMaterial( const char* s ) {
    if( g_genTreeShapes != 3 || !s || !s[0] ) return false;
    return GenHasWord( s, "TREE", true ) || GenHasAnyWord( s, "TREETOP|BIGTREE|BRANCH" ) || GenHasAnyWord( s, "BUSH|FARN|FERN|PALM|PINE" , true )
        || GenHasWord( s, "LIANA" ) || GenHasWord( s, "IVY", true, true )
        || GenHasAnyWord( s, "BAUM|EFEU|RANKEN|WATREE|WATRTREE|FLTREE" );
  }
  inline bool GenCrownPoly( unsigned char cls, const char* name ) {
    if( g_genTreeShapes != 3 || cls == SURF_WATER || cls == SURF_SNOW || cls == SURF_SKIP ) return false;
    return GenCrownSkip( cls ) || GenCrownMaterial( name );
  }
  const float TREE_TALL_H  = 800.0f;
  const float TREE_NAMED_H = 300.0f;
  inline bool TreeVisualName( const char* s ) {
    if( !s || !s[0] ) return false;
    return GenHasWord( s, "TREE", true ) || GenHasWord( s, "TREE", false, true )
        || GenHasAnyWord( s, "TANNE|BAUM" )
        || GenHasAnyWord( s, "PINE|PALM" , true )
        || GenHasWord( s, "FIR", true, true ) || GenHasWord( s, "OAK", true )
        || GenHasAnyWord( s, "EICHE|BIRCH|BIRKE" );
  }
  inline bool TreeGiantSize( float sx, float sz, float sy, const char* vname = 0 ) {
    if( g_genTreeShapes != 3 ) return false;
    const float big = ( sx > sz ) ? sx : sz;
    if( big >= TREE_GIANT_W ) return true;
    if( sy >= TREE_TALL_H ) return true;
    if( sy >= TREE_NAMED_H && TreeVisualName( vname ) ) return true;
    return big >= TREE_FLAT_W && sy * 2.0f <= big;
  }
  inline void GenCacheName( const char* world, char* out, int max,
                            bool withTag = true ) {
    char shortName[37];
    StrCopy( shortName, world, 37 );
    unsigned int h = 2166136261u;
    for( int i = 0; world[i]; i++ ) {
      h ^= (unsigned int)(unsigned char)UpChar( world[i] );
      h *= 16777619u;
    }
    if( withTag && g_worldTag )
      h ^= g_worldTag;
    snprintf( out, (size_t)max, "MiniMap_gen_%s_%08X.cache", shortName, h );
  }
  inline int GenSignature( const TMapInfo& m ) {
    const int vals[20] = {
      GEN_ALGO_VERSION,
      (int)m.x1, (int)m.y1, (int)m.x2, (int)m.y2,
      g_genVivid, g_genBright, g_genRelief, g_genEdge,
      g_genVobs, g_genVobMin, g_genPlantMin, g_genFill,
      g_genOutside, g_genSmooth, g_genPaths, g_genPathWide,
      g_genPathsWater, g_genContour, g_genContourDark
    };
    unsigned int h = FnvHashInts( vals, 20 );
    if( g_genSculpt != 0 ) {
      h ^= 0x5D000000u | (unsigned int)g_genSculpt;
      h *= 16777619u;
    }
    if( g_genTexColors != 0 ) {
      h ^= 0x7E000000u | (unsigned int)GenAlgoVersion();
      h *= 16777619u;
    }
    if( g_genTreeShapes != 0 ) {
      h ^= 0x3D000000u | ( (unsigned int)GEN_TREE_VERSION << 8 )
         | (unsigned int)g_genTreeShapes;
      h *= 16777619u;
    }
    if( g_genTreeShapes == 3 ) {
      h ^= 0x3E000000u ^ ( (unsigned int)TREE_GIANT_W * 7u ) ^ 500u
         ^ ( (unsigned int)TREE_FLAT_W << 12 ) ^ 0x20000000u ^ 0x00B20000u
         ^ ( (unsigned int)TREE_TALL_H * 3u ) ^ ( (unsigned int)TREE_NAMED_H << 4 );
      h *= 16777619u;
    }
    if( g_genPaths != 0 ) {
      h ^= 0x50000000u | (unsigned int)GEN_PATH_VERSION;
      h *= 16777619u;
    }
    if( g_tcSig ) {
      h ^= g_tcSig;
      h *= 16777619u;
    }
    if( g_txSig ) {
      h ^= g_txSig ^ 0x0E000000u;
      h *= 16777619u;
    }
    return (int)( h & 0x7FFFFFFF );
  }
  inline int GenOptionsKey() {
    TMapInfo z;
    z.x1 = z.y1 = z.x2 = z.y2 = 0.0f;
    unsigned int h = (unsigned int)GenSignature( z );
    const int more[3] = { g_genDetail, g_genPatchSize, g_genIndoor };
    h = FnvHashInts( more, 3, h );
    return (int)( h & 0x7FFFFFFF );
  }
  inline bool GenUploadRect( zCTexture* tex, const unsigned char* px, int w, int h, int bpp ) {
    if( !tex || !px || w <= 0 || h <= 0 ) return false;
    bool ok = false;
    if( tex->Lock( 1 ) ) {
      void* buf = 0; int bytes = 0;
      if( tex->GetTextureBuffer( 0, buf, bytes ) && buf ) {
        int pitch = ( bytes >= w * h * bpp ) ? ( bytes / h ) : bytes;
        if( pitch < w * bpp ) pitch = w * bpp;
        for( int row = 0; row < h; row++ ) memcpy( (char*)buf + row * pitch, px + row * w * bpp, (size_t)( w * bpp ) );
        ok = true;
      }
      tex->Unlock();
    }
    return ok;
  }
  inline bool GenUpload( zCTexture* tex, const unsigned char* px, int n, int bpp ) {
    return GenUploadRect( tex, px, n, n, bpp );
  }
  inline zCTexture* CreateAndUploadTex( int w, int h, zTRnd_TextureFormat fmt, const zCOLOR& avg, const unsigned char* px, int bpp ) {
    zCTexture* tex = CreateTex2D( w, h, fmt, avg, true );
    if( tex && !GenUploadRect( tex, px, w, h, bpp ) ) { tex->Release(); tex = 0; }
    return tex;
  }
  inline bool LandFromPixels( const TMapInfo& m, const unsigned char* px,
                              int n, int bpp ) {
    if( !px || n < 64 ) return false;
    int* wRow = MiniMapCommon::MmAlloc<int>( (size_t)( n ) );
    int* wCol = MiniMapCommon::MmAlloc<int>( (size_t)( n ) );
    if( !wRow || !wCol ) {
      MiniMapCommon::MmFree( wRow );
      MiniMapCommon::MmFree( wCol );
      return false;
    }
    memset( wRow, 0, (size_t)( n ) * sizeof( wRow[0] ) );
    memset( wCol, 0, (size_t)( n ) * sizeof( wCol[0] ) );
    for( int iz = 0; iz < n; iz++ ) {
      for( int ix = 0; ix < n; ix++ ) {
        const int q = iz * n + ix;
        int r, g, b, a = 255;
        if( bpp == 4 ) {
          const unsigned long c = ( (const unsigned long*)px )[q];
          a = (int)( ( c >> 24 ) & 0xFF );
          r = (int)( ( c >> 16 ) & 0xFF );
          g = (int)( ( c >>  8 ) & 0xFF );
          b = (int)(   c         & 0xFF );
        }
        else {
          const unsigned short c = ( (const unsigned short*)px )[q];
          if( g_genFmt == (int)zRND_TEX_FORMAT_ARGB_1555 ) {
            a = ( c & 0x8000 ) ? 255 : 0;
            r = ( ( c >> 10 ) & 0x1F ) << 3;
            g = ( ( c >>  5 ) & 0x1F ) << 3;
            b = (   c         & 0x1F ) << 3;
          }
          else if( g_genFmt == (int)zRND_TEX_FORMAT_RGB_565 ) {
            r = ( ( c >> 11 ) & 0x1F ) << 3;
            g = ( ( c >>  5 ) & 0x3F ) << 2;
            b = (   c         & 0x1F ) << 3;
          }
          else {
            a = ( ( c >> 12 ) & 0x0F ) << 4;
            r = ( ( c >>  8 ) & 0x0F ) << 4;
            g = ( ( c >>  4 ) & 0x0F ) << 4;
            b = (   c         & 0x0F ) << 4;
          }
        }
        if( a < 128 ) continue;
        if( b > r + 24 && b > g + 12 ) continue;
        wRow[iz]++;
        wCol[ix]++;
      }
    }
    int prog = n / 50;
    if( prog < 1 ) prog = 1;
    int r1 = -1, r2 = -1, c1 = -1, c2 = -1;
    for( int i = 0; i < n; i++ ) {
      if( wRow[i] >= prog ) { if( r1 < 0 ) r1 = i; r2 = i; }
      if( wCol[i] >= prog ) { if( c1 < 0 ) c1 = i; c2 = i; }
    }
    MiniMapCommon::MmFree( wRow );
    MiniMapCommon::MmFree( wCol );
    if( r1 < 0 || c1 < 0 || r2 <= r1 || c2 <= c1 ) return false;
    const float sx = ( m.x2 - m.x1 ) / (float)n;
    const float sz = ( m.y2 - m.y1 ) / (float)n;
    float lx1 = m.x1 + sx * (float)c1;
    float lx2 = m.x1 + sx * (float)( c2 + 1 );
    float lz1 = m.y1 + sz * (float)r1;
    float lz2 = m.y1 + sz * (float)( r2 + 1 );
    if( lx1 > lx2 ) { std::swap( lx1, lx2 ); }
    if( lz1 > lz2 ) { std::swap( lz1, lz2 ); }
    g_landX1 = lx1;
    g_landX2 = lx2;
    g_landZ1 = lz1;
    g_landZ2 = lz2;
    g_landReady = true;
    return true;
  }
  inline int GenHPack( float hmin, float hmax ) {
    int a = (int)floor( hmin / 8.0f );
    int b = (int)ceil( hmax / 8.0f );
    if( a < -32768 ) a = -32768; else if( a > 32767 ) a = 32767;
    if( b < -32768 ) b = -32768; else if( b > 32767 ) b = 32767;
    if( b <= a ) return 0;
    return (int)( ( (unsigned int)(unsigned short)(short)b << 16 )
                |   (unsigned int)(unsigned short)(short)a );
  }
  inline bool GenHUnpack( int v, float& hmin, float& hmax ) {
    if( v == 0 ) return false;
    const int a = (int)(short)( (unsigned int)v & 0xFFFF );
    const int b = (int)(short)( ( (unsigned int)v >> 16 ) & 0xFFFF );
    if( b <= a ) return false;
    hmin = (float)a * 8.0f;
    hmax = (float)b * 8.0f;
    return true;
  }
  const int GEN_OLD_DAYS = 30;
  inline bool GenCacheNameIs( const char* name, const char* pre ) {
    int i = (int)strlen( pre );
    if( _strnicmp( name, pre, (size_t)i ) != 0 ) return false;
    for( int j = 0; j < 8; j++, i++ )
      if( !HexChar( name[i] ) ) return false;
    return StrEqUp( name + i, ".cache" );
  }
  inline void GenCachePruneOld( const char* world, const char* keepName ) {
    if( !world || !world[0] || !keepName || !keepName[0] ) return;
    char shortName[37];
    StrCopy( shortName, world, 37 );
    char pre[NAME_MAX], pat[NAME_MAX + 8];
    snprintf( pre, sizeof( pre ), "MiniMap_gen_%s_", shortName );
    snprintf( pat, sizeof( pat ), "%s*.cache", pre );
    char mask[NAME_MAX + 32];
    if( !FilePath( mask, NAME_MAX + 32, FILES_GEN_DIR, pat ) ) return;
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( mask, &fd );
    if( fh == INVALID_HANDLE_VALUE ) return;
    int removed = 0;
    do {
      if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) || StrEqUp( fd.cFileName, keepName ) ) continue;
      if( !GenCacheNameIs( fd.cFileName, pre ) ) continue;
      if( !FileAgeOld( fd.ftLastWriteTime, GEN_OLD_DAYS ) ) continue;
      char f[NAME_MAX + 32];
      if( FilePath( f, NAME_MAX + 32, FILES_GEN_DIR, fd.cFileName ) && DeleteFileA( f ) )
        removed++;
    } while( FindNextFileA( fh, &fd ) );
    FindClose( fh );
    if( removed > 0 )
      MM_LOG( zSTRING( "terrain map: removed maps of this world with another fingerprint (unused for " ) + zSTRING( GEN_OLD_DAYS ) + " days): " + zSTRING( removed ) );
  }
  inline int GenCacheProbe( const char* world, const TMapInfo& m, int bpp ) {
    if( !g_genCache || !world || !world[0] ) return 0;
    char name[NAME_MAX], file[NAME_MAX + 24];
    GenCacheName( world, name, NAME_MAX );
    FilesEnsure();
    FilePath( file, NAME_MAX + 24, FILES_GEN_DIR, name );
    HANDLE h = CreateFileA( file, GENERIC_READ, FILE_SHARE_READ, 0,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) return 0;
    int   head[6] = { 0, 0, 0, 0, 0, 0 };
    DWORD read = 0;
    const bool ok = ReadFile( h, head, sizeof( head ), &read, 0 ) && read == sizeof( head )
                 && head[0] == GEN_CACHE_MAGIC && head[2] == g_genFmt && head[3] == bpp
                 && head[4] == GenSignature( m ) && head[1] >= 256 && head[1] <= 16384;
    CloseHandle( h );
    return ok ? head[1] : 0;
  }
  inline bool GenCacheLoad( const char* world, const TMapInfo& m,
                            int n, int bpp, zCTexture* tex ) {
    g_genHKnown = false;
    if( !g_genCache || !world || !world[0] ) return false;
    char name[NAME_MAX], file[NAME_MAX + 24];
    GenCacheName( world, name, NAME_MAX );
    FilesEnsure();
    FilePath( file, NAME_MAX + 24, FILES_GEN_DIR, name );
    HANDLE h = CreateFileA( file, GENERIC_READ, FILE_SHARE_READ, 0,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    bool legacy = false;
    if( h == INVALID_HANDLE_VALUE && g_worldTag ) {
      char oldName[NAME_MAX], old[NAME_MAX + 24];
      GenCacheName( world, oldName, NAME_MAX, false );
      FilePath( old, NAME_MAX + 24, FILES_GEN_DIR, oldName );
      h = CreateFileA( old, GENERIC_READ, FILE_SHARE_READ, 0,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
      legacy = ( h != INVALID_HANDLE_VALUE );
    }
    if( h == INVALID_HANDLE_VALUE ) return false;
    int   head[6] = { 0, 0, 0, 0, 0, 0 };
    DWORD read = 0;
    const bool headOk = ReadFile( h, head, sizeof( head ), &read, 0 )
                     && read == sizeof( head )
                     && head[0] == GEN_CACHE_MAGIC
                     && head[1] == n
                     && head[2] == g_genFmt
                     && head[3] == bpp
                     && head[4] == GenSignature( m );
    if( !headOk ) {
      CloseHandle( h );
      const char* why = ( read != sizeof( head ) || head[0] != GEN_CACHE_MAGIC ) ? "damaged file"
                      : ( head[1] != n ) ? "different map size"
                      : ( head[2] != g_genFmt || head[3] != bpp ) ? "different texture format"
                      : "different map settings or new plugin version";
      MM_LOG( zSTRING( legacy
                ? "terrain map: old cache is from another mod, recalculating"
                : "terrain map: cache out of date, recalculating" ) + " (" + zSTRING( why ) + ": in file " + zSTRING( head[1] ) + ", needed " + zSTRING( n ) + ")" );
      return false;
    }
    const int      bytes = n * n * bpp;
    unsigned char* px    = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( bytes ) );
    if( !px ) {
      CloseHandle( h );
      return false;
    }
    const DWORD t0 = GetTickCount();
    bool packed = false;
    bool ok = PkReadAny( h, px, (unsigned)bytes, &packed );
    CloseHandle( h );
    if( ok ) ok = GenUpload( tex, px, n, bpp );
    if( ok ) g_genHKnown = GenHUnpack( head[5], g_genHMin, g_genHMax );
    if( ok && !g_landReady && LandFromPixels( m, px, n, bpp ) ) {
      MM_LOG( zSTRING( "land area from cache: " ) + zSTRING( (int)g_landX1 ) + " " + zSTRING( (int)g_landZ1 ) + " .. " + zSTRING( (int)g_landX2 ) + " " + zSTRING( (int)g_landZ2 ) );
      SaveMaps();
    }
    MiniMapCommon::MmFree( px );
    if( ok ) {
      MM_LOG( zSTRING( "terrain map: loaded from cache " ) + zSTRING( n ) + "x" + zSTRING( n ) + ", " + zSTRING( (int)( GetTickCount() - t0 ) ) + " ms" + zSTRING( legacy ? " (file from before mod fingerprints)" : "" ) );
      if( !legacy ) {
        FileTouch( file );
        GenCachePruneOld( world, name );
        if( !packed && g_genPack != 0 )
          PkRepackAsync( file, 24, (unsigned)bytes, g_genPack );
      }
    }
    return ok;
  }
  inline void GenCacheSave( const char* world, const TMapInfo& m, int n,
                            int bpp, const unsigned char* px ) {
    if( !g_genCache || !world || !world[0] || !px ) return;
    char name[NAME_MAX], file[NAME_MAX + 24];
    GenCacheName( world, name, NAME_MAX );
    FilesEnsure();
    FilePath( file, NAME_MAX + 24, FILES_GEN_DIR, name );
    int head[6];
    head[0] = GEN_CACHE_MAGIC;
    head[1] = n;
    head[2] = g_genFmt;
    head[3] = bpp;
    head[4] = GenSignature( m );
    head[5] = g_genHKnown ? GenHPack( g_genHMin, g_genHMax ) : 0;
    FilesDirs();
    const bool ok = PkSaveFile( file, head, sizeof( head ), px,
                                (unsigned)( n * n * bpp ), g_genPack );
    const DWORD err = ok ? 0 : GetLastError();
    if( ok ) {
      MM_LOG( zSTRING( "terrain map: saved to cache (" ) + zSTRING( n * n * bpp / 1024 ) + " kB, on disk " + zSTRING( PkFileKB( file ) ) + " kB)" );
      GenCachePruneOld( world, name );
    }
    else {
      MM_LOG( zSTRING( "terrain map: NOT saved to cache (error " ) + zSTRING( (int)err ) + ")" );
    }
  }
  const float GEN_FAST_EMPTY = -1.0e30f;
  struct TGenFastCtx {
    zCPolygon**           polys;
    int                   numPoly;
    int                   n;
    double                x1, z1, sx, sz;
    float                 ceil;
    float*                hgt;
    unsigned char*                lit;
    unsigned char*        mat;
    const void**          hkey;
    const unsigned char*  hval;
    unsigned int          hmask;
    const unsigned char*  htint;
    unsigned char*        tint;
    float                 lx, ly, lz;
    int                   rows;
    int                   chunks;
    volatile LONG         next;
    volatile LONG         failed;
  };
  inline unsigned int GenFastHash( const void* key, unsigned int mask ) {
    return ( ( (unsigned int)key >> 3 ) * 2654435761u ) & mask;
  }
  inline bool GenFastPolyOk( const zCPolygon* poly ) {
    if( !poly || !poly->vertex || poly->polyNumVert < 3 ) return false;
    if( poly->flags.portalPoly || poly->flags.ghostOccluder ) return false;
    const float ny = poly->polyPlane.normal[1];
    return ny >= 0.01f;
  }
  inline void GenRasterTri( TGenFastCtx& C, int z0, int z1,
                            double ax, double az, double ay,
                            double gbx, double gbz, double gby,
                            double gcx, double gcz, double gcy,
                            unsigned char cls, unsigned char tnt, unsigned char litv ) {
    const int n = C.n;
    const double bx = gbx - ax, bz = gbz - az, by = gby - ay;
    const double cx = gcx - ax, cz = gcz - az, cy = gcy - ay;
    const double det = bx * cz - bz * cx;
    if( det > -1.0e-12 && det < 1.0e-12 ) return;
    const double inv = 1.0 / det;
    double tminx = 0.0, tmaxx = 0.0, tminz = 0.0, tmaxz = 0.0;
    if( bx < tminx ) tminx = bx;
    if( bx > tmaxx ) tmaxx = bx;
    if( cx < tminx ) tminx = cx;
    if( cx > tmaxx ) tmaxx = cx;
    if( bz < tminz ) tminz = bz;
    if( bz > tmaxz ) tmaxz = bz;
    if( cz < tminz ) tminz = cz;
    if( cz > tmaxz ) tmaxz = cz;
    double fx0 = ceil( ax + tminx ), fx1 = floor( ax + tmaxx );
    double fz0 = ceil( az + tminz ), fz1 = floor( az + tmaxz );
    if( fx0 < 0.0 )                  fx0 = 0.0;
    if( fx1 > (double)( n - 1 ) )    fx1 = (double)( n - 1 );
    if( fz0 < (double)z0 )           fz0 = (double)z0;
    if( fz1 > (double)( z1 - 1 ) )   fz1 = (double)( z1 - 1 );
    if( fx0 > fx1 || fz0 > fz1 ) return;
    const int ix0 = (int)fx0, ix1 = (int)fx1;
    const int iz0 = (int)fz0, iz1 = (int)fz1;
    const double eps = -1.0e-7;
    for( int iz = iz0; iz <= iz1; iz++ ) {
      const double pz = (double)iz - az;
      float* const        hrow = C.hgt + iz * n;
      unsigned char* const lrow = C.lit + iz * n;
      unsigned char* const mrow = C.mat + iz * n;
      unsigned char* const trow = C.tint ? C.tint + iz * n : 0;
      for( int ix = ix0; ix <= ix1; ix++ ) {
        const double px = (double)ix - ax;
        const double w1 = ( px * cz - cx * pz ) * inv;
        const double w2 = ( bx * pz - bz * px ) * inv;
        if( w1 < eps || w2 < eps || 1.0 - w1 - w2 < eps ) continue;
        const float y = (float)( ay + w1 * by + w2 * cy );
        if( y > hrow[ix] && y < C.ceil ) {
          hrow[ix] = y;
          lrow[ix] = litv;
          mrow[ix] = cls;
          if( trow ) trow[ix] = tnt;
        }
      }
    }
  }
  inline void GenFastRaster( TGenFastCtx& C, int z0, int z1 ) {
    double gx[256], gz[256], gy[256];
    for( int p = 0; p < C.numPoly; p++ ) {
      const zCPolygon* const poly = C.polys[p];
      if( !GenFastPolyOk( poly ) ) continue;
      const int nv = (int)poly->polyNumVert;
      double zmin = 1.0e30, zmax = -1.0e30;
      bool   bad  = false;
      for( int v = 0; v < nv; v++ ) {
        const zCVertex* const vx = poly->vertex[v];
        if( !vx ) {
          bad = true;
          break;
        }
        gx[v] = ( (double)vx->position[0] - C.x1 ) * C.sx - 0.5;
        gz[v] = ( (double)vx->position[2] - C.z1 ) * C.sz - 0.5;
        gy[v] = (double)vx->position[1];
        if( gz[v] < zmin ) zmin = gz[v];
        if( gz[v] > zmax ) zmax = gz[v];
      }
      if( bad || zmax < (double)z0 || zmin > (double)( z1 - 1 ) ) continue;
      unsigned char cls = 0;
      unsigned char tnt = 0;
      const zCMaterial* const mtl = poly->material;
      if( mtl ) {
        unsigned int s = GenFastHash( mtl, C.hmask );
        while( C.hkey[s] && C.hkey[s] != mtl )
          s = ( s + 1 ) & C.hmask;
        cls = C.hkey[s] ? C.hval[s] : 0;
        if( C.htint && C.hkey[s] ) tnt = C.htint[s];
      }
      if( cls == SURF_SKIP ) continue;
      const zVEC3& nrm = poly->polyPlane.normal;
      float d = nrm[0] * C.lx + nrm[1] * C.ly + nrm[2] * C.lz;
      if( d < 0.0f ) d = -d;
      if( d > 1.0f ) d = 1.0f;
      const unsigned char litv = PackLit( 0.58f + 0.42f * d );
      for( int k = 1; k + 1 < nv; k++ )
        GenRasterTri( C, z0, z1, gx[0], gz[0], gy[0], gx[k], gz[k], gy[k],
                      gx[k + 1], gz[k + 1], gy[k + 1], cls, tnt, litv );
    }
  }
  static DWORD WINAPI GenFastWorker( LPVOID arg ) {
    TGenFastCtx* const C = (TGenFastCtx*)arg;
    __try {
      for( ;; ) {
        if( C->failed ) break;
        const LONG k = InterlockedIncrement( &C->next ) - 1;
        if( k < 0 || (int)k >= C->chunks ) break;
        const int z0 = (int)k * C->rows;
        int       z1 = z0 + C->rows;
        if( z1 > C->n ) z1 = C->n;
        GenFastRaster( *C, z0, z1 );
      }
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      InterlockedExchange( &C->failed, 1 );
    }
    return 0;
  }
  struct TGenFastStat {
    int polys, threads, vobRays, mats;
    const char* why;
  };
  struct TGenShade {
    int n, bpp;
    const float* hgt;
    const unsigned char* lit;
    const unsigned char* mat;
    unsigned char* px;
    float hmin, range, relief, cellX, cellZ;
    float lx, ly, lz;
    int fmt, edge, contour, contourDark;
    const unsigned char* wall;
    unsigned long        emptyC;
    float                lowY;
    int                  sculpt;
    int                  tex;
    const unsigned char* tint;
    int                  ox, oz;
  };
  const float SCULPT_FINE_CM  = 15.0f;
  const float SCULPT_WIDE_CM  = 60.0f;
  const float SCULPT_BROAD_CM = 320.0f;
  inline int GenSculptCells( float cm, float cell, int lo, int hi ) {
    int k = ( cell > 0.01f ) ? (int)( cm / cell ) : lo;
    if( k > hi ) k = hi;
    if( k < lo ) k = lo;
    return k;
  }
  inline float GenSculptH( const float* hgt, const unsigned char* lit, int q, float h0 ) {
    return ( lit[q] != 0 ) ? hgt[q] : h0;
  }
  inline float GenSculptKeep( float v, float h0, float jump ) {
    const float d = v - h0;
    return ( d > jump || d < -jump ) ? h0 : v;
  }
  inline bool GenSculptRing( const float* hgt, const unsigned char* lit, int n,
                             int ix, int iz, int k, float h0, float jump,
                             float& gx, float& gz, float& mean ) {
    const int xa = ( ix - k < 0 )     ? 0     : ix - k;
    const int xb = ( ix + k > n - 1 ) ? n - 1 : ix + k;
    const int ra = ( ( iz - k < 0 )     ? 0     : iz - k ) * n;
    const int rb = ( ( iz + k > n - 1 ) ? n - 1 : iz + k ) * n;
    const int rc = iz * n;
    const float tl = GenSculptH( hgt, lit, ra + xa, h0 );
    const float tc = GenSculptH( hgt, lit, ra + ix, h0 );
    const float tr = GenSculptH( hgt, lit, ra + xb, h0 );
    const float ml = GenSculptH( hgt, lit, rc + xa, h0 );
    const float mr = GenSculptH( hgt, lit, rc + xb, h0 );
    const float bl = GenSculptH( hgt, lit, rb + xa, h0 );
    const float bc = GenSculptH( hgt, lit, rb + ix, h0 );
    const float br = GenSculptH( hgt, lit, rb + xb, h0 );
    const float inv = 1.0f / ( 8.0f * (float)k );
    gx   = ( ( tr + 2.0f * mr + br ) - ( tl + 2.0f * ml + bl ) ) * inv;
    gz   = ( ( bl + 2.0f * bc + br ) - ( tl + 2.0f * tc + tr ) ) * inv;
    mean = ( 2.0f * ( GenSculptKeep( tc, h0, jump ) + GenSculptKeep( ml, h0, jump )
                    + GenSculptKeep( mr, h0, jump ) + GenSculptKeep( bc, h0, jump ) )
             + GenSculptKeep( tl, h0, jump ) + GenSculptKeep( tr, h0, jump )
             + GenSculptKeep( bl, h0, jump ) + GenSculptKeep( br, h0, jump ) )
           * ( 1.0f / 12.0f );
    const float lo = h0 - jump, hi = h0 + jump;
    return tl >= lo && tl <= hi && tc >= lo && tc <= hi && tr >= lo && tr <= hi
        && ml >= lo && ml <= hi && mr >= lo && mr <= hi
        && bl >= lo && bl <= hi && bc >= lo && bc <= hi && br >= lo && br <= hi;
  }
  inline float GenSculptSoft( float c ) {
    return c / ( ( c < 0.0f ? -c : c ) + 0.50f );
  }
  inline float GenSculptMean( const float* hgt, const unsigned char* lit, int n,
                              int ix, int iz, int k, float h0, float cell ) {
    static const int   kDx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static const int   kDz[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    static const float kW[8]  = { 1.0f, 2.0f, 1.0f, 2.0f, 2.0f, 1.0f, 2.0f, 1.0f };
    int j[5];
    j[0] = 0;
    for( int s = 1; s <= 4; s++ ) {
      j[s] = ( k * s + 2 ) / 4;
      if( j[s] < j[s - 1] ) j[s] = j[s - 1];
      if( j[s] < 1 ) j[s] = 1;
    }
    float sum = 0.0f;
    for( int d = 0; d < 8; d++ ) {
      float prev = h0;
      bool  ok   = true;
      for( int s = 1; s <= 4; s++ ) {
        int x = ix + kDx[d] * j[s], z = iz + kDz[d] * j[s];
        if( x < 0 ) x = 0;  if( x > n - 1 ) x = n - 1;
        if( z < 0 ) z = 0;  if( z > n - 1 ) z = n - 1;
        if( ok ) {
          const float v   = GenSculptH( hgt, lit, z * n + x, h0 );
          const int   seg = ( j[s] - j[s - 1] > 1 ) ? j[s] - j[s - 1] : 1;
          const float lim = 1.5f * (float)seg * cell;
          if( v - prev > lim || prev - v > lim )
            ok = false;
          else
            prev = v;
        }
        sum += kW[d] * ( ok ? prev : h0 );
      }
    }
    return sum * ( 1.0f / 48.0f );
  }
  inline void GenShadeRows( const TGenShade& S, int z0, int z1 ) {
    const int            n   = S.n;
    const float* const   hgt = S.hgt;
    const unsigned char* const lit = S.lit;
    const unsigned char* mat = S.mat;
    unsigned char* const px  = S.px;
    const float hmin   = S.hmin;
    const float range  = S.range;
    const float relief = S.relief;
    const float cellX  = S.cellX;
    const float cellZ  = S.cellZ;
    const float lx = S.lx, ly = S.ly, lz = S.lz;
    const int   bpp = S.bpp;
    const int   sculpt = S.sculpt;
    const bool  nat    = ( S.tex == 2 );
    const float cAbsX  = ( cellX < 0.0f ) ? -cellX : cellX;
    const float cAbsZ  = ( cellZ < 0.0f ) ? -cellZ : cellZ;
    const float cAvg   = ( cAbsX + cAbsZ ) * 0.5f;
    int kWide = 1, kBroad = 2;
    float l1x = lx, l1z = lz, l2x = lx, l2z = lz;
    float wMain = 1.0f, wSide = 0.0f, curvAmp = 0.0f;
    if( sculpt > 0 ) {
      const int kHi = ( n / 8 > 2 ) ? n / 8 : 2;
      kWide  = GenSculptCells( SCULPT_WIDE_CM,  cAvg, 1, kHi );
      kBroad = GenSculptCells( SCULPT_BROAD_CM, cAvg, kWide + 1, kHi );
      if( kBroad <= kWide ) kBroad = kWide + 1;
      const float ca = 0.342f, sa = 0.940f;
      l1x = lx * ca - lz * sa;  l1z = lx * sa + lz * ca;
      l2x = lx * ca + lz * sa;  l2z = -lx * sa + lz * ca;
      if( sculpt >= 2 ) { wMain = 0.50f; wSide = 0.25f; curvAmp = 0.30f; }
      else              { wMain = 0.60f; wSide = 0.20f; curvAmp = 0.18f; }
      if( nat )         { wMain = 0.70f; wSide = 0.15f; curvAmp = 0.45f; }
    }
    for( int iz = z0; iz < z1; iz++ ) {
      for( int ix = 0; ix < n; ix++ ) {
        const int p = iz * n + ix;
        const int gx0 = S.ox + ix, gz0 = S.oz + iz;
        unsigned long c = S.emptyC;
        if( S.wall && S.wall[p] ) {
          c = 0xFF2A2420ul;
        }
        else if( lit[p] != 0 ) {
          static const unsigned char kSurfRgb[SURF_COUNT][3] = {
            {0,0,0}, {62,96,134}, {232,236,240}, {126,100,74}, {222,196,142}, {98,136,62}, {48,84,50}, {148,144,138},
            {84,126,56}, {152,88,68}, {152,148,140}, {172,156,92}, {84,96,66}, {198,180,130}, {92,86,84}
          };
          static const signed char kSurfTk[SURF_COUNT] = { -1, 0, 1, 4, 6, 7, 8, 9, 10, 2, 3, 5, 11, 12, 13 };
          float r, g, b;
          const unsigned char mp = mat[p];
          if( mp >= SURF_WATER && mp < SURF_COUNT ) {
            r = (float)kSurfRgb[mp][0]; g = (float)kSurfRgb[mp][1]; b = (float)kSurfRgb[mp][2];
          } else {
            GenRamp( ( hgt[p] - hmin ) / range, r, g, b );
          }
          if( g_tcSig ) TerrainColor( ( mp < SURF_COUNT ) ? (int)kSurfTk[mp] : -1, r, g, b );
          if( mat[p] != SURF_TERRAIN && mat[p] != SURF_WATER
              && mat[p] != SURF_ROOF && mat[p] != SURF_PATH ) {
            float hr, hg, hb;
            GenRamp( ( hgt[p] - hmin ) / range, hr, hg, hb );
            r = r * 0.68f + hr * 0.32f;
            g = g * 0.68f + hg * 0.32f;
            b = b * 0.68f + hb * 0.32f;
          }
          if( S.tex && S.tint && S.tint[p] ) {
            const TTexPal& e = g_texPal[S.tint[p]];
            if( e.cls == mat[p] ) {
              float w;
              switch( mat[p] ) {
                case SURF_WATER:     w = 0.30f;  break;
                case SURF_SNOW:      w = 0.25f;  break;
                case SURF_PATH:      w = 0.20f;  break;
                case SURF_ROOF:
                case SURF_BUILDING:
                case SURF_STONEWORK: w = 0.55f;  break;
                default:             w = 0.70f;  break;
              }
              const float L = 0.299f * r + 0.587f * g + 0.114f * b;
              r = r * ( 1.0f - w ) + e.nr * L * w;
              g = g * ( 1.0f - w ) + e.ng * L * w;
              b = b * ( 1.0f - w ) + e.nb * L * w;
            }
          }
          if( nat && ( mat[p] == SURF_PLANT || mat[p] == SURF_FOREST ) ) {
            const bool  pl = ( mat[p] == SURF_PLANT );
            const float cr = pl ? 58.0f : 52.0f;
            const float cg = pl ? 100.0f : 86.0f;
            const float mx = pl ? 0.52f : 0.45f;
            float k = ( 0.299f * r + 0.587f * g + 0.114f * b ) / 70.0f;
            if( k < 0.75f ) k = 0.75f; else if( k > 1.25f ) k = 1.25f;
            r = r * ( 1.0f - mx ) + cr * k * mx;
            g = g * ( 1.0f - mx ) + cg * k * mx;
            b = b * ( 1.0f - mx ) + 44.0f * k * mx;
          }
          float waveK = 1.0f;
          if( nat && mat[p] == SURF_WATER ) {
            static const int kShore[6] = { 1, 2, 3, 4, 6, 8 };
            int dist = 9;
            for( int s = 5; s >= 0; s-- ) {
              const int d = kShore[s];
              const int el = ( ix >= d )     ? p - d     : p;
              const int er = ( ix < n - d )  ? p + d     : p;
              const int eu = ( iz >= d )     ? p - d * n : p;
              const int ed = ( iz < n - d )  ? p + d * n : p;
              if( mat[el] != SURF_WATER || mat[er] != SURF_WATER
                  || mat[eu] != SURF_WATER || mat[ed] != SURF_WATER )
                dist = d;
            }
            if( dist <= 8 ) {
              float k = 1.0f - (float)( dist - 1 ) / 8.0f;
              k *= (float)sqrt( k );
              const float t = 0.45f * k;
              r = r * ( 1.0f - t ) + 150.0f * t;
              g = g * ( 1.0f - t ) + 190.0f * t;
              b = b * ( 1.0f - t ) + 206.0f * t;
              if( dist == 1 ) {
                r = r * 0.70f + 215.0f * 0.30f;
                g = g * 0.70f + 228.0f * 0.30f;
                b = b * 0.70f + 232.0f * 0.30f;
              }
            }
            if( dist > 2 )
              waveK = 1.0f + 0.07f * ( GenNoise( gx0 >> 4, gz0 >> 4 ) * 0.6f
                                     + GenNoise( gx0 >> 2, gz0 >> 2 ) * 0.4f );
          }
          if( mat[p] == SURF_WATER && !nat ) {
            int dist = 4;
            for( int d = 1; d <= 3 && dist > 3; d++ ) {
              const int el = ( ix >= d )     ? p - d     : p;
              const int er = ( ix < n - d )  ? p + d     : p;
              const int eu = ( iz >= d )     ? p - d * n : p;
              const int ed = ( iz < n - d )  ? p + d * n : p;
              if( mat[el] != SURF_WATER || mat[er] != SURF_WATER
                  || mat[eu] != SURF_WATER || mat[ed] != SURF_WATER )
                dist = d;
            }
            if( dist <= 3 ) {
              const float k = ( 4.0f - (float)dist ) / 4.0f;
              r = r * ( 1.0f - k * 0.30f ) + 132.0f * k * 0.30f;
              g = g * ( 1.0f - k * 0.30f ) + 172.0f * k * 0.30f;
              b = b * ( 1.0f - k * 0.30f ) + 194.0f * k * 0.30f;
            }
          }
          float amp = 0.08f;
          switch( mat[p] ) {
            case SURF_FOREST:
            case SURF_PLANT:     amp = 0.20f;  break;
            case SURF_GRASS:
            case SURF_SWAMP:     amp = 0.13f;  break;
            case SURF_FIELD:
            case SURF_ROCK:      amp = 0.11f;  break;
            case SURF_BEACH:     amp = 0.09f;  break;
            case SURF_PATH:      amp = 0.04f;  break;
            case SURF_ROOF:
            case SURF_STONEWORK: amp = 0.05f;  break;
            case SURF_WATER:     amp = 0.04f;  break;
            default:             amp = 0.08f;  break;
          }
          const float var = 1.0f + amp * ( GenNoise( gx0, gz0 ) * 0.55f
                                         + GenNoise( gx0 >> 2, gz0 >> 2 ) * 0.45f );
          r *= var;
          g *= var;
          b *= var;
          if( waveK != 1.0f ) {
            r *= waveK;
            g *= waveK;
            b *= waveK;
          }
          const int xl = ( ix > 0 )     ? p - 1 : p;
          const int xr = ( ix < n - 1 ) ? p + 1 : p;
          const int zu = ( iz > 0 )     ? p - n : p;
          const int zd = ( iz < n - 1 ) ? p + n : p;
          const float hl = ( lit[xl] != 0 ) ? hgt[xl] : hgt[p];
          const float hr = ( lit[xr] != 0 ) ? hgt[xr] : hgt[p];
          const float hu = ( lit[zu] != 0 ) ? hgt[zu] : hgt[p];
          const float hd = ( lit[zd] != 0 ) ? hgt[zd] : hgt[p];
          float d;
          float sculptK = 1.0f;
          float natSlope = 0.0f;
          if( sculpt <= 0 ) {
            const int   s  = ( n >= 512 ) ? 3 : 1;
            const int   x2l = ( ix > s )         ? p - s     : p;
            const int   x2r = ( ix < n - 1 - s ) ? p + s     : p;
            const int   z2u = ( iz > s )         ? p - s * n : p;
            const int   z2d = ( iz < n - 1 - s ) ? p + s * n : p;
            const float h2l = ( lit[x2l] != 0 ) ? hgt[x2l] : hgt[p];
            const float h2r = ( lit[x2r] != 0 ) ? hgt[x2r] : hgt[p];
            const float h2u = ( lit[z2u] != 0 ) ? hgt[z2u] : hgt[p];
            const float h2d = ( lit[z2d] != 0 ) ? hgt[z2d] : hgt[p];
            const float dhdx = relief * ( ( hr - hl ) / ( 2.0f * cellX ) * 0.62f + ( h2r - h2l ) / ( 2.0f * cellX * (float)s ) * 0.38f );
            const float dhdz = relief * ( ( hd - hu ) / ( 2.0f * cellZ ) * 0.62f + ( h2d - h2u ) / ( 2.0f * cellZ * (float)s ) * 0.38f );
            float nx = -dhdx, ny = 1.0f, nz = -dhdz;
            const float len = (float)sqrt( nx * nx + ny * ny + nz * nz );
            if( len > 0.0001f ) {
              nx /= len;  ny /= len;  nz /= len;
            }
            d = nx * lx + ny * ly + nz * lz;
            if( d < 0.0f ) d = 0.0f; else if( d > 1.0f ) d = 1.0f;
          }
          else {
            const float h0 = hgt[p];
            const float rWide  = (float)kWide  * cAvg;
            const float rBroad = (float)kBroad * cAvg;
            float g1x, g1z, m1, g2x, g2z, m2;
            GenSculptRing( hgt, lit, n, ix, iz, 1,     h0, 1.5f * cAvg,  g1x, g1z, m1 );
            if( !GenSculptRing( hgt, lit, n, ix, iz, kWide, h0, 1.5f * rWide, g2x, g2z, m2 ) ) {
              g2x = g1x;
              g2z = g1z;
            }
            const float m3 = GenSculptMean( hgt, lit, n, ix, iz, kBroad, h0, cAvg );
            const float dhdx = relief * ( g1x * 0.55f + g2x * 0.45f ) / cellX;
            const float dhdz = relief * ( g1z * 0.55f + g2z * 0.45f ) / cellZ;
            if( nat ) {
              const float sx = g1x / cellX, sz = g1z / cellZ;
              natSlope = (float)sqrt( sx * sx + sz * sz );
            }
            float nx = -dhdx, ny = 1.0f, nz = -dhdz;
            const float len = (float)sqrt( nx * nx + ny * ny + nz * nz );
            if( len > 0.0001f ) {
              nx /= len;  ny /= len;  nz /= len;
            }
            float d0 = nx * lx  + ny * ly + nz * lz;
            float d1 = nx * l1x + ny * ly + nz * l1z;
            float d2 = nx * l2x + ny * ly + nz * l2z;
            if( d0 < 0.0f ) d0 = 0.0f;
            if( d1 < 0.0f ) d1 = 0.0f;
            if( d2 < 0.0f ) d2 = 0.0f;
            d = wMain * d0 + wSide * ( d1 + d2 );
            if( d > 1.0f ) d = 1.0f;
            const float t2 = ( h0 - m2 ) / rWide  * relief;
            const float t3 = ( h0 - m3 ) / rBroad * relief;
            float cv = 0.45f * GenSculptSoft( t2 * 2.0f )
                     + 0.55f * GenSculptSoft( t3 * 2.0f );
            if( cv > 0.0f )
              cv *= 0.75f;
            sculptK = 1.0f + curvAmp * cv;
          }
          const float lv = UnpackLit( lit[p] );
          float sh = nat ? ( 0.43f + 0.70f * d ) * ( 0.90f + 0.10f * lv )
                         : ( 0.46f + 0.54f * d ) * ( 0.88f + 0.12f * lv );
          if( sculpt > 0 )
            sh *= sculptK;
          if( S.edge
              && ( mat[xl] != mat[p] || mat[xr] != mat[p]
                   || mat[zu] != mat[p] || mat[zd] != mat[p] ) )
            sh *= 1.0f - ( nat ? (float)S.edge * 0.00667f : (float)S.edge / 100.0f );
          if( S.contour > 0 && mat[p] != SURF_WATER ) {
            const float step = (float)S.contour;
            const float hh   = hgt[p];
            const float a = hh   / step;
            const float b = hr   / step;
            const float c2 = hd  / step;
            const int   ia = (int)( a >= 0.0f ? a : a - 1.0f );
            const int   ib = (int)( b >= 0.0f ? b : b - 1.0f );
            const int   ic = (int)( c2 >= 0.0f ? c2 : c2 - 1.0f );
            if( ia != ib || ia != ic ) {
              if( !nat )
                sh *= 1.0f - (float)S.contourDark / 100.0f;
              else if( natSlope < 1.2f ) {
                const int ja = ( ia >= 0 ) ? ia / 5 : -( ( -ia + 4 ) / 5 );
                const int jb = ( ib >= 0 ) ? ib / 5 : -( ( -ib + 4 ) / 5 );
                const int jc = ( ic >= 0 ) ? ic / 5 : -( ( -ic + 4 ) / 5 );
                const bool idx = ( ja != jb || ja != jc );
                sh *= 1.0f - (float)S.contourDark * ( idx ? 0.01833f : 0.00833f );
              }
            }
          }
          if( S.edge ) {
            const float cell = ( cellX > 0.0f ) ? cellX : -cellX;
            const float dh   = ( ( hr > hl ) ? hr - hl : hl - hr ) + ( ( hd > hu ) ? hd - hu : hu - hd );
            if( dh > cell * 4.0f )
              sh *= 0.86f;
          }
          if( hgt[p] < S.lowY )
            sh *= 0.62f;
          if( S.tex ) {
            const unsigned char mc = mat[p];
            const bool solid = ( mc == SURF_ROOF || mc == SURF_BUILDING
                                 || mc == SURF_STONEWORK );
            const float h0 = hgt[p];
            if( mc == SURF_PLANT )
              sh *= nat ? 0.92f : 0.88f;
            if( solid || mc == SURF_PLANT ) {
              float lowN = hl;
              if( hr < lowN ) lowN = hr;
              if( hu < lowN ) lowN = hu;
              if( hd < lowN ) lowN = hd;
              if( h0 - lowN > cAvg * 3.0f )
                sh *= nat ? ( solid ? 0.76f : 0.90f ) : ( solid ? 0.74f : 0.84f );
            }
            const float hlen = (float)sqrt( lx * lx + lz * lz );
            if( hlen > 0.01f && cAbsX > 0.001f && cAbsZ > 0.001f ) {
              const float ux = lx / hlen, uz = lz / hlen;
              const float slope = ly / hlen;
              float stepW = 800.0f / 16.0f;
              if( stepW < cAvg ) stepW = cAvg;
              int K = (int)( 800.0f / stepW );
              if( K < 1 ) K = 1; else if( K > 16 ) K = 16;
              float shade = 1.0f;
              for( int k = 1; k <= K && shade > 0.9f; k++ ) {
                const float dist = stepW * (float)k;
                const int gx = ix + (int)floor( ux * dist / cellX + 0.5f );
                const int gz = iz + (int)floor( uz * dist / cellZ + 0.5f );
                if( gx < 0 || gx >= n || gz < 0 || gz >= n ) break;
                const int q = gz * n + gx;
                if( !lit[q] ) continue;
                const unsigned char mq = mat[q];
                if( mq != SURF_PLANT && mq != SURF_ROOF && mq != SURF_BUILDING
                    && mq != SURF_STONEWORK )
                  continue;
                if( hgt[q] > h0 + dist * slope + 30.0f )
                  shade = nat ? ( ( mq == SURF_PLANT ) ? 0.86f : 0.74f )
                              : ( ( mq == SURF_PLANT ) ? 0.84f : 0.78f );
              }
              sh *= shade;
            }
          }
          if( nat && natSlope > 0.55f
              && ( mat[p] == SURF_TERRAIN || mat[p] == SURF_GRASS
                   || mat[p] == SURF_FOREST || mat[p] == SURF_FIELD ) ) {
            float u = ( natSlope - 0.55f ) / 0.75f;
            if( u > 1.0f ) u = 1.0f;
            const float t = 0.60f * u * u * ( 3.0f - 2.0f * u );
            float kval = ( 0.299f * r + 0.587f * g + 0.114f * b ) / 114.0f;
            if( kval < 0.7f ) kval = 0.7f; else if( kval > 1.3f ) kval = 1.3f;
            r = r * ( 1.0f - t ) + 122.0f * kval * t;
            g = g * ( 1.0f - t ) + 114.0f * kval * t;
            b = b * ( 1.0f - t ) + 102.0f * kval * t;
          }
          if( nat ) {
            float tl = ( sh - 0.85f ) / 0.30f;
            if( tl < -1.0f ) tl = -1.0f; else if( tl > 1.0f ) tl = 1.0f;
            r *= 1.0f + 0.050f * tl;
            g *= 1.0f + 0.010f * tl;
            b *= 1.0f - 0.070f * tl;
          }
          r *= sh;
          g *= sh;
          b *= sh;
          GenVividPx( r, g, b );
          c = 0xFF000000ul | ( (unsigned long)r << 16 )
                           | ( (unsigned long)g << 8 ) | (unsigned long)b;
        }
        if( bpp == 4 )
          ( (unsigned long*)px )[p] = c;
        else
          ( (unsigned short*)px )[p] = GenPack16( c, S.fmt, GenDither( S.ox + ix, S.oz + iz ) );
      }
    }
  }
  typedef void (*TGenParFn)( void* ctx, int a, int b );
  struct TGenPar {
    TGenParFn     fn;
    void*         ctx;
    int           count, chunk;
    volatile LONG next;
  };
  static DWORD WINAPI GenParWorker( LPVOID arg ) {
    TGenPar* const P = (TGenPar*)arg;
    for( ;; ) {
      const LONG k = InterlockedIncrement( &P->next ) - 1;
      const int  a = (int)k * P->chunk;
      if( a >= P->count || a < 0 ) break;
      int b = a + P->chunk;
      if( b > P->count ) b = P->count;
      P->fn( P->ctx, a, b );
    }
    return 0;
  }
  inline int GenParThreads() {
    SYSTEM_INFO si;
    GetSystemInfo( &si );
    int t = (int)si.dwNumberOfProcessors;
    if( t < 1 ) t = 1; else if( t > GEN_MAX_THREADS ) t = GEN_MAX_THREADS;
    return t;
  }
  inline void GenParallel( TGenParFn fn, void* ctx, int count, int chunk ) {
    if( count <= 0 ) return;
    if( chunk < 1 ) chunk = 1;
    TGenPar P;
    P.fn    = fn;
    P.ctx   = ctx;
    P.count = count;
    P.chunk = chunk;
    P.next  = 0;
    int want = GenParThreads();
    const int chunks = ( count + chunk - 1 ) / chunk;
    if( want > chunks ) want = chunks;
    HANDLE h[GEN_MAX_THREADS];
    int    made = 0;
    for( int i = 1; i < want; i++ ) {
      HANDLE t = CreateThread( 0, 64 * 1024, GenParWorker, &P,
                               STACK_SIZE_PARAM_IS_A_RESERVATION, 0 );
      if( !t ) break;
      h[made++] = t;
    }
    GenParWorker( &P );
    if( made > 0 ) {
      WaitForMultipleObjects( (DWORD)made, h, TRUE, INFINITE );
      for( int i = 0; i < made; i++ )
        CloseHandle( h[i] );
    }
  }
  static void GenShadeJob( void* ctx, int a, int b ) {
    GenShadeRows( *(const TGenShade*)ctx, a, b );
  }
  struct TGenSmooth {
    float*       hgt;
    const unsigned char* lit;
    float*       prev;
    int          n;
  };
  static void GenSmoothRowsJob( void* ctx, int z0, int z1 ) {
    const TGenSmooth& S = *(const TGenSmooth*)ctx;
    float* const       hgt = S.hgt;
    const unsigned char* const lit = S.lit;
    const int          n   = S.n;
    for( int iz = z0; iz < z1; iz++ ) {
      const int base = iz * n;
      float     prv  = 0.0f;
      for( int ix = 0; ix < n; ix++ ) {
        const int   q   = base + ix;
        const float org = hgt[q];
        if( lit[q] != 0 ) {
          float sum = org * 2.0f;
          int   cnt = 2;
          if( ix > 0     && lit[q - 1] != 0 ) { sum += prv;        cnt++; }
          if( ix < n - 1 && lit[q + 1] != 0 ) { sum += hgt[q + 1]; cnt++; }
          hgt[q] = sum / (float)cnt;
        }
        prv = org;
      }
    }
  }
  static void GenSmoothColsJob( void* ctx, int x0, int x1 ) {
    const TGenSmooth& S = *(const TGenSmooth*)ctx;
    float* const       hgt = S.hgt;
    const unsigned char* const lit = S.lit;
    const int          n   = S.n;
    float* const       prv = S.prev;
    for( int iz = 0; iz < n; iz++ ) {
      const int base = iz * n;
      for( int ix = x0; ix < x1; ix++ ) {
        const int   q   = base + ix;
        const float org = hgt[q];
        if( lit[q] != 0 ) {
          float sum = org * 2.0f;
          int   cnt = 2;
          if( iz > 0     && lit[q - n] != 0 ) { sum += prv[ix];    cnt++; }
          if( iz < n - 1 && lit[q + n] != 0 ) { sum += hgt[q + n]; cnt++; }
          hgt[q] = sum / (float)cnt;
        }
        prv[ix] = org;
      }
    }
  }
  inline void BlitTexDisc( int px, int py, int pw, int ph,
                           float u0, float v0, float u1, float v1 ) {
    const float x0 = (float)px, y0 = (float)py;
    const float fw = (float)pw, fh = (float)ph;
    const float cx = g_cx, cy = g_cy;
    const float r  = DiscR();
    if( r <= 1.0f || fw <= 0.0f || fh <= 0.0f ) return;
    const float band = 2.0f;
    for( float ya = y0; ya < y0 + fh; ya += band ) {
      float yb = ya + band;
      if( yb > y0 + fh ) yb = y0 + fh;
      float dy = 0.0f;
      if( ya > cy )      dy = ya - cy;
      else if( yb < cy ) dy = cy - yb;
      const float t = r * r - dy * dy;
      if( t <= 0.0f ) continue;
      const float half = sqrtf( t );
      float xa = cx - half;
      float xb = cx + half;
      if( xa < x0 )      xa = x0;
      if( xb > x0 + fw ) xb = x0 + fw;
      if( xb - xa < 0.5f ) continue;
      BlitTexF( xa, ya, xb - xa, yb - ya,
                u0 + ( u1 - u0 ) * ( xa - x0 ) / fw,
                v0 + ( v1 - v0 ) * ( ya - y0 ) / fh,
                u0 + ( u1 - u0 ) * ( xb - x0 ) / fw,
                v0 + ( v1 - v0 ) * ( yb - y0 ) / fh );
    }
  }
  inline float Uv01( float u ) {
    return ( u < 0.0f ) ? 0.0f : ( ( u > 1.0f ) ? 1.0f : u );
  }
  inline bool MapSpanPx( float r1, float r2, float wA, float wB, int x, int w,
                         int& p0, int& p1 ) {
    const float d = wB - wA;
    if( d == 0.0f || w <= 0 ) return false;
    float fa = ( r1 - wA ) / d, fb = ( r2 - wA ) / d;
    if( fa > fb ) { std::swap( fa, fb ); }
    if( fb <= 0.0f || fa >= 1.0f ) return false;
    if( fa < 0.0f ) fa = 0.0f;
    if( fb > 1.0f ) fb = 1.0f;
    p0 = x + (int)floor( fa * (float)w + 0.5f );
    p1 = x + (int)floor( fb * (float)w + 0.5f );
    return p1 > p0;
  }
  inline void DrawMapTile( int x, int y, int w, int h ) {
    const TMapInfo& m = CurRect();
    const float dx = m.x2 - m.x1;
    const float dz = m.y2 - m.y1;
    if( dx == 0.0f || dz == 0.0f ) return;
    float halfX, halfZ;
    VisibleHalf( w, h, halfX, halfZ );
    const float wxL = g_px - halfX;
    const float wxR = g_px + halfX;
    const float wzT = g_pz + halfZ;
    const float wzB = g_pz - halfZ;
    const float uA = ( wxL - m.x1 ) / dx;
    const float uB = ( wxR - m.x1 ) / dx;
    const float vA = ( wzT - m.y1 ) / dz;
    const float vB = ( wzB - m.y1 ) / dz;
    float t0, t1, s0, s1;
    if( !ClipParam( uA, uB, t0, t1 ) ) return;
    if( !ClipParam( vA, vB, s0, s1 ) ) return;
    const float du = uB - uA;
    const float dv = vB - vA;
    int bx0, bx1, by0, by1;
    if( !MapSpanPx( m.x1, m.x2, wxL, wxR, x, w, bx0, bx1 ) ) return;
    if( !MapSpanPx( m.y1, m.y2, wzT, wzB, y, h, by0, by1 ) ) return;
    const int bx = bx0, by = by0, bw = bx1 - bx0, bh = by1 - by0;
    const float ta = (float)( bx0 - x ) / (float)w, tb = (float)( bx1 - x ) / (float)w;
    const float sa = (float)( by0 - y ) / (float)h, sb = (float)( by1 - y ) / (float)h;
    (void)t0; (void)t1; (void)s0; (void)s1;
    const float u0 = Uv01( uA + ta * du ), u1 = Uv01( uA + tb * du );
    const float v0 = Uv01( vA + sa * dv ), v1 = Uv01( vA + sb * dv );
    if( g_shape )
      BlitTexDisc( bx, by, bw, bh, u0, v0, u1, v1 );
    else
      BlitTex( bx, by, bw, bh, u0, v0, u1, v1 );
  }
  inline bool MapUV( float px, float py, float& u, float& v ) {
    const TMapInfo& m = CurRect();
    float wx, wz;
    PxToWorld( px, py, wx, wz );
    u = ( wx - m.x1 ) / ( m.x2 - m.x1 );
    v = ( wz - m.y1 ) / ( m.y2 - m.y1 );
    const float eps = 0.004f;
    if( u < -eps || u > 1.0f + eps || v < -eps || v > 1.0f + eps ) return false;
    if( u < 0.0f ) u = 0.0f; else if( u > 1.0f ) u = 1.0f;
    if( v < 0.0f ) v = 0.0f; else if( v > 1.0f ) v = 1.0f;
    return true;
  }
  inline void PolyQuad( float x0, float y0, float x1, float y1 ) {
    const float qx[4] = { x0, x1, x1, x0 };
    const float qy[4] = { y0, y0, y1, y1 };
    const unsigned char b = (unsigned char)g_bright;
    const unsigned char a = CurAlpha();
    const float z = MINIMAP_Z;
    zTRndSimpleVertex vert[4];
    for( int k = 0; k < 4; k++ ) {
      float u = 0.0f, v = 0.0f;
      MapUV( qx[k], qy[k], u, v );
      vert[k].pos   = zVEC2( qx[k], qy[k] );
      vert[k].z     = z;
      vert[k].uv    = zVEC2( u, v );
      vert[k].color = zCOLOR( b, b, b, a );
    }
    zrenderer->DrawPolySimple( CurTex(), vert, 4 );
  }
  struct TClipPt { float sx, sy, wx, wz; };
  inline TClipPt ClipCut( const TClipPt& a, const TClipPt& b, int axis, float lim ) {
    const bool sw = ( a.wx > b.wx ) || ( a.wx == b.wx && a.wz > b.wz );
    const TClipPt& p = sw ? b : a;
    const TClipPt& q = sw ? a : b;
    const float vp = axis ? p.wz : p.wx, vq = axis ? q.wz : q.wx;
    const float t  = ( vq != vp ) ? ( lim - vp ) / ( vq - vp ) : 0.0f;
    TClipPt r;
    r.sx = p.sx + ( q.sx - p.sx ) * t;
    r.sy = p.sy + ( q.sy - p.sy ) * t;
    r.wx = axis ? p.wx + ( q.wx - p.wx ) * t : lim;
    r.wz = axis ? lim : p.wz + ( q.wz - p.wz ) * t;
    return r;
  }
  inline int ClipHalf( const TClipPt* in, int n, TClipPt* out, int axis, float lim,
                       bool keepHi ) {
    int k = 0;
    for( int i = 0; i < n; i++ ) {
      const TClipPt& a = in[i];
      const TClipPt& b = in[( i + 1 < n ) ? i + 1 : 0];
      const float va = axis ? a.wz : a.wx, vb = axis ? b.wz : b.wx;
      const bool ia = keepHi ? ( va >= lim ) : ( va <= lim );
      const bool ib = keepHi ? ( vb >= lim ) : ( vb <= lim );
      if( ia )
        out[k++] = a;
      if( ia != ib )
        out[k++] = ClipCut( a, b, axis, lim );
    }
    return k;
  }
  inline int CellClipRect( float x0, float y0, float x1, float y1, const TMapInfo& m,
                           TClipPt* out ) {
    TClipPt a[8], b[8];
    const float px[4] = { x0, x1, x1, x0 };
    const float py[4] = { y0, y0, y1, y1 };
    for( int k = 0; k < 4; k++ ) {
      a[k].sx = px[k];
      a[k].sy = py[k];
      PxToWorld( px[k], py[k], a[k].wx, a[k].wz );
    }
    const float xlo = ( m.x1 < m.x2 ) ? m.x1 : m.x2, xhi = ( m.x1 < m.x2 ) ? m.x2 : m.x1;
    const float zlo = ( m.y1 < m.y2 ) ? m.y1 : m.y2, zhi = ( m.y1 < m.y2 ) ? m.y2 : m.y1;
    int n = ClipHalf( a, 4, b, 0, xlo, true );
    if( n < 3 ) return 0;
    n = ClipHalf( b, n, a, 0, xhi, false );
    if( n < 3 ) return 0;
    n = ClipHalf( a, n, b, 1, zlo, true );
    if( n < 3 ) return 0;
    n = ClipHalf( b, n, out, 1, zhi, false );
    return ( n >= 3 ) ? n : 0;
  }
  inline bool PtInRect( float px, float py, const TMapInfo& m ) {
    float wx, wz;
    PxToWorld( px, py, wx, wz );
    const float xlo = ( m.x1 < m.x2 ) ? m.x1 : m.x2, xhi = ( m.x1 < m.x2 ) ? m.x2 : m.x1;
    const float zlo = ( m.y1 < m.y2 ) ? m.y1 : m.y2, zhi = ( m.y1 < m.y2 ) ? m.y2 : m.y1;
    return wx >= xlo && wx <= xhi && wz >= zlo && wz <= zhi;
  }
  inline bool CellInRect( float x0, float y0, float x1, float y1 ) {
    const TMapInfo& m = CurRect();
    return PtInRect( x0, y0, m ) && PtInRect( x1, y0, m )
        && PtInRect( x1, y1, m ) && PtInRect( x0, y1, m );
  }
  inline void RectScreenBox( float& x0, float& y0, float& x1, float& y1 ) {
    const TMapInfo& m = CurRect();
    float cx[4], cy[4];
    WorldToPx( m.x1, m.y1, cx[0], cy[0] );
    WorldToPx( m.x2, m.y1, cx[1], cy[1] );
    WorldToPx( m.x2, m.y2, cx[2], cy[2] );
    WorldToPx( m.x1, m.y2, cx[3], cy[3] );
    x0 = x1 = cx[0];
    y0 = y1 = cy[0];
    for( int i = 1; i < 4; i++ ) {
      if( cx[i] < x0 ) x0 = cx[i];
      if( cx[i] > x1 ) x1 = cx[i];
      if( cy[i] < y0 ) y0 = cy[i];
      if( cy[i] > y1 ) y1 = cy[i];
    }
  }
  inline void PolyCellClip( float x0, float y0, float x1, float y1 ) {
    const TMapInfo& m = CurRect();
    const float dx = m.x2 - m.x1, dz = m.y2 - m.y1;
    if( dx == 0.0f || dz == 0.0f ) return;
    TClipPt p[8];
    const int n = CellClipRect( x0, y0, x1, y1, m, p );
    if( n < 3 ) return;
    const unsigned char b = (unsigned char)g_bright;
    const unsigned char a = CurAlpha();
    zTRndSimpleVertex vert[4];
    for( int i = 1; i + 1 < n; i += 2 ) {
      const int idx[4] = { 0, i, i + 1, ( i + 2 < n ) ? i + 2 : i + 1 };
      for( int k = 0; k < 4; k++ ) {
        const TClipPt& q = p[idx[k]];
        vert[k].pos   = zVEC2( q.sx, q.sy );
        vert[k].z     = MINIMAP_Z;
        vert[k].uv    = zVEC2( Uv01( ( q.wx - m.x1 ) / dx ), Uv01( ( q.wz - m.y1 ) / dz ) );
        vert[k].color = zCOLOR( b, b, b, a );
      }
      zrenderer->DrawPolySimple( CurTex(), vert, 4 );
    }
  }
  inline void DrawMapPoly( int x, int y, int w, int h ) {
    if( !zrenderer || !CurTex() ) return;
    const TMapInfo& m = CurRect();
    if( m.x2 == m.x1 || m.y2 == m.y1 ) return;
    CurTex()->CacheIn( -1.0f );
    const zTRnd_AlphaBlendFunc oldFunc = zrenderer->GetAlphaBlendFunc();
    zrenderer->SetAlphaBlendFunc( ( g_alpha >= 255 && !g_curBlend )
                                  ? zRND_ALPHA_FUNC_NONE : zRND_ALPHA_FUNC_BLEND );
    const float fx = (float)x, fy = (float)y;
    const float fw = (float)w, fh = (float)h;
    float u = 0.0f, v = 0.0f;
    const bool inside = g_curRect ? CellInRect( fx, fy, fx + fw, fy + fh )
                     : ( MapUV( fx,      fy,      u, v )
                         && MapUV( fx + fw, fy,      u, v )
                         && MapUV( fx + fw, fy + fh, u, v )
                         && MapUV( fx,      fy + fh, u, v ) );
    if( inside ) {
      PolyQuad( fx, fy, fx + fw, fy + fh );
    }
    else if( !g_rotate ) {
      float ax, ay, bx, by;
      RectScreenBox( ax, ay, bx, by );
      if( ax < fx )      ax = fx;
      if( ay < fy )      ay = fy;
      if( bx > fx + fw ) bx = fx + fw;
      if( by > fy + fh ) by = fy + fh;
      if( bx - ax >= 1.0f && by - ay >= 1.0f )
        PolyQuad( ax, ay, bx, by );
    }
    else {
      const int n = g_full ? 40 : 20;
      float kx0 = -1.0e30f, ky0 = -1.0e30f, kx1 = 1.0e30f, ky1 = 1.0e30f;
      if( g_curRect )
        RectScreenBox( kx0, ky0, kx1, ky1 );
      for( int iy = 0; iy < n; iy++ ) {
        const float ya = fy + fh * (float)iy / (float)n;
        const float yb = fy + fh * (float)( iy + 1 ) / (float)n;
        for( int ix = 0; ix < n; ix++ ) {
          const float xa = fx + fw * (float)ix / (float)n;
          const float xb = fx + fw * (float)( ix + 1 ) / (float)n;
          if( xb < kx0 || xa > kx1 || yb < ky0 || ya > ky1 ) continue;
          if( g_curRect && !CellInRect( xa, ya, xb, yb ) ) {
            PolyCellClip( xa, ya, xb, yb );
            continue;
          }
          if( !MapUV( xa, ya, u, v ) ) continue;
          if( !MapUV( xb, ya, u, v ) ) continue;
          if( !MapUV( xb, yb, u, v ) ) continue;
          if( !MapUV( xa, yb, u, v ) ) continue;
          PolyQuad( xa, ya, xb, yb );
        }
      }
    }
    zrenderer->SetAlphaBlendFunc( oldFunc );
    zrenderer->SetTexture( 0, 0 );
    static bool logged = false;
    if( !logged ) {
      logged = true;
      MM_LOG( zSTRING( "rotated map: polygons " ) + zSTRING( inside ? 1 : 144 ) );
    }
  }
  inline void DrawMapPolyDisc( int x, int y, int w, int h ) {
    if( !zrenderer || !CurTex() ) return;
    const TMapInfo& m = CurRect();
    if( m.x2 == m.x1 || m.y2 == m.y1 ) return;
    CurTex()->CacheIn( -1.0f );
    const zTRnd_AlphaBlendFunc oldFunc = zrenderer->GetAlphaBlendFunc();
    zrenderer->SetAlphaBlendFunc( ( g_alpha >= 255 && !g_curBlend )
                                  ? zRND_ALPHA_FUNC_NONE : zRND_ALPHA_FUNC_BLEND );
    const float r  = DiscR();
    const float cx = (float)x + (float)w * 0.5f;
    const float cy = (float)y + (float)h * 0.5f;
    float kx0 = -1.0e30f, ky0 = -1.0e30f, kx1 = 1.0e30f, ky1 = 1.0e30f;
    if( g_curRect )
      RectScreenBox( kx0, ky0, kx1, ky1 );
    for( float py = (float)y; py < (float)( y + h ); py += 2.0f ) {
      const float dy = py + 1.0f - cy;
      const float t  = r * r - dy * dy;
      if( t <= 0.0f ) continue;
      const float half = sqrtf( t );
      float xa = cx - half;
      float xb = cx + half;
      if( xa < (float)x )         xa = (float)x;
      if( xb > (float)( x + w ) ) xb = (float)( x + w );
      if( xb - xa < 1.0f ) continue;
      float py2 = py + 2.0f;
      if( py2 > (float)( y + h ) ) py2 = (float)( y + h );
      float u, v;
      const bool bandIn = g_curRect ? CellInRect( xa, py, xb, py2 )
                        : ( MapUV( xa, py, u, v ) && MapUV( xb, py, u, v )
                            && MapUV( xb, py2, u, v ) && MapUV( xa, py2, u, v ) );
      if( bandIn ) {
        PolyQuad( xa, py, xb, py2 );
        continue;
      }
      if( !g_rotate ) {
        float lo, top, hi, bot;
        RectScreenBox( lo, top, hi, bot );
        if( py2 <= top || py >= bot ) continue;
        float sa = ( xa > lo ) ? xa : lo;
        float sb = ( xb < hi ) ? xb : hi;
        float ta = ( py  > top ) ? py  : top;
        float tb = ( py2 < bot ) ? py2 : bot;
        if( sb - sa >= 1.0f && tb - ta >= 0.5f )
          PolyQuad( sa, ta, sb, tb );
        continue;
      }
      const int n = 16;
      for( int i = 0; i < n; i++ ) {
        const float sa = xa + ( xb - xa ) * (float)i / (float)n;
        const float sb = xa + ( xb - xa ) * (float)( i + 1 ) / (float)n;
        if( sb < kx0 || sa > kx1 || py2 < ky0 || py > ky1 ) continue;
        if( g_curRect && !CellInRect( sa, py, sb, py2 ) ) {
          PolyCellClip( sa, py, sb, py2 );
          continue;
        }
        if( !MapUV( sa, py,  u, v ) ) continue;
        if( !MapUV( sb, py,  u, v ) ) continue;
        if( !MapUV( sb, py2, u, v ) ) continue;
        if( !MapUV( sa, py2, u, v ) ) continue;
        PolyQuad( sa, py, sb, py2 );
      }
    }
    zrenderer->SetAlphaBlendFunc( oldFunc );
    zrenderer->SetTexture( 0, 0 );
  }
  struct TIconFilter {
    int old;
    TIconFilter() : old( 1 ) {
      if( !zrenderer ) return;
      old = zrenderer->GetBilerpFilterEnabled();
      if( !old ) zrenderer->SetBilerpFilterEnabled( 1 );
    }
    ~TIconFilter() { if( zrenderer && !old ) zrenderer->SetBilerpFilterEnabled( old ); }
  };
  struct TAlphaBlend {
    zTRnd_AlphaBlendFunc old;
    TAlphaBlend() : old( zRND_ALPHA_FUNC_BLEND ) {
      if( !zrenderer ) return;
      old = zrenderer->GetAlphaBlendFunc();
      if( old != zRND_ALPHA_FUNC_BLEND ) zrenderer->SetAlphaBlendFunc( zRND_ALPHA_FUNC_BLEND );
    }
    ~TAlphaBlend() {
      if( zrenderer && old != zRND_ALPHA_FUNC_BLEND ) zrenderer->SetAlphaBlendFunc( old );
    }
  };
  inline void DrawMapTexture( int x, int y, int w, int h ) {
    if( g_rotate || AlignRaw() != 0 ) {
      TIconFilter filt;
      if( g_shape )
        DrawMapPolyDisc( x, y, w, h );
      else
        DrawMapPoly( x, y, w, h );
    }
    else
      DrawMapTile( x, y, w, h );
  }
  enum {
    DP_IDLE = 0, DP_POLYS, DP_RASTER, DP_TREES, DP_VOBS, DP_RAYS, DP_PLANTS,
    DP_FILL_FWD, DP_FILL_BWD, DP_PATHS, DP_SMOOTH, DP_SHADE, DP_UPLOAD
  };
  const int    DP_VOB_RAYS = 30000;
  const int    DP_TILES    = 8;
  const int    DP_TEX_BUDGET_MB = 160;
  const int    DP_TEX_BUDGET_LOW_MB = 48;
  const int    DP_CACHE_MAGIC   = 0x43545044;
  const int    DP_TILE_VERSION  = 2;
  struct TDpTile {
    zCTexture* tex;
    int        size;
    int        ix, iz;
    bool       ready;
    TMapInfo   rect;
    DWORD      used;
  };
  static TDpTile        g_dpTile[DP_TILES];
  static int            g_dpCurSlot  = -1;
  static int            g_dpCurIx    = 0, g_dpCurIz = 0;
  static int            g_dpWantCnt   = 0;
  static int            g_dpWantReady = 0;
  static int            g_dpTileLim  = DP_TILES;
  static int            g_dpSigUsed  = 0;
  static int            g_dpFmt      = -1;
  static int            g_dpBpp      = 0;
  static bool           g_dpFailed   = false;
  static TMapInfo       g_dpNext;
  static TMapInfo       g_dpCoreRect;
  static int            g_dpPhase    = DP_IDLE;
  static int            g_dpN        = 0;
  static int            g_dpCoreN    = 0;
  static int            g_dpPad      = 0;
  static int            g_dpOx       = 0;
  static int            g_dpOz       = 0;
  static double         g_dpX1d      = 0.0;
  static double         g_dpZ1d      = 0.0;
  static double         g_dpSXd      = 1.0;
  static double         g_dpSZd      = 1.0;
  static int            g_dpMaxN     = 0;
  static int            g_dpBufN     = 0;
  static float*         g_dpHgt      = 0;
  static unsigned char* g_dpLit       = 0;
  static unsigned char* g_dpMat      = 0;
  static unsigned char* g_dpAux      = 0;
  static unsigned char* g_dpPx       = 0;
  static TPkJob         g_dpSaveJob;
  static bool           g_dpSavePending = false;
  static int            g_dpSaveSig     = 0;
  static char           g_dpSaveFile[160];
  inline void DpSaveFinish( bool block );
}
