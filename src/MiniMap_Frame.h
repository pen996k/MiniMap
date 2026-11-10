#pragma once
namespace MiniMap {
  inline void StyleFree( bool release ) {
    for( int i = 0; i < 2; i++ ) {
      if( g_styleTex[i].tex && release )
        g_styleTex[i].tex->Release();
      g_styleTex[i].tex   = 0;
      g_styleTex[i].tried = false;
      g_styleTex[i].at.style = 0;
    }
    g_titleFont      = 0;
    g_titleFontTried = false;
  }
  inline int FrameStyleNow() {
    if( !g_border ) return 0;
    const int s = g_full ? g_fullStyle : g_miniStyle;
    return ( s >= ST_PARCH && s <= ST_GOLD ) ? s : 0;
  }
  static const char* const kFrameStyle[4] = { 0, "parchment", "iron", "gold" };
  static const char* const kFramePart[3]  = { "corner", "edge", "ornament" };
  inline bool FrameFileOne( unsigned long* px, int x0, int y0, int w, int h, const MiniMapCfg::TItem& t ) {
    if( w <= 0 || h <= 0 || x0 < 0 || y0 < 0 || x0 + w > ST_ATLAS || y0 + h > ST_ATLAS ) return false;
    char file[MAX_PATH];
    unsigned flags = 0;
    const char* err = 0;
    if( !MiniMapCfg::ParseIconVal( t.val, file, MAX_PATH, flags, &err ) ) {
      CfgMsg( 0, t.src, t.line, err ? err : "[frames]: bad value" );
      return false;
    }
    char path[MAX_PATH];
    if( !FilePath( path, MAX_PATH, CFG_ICONS_DIR, file ) ) return false;
    int len = 0;
    unsigned char* data = CfgReadFile( path, len, MiniMapImage::IMG_MAX_BYTES );
    if( !data ) {
      CfgMsg( 0, t.src, t.line, "[frames]: file not found in MiniMap\\Icons - built-in frame" );
      return false;
    }
    unsigned char* rgba = 0;
    int iw = 0, ih = 0;
    const char* derr = MiniMapImage::Decode( data, len, rgba, iw, ih );
    MiniMapCommon::MmFree( data );
    if( derr ) {
      char m[160];
      snprintf( m, sizeof( m ), "[frames]: %s", derr );
      CfgMsg( 0, t.src, t.line, m );
      return false;
    }
    MiniMapImage::Resize( rgba, iw, ih, px + y0 * ST_ATLAS + x0, ST_ATLAS, w, h );
    MiniMapImage::Free( rgba );
    return true;
  }
  inline int FrameFilesBake( int st, bool full, unsigned long* px, const TStAtlas& at ) {
    if( !g_modFiles || st < ST_PARCH || st > ST_GOLD || !px ) return 0;
    const int sec = MiniMapCfg::SecFind( g_cfg, "frames" );
    if( sec < 0 ) return 0;
    int done = 0;
    char key[MiniMapCfg::KEY_LEN];
    for( int part = 0; part < 3; part++ ) {
      snprintf( key, sizeof( key ), "%s_%s_%s", kFrameStyle[st], full ? "full" : "mini", kFramePart[part] );
      const MiniMapCfg::TItem* t = MiniMapCfg::Find( g_cfg, "frames", key );
      if( !t ) continue;
      bool ok = false;
      if( part == 0 )      ok = FrameFileOne( px, at.cx, at.cy, at.eo + 1, at.eo + 1, *t );
      else if( part == 1 ) {
        ok = FrameFileOne( px, at.ex, at.ey, at.ew, at.eh, *t );
        if( ok )
          for( int y = 0; y < at.eh; y++ ) {
            unsigned long* row = px + ( at.ey + y ) * ST_ATLAS + at.ex;
            row[-1]    = row[at.ew - 1];
            row[at.ew] = row[0];
          }
      }
      else if( at.os > 0 ) ok = FrameFileOne( px, at.ox, at.oy, at.os, at.os, *t );
      else CfgMsg( 0, t->src, t->line, "[frames]: this style has no ornament (iron) - ignored" );
      if( ok ) done++;
    }
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      bool known = false;
      for( int s2 = 1; s2 <= 3 && !known; s2++ )
        for( int f2 = 0; f2 < 2 && !known; f2++ )
          for( int p2 = 0; p2 < 3 && !known; p2++ ) {
            char k2[MiniMapCfg::KEY_LEN];
            snprintf( k2, sizeof( k2 ), "%s_%s_%s", kFrameStyle[s2], f2 ? "full" : "mini", kFramePart[p2] );
            known = MiniMapCfg::EqNoCase( k2, t.key );
          }
      if( !known )
        CfgMsg( 0, t.src, t.line, "[frames]: unknown name (style_map_part, e.g. gold_full_corner) - ignored" );
    }
    return done;
  }
  inline const TStAtlas* StyleAtlas( int st ) {
    if( st < ST_PARCH || st > ST_GOLD || !zrenderer ) return 0;
    TStyleTex& s = g_styleTex[g_full ? 1 : 0];
    const float k = StScaleFor( st, g_full, UiScale() );
    if( s.tex && s.at.style == st && fabsf( s.at.k - k ) < 0.001f ) return &s.at;
    if( s.tried && s.at.style == st && fabsf( s.at.k - k ) < 0.001f ) return 0;
    if( s.tex ) { s.tex->Release(); s.tex = 0; }
    s.tried = true; s.at.style = st; s.at.k = k;
    zTRnd_TextureFormat tfmt = zRND_TEX_FORMAT_ARGB_4444;
    int bpp = 0;
    if( !PickTexFormat( true, tfmt, bpp ) ) {
      MM_LOG( "frame style: no texture format with alpha, using the plain frame" );
      return 0;
    }
    const int fmt = (int)tfmt;
    unsigned long* px = MiniMapCommon::MmAlloc<unsigned long>( (size_t)( ST_ATLAS * ST_ATLAS ) );
    if( !px ) return 0;
    const DWORD t0 = GetTickCount();
    TStAtlas at;
    const bool built = StBuildAtlas( st, g_full, k, px, at );
    const int fromFiles = built ? FrameFilesBake( st, g_full, px, at ) : 0;
    if( built ) PackAtlas16InPlace( px, ST_ATLAS * ST_ATLAS, bpp, fmt );
    zCTexture* tex = built ? CreateAndUploadTex( ST_ATLAS, ST_ATLAS, tfmt, zCOLOR( 128, 128, 128, 128 ), (const unsigned char*)px, bpp ) : 0;
    MiniMapCommon::MmFree( px );
    MM_LOG( zSTRING( tex ? "frame style ready: " : "frame style failed: " ) + zSTRING( st ) + ( g_full ? " (full map), scale " : " (minimap), scale " ) + zSTRING( (int)( k * 100.0f + 0.5f ) ) + "%, format " + zSTRING( fmt ) + ", " + zSTRING( (int)( GetTickCount() - t0 ) ) + " ms" + ( fromFiles ? zSTRING( ", pictures from files " ) + zSTRING( fromFiles ) : zSTRING( "" ) ) );
    if( !tex ) return 0;
    s.tex = tex; s.at = at;
    return &s.at;
  }
  inline void StyleDrawQuads( zCTexture* tex, const TStQuad* q, int n ) {
    if( !zrenderer || !tex || n <= 0 ) return;
    zCTexture* const white = WhiteTex();
    TAlphaBlend blend;
    TIconFilter filt;
    zTRndSimpleVertex v[4];
    zCTexture* last = 0;
    for( int i = 0; i < n; i++ ) {
      zCTexture* const t = q[i].white ? white : tex;
      if( !t ) continue;
      if( t != last ) { t->CacheIn( -1.0f ); last = t; }
      for( int k = 0; k < 4; k++ ) {
        v[k].pos   = zVEC2( q[i].x[k], q[i].y[k] );
        v[k].z     = MINIMAP_Z;
        v[k].uv    = zVEC2( q[i].u[k], q[i].v[k] );
        v[k].color = zCOLOR( q[i].r, q[i].g, q[i].b, q[i].a );
      }
      zrenderer->DrawPolySimple( t, v, 4 );
    }
  }
  inline bool DrawFrameStyled( int x, int y, int w, int h ) {
    const int st = FrameStyleNow();
    if( st == 0 || !screen ) return false;
    const TStAtlas* at = StyleAtlas( st );
    if( !at ) return false;
    int sw, sh;
    ScreenSizePx( sw, sh );
    const int n = StLayoutFrame( *at, x, y, w, h, g_shape != 0, g_cx, g_cy, DiscR(),
                                 sw, sh, g_styleQ, ST_QUADS );
    StyleDrawQuads( g_styleTex[g_full ? 1 : 0].tex, g_styleQ, n );
    return true;
  }
  inline float FrameOuter() {
    const int st = FrameStyleNow();
    if( st == 0 ) return FrameT();
    const TStAtlas* at = StyleAtlas( st );
    return at ? at->tpx : FrameT();
  }
  inline float NorthInset() {
    if( !g_full ) return 0.0f;
    const int st = FrameStyleNow();
    if( st == 0 ) return 0.0f;
    const TStAtlas* at = StyleAtlas( st );
    if( !at ) return 0.0f;
    const float s   = UiScale();
    const float pad = ( 2.0f * s < 1.5f ) ? 1.5f : 2.0f * s;
    return StPlateBelow( *at ) + 2.0f * s + NorthH() * 0.5f + pad;
  }
  inline float GlyphTextW( zCFont* f, const char* s, float hPx ) {
    if( !f || !s || f->height <= 0 ) return 0.0f;
    const float k = hPx / (float)f->height;
    float w = 0.0f;
    for( const unsigned char* p = (const unsigned char*)s; *p; p++ )
      w += (float)( f->width[*p] + 1 ) * k;
    return w;
  }
  inline void GlyphText( zCFont* f, const char* s, float x, float y, float hPx,
                         const zCOLOR& c ) {
    if( !f || !s || !zrenderer || f->height <= 0 || !f->fontTex ) return;
    const float k = hPx / (float)f->height;
    f->fontTex->CacheIn( -1.0f );
    for( const unsigned char* p = (const unsigned char*)s; *p; p++ ) {
      const float gw = (float)f->width[*p] * k;
      if( gw > 0.0f )
        zrenderer->DrawTile( f->fontTex, zVEC2( x, y ), zVEC2( x + gw, y + hPx ), MINIMAP_Z,
                             f->fontuv1[*p], f->fontuv2[*p], c );
      x += gw + k;
    }
  }
  inline void DrawMapTitle( int x, int y, int w ) {
    if( !screen || !g_full ) return;
    const int st = FrameStyleNow();
    if( st == 0 ) return;
    const char* const txt = T( TX_MAP_TITLE );
    if( !txt || !txt[0] ) return;
    if( !g_titleFontTried && zfontman ) {
      g_titleFontTried = true;
      const int fi = zfontman->SearchFont( zSTRING( kMapTitleFont ) );
      g_titleFont = ( fi >= 0 ) ? zfontman->GetFont( fi ) : 0;
    }
    zCFont* f = g_titleFont;
    if( !f || !f->fontTex ) f = screen->font;
    const TStAtlas* at = StyleAtlas( st );
    const float midX = (float)x + (float)w * 0.5f;
    if( at && f && f->fontTex ) {
      const float th = at->tTextH;
      const float tw = GlyphTextW( f, txt, th );
      const float cy = StTitleCY( *at, y );
      const int   n  = StLayoutTitle( *at, midX, cy, tw, g_styleQ, ST_QUADS );
      StyleDrawQuads( g_styleTex[1].tex, g_styleQ, n );
      zCOLOR tc, sc;
      if( st == ST_PARCH )     { tc = zCOLOR( 58, 38, 20, 255 );   sc = zCOLOR( 250, 238, 206, 190 ); }
      else if( st == ST_IRON ) { tc = zCOLOR( 230, 234, 242, 255 ); sc = zCOLOR( 0, 0, 0, 190 ); }
      else                     { tc = zCOLOR( 255, 224, 140, 255 ); sc = zCOLOR( 0, 0, 0, 190 ); }
      {
        TAlphaBlend blend;
        TIconFilter filt;
        const float o  = ( th * 0.07f < 1.0f ) ? 1.0f : th * 0.07f;
        const float tx = midX - tw * 0.5f, ty = cy - th * 0.5f;
        GlyphText( f, txt, tx + o, ty + o, th, sc );
        GlyphText( f, txt, tx, ty, th, tc );
      }
      return;
    }
    const float ui  = UiScale();
    const float lh  = LineHPx();
    const float tw  = TextWidthPx( txt );
    const float pad = 10.0f * ui;
    const float pw  = tw + 2.0f * pad, ph = lh + 4.0f * ui;
    float py = (float)y - FrameT() - ph - 3.0f * ui;
    if( py < 2.0f ) py = 2.0f;
    const float px = midX - pw * 0.5f;
    Pane( px, py, pw, ph );
    MarkText( px + pad, py + 2.0f * ui, txt, ColMarkLight() );
  }
  inline void DrawNorth( int x, int y, int w, int h ) {
    if( !screen ) return;
    float nx, ny;
    DirToPx( 0.0f, 1.0f, nx, ny );
    {
      const float len = sqrtf( nx * nx + ny * ny );
      if( len < 0.0001f ) return;
      nx /= len;
      ny /= len;
    }
    float d;
    if( g_shape )
      d = DiscR();
    else {
      const float hw = (float)w * 0.5f, hh = (float)h * 0.5f;
      const float ax = ( nx < 0.0f ) ? -nx : nx;
      const float ay = ( ny < 0.0f ) ? -ny : ny;
      const float kx = ( ax > 0.0001f ) ? hw / ax : 1.0e9f;
      const float ky = ( ay > 0.0001f ) ? hh / ay : 1.0e9f;
      d = ( kx < ky ) ? kx : ky;
    }
    if( g_full && ny < -0.7f )
      d -= NorthInset() / -ny;
    float cx = g_cx + nx * d;
    float cy = g_cy + ny * d;
    const float s   = UiScale();
    const float hN  = NorthH();
    const float wN  = hN * 0.72f;
    float       st  = s * 1.25f;
    if( st < 1.0f ) st = 1.0f;
    const float pad = ( 2.0f * s < 1.5f ) ? 1.5f : 2.0f * s;
    int sw, sh;
    ScreenSizePx( sw, sh );
    const float ex = wN * 0.5f + pad + 1.0f, ey = hN * 0.5f + pad + 1.0f;
    if( cx < ex )               cx = ex;
    if( cy < ey )               cy = ey;
    if( cx > (float)sw - ex )   cx = (float)sw - ex;
    if( cy > (float)sh - ey )   cy = (float)sh - ey;
    const float bx = cx - wN * 0.5f - pad, by = cy - hN * 0.5f - pad;
    const float bw = wN + 2.0f * pad,      bh = hN + 2.0f * pad;
    TexRect( bx, by, bw, bh, zCOLOR( 18, 16, 15, 215 ) );
    const float l = cx - wN * 0.5f + st * 0.5f, r = cx + wN * 0.5f - st * 0.5f;
    const float t = cy - hN * 0.5f,             b = cy + hN * 0.5f;
    const zCOLOR g = zCOLOR( 236, 206, 140, 255 );
    ThickLine( l, b, l, t, st, g );
    ThickLine( l, t, r, b, st, g );
    ThickLine( r, b, r, t, st, g );
  }
  inline void DrawLabel( int x, int y, int w, int h ) {
    if( !screen ) return;
    int       vx    = VX( (float)x );
    int sw, sh;
    ScreenSizePx( sw, sh );
    const float lh    = LineHPx();
    float       gap   = FrameOuter() + 2.0f;
    const bool  below = ( (float)( y + h ) + gap + lh * 1.25f < (float)sh );
    float       upY   = (float)y - gap - lh * 1.25f;
    if( g_showNorth )
      upY -= NorthH() * 0.5f;
    if( below && g_showNorth && g_rotate )
      gap += NorthH() * 0.5f + 2.0f * UiScale() + 1.0f;
    if( upY < 0.0f ) upY = 0.0f;
    int vy = below ? VY( (float)( y + h ) + gap ) : VY( upY );
    if( g_full ) {
      vx = VX( (float)x + 8.0f * UiScale() );
      vy = VY( (float)y + 6.0f * UiScale() );
    }
    const bool skalaTeraz = ( g_showScale == 1 )
                            || ( g_showScale == 2
                                 && ( g_full || g_scaleFrames > 0 ) );
    if( g_scaleFrames > 0 )
      g_scaleFrames--;
    zSTRING line;
    if( skalaTeraz ) {
      if( g_full && g_fullPct < 12 ) {
        float hx = 0.0f, hz = 0.0f;
        VisibleHalf( w, h, hx, hz );
        line = zSTRING( (int)( hx * 2.0f / 100.0f + 0.5f ) ) + " m";
      }
      else if( g_full )
        line = zSTRING( g_fullPct ) + zSTRING( T( TX_OF_WORLD ) );
      else if( g_mapCells > 0 )
        line = zSTRING( "1/" ) + zSTRING( g_mapCells * g_mapCells );
      else {
        const int m = ( g_zoom + 50 ) / 100;
        if( m >= 1000 ) {
          const int km10 = ( m + 50 ) / 100;
          line = zSTRING( km10 / 10 );
          if( km10 % 10 ) line = line + "." + zSTRING( km10 % 10 );
          line = line + " km";
        }
        else
          line = zSTRING( m ) + " m";
      }
    }
    if( line.Length() > 0 ) {
      screen->SetFontColor( ColDim() );
      if( g_shape && !g_full ) vx = VX( g_cx ) - TextWidth( line.ToChar() ) / 2;
      screen->Print( vx, vy, line );
    }
  }
  static const int kDpPhW[DP_PH_COUNT] = { 0, 3, 30, 10, 1, 30, 1, 5, 5, 5, 3, 15, 2 };
  inline float DpPhaseFrac() {
    const int n = ( g_dpN > 0 ) ? g_dpN : 1;
    float f = 0.0f;
    switch( g_dpPhase ) {
      case DP_POLYS:
        f = ( g_dpSrcNum > 0 ) ? (float)g_dpCur / (float)g_dpSrcNum : 0.0f;
        break;
      case DP_RASTER: case DP_FILL_FWD: case DP_PATHS: case DP_SHADE:
        f = (float)g_dpCur / (float)n;
        break;
      case DP_FILL_BWD:
        f = (float)( n - g_dpCur ) / (float)n;
        break;
      case DP_TREES:
        f = ( g_tvCount > 0 ) ? (float)g_dpTB.vi / (float)g_tvCount : 0.0f;
        break;
      case DP_RAYS: {
        const int all = g_dpNB * g_dpNB;
        f = ( all > 0 ) ? (float)g_dpCur / (float)all : 0.0f;
        break;
      }
      case DP_SMOOTH: {
        const int sw = ( g_dpSweepCnt > 0 ) ? g_dpSweepCnt : 1;
        f = ( (float)g_dpStep + (float)g_dpCur / (float)n ) / (float)sw;
        break;
      }
      default:
        f = 0.0f;
    }
    if( f < 0.0f ) f = 0.0f; else if( f > 1.0f ) f = 1.0f;
    return f;
  }
  inline float DpProgress( int& leftS ) {
    leftS = -1;
    if( g_dpPhase <= DP_IDLE || g_dpPhase >= DP_PH_COUNT ) return 0.0f;
    DWORD lastSum = 0;
    for( int i = 1; i < DP_PH_COUNT; i++ )
      lastSum += g_dpPhMsLast[i];
    const bool useLast = ( lastSum >= 1000 );
    double total = 0.0, done = 0.0;
    for( int i = 1; i < DP_PH_COUNT; i++ ) {
      const double w = useLast ? (double)g_dpPhMsLast[i] : (double)kDpPhW[i];
      total += w;
      if( i < g_dpPhase )
        done += w;
      else if( i == g_dpPhase )
        done += w * (double)DpPhaseFrac();
    }
    if( total <= 0.0 ) return 0.0f;
    float f = (float)( done / total );
    const DWORD el = GetTickCount() - g_dpT0;
    if( useLast )
      leftS = (int)( (double)lastSum * ( 1.0 - (double)f ) / 1000.0 + 0.999 );
    else if( f >= 0.10f && el >= 3000 )
      leftS = (int)( (double)el * ( 1.0 - (double)f ) / (double)f / 1000.0 + 0.999 );
    if( g_dpWantCnt > 1 && g_dpWantReady < g_dpWantCnt ) {
      const int   rest = g_dpWantCnt - g_dpWantReady - 1;
      const float f1   = f;
      f = ( (float)g_dpWantReady + f1 ) / (float)g_dpWantCnt;
      if( leftS > 0 && rest > 0 ) {
        const double per = useLast ? (double)lastSum
                         : ( f1 > 0.0f ? (double)el / (double)f1 : 0.0 );
        leftS += (int)( per * (double)rest / 1000.0 + 0.999 );
      }
    }
    if( f > 0.99f ) f = 0.99f;
    return f;
  }
  inline void DrawTopProgressBanner( const char* line, const char* line2, bool run, float f ) {
    int sw = 0, sh = 0;
    ScreenSizePx( sw, sh );
    const float ui  = UiScale(), lh = LineHPx(), pad = 5.0f * ui, bar = 8.0f * ui + 1.0f;
    const float tw  = TextWidthPx( line ), tw2 = line2 ? TextWidthPx( line2 ) : 0.0f;
    float bw = (float)sw * 0.30f;
    const float need = ( ( tw > tw2 ) ? tw : tw2 ) + pad * 4.0f;
    if( bw < need ) bw = need;
    if( bw > (float)sw - 4.0f ) bw = (float)sw - 4.0f;
    const float bh = run ? lh * 2.0f + bar + pad * 4.0f : lh + pad * 2.0f;
    float bx = (float)sw * 0.5f - bw * 0.5f, by = (float)sh * 0.10f;
    if( bx < 2.0f ) bx = 2.0f;
    if( by + bh > (float)sh - 2.0f ) by = (float)sh - 2.0f - bh;
    if( by < 2.0f ) by = 2.0f;
    Pane( bx, by, bw, bh );
    MarkText( bx + ( bw - tw ) * 0.5f, by + pad, line, run ? ColMarkLight() : ColGold() );
    if( !run ) return;
    const float iy = by + pad * 2.0f + lh, iw = bw - pad * 2.0f;
    TexRect( bx + pad, iy, iw, bar, zCOLOR( 60, 54, 44, 230 ) );
    TexRect( bx + pad, iy, iw * f, bar, ColGold() );
    if( line2 ) MarkText( bx + ( bw - tw2 ) * 0.5f, iy + bar + pad, line2, ColMarkText() );
  }
  inline void DrawWorldProgress() {
    if( !screen || !g_genRunning ) return;
    float f = g_genFrac;
    if( f < 0.0f ) f = 0.0f; else if( f > 1.0f ) f = 1.0f;
    const int pct  = (int)( f * 100.0f );
    const int left = ( g_genEtaMs > 0 ) ? ( g_genEtaMs + 999 ) / 1000 : -1;
    char line[96];
    if( left > 0 ) snprintf( line, sizeof( line ), "%s%d%%%s%d s", T( TX_GEN_PROG ), pct, T( TX_LEFT_ABOUT ), left );
    else           snprintf( line, sizeof( line ), "%s%d%%", T( TX_GEN_PROG ), pct );
    DrawTopProgressBanner( line, T( TX_GEN_PAUSED ), true, f );
  }
  inline void DrawGenProgress( int x, int y, int w, int h, bool haveMap ) {
    if( !screen ) return;
    if( g_genRunning && !g_genHeld ) {
      DrawWorldProgress();
      return;
    }
    const bool held   = g_genRunning;
    const bool ask    = held || g_regenAsk;
    const bool detail = !ask && g_showDetailBar && g_dpPhase != DP_IDLE && g_genDetail >= 1;
    if( !ask && !detail ) return;
    int left = -1;
    float f = 0.0f;
    if( detail ) f = DpProgress( left );
    if( f < 0.0f ) f = 0.0f; else if( f > 1.0f ) f = 1.0f;
    const int pct = (int)( f * 100.0f );
    char line[96];
    if( ask )           snprintf( line, sizeof( line ), "%s", T( held ? TX_GEN_AFTER_TALK : TX_GEN_BUSY ) );
    else if( left > 0 ) snprintf( line, sizeof( line ), "%s%d%%%s%d s", T( TX_DETAIL_PROG ), pct < 0 ? 0 : pct, T( TX_LEFT_ABOUT ), left );
    else                snprintf( line, sizeof( line ), "%s%d%%", T( TX_DETAIL_PROG ), pct < 0 ? 0 : pct );
    const float ui  = UiScale();
    const float lh  = LineHPx();
    const float pad = 3.0f * ui;
    const float bar = 4.0f * ui + 1.0f;
    const float tw  = TextWidthPx( line );
    float bw = tw + pad * 4.0f;
    if( bw < 140.0f * ui ) bw = 140.0f * ui;
    const float bh = ask ? lh + pad * 2.0f : lh + bar + pad * 3.0f;
    int sw = 0, sh = 0;
    ScreenSizePx( sw, sh );
    float bx, by;
    if( g_full ) {
      bx = (float)x + (float)w * 0.5f - bw * 0.5f;
      by = (float)y + 6.0f * ui;
    }
    else if( !haveMap ) {
      bx = (float)x + (float)w * 0.5f - bw * 0.5f;
      by = (float)y + (float)h * 0.5f - bh * 0.5f;
    }
    else {
      float gap = FrameOuter() + 2.0f;
      if( g_showNorth && g_rotate )
        gap += NorthH() * 0.5f + 2.0f * ui + 1.0f;
      const float under = (float)( y + h ) + gap + lh * 1.3f;
      if( under + bh < (float)sh - 2.0f )
        by = under;
      else {
        by = (float)y - gap - lh * 1.3f - bh;
        if( g_showNorth )
          by -= NorthH() * 0.5f;
        if( by < 0.0f ) by = 0.0f;
      }
      bx = (float)x + (float)w * 0.5f - bw * 0.5f;
    }
    if( bx + bw > (float)sw - 2.0f ) bx = (float)sw - 2.0f - bw;
    if( bx < 2.0f ) bx = 2.0f;
    Pane( bx, by, bw, bh );
    MarkText( bx + ( bw - tw ) * 0.5f, by + pad, line, ColMarkLight() );
    if( ask ) return;
    const float iy = by + pad * 2.0f + lh;
    const float iw = bw - pad * 2.0f;
    TexRect( bx + pad, iy, iw, bar, zCOLOR( 60, 54, 44, 230 ) );
    TexRect( bx + pad, iy, iw * f, bar, ColGold() );
  }
  inline void DrawCircle( float cx, float cy, float r, const zCOLOR& c ) {
    int n = (int)( r * 1.2f );
    if( n < 64 ) n = 64; else if( n > 360 ) n = 360;
    float px = cx + r;
    float py = cy;
    for( int i = 1; i <= n; i++ ) {
      const float a  = 6.2831853f * (float)i / (float)n;
      const float qx = cx + r * cosf( a );
      const float qy = cy + r * sinf( a );
      RawLine( px, py, qx, qy, c );
      px = qx;
      py = qy;
    }
  }
  inline void DrawFrame( int x, int y, int w, int h ) {
    const zCOLOR e = zCOLOR( 18, 16, 15, 220 );
    const zCOLOR g = ColGold();
    const float  t = FrameT();
    if( g_shape ) {
      const float r = DiscR();
      for( float k = 0.0f; k < t; k += 1.0f )
        DrawCircle( g_cx, g_cy, r + 0.5f + k, e );
      for( float k = 0.0f; k < t; k += 1.0f )
        DrawCircle( g_cx, g_cy, r - k, g );
      return;
    }
    const float fx = (float)x, fy = (float)y, fw = (float)w, fh = (float)h;
    TexRect( fx - t,  fy - t,  fw + 2.0f * t, t, e );
    TexRect( fx - t,  fy + fh, fw + 2.0f * t, t, e );
    TexRect( fx - t,  fy - t,  t, fh + 2.0f * t, e );
    TexRect( fx + fw, fy - t,  t, fh + 2.0f * t, e );
    TexRect( fx,          fy,          fw, t,  g );
    TexRect( fx,          fy + fh - t, fw, t,  g );
    TexRect( fx,          fy,          t,  fh, g );
    TexRect( fx + fw - t, fy,          t,  fh, g );
  }
  const int KEY_SLOTS = 44;
  static bool g_keyWas[KEY_SLOTS];
  inline bool KeyEdgeFrom( int slot, bool now, bool repeat );
  static DWORD g_keyNextAt[KEY_SLOTS];
  static DWORD g_keyHeldSince[KEY_SLOTS];
  inline bool KeyEdge( int slot, int code, bool repeat = false ) {
    if( slot < 0 || slot >= KEY_SLOTS ) return false;
    if( code <= 0 || !zinput ) {
      g_keyWas[slot]  = false;
      return false;
    }
    const bool now = ( zinput->KeyPressed( code ) != 0 );
    return KeyEdgeFrom( slot, now, repeat );
  }
  inline bool PreKeyEdge( int slot, int code, bool repeat = false ) {
    if( slot < 0 || slot >= KEY_SLOTS ) return false;
    if( code <= 0 || code >= 256 || !zinput ) {
      g_keyWas[slot] = false;
      return false;
    }
    const bool now = g_preKeysSet
        ? ( g_preKeys[code] != 0 || ( g_preTogSet && g_preTog[code] != 0 ) || DikHeld( code ) )
        : ( zinput->KeyPressed( code ) != 0 );
    return KeyEdgeFrom( slot, now, repeat );
  }
  inline void ReadSearchCat() {
    const bool l = PreKeyEdge( 24, KEY_LEFTARROW, true );
    const bool r = PreKeyEdge( 25, KEY_RIGHTARROW, true );
    const bool u = PreKeyEdge( 26, KEY_UPARROW, true );
    const bool dn = PreKeyEdge( 27, KEY_DOWNARROW, true );
    if( !g_full || g_search == 0 || g_edit >= 0 ) return;
    if( zCMenu::GetActive() || ConsoleOpen() ) return;
    if( l != r )
      SearchCatStep( r ? 1 : -1 );
    if( g_search == 1 && u != dn && g_suggN > 0 ) {
      const int last = ( g_suggN < SUGG_SHOW ? g_suggN : SUGG_SHOW ) - 1;
      if( u )
        g_suggSel = ( g_suggSel < 0 ) ? last : ( g_suggSel > 0 ? g_suggSel - 1 : 0 );
      else
        g_suggSel = ( g_suggSel < 0 || g_suggSel >= last ) ? -1 : g_suggSel + 1;
    }
  }
  inline bool KeyEdgeFrom( int slot, bool now, bool repeat ) {
    const bool was = g_keyWas[slot];
    g_keyWas[slot] = now;
    if( !now ) {
      g_keyHeldSince[slot] = 0;
      return false;
    }
    const DWORD t = GetTickCount();
    if( !was || !g_keyHeldSince[slot] ) g_keyHeldSince[slot] = t ? t : 1;
    const bool slow = ( slot >= 24 && slot <= 27 );
    const bool slowIl = ( slot >= 40 && slot <= 43 );
    const bool listIl = ( slot == 42 || slot == 43 );
    if( !was ) {
      g_keyNextAt[slot] = t + ( listIl ? 450 : slowIl ? 500 : slow ? 450 : 400 );
      return true;
    }
    if( !repeat ) return false;
    if( (LONG)( t - g_keyNextAt[slot] ) < 0 ) return false;
    g_keyNextAt[slot] = t + ( listIl ? 90 : slowIl ? 220 : slow ? 200 : 70 );
    return true;
  }
  static int g_saveDelay = 0;
  inline void RequestSave() {
    g_saveDelay = 120;
  }
  inline void UpdateSave() {
    if( g_saveDelay <= 0 ) return;
    g_saveDelay--;
    if( g_saveDelay == 0 )
      SaveMaps();
  }
  inline void FlashScale() {
    if( g_showScale == 2 ) g_scaleFrames = 120;
  }
  inline void FullZoomToCursor( int oldPct ) {
    if( g_fullPct >= 100 ) {
      g_fullCamSet = false;
      return;
    }
    if( oldPct == g_fullPct || !g_full || !g_curReady || !g_fullMapped || !player ) return;
    float wx, wz;
    PxToWorld( g_curX, g_curY, wx, wz );
    g_fullCamX   = g_px;
    g_fullCamZ   = g_pz;
    g_fullCamSet = true;
    int x, y, w, h;
    WindowRect( x, y, w, h );
    for( int pass = 0; pass < 2; pass++ ) {
      SetupMapping( x, y, w, h );
      float nx, nz;
      PxToWorld( g_curX, g_curY, nx, nz );
      g_fullCamX += wx - nx;
      g_fullCamZ += wz - nz;
    }
    SetupMapping( x, y, w, h );
  }
  inline void FullEdgePan() {
    static DWORD last = 0;
    const DWORD now = GetTickCount();
    float dt = last ? (float)( now - last ) * 0.001f : 0.0f;
    last = now;
    if( dt > 0.1f ) dt = 0.1f;
    if( !g_fullEdgeScroll || !g_full || !g_curReady || g_fullPct >= 100 || g_fullEdgePan <= 0
        || !g_fullMapped || !g_fullCamSet || dt <= 0.0f )
      return;
    int x, y, w, h;
    WindowRect( x, y, w, h );
    const float zone = (float)g_fullEdgeZone * UiScale();
    float vx = 0.0f, vy = 0.0f;
    if( g_curX <= (float)x + zone )       vx = -1.0f;
    if( g_curX >= (float)( x + w ) - zone ) vx =  1.0f;
    if( g_curY <= (float)y + zone )       vy = -1.0f;
    if( g_curY >= (float)( y + h ) - zone ) vy =  1.0f;
    if( vx == 0.0f && vy == 0.0f ) return;
    const float k = (float)g_fullEdgePan * 0.01f * dt;
    float ax, az, bx, bz;
    PxToWorld( g_cx, g_cy, ax, az );
    PxToWorld( g_cx + vx * (float)w * k, g_cy + vy * (float)h * k, bx, bz );
    g_fullCamX += bx - ax;
    g_fullCamZ += bz - az;
  }
  static bool  g_mapDrag      = false;
  static bool  g_mapDragMoved = false;
  static float g_mapDragAcc   = 0.0f;
  static bool  g_mapDragFullLogged = false;
  inline void MapDragMove() {
    const float k  = (float)g_cursorSpeed * 0.01f;
    const float dx = g_mdx * k, dy = -g_mdy * k;
    if( !g_mapDragMoved ) {
      g_mapDragAcc += ( dx < 0.0f ? -dx : dx ) + ( dy < 0.0f ? -dy : dy );
      const float th = 6.0f * UiScale();
      if( g_mapDragAcc < ( th < 4.0f ? 4.0f : th ) ) return;
      g_mapDragMoved = true;
      if( g_fullPct >= 100 && !g_mapDragFullLogged ) {
        g_mapDragFullLogged = true;
        MM_LOG( "full map: drag at 100% zoom - whole map in the window, nothing to move (zoom in first)" );
      }
    }
    if( g_fullPct >= 100 || !g_fullMapped || !g_fullCamSet || ( dx == 0.0f && dy == 0.0f ) ) return;
    float ax, az, bx, bz;
    PxToWorld( g_cx, g_cy, ax, az );
    PxToWorld( g_cx + dx, g_cy + dy, bx, bz );
    g_fullCamX += ax - bx;
    g_fullCamZ += az - bz;
  }
  inline void FullZoomBy( int step ) {
    const int oldPct = g_fullPct;
    static const int kPct[11] = { 100, 75, 55, 40, 28, 18, 12, 8, 5, 3, 2 };
    const int kLast = 10;
    int i = 0;
    while( i < kLast && kPct[i] > g_fullPct )
      i++;
    i += step;
    if( i < 0 ) i = 0; else if( i > kLast ) i = kLast;
    if( kPct[i] != g_fullPct )
      FlashScale();
    g_fullPct = kPct[i];
    FullZoomToCursor( oldPct );
  }
  inline void ZoomBy( int dir ) {
    int z = g_zoom;
    const int cur = ZoomNearestStep( g_zoom );
    if( cur >= 0 ) {
      int i = cur;
      const int curZ = kZoomLadderM[cur] * 100;
      if( !( ( dir > 0 && curZ > g_zoom ) || ( dir < 0 && curZ < g_zoom ) ) ) {
        i += ( dir > 0 ) ? 1 : -1;
        while( i >= 0 && i < kZoomLadderN && !ZoomStepOk( i ) )
          i += ( dir > 0 ) ? 1 : -1;
      }
      if( i >= 0 && i < kZoomLadderN ) z = kZoomLadderM[i] * 100;
    }
    if( z < g_zoomMin ) z = g_zoomMin; else if( z > g_zoomMax ) z = g_zoomMax;
    if( z != g_zoom ) {
      g_zoom = z;
      g_viewTouched = true;
      FlashScale();
      RequestSave();
    }
  }
  inline void CellsBy( int delta ) {
    int n = g_mapCells + delta;
    if( n < 1 ) n = 1; else if( n > 32 ) n = 32;
    if( n != g_mapCells ) {
      g_mapCells    = n;
      g_viewTouched = true;
      RequestSave();
    }
  }
  inline void SizeBy( int deltaPx ) {
    const float aspect = ( g_sizeY > 0 && g_size > 0 )
                       ? ( (float)g_sizeY / (float)g_size ) : 0.0f;
    if( deltaPx == 0 ) return;
    const int dir = ( deltaPx > 0 ) ? 1 : -1;
    int lvl = SizeLevelOf( g_size );
    int i   = lvl + dir;
    while( i >= 0 && i < kSizeLevels && !SizeLevelOk( i ) )
      i += dir;
    if( i >= 0 && i < kSizeLevels ) lvl = i;
    const int s = SizeLevelPx( lvl );
    if( s != g_size ) {
      g_size = s;
      if( aspect > 0.0f ) {
        g_sizeY = (int)( (float)g_size * aspect + 0.5f );
        if( g_sizeY < 16 ) g_sizeY = 16;
      }
      if( zoptions ) {
        zoptions->WriteInt( zSTRING( "MINIMAP" ), "SizeLevel", lvl, 0 );
        g_menuSizeLvl = lvl;
      }
      g_viewTouched = true;
      RequestSave();
    }
  }
  inline void RefreshMapSource() {
    if( !ogame || !g_world[0] ) return;
    oCWorld* const world = ogame->GetGameWorld();
    if( !world ) return;
    if( g_mapIdx < 0 ) g_mapIdx = MakeMap( g_world );
    if( g_mapIdx < 0 ) return;
    TMapInfo& m = g_maps[g_mapIdx];
    if( !m.hasCoords )
      DeriveCoords( world, m );
    g_texTried = false;
    if( m.hasCoords && ( g_genRedo || !StrEqUp( g_genWorld, g_world ) ) )
      GenerateMap( world, m, 0 );
    ApplyTexture();
  }
  inline bool ConsoleOpen();
}
#include "MiniMap_ItemList.h"
#include "MiniMap_Teleport.h"
#include "MiniMap_WorldTpEngine.h"
#include "MiniMap_Fog.h"
#include "MiniMap_Export.h"
#include "MiniMap_ModAuto.h"
namespace MiniMap {
  inline void ReadMapMouse() {
    static bool wasL = false, wasR = false;
    if( !zinput || !g_full || !g_curReady ) {
      wasL = wasR = false;
      g_mapDrag = g_mapDragMoved = false;
      return;
    }
    if( !g_enabled || ( g_hideInModal && zCView::modal ) || DialogNow() ) {
      wasL = wasR = false;
      g_mapDrag = g_mapDragMoved = false;
      return;
    }
    FullEdgePan();
    if( g_mapDrag ) {
      const bool l = MouseBtn( false );
      if( l ) {
        MapDragMove();
        wasL = l;
        wasR = MouseBtn( true );
        return;
      }
      const bool click = !g_mapDragMoved;
      g_mapDrag = g_mapDragMoved = false;
      g_mapDragAcc = 0.0f;
      if( click && g_marks ) {
        SearchCommit();
        if( g_edit >= 0 )
          EndEdit( true );
        else {
          const int hit = OwnMarkAtCursor();
          if( hit < 0 )
            AddOwnMark();
          else {
            BeginEdit( hit );
            GuideSet( hit );
          }
        }
      }
      else if( click && g_edit >= 0 )
        EndEdit( true );
      wasL = l;
      wasR = MouseBtn( true );
      return;
    }
    {
      const bool l = MouseBtn( false );
      if( ItemListMouse( l, wasL ) ) {
        wasL = l;
        wasR = MouseBtn( true );
        return;
      }
    }
    if( g_search == 1 && g_suggShown > 0 && g_curX >= g_suggX0 && g_curX < g_suggX1
        && g_curY >= g_suggY0 && g_curY < g_suggY0 + (float)g_suggShown * g_suggRowH ) {
      const bool l = MouseBtn( false );
      if( l && !wasL ) {
        const int row = g_suggFirst + (int)( ( g_curY - g_suggY0 ) / g_suggRowH );
        if( SearchSuggestApply( row ) ) {
          SearchTextChanged();
          SearchCommit();
          MM_LOG( zSTRING( "search: suggestion (mouse) '" ) + zSTRING( g_searchBuf ) + "', found " + zSTRING( g_searchFound ) );
        }
      }
      wasL = l;
      wasR = MouseBtn( true );
      return;
    }
    const bool  nowL = MouseBtn( false );
    {
      int mx, my, mw, mh;
      WindowRect( mx, my, mw, mh );
      if( g_curX < (float)mx || g_curY < (float)my
          || g_curX > (float)( mx + mw ) || g_curY > (float)( my + mh ) ) {
        wasL = nowL;
        wasR = MouseBtn( true );
        return;
      }
    }
    if( nowL && !wasL && g_mapDragOn ) {
      g_mapDrag      = true;
      g_mapDragMoved = false;
      g_mapDragAcc   = 0.0f;
    }
    if( !g_marks ) {
      wasL = nowL;
      wasR = false;
      if( g_edit >= 0 )
        EndEdit( true );
      return;
    }
    const bool  nowR = MouseBtn( true );
    if( !g_mapDragOn && nowL && !wasL ) {
      SearchCommit();
      if( g_edit >= 0 )
        EndEdit( true );
      else {
        const int hit = OwnMarkAtCursor();
        if( hit < 0 )
          AddOwnMark();
        else {
          BeginEdit( hit );
          GuideSet( hit );
        }
      }
    }
    if( nowR && !wasR ) {
      if( g_edit >= 0 )
        EndEdit( false );
      else
        RemoveOwnMark( OwnMarkAtCursor() );
    }
    wasL = nowL;
    wasR = nowR;
  }
  inline bool DocumentOpen();
  inline bool ConsoleOpen();
  inline void ReadInput() {
    if( !zinput ) return;
    if( !g_enabled ) {
      if( g_full ) {
        g_full = false;
        MM_LOG( "full map closed - plugin disabled" );
      }
      if( g_edit >= 0 )
        EndEdit( true );
      return;
    }
    if( ( g_hideInModal && zCView::modal ) || DialogNow() ) {
      if( g_full ) {
        g_full = false;
        MM_LOG( "full map closed - dialogue opened" );
      }
      if( g_edit >= 0 )
        EndEdit( true );
      return;
    }
    if( DocumentOpen() ) {
      if( g_full ) {
        g_full = false;
        MM_LOG( "full map closed - game map / document opened" );
      }
      if( g_edit >= 0 )
        EndEdit( true );
      memset( g_keyWas, 0, sizeof( g_keyWas ) );
      return;
    }
    if( zCMenu::GetActive() ) {
      memset( g_keyWas, 0, sizeof( g_keyWas ) );
      static bool logged = false;
      if( !logged ) {
        logged = true;
        MM_LOG( "game menu opened - plugin keys paused" );
      }
      return;
    }
    if( ConsoleOpen() ) {
      memset( g_keyWas, 0, sizeof( g_keyWas ) );
      return;
    }
    if( g_edit >= 0 || g_search == 1 ) {
      for( int i = 0; i < 24; i++ ) {
        g_keyWas[i]  = false;
      }
      return;
    }
    if( KeyEdge( 0, g_keyToggle ) ) {
      g_visible = !g_visible;
      MM_LOG( g_visible ? "minimap on" : "minimap off" );
    }
    if( KeyEdge( 14, g_keyAlign ) ) {
      g_align = ( AlignRaw() + 1 ) & 7;
      MM_LOG( zSTRING( "map layout: rotation " ) + zSTRING( ( g_align & 3 ) * 90 ) + zSTRING( ( g_align & 4 ) ? " degrees, mirrored" : " degrees" ) + "  (Align=" + zSTRING( g_align ) + ")" );
    }
    if( KeyEdge( 13, g_keyFull ) ) {
      static bool offLogged = false;
      if( !g_useFullMap ) {
        if( !offLogged ) {
          offLogged = true;
          MM_LOG( "full map key: full map disabled in menu (FullMap=0)" );
        }
      }
      else {
        g_full = !g_full;
        if( g_full ) {
          g_fullPct  = g_fullStart;
          g_fullCamSet = false;
          g_curReady = false;
          g_mapDragFullLogged = false;
          if( player && player->inventory2.IsOpen() ) {
            if( player->GetInteractMob() ) {
              g_full = false;
              MM_LOG( "full map: not opened - container open" );
            }
            else {
              player->CloseInventory();
              MM_LOG( "full map: player inventory closed (map opened over it)" );
            }
          }
        }
        MM_LOG( g_full ? "full map opened" : "full map closed" );
        if( g_full )
          ClashReport();
        if( g_full ) {
          static int s_seen = -1;
          const int now = g_mapDragOn | ( g_marks ? 2 : 0 ) | ( g_fullEdgeScroll ? 4 : 0 ) | ( g_fullStart << 3 );
          if( now != s_seen ) {
            s_seen = now;
            MM_LOG( zSTRING( "full map: drag " ) + zSTRING( g_mapDragOn ? "on" : "off (FullDragMap=0)" ) + ", own marks " + zSTRING( g_marks ? "on" : "off" ) + ", edge scroll " + zSTRING( g_fullEdgeScroll ? "on" : "off" ) + ", start zoom " + zSTRING( g_fullStart ) + "%" );
          }
        }
      }
    }
    if( KeyEdge( 15, g_keyScreen ) && g_useFullMap && g_full ) {
      FullScreenSet( g_fullScreen ? 0 : 1 );
      g_curReady = false;
    }
    if( g_full && g_marks && g_keyMarkIcon > 0 && KeyEdge( 16, g_keyMarkIcon ) )
      CycleIcon( 1 );
    if( g_full && g_marks ) {
      static const int digits[MARK_ICONS] = {
        KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0 };
      for( int n = 0; n < MARK_ICONS; n++ ) {
        const int k = digits[n];
        if( k == g_keyToggle || k == g_keyZoomIn || k == g_keyZoomOut
            || k == g_keySizeUp || k == g_keySizeDown || k == g_keyRotate
            || k == g_keyFull || k == g_keySearch || k == g_keyAlign
            || k == g_keyMarkIcon || k == g_keyScreen || k == g_keyItemList )
          continue;
        if( PreKeyEdge( 28 + n, k, false ) && g_markIcon != n ) {
          g_markIcon = n;
          MM_LOG( zSTRING( "mark icon (digit): " ) + zSTRING( IconNameEn( g_markIcon ) ) );
        }
      }
    }
    if( g_full && KeyEdge( 17, g_keySearch ) )
      SearchStart();
    if( KeyEdge( 18, g_keyItemList ) && g_full && g_itemList ) {
      g_ilShown = !g_ilShown;
      MM_LOG( g_ilShown ? "item list: shown (key)" : "item list: hidden (key)" );
    }
    if( g_full && ( g_mdz > 0.5f || g_mdz < -0.5f )
        && ItemListWheel( g_mdz > 0.0f ? 1 : -1 ) )
      g_mdz = 0.0f;
    if( g_full && g_wheelMarkIcon ) {
      if( g_marks && g_mdz >  0.5f ) CycleIcon( 1 );
      if( g_marks && g_mdz < -0.5f ) CycleIcon( -1 );
    }
    const bool  wheelOk = ( !g_full && ( g_wheelZoom >= 2 ) )
                          || ( g_full && g_wheelFull == 2 );
    const bool  wheelIn  = wheelOk && ( g_mdz >  0.5f );
    const bool  wheelOut = wheelOk && ( g_mdz < -0.5f );
    if( wheelIn || KeyEdge( 1, g_keyZoomIn, true ) ) {
      if( g_full )              FullZoomBy( 1 );
      else if( g_mapCells > 0 ) CellsBy( 1 );
      else                      ZoomBy( -1 );
    }
    if( wheelOut || KeyEdge( 2, g_keyZoomOut, true ) ) {
      if( g_full )              FullZoomBy( -1 );
      else if( g_mapCells > 0 ) CellsBy( -1 );
      else                      ZoomBy( 1 );
    }
    if( KeyEdge( 3, g_keySizeUp, true ) )
      SizeBy( g_sizeStep );
    if( KeyEdge( 4, g_keySizeDown, true ) )
      SizeBy( -g_sizeStep );
    if( KeyEdge( 12, g_keyRotate ) ) {
      g_rotate = g_rotate ? 0 : 1;
      g_viewTouched = true;
      MM_LOG( g_rotate ? "rotation on" : "north up" );
    }
  }
  inline bool WorldKey( oCWorld* world, char* out, int max ) {
    if( !world ) return false;
    zSTRING name = world->GetWorldName();
    if( name.IsEmpty() ) name = world->worldFilename;
    if( name.IsEmpty() ) return false;
    NormalizeKey( name.ToChar(), out, max );
    return out[0] != 0;
  }
  inline void OnWorldChanged( oCWorld* world ) {
    if( g_edit >= 0 )
      EndEdit( true );
    FogDetach();
    if( g_expState == EXP_RUN )
      ExportFinish( false, "world change" );
    g_expPending = false;
    GenAbort( "world change" );
    FreeEdges();
    LayFree();
    IconFree();
    CfgLoad();
    NpcCacheFree( false );
    ItemCacheFree( false );
    ChestCacheFree( false );
    SwitchCacheFree( false );
    PlaceCacheFree( false );
    ExitFree();
    WtpFree();
    SeenClear();
    g_partyIdx   = -2;
    g_totState   = 0;
    g_mapIdx     = -1;
    g_texReady   = false;
    g_texTried   = false;
    g_world[0]   = 0;
    g_alignKnown = false;
    g_alignAuto  = 0;
    g_worldTag   = 0;
    g_landReady = false;
    g_landX1 = g_landZ1 = g_landX2 = g_landZ2 = 0.0f;
    if( !world ) return;
    if( !WorldKey( world, g_world, NAME_MAX ) ) return;
    g_worldTag = WorldFingerprint( world );
    char wtag[9]; snprintf( wtag, sizeof( wtag ), "%08X", g_worldTag );
    MM_LOG( zSTRING( "world fingerprint " ) + zSTRING( g_world ) + ": " + zSTRING( wtag ) );
    UseSavedLand();
    LoadOwnMarks();
    PointsForWorld( g_world );
    g_mapIdx = MakeMap( g_world );
    if( g_mapIdx < 0 ) {
      MM_LOG( zSTRING( "world " ) + zSTRING( g_world ) + " - no room for another world" );
      return;
    }
    TMapInfo& m = g_maps[g_mapIdx];
    if( !m.hasCoords )
      DeriveCoords( world, m );
    if( m.hasCoords && ( g_genRedo || !StrEqUp( g_genWorld, g_world ) ) )
      GenerateMap( world, m, GEN_START_DELAY_MS );
    ApplyTexture();
    MM_LOG( zSTRING( "world " ) + zSTRING( g_world ) + zSTRING( g_texReady ? ", terrain map" : ", no map" ) );
  }
  inline void ScriptAlignRetry() {
    if( g_alignKnown || !g_world[0] ) return;
    static int wait = 0;
    if( wait > 0 ) {
      wait--;
      return;
    }
    wait = 120;
    float dist;
    if( ScriptFloat( "SPRITEMAP_DISTXF", dist ) && dist > 100.0f ) {
      g_alignAuto  = ScriptRotate90() ? 3 : 0;
      g_alignKnown = true;
      MM_LOG( zSTRING( "mod map: rotation " ) + zSTRING( g_alignAuto ? "270 degrees (set by the mod)" : "none" ) );
    }
  }
  inline void CheckWorld() {
    if( g_vidW == 0 )
      VideoSizePx( g_vidW, g_vidH );
    oCWorld* world = ogame ? ogame->GetGameWorld() : 0;
    if( world == g_worldSeen ) return;
    g_worldSeen = world;
    OnWorldChanged( world );
  }
  inline void GamePause( bool on );
  inline void Detach() {
    FogDetach();
    if( g_expState == EXP_RUN )
      ExportFinish( false, "game save/load or world change" );
    g_expPending = false;
    g_full = false;
    GamePause( false );
    GenAbort( "game save/load" );
    g_worldSeen = 0;
    g_mapIdx    = -1;
    g_texReady  = false;
    g_texTried  = false;
    g_world[0]  = 0;
    FreeEdges();
    LayFree();
    IconFree();
    NpcCacheFree( true );
    ItemCacheFree( true );
    ChestCacheFree( true );
    SwitchCacheFree( true );
    PlaceCacheFree( true );
    ExitFree();
    WtpFree();
    SeenClear();
    DetachViewTexture();
    TraderFree();
    g_invPendN = 0;
    for( int k = 0; k < INVN_SIZE; k++ )
      g_invName[k].queued = 0;
    g_totState = 0;
    LayLogTick( -1 );
    LogFlush();
  }
  inline void ForgetTexture() {
    g_texReady = false;
    g_texTried = false;
  }
  inline void Shutdown() {
    g_full = false;
    FogSave();
    if( g_expState == EXP_RUN )
      ExportFinish( false, "game exit" );
    GenAbort( "game exit" );
    MouseAccSet( false );
    MouseWrapSet( false );
    MouseEvtSet( false );
    PkRepackAbort();
    g_pausedByUs = false;
    FreeEdges();
    LayFree();
    IconFree();
    TraderFree();
    HideFree();
    ItemKindsFree();
    NamesFree();
    PointsFree();
    ModAutoFree();
    NpcIcoFree();
    ItemIcoFree();
    CfgFree();
    NpcCacheFree( false );
    ItemCacheFree( false );
    ChestCacheFree( false );
    SwitchCacheFree( false );
    PlaceCacheFree( false );
    ExitFree();
    WtpFree();
    SeenClear();
    FreeGenTexture();
    if( g_blit ) {
      g_blit->Release();
      g_blit = 0;
    }
    if( g_white ) {
      g_white->Release();
      g_white = 0;
    }
    if( g_posBlit ) {
      g_posBlit->Release();
      g_posBlit = 0;
    }
    g_texReady = false;
    g_texTried = false;
    g_posTried = false;
    LayLogTick( -1 );
    MM_LOG( "=== MiniMap: cleanup done, exiting ===" );
    LogClose();
  }
  static int g_menuLangSeen = -1;
  inline void OnMenu() {
    g_paint = false;
    if( g_edit >= 0 )
      EndEdit( true );
    SearchCommit();
    if( g_full ) {
      g_full = false;
      MM_LOG( "full map closed - game menu/screen opened" );
    }
    if( zoptions ) {
      MenuDefaults();
      int lang = zoptions->ReadInt( zSTRING( "MINIMAP" ), "Language", g_lang );
      if( lang < 0 || lang >= LANG_COUNT ) lang = LANG_PL;
      if( lang != g_menuLangSeen ) {
        g_menuLangSeen = lang;
        if( lang != g_lang )
          MM_LOG( zSTRING( "language: " ) + zSTRING( LANG_NAME[lang] ) );
        g_lang = lang;
      }
      ApplyMenuLanguage();
      UpdateMenuInfo();
      {
        const int lg = zoptions->ReadInt( zSTRING( "MINIMAP" ), "Log", g_log ) ? 1 : 0;
        if( lg != g_log ) {
          if( lg ) {
            g_log = 1;
            MM_LOG( "log: turned on in menu" );
          } else {
            MM_LOG( "log: turned off in menu" );
            g_log = 0;
            LogClose();
          }
        }
      }
      if( CaptureKey() ) {
        const int oldFull   = g_keyFull;
        const int oldSearch = g_keySearch;
        const int oldScreen = g_keyScreen;
        const int oldIl     = g_keyItemList;
        ReloadKeys();
        MM_LOG( "keys reloaded from menu" );
        const bool newKey = ( g_keyFull > 0 && g_keyFull != oldFull )
                         || ( g_keyScreen > 0 && g_keyScreen != oldScreen )
                         || ( g_keyItemList > 0 && g_keyItemList != oldIl )
                         || ( g_keySearch > 0 && g_keySearch != oldSearch );
        if( newKey && ( !g_useFullMap || Opt( "FullMap", 1 ) == 0 ) ) {
          zoptions->WriteInt( zSTRING( "MINIMAP" ), "FullMap", 1, 0 );
          g_useFullMap = 1;
          MM_LOG( "menu: full map/search key set - full map enabled (FullMap=1)" );
        }
      }
      if( g_search != 0 && ( !g_keySearch || !g_useFullMap ) )
        SearchClear();
      ApplyKeyNames();
      ExportMenu();
    }
  }
  inline void OnOptionsApplied() {
    const int keepGenSize = g_genSize;
    const int keepGenKey = GenOptionsKey();
    const int keepNpcIcons = g_npcIcons; const int keepIconStyle = g_iconStyle;
    const int keepItemIcons = g_itemIcons;
    int keepColMenu[40];
    memcpy( keepColMenu, g_colMenu, sizeof( keepColMenu ) );
    g_configLoaded = false;
    ForgetTexture();
    LoadConfig();
    if( g_search != 0 && ( !g_keySearch || !g_useFullMap ) )
      SearchClear();
    if( g_npcIcons != keepNpcIcons ) g_npcNext = GetTickCount();
    if( g_iconStyle != keepIconStyle ) { IconFree( true ); g_npcNext = GetTickCount(); }
    if( g_itemIcons != keepItemIcons || g_iconStyle != keepIconStyle ) g_itemNext = GetTickCount();
    if( memcmp( keepColMenu, g_colMenu, sizeof( keepColMenu ) ) != 0 ) {
      IconFree( true );
      g_npcNext = GetTickCount();
      MM_LOG( "menu: map colors changed" );
    }
    int sw = 0, sh = 0;
    VideoSizePx( sw, sh );
    if( g_vidW == 0 ) {
      g_vidW = sw;
      g_vidH = sh;
      return;
    }
    const bool sizeChanged = ( g_genSize != keepGenSize )
                          && ( g_genSizeUsed == 0 || g_genSize != g_genSizeUsed );
    const bool videoChanged = ( sw != g_vidW || sh != g_vidH );
    const bool genChanged   = ( GenOptionsKey() != keepGenKey );
    if( !videoChanged && !sizeChanged && !genChanged ) return;
    g_vidW = sw;
    g_vidH = sh;
    GenAbort( "settings changed" );
    if( videoChanged || sizeChanged ) {
      FreeGenTexture();
      g_genWorld[0] = 0;
    }
    else
      g_genRedo = true;
    LayFree();
    IconFree();
    g_regenAsk  = true;
    g_regenWait = 2;
    MM_LOG( zSTRING( "terrain map will be recalculated: " ) + zSTRING( videoChanged ? "display " : ( sizeChanged ? "grid " : "generator options " ) ) + zSTRING( sw ) + "x" + zSTRING( sh ) + ", grid " + zSTRING( g_genSize ) );
  }
  inline bool DocumentOpen() {
    oCDocumentManager& dm = oCDocumentManager::GetDocumentManager();
    zCListSort<oCViewDocument>* const list = dm.ListDocuments;
    return list && list->next;
  }
  inline bool ConsoleOpen() {
    __try {
      zCConsole* const c = zCConsole::cur_console;
      return c && c->IsVisible();
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline bool DialogNowCore() {
    __try {
      if( zCView::modal ) return true;
      if( ogame && TalkActive() ) return true;
      return zCCSCamera::playing != 0;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return zCView::modal != 0;
    }
  }
  inline bool DialogNow() {
    return g_hideInModal && DialogNowCore();
  }
  inline bool Hidden() {
    int reason = 0;
    if( !g_enabled || ( !g_visible && !g_full ) )
      reason = 1;
    else if( g_hideInModal && zCView::modal )
      reason = 2;
    else if( DialogNow() )
      reason = 2;
    else if( g_hideInInv && player->inventory2.IsOpen() )
      reason = 3;
    else if( g_hideInStatus && ogame->GetShowPlayerStatus() )
      reason = 4;
    else if( zCMenu::GetActive() )
      reason = 5;
    else if( DocumentOpen() )
      reason = 6;
    static int lastReason = -1;
    if( reason != lastReason ) {
      lastReason = reason;
      const char* names[7] = { "visible", "off", "dialogue",
                               "inventory", "status bars", "game menu/screen",
                               "game map / document" };
      MM_LOG( zSTRING( "state: " ) + zSTRING( names[reason] ) );
    }
    return reason != 0;
  }
  inline void EatKeysExport() {
    unsigned char* pressed = KeyTable();
    unsigned char* toggled = ToggleTable();
    const bool esc = ( pressed && pressed[KEY_ESCAPE] ) || ( toggled && toggled[KEY_ESCAPE] )
                  || ( !pressed && zinput->KeyPressed( KEY_ESCAPE ) );
    if( esc ) g_expCancel = true;
    for( int i = 0; i < 256; i++ ) {
      if( pressed ) pressed[i] = 0;
      if( toggled ) toggled[i] = 0;
    }
    zinput->ClearKeyBuffer();
  }
  inline void EatKeysEarly() {
    g_preKeysSet = false;
    g_preTogSet  = false;
    g_preBtnSet = false;
    if( !zinput ) return;
    if( !g_enabled || ( g_hideInModal && zCView::modal ) || DialogNow() ) return;
    const bool genBar = g_genRunning && !g_genHeld;
    if( g_expState == EXP_RUN && !zCMenu::GetActive() && !ConsoleOpen() ) {
      EatKeysExport();
      return;
    }
    if( !( g_edit >= 0 || g_search == 1 || g_full || genBar ) ) return;
    if( zCMenu::GetActive() ) return;
    if( ConsoleOpen() ) return;
    unsigned char* pressed = KeyTable();
    unsigned char* toggled = ToggleTable();
    const bool esc = ( pressed && pressed[KEY_ESCAPE] ) || ( toggled && toggled[KEY_ESCAPE] );
    if( esc && g_edit >= 0 ) {
      EndEdit( false );
      zinput->ClearKeyBuffer();
      MM_LOG( "note cancelled (Esc)" );
      return;
    }
    if( esc && ( g_search == 1 || ( g_search == 2 && g_full ) ) ) {
      SearchClear();
      zinput->ClearKeyBuffer();
      MM_LOG( "search cleared (Esc)" );
      return;
    }
    if( esc && g_full ) {
      g_full = false;
      zinput->ClearKeyBuffer();
      MM_LOG( "full map closed (Esc)" );
      return;
    }
    const bool typing = ( g_edit >= 0 || g_search == 1 );
    const bool barOnly = genBar && !g_full && !typing;
    const bool keepKeys = !typing && !barOnly;
    const int  KN = 12;
    const int  ourKeys[KN] = {
      keepKeys ? g_keyFull : 0,
      keepKeys ? g_keyToggle : 0,
      keepKeys ? g_keyZoomIn : 0,
      keepKeys ? g_keyZoomOut : 0,
      keepKeys ? g_keySizeUp : 0,
      keepKeys ? g_keySizeDown : 0,
      keepKeys ? g_keyMarkIcon : 0,
      keepKeys ? g_keyRotate : 0,
      keepKeys ? g_keyAlign : 0,
      keepKeys ? g_keySearch : 0,
      keepKeys ? g_keyScreen : 0,
      keepKeys ? g_keyItemList : 0
    };
    for( int i = 0; i < 256; i++ ) {
      g_preKeys[i] = pressed ? pressed[i] : 0;
      g_preTog[i]  = toggled ? toggled[i] : 0;
      if( i == KEY_ESCAPE && !barOnly ) continue;
      bool isOwn = false;
      for( int k = 0; k < KN; k++ )
        if( ourKeys[k] > 0 && ourKeys[k] == i ) {
          isOwn = true;
          break;
        }
      if( isOwn ) continue;
      if( pressed ) pressed[i] = 0;
      if( toggled ) toggled[i] = 0;
    }
    g_preKeysSet = ( pressed != 0 );
    g_preTogSet  = ( toggled != 0 );
    if( typing )
      FetchCharsEarly();
    unsigned char keepP[KN], keepT[KN];
    for( int k = 0; k < KN; k++ ) {
      const int c = ourKeys[k];
      keepP[k] = ( c > 0 && c < 256 && pressed ) ? pressed[c] : 0;
      keepT[k] = ( c > 0 && c < 256 && toggled ) ? toggled[c] : 0;
    }
    if( g_full && RawMouse() ) {
      for( int a = 0; a < 3; a++ ) {
        g_preMouse[a] += *g_rawMouse[a];
        *g_rawMouse[a] = 0;
      }
    }
    g_preBtnL = ( zinput->GetMouseButtonPressedLeft()  != 0 ) || BtnHeld( false );
    g_preBtnR = ( zinput->GetMouseButtonPressedRight() != 0 ) || BtnHeld( true );
    g_preBtnSet = true;
    zinput->ClearKeyBuffer();
    for( int k = 0; k < KN; k++ ) {
      const int c = ourKeys[k];
      if( c <= 0 || c >= 256 ) continue;
      if( pressed && keepP[k] ) pressed[c] = keepP[k];
      if( toggled && keepT[k] ) toggled[c] = keepT[k];
    }
    if( barOnly && !g_barKeysLogged ) {
      g_barKeysLogged = true;
      MM_LOG( "map progress bar: all keys blocked (Esc too) until the map is ready" );
    }
    static bool logged = false;
    if( !logged ) {
      logged = true;
      MM_LOG( "early key block active" );
    }
  }
  inline void GamePause( bool on ) {
    if( !ogame ) return;
    if( on ) {
      if( g_pausedByUs || ogame->IsOnPause() ) return;
      ogame->singleStep = 1;
      ogame->timeStep   = 0.0f;
      g_pausedByUs = true;
      MM_LOG( zSTRING( ( g_full && g_fullPause )
                       ? "game paused (full map)" : "game paused (world map calculation)" ) );
    }
    else {
      if( !g_pausedByUs ) return;
      g_pausedByUs = false;
      ogame->singleStep = 0;
      ogame->timeStep   = -1.0f;
      MM_LOG( "game unpaused" );
    }
  }
  inline void SyncPause() {
    GamePause( ( g_full && g_fullPause && g_enabled ) || ( g_genRunning && !g_genHeld && g_enabled )
               || ( g_expState == EXP_RUN ) );
  }
  inline bool TalkActive() {
    const oCInformationManager& im = oCInformationManager::GetInformationManager();
    return im.IsDone == 0 && im.Npc != 0;
  }
  static DWORD g_genHeldAt = 0;
  inline void GenHoldCheck() {
    const bool talk = g_genRunning && TalkActive();
    if( talk == g_genHeld ) return;
    const DWORD now = GetTickCount();
    g_genHeld = talk;
    if( talk ) {
      g_genHeldAt = now;
      MM_LOG( "terrain map: dialogue - calculation waits" );
    }
    else {
      const DWORD held = now - g_genHeldAt;
      g_gmT0      += held;
      g_gmLastTick = now;
      MM_LOG( zSTRING( "terrain map: dialogue ended after " ) + zSTRING( (int)held ) + " ms - continuing" );
    }
  }
  static bool g_pluginOff = false;
  inline void PluginOff() {
    if( g_pluginOff ) return;
    g_pluginOff = true;
    g_full = false;
    if( g_edit >= 0 )
      EndEdit( true );
    if( g_saveDelay > 0 ) {
      g_saveDelay = 0;
      SaveMaps();
    }
    GenAbort( "plugin disabled" );
    PkRepackAbort();
    if( g_expState == EXP_RUN )
      ExportFinish( false, "plugin disabled" );
    g_expPending = false;
    g_expCancel  = false;
    FogDetach();
    GamePause( false );
    LayFree();
    DetailPatchFree();
    IconFree();
    NpcCacheFree( true );
    ItemCacheFree( true );
    ChestCacheFree( true );
    SwitchCacheFree( true );
    PlaceCacheFree( true );
    SeenClear();
    g_regenAsk  = false;
    g_worldSeen = 0;
    MM_LOG( "plugin disabled - nothing is calculated" );
  }
  inline void PluginOn() {
    if( !g_pluginOff ) return;
    g_pluginOff = false;
    g_regenAsk  = false;
    g_worldSeen = 0;
    MM_LOG( "plugin enabled - normal start (cache or calculation)" );
  }
  inline void Update() {
    g_frameNo++;
    g_paint = false;
    g_paintBar = false;
    g_stateTime = GetTickCount();
    LoadConfig();
    if( !screen || !player || !ogame ) {
      if( g_pausedByUs ) {
        g_full = false;
        GamePause( false );
      }
      return;
    }
    if( !g_enabled ) {
      PluginOff();
      return;
    }
    PluginOn();
    if( g_screenSeen != screen ) {
      g_screenSeen = screen;
      ForgetTexture();
    }
    CheckWorld();
    if( g_regenAsk ) {
      if( g_regenWait > 0 )
        g_regenWait--;
      else {
        g_regenAsk = false;
        RefreshMapSource();
      }
    }
    GenHoldCheck();
    if( g_genRunning )
      SyncPause();
    GenUpdate();
    MouseAccSync();
    ReadMouse();
    MoveCursor();
    ReadNote();
    ReadSearch();
    ReadSearchCat();
    ReadItemList();
    InvNameTick();
    ModAutoTick();
    ReadMapMouse();
    ScriptAlignRetry();
    RefreshScale();
    ReadInput();
    SyncPause();
    if( g_expPending && !zCMenu::GetActive() ) {
      g_expPending = false;
      g_full = false;
      ExportStart();
    }
    ExportStep();
    UpdatePlayerLayer();
    FogUpdate();
    UpdateSave();
    if( g_full )
      EatKeys();
    g_paint = !Hidden();
    g_paintBar = g_enabled && ( ( g_genRunning && !g_genHeld ) || ExportRunning()
                 || ( g_expResult != 0 && GetTickCount() - g_expDoneAt < 6000 ) );
    g_stateTime = GetTickCount();
  }
  struct TRenderState {
    int                  oldZWrite;
    zTRnd_ZBufferCmp     oldZCmp;
    zTRnd_AlphaBlendFunc oldBlend;
    int                  oldCacheA;
    bool                 valid;
    TRenderState() : oldZWrite( 0 ), oldZCmp( zRND_ZBUFFER_CMP_LESS ),
                     oldBlend( zRND_ALPHA_FUNC_NONE ), oldCacheA( 0 ),
                     valid( false ) {
      if( !zrenderer ) return;
      if( zrenderer->vid_xdim > 0 && zrenderer->vid_ydim > 0 ) {
        zrenderer->SetViewport( 0, 0, zrenderer->vid_xdim,
                                zrenderer->vid_ydim );
        static bool loggedVp = false;
        if( !loggedVp ) {
          loggedVp = true;
          MM_LOG( zSTRING( "viewport set to full screen " ) + zSTRING( zrenderer->vid_xdim ) + "x" + zSTRING( zrenderer->vid_ydim ) );
        }
      }
      const zTRnd_ZBufferCmp     always = zRND_ZBUFFER_CMP_ALWAYS;
      const zTRnd_AlphaBlendFunc blend  = zRND_ALPHA_FUNC_BLEND;
      oldZWrite = zrenderer->GetZBufferWriteEnabled();
      oldZCmp   = zrenderer->GetZBufferCompare();
      oldBlend  = zrenderer->GetAlphaBlendFunc();
      valid     = true;
      zrenderer->SetZBufferWriteEnabled( 0 );
      zrenderer->SetZBufferCompare( always );
      zrenderer->SetAlphaBlendFunc( blend );
      oldCacheA = zrenderer->GetCacheAlphaPolys();
      if( oldCacheA )
        zrenderer->SetCacheAlphaPolys( 0 );
      static bool loggedA = false;
      if( !loggedA ) {
        loggedA = true;
        MM_LOG( zSTRING( "renderer: alpha polygon cache = " ) + zSTRING( oldCacheA ) + zSTRING( oldCacheA ? " (off while the map is drawn)" : "" ) );
      }
      static bool loggedZ = false;
      if( !loggedZ ) {
        loggedZ = true;
        MM_LOG( zSTRING( "z-buffer: game write " ) + zSTRING( oldZWrite ) + ", compare " + zSTRING( (int)oldZCmp ) + " (map drawn without depth test)" );
      }
    }
    ~TRenderState() {
      if( !valid || !zrenderer ) return;
      if( oldCacheA )
        zrenderer->SetCacheAlphaPolys( oldCacheA );
      zrenderer->SetAlphaBlendFunc( oldBlend );
      zrenderer->SetZBufferCompare( oldZCmp );
      zrenderer->SetZBufferWriteEnabled( oldZWrite );
    }
  };
  inline void TextFontCheck() {
    static const zCFont* s_last = 0;
    const zCFont* const f = screen ? screen->font : 0;
    if( !f || f == s_last ) return;
    s_last = f;
    const int pl = FontHasPL( f ) ? 1 : 0;
    if( pl != g_textFontPL ) {
      g_textFontPL = pl;
      MM_LOG( pl ? "text font has Polish letters"
                          : "text font without Polish letters - plugin texts without diacritics" );
    }
  }
  inline void PaintBarOnly() {
    if( !screen ) return;
    TScreenState keepScreen;
    screen->SetFont( zSTRING( g_fontName ) );
    TextFontCheck();
    TRenderState keepRnd;
    if( g_genRunning )
      DrawWorldProgress();
    DrawExportProgress();
  }
  inline bool BlitPaintNow() {
    if( g_full && g_paint && g_lateFrame == g_frameNo - 1 ) {
      g_pendingFrame = g_frameNo;
      return false;
    }
    return true;
  }
  inline bool LatePaintNow() {
    const bool now = ( g_pendingFrame == g_frameNo );
    g_lateFrame    = g_frameNo;
    g_pendingFrame = -100;
    if( now && !g_lateLogged ) {
      g_lateLogged = true;
      MM_LOG( "full map: drawn over the game interface (end of frame)" );
    }
    return now;
  }
  inline void Paint() {
    const bool full = g_paint;
    const bool bar  = g_paintBar;
    g_paint    = false;
    g_paintBar = false;
    if( !full && !bar ) return;
    if( !full ) {
      if( GetTickCount() - g_stateTime > 250 ) return;
      PaintBarOnly();
      return;
    }
    g_ringsLast = g_ringsNow;
    g_ringsNow  = 0;
    if( GetTickCount() - g_stateTime > 250 ) return;
    if( !screen || !player || !ogame ) return;
    if( !g_texReady && !g_texTried && g_mapIdx >= 0 )
      ApplyTexture();
    int x, y, w, h;
    WindowRect( x, y, w, h );
    const int shapeKeep = g_shape;
    if( g_full ) g_shape = 0;
    g_clipX0 = (float)x;
    g_clipY0 = (float)y;
    g_clipX1 = (float)( x + w );
    g_clipY1 = (float)( y + h );
    SetupMapping( x, y, w, h );
    TScreenState keepScreen;
    screen->SetFont( zSTRING( g_fontName ) );
    TextFontCheck();
    TRenderState keepRnd;
    const bool haveMap = g_texReady && g_mapIdx >= 0 && g_maps[g_mapIdx].hasCoords;
    g_rayBudgetNpc  = g_sightRaysPerFrame * 2 / 3;
    if( g_rayBudgetNpc < 4 ) g_rayBudgetNpc = 4;
    g_rayBudgetItem = g_sightRaysPerFrame - g_rayBudgetNpc;
    if( g_rayBudgetItem < 2 ) g_rayBudgetItem = 2;
    g_rayBudget = 0;
    if( g_shape )
      FillDisc( x, y, w, h, ColBack() );
    else if( g_full && g_fullScreen )
      TexRect( (float)x, (float)y, (float)w, (float)h, ColBack() );
    else
      FillPx( x, y, w, h, ColBack() );
    if( haveMap )
      DrawMapTexture( x, y, w, h ); if( haveMap ) DrawDetailPatch( x, y, w, h );
    if( haveMap )
      DrawPlayerLayer( x, y, w, h );
    if( haveMap )
      DrawFog( x, y, w, h );
    if( !haveMap || g_showWaynet )
      DrawWaynet( haveMap );
    if( g_showChests || SearchFilterOn() )
      DrawChests();
    if( g_showSwitches )
      DrawSwitches();
    if( g_showPlaces || SearchFilterOn() )
      DrawPlaces();
    if( g_showExits || SearchFilterOn() )
      DrawExits();
    DrawTeleports();
    DrawWorldTeleports();
    if( g_showItems || SearchFilterOn() )
      DrawItems();
    if( g_showNpc || SearchFilterOn() )
      DrawNpcs();
    else if( g_showTraders || g_showCorpses )
      DrawNpcs();
    DrawUserMarks();
    DrawGuide();
    if( g_showPlayer ) {
      if( g_full && g_playerRing && player ) {
        float px, py;
        PlayerPx( px, py );
        if( px >= g_clipX0 && px <= g_clipX1 && py >= g_clipY0 && py <= g_clipY1 )
          SearchRing( px, py, 16.0f * UiScale() * IconScaleK() );
      }
      DrawPlayerMark();
    }
    DrawItemList();
    DrawCursor();
    DrawNoteBar();
    DrawTeleportHover();
    DrawSearchBar();
    if( g_border && !DrawFrameStyled( x, y, w, h ) )
      DrawFrame( x, y, w, h );
    DrawMapTitle( x, y, w );
    if( g_showNorth )
      DrawNorth( x, y, w, h );
    if( g_showLabel )
      DrawLabel( x, y, w, h );
    DrawGenProgress( x, y, w, h, haveMap );
    DrawExportProgress();
    HoverDraw();
    g_shape = shapeKeep;
  }
}
