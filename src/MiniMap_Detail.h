#pragma once
namespace MiniMap {
  inline int  DpFloorMod( int v, int m );
  inline int  DpBlockFirst( int c, int off, int s );
  inline int  DpBlockStart( int j, int off, int s );
  inline int  DpRayBlock( int n, float cell );
  static float*         g_dpPrev     = 0;
  static zCPolygon**    g_dpSrc      = 0;
  static int            g_dpSrcNum   = 0;
  static zCPolygon**    g_dpPolys    = 0;
  static int            g_dpPolyCap  = 0;
  static int            g_dpSub      = 0;
  static const void**   g_dpHKey     = 0;
  static unsigned char* g_dpHVal     = 0;
  static unsigned char* g_dpHTint    = 0;
  static unsigned char* g_dpTint     = 0;
  static int            g_dpTintN    = 0;
  static TEdge*         g_dpEdges    = 0;
  static int            g_dpEdgeCnt  = 0;
  static int            g_dpCur      = 0;
  static int            g_dpStep     = 0;
  static int            g_dpSlice    = 16;
  static int            g_dpPolySlice = 20000;
  static int            g_dpS        = 1;
  static int            g_dpNB       = 0;
  static int            g_dpBx       = 0;
  static int            g_dpBz       = 0;
  static int            g_dpFillMax  = 16;
  static int            g_dpRays     = 0;
  static int            g_dpTreeTris = 0, g_dpTreeStamps = 0;
  static double         g_dpTreeMs   = 0.0;
  static bool           g_dpTreeIn   = false;
  static int            g_dpFrames   = 0;
  static double         g_dpWorkMs   = 0.0;
  static DWORD          g_dpT0       = 0;
  static float          g_dpGCell    = 0.0f;
  static float          g_dpPCell    = 0.0f;
  static int            g_dpNeqUsed  = 0;
  static int            g_dpKeyDetail = -1, g_dpKeyN = 0, g_dpKeyNeq = 0;
  static int            g_dpKeyGN     = 0,  g_dpKeySig = 0;
  static TGenFastCtx    g_dpFast;
  static TGenSmooth     g_dpSm;
  static TGenShade      g_dpShade;
  const int             DP_PH_COUNT  = 13;
  static DWORD          g_dpPhMs[DP_PH_COUNT];
  static DWORD          g_dpPhMsLast[DP_PH_COUNT];
  static DWORD          g_dpLastTick = 0;
  struct TDpSweep { TGenParFn fn; void* ctx; };
  static TDpSweep       g_dpSweep[16];
  static int            g_dpSweepCnt = 0;
  struct TDpPaths {
    const TEdge*         e;
    int                  ne, n, water;
    double               x1, z1, cw, ch;
    float                r, r2;
    unsigned char*       mat;
    const unsigned char* lit;
  };
  static TDpPaths       g_dpPaths;
  inline double DpMs() {
    static __int64 freq = 0;
    if( !freq ) {
      LARGE_INTEGER f;
      QueryPerformanceFrequency( &f );
      freq = ( f.QuadPart > 0 ) ? f.QuadPart : 1;
    }
    LARGE_INTEGER t;
    QueryPerformanceCounter( &t );
    return (double)t.QuadPart * 1000.0 / (double)freq;
  }
  inline float DpAbs( float v ) { return ( v < 0.0f ) ? -v : v; }
  struct TTreeVob {
    zCVob*           vob;
    zCProgMeshProto* pm;
    unsigned char    cls;
    unsigned char    plant;
  };
  static TTreeVob* g_tv       = 0;
  static int       g_tvCount  = 0;
  static int       g_tvCap    = 0;
  static oCWorld*  g_tvWorld  = 0;
  static int       g_tvKey    = 0;
  static DWORD     g_tvAt     = 0;
  static int       g_tvNodes  = 0, g_tvPlants = 0, g_tvOthers = 0, g_tvTris = 0;
  static int       g_tvSkipHidden = 0, g_tvSkipTrace = 0, g_tvSkipGiant = 0;
  static double    g_tvCollectMs = 0.0;
  const int        TREE_NODE_LIMIT = 200000;
  const float      TREE_PLANT_MIN  = 30.0f;
  struct TTreeBatch {
    float*         v;
    unsigned char* cls;
    unsigned char* tnt;
    unsigned char* lit;
    int            cap, cnt;
    int            vi, ti;
  };
  static TTreeBatch g_gmTB = { 0, 0, 0, 0, 0, 0, 0, 0 };
  static TTreeBatch g_dpTB = { 0, 0, 0, 0, 0, 0, 0, 0 };
  inline void TreeBatchFree( TTreeBatch& B ) {
    MiniMapCommon::MmFree( B.v );    B.v   = 0;
    MiniMapCommon::MmFree( B.cls );  B.cls = 0;
    MiniMapCommon::MmFree( B.tnt );  B.tnt = 0;
    MiniMapCommon::MmFree( B.lit );  B.lit = 0;
    B.cap = B.cnt = B.vi = B.ti = 0;
  }
  inline bool TreeBatchAlloc( TTreeBatch& B, int cap ) {
    if( B.v && B.cap == cap ) {
      B.cnt = B.vi = B.ti = 0;
      return true;
    }
    TreeBatchFree( B );
    B.v   = MiniMapCommon::MmAlloc<float>( (size_t)( cap * 9 ) );
    B.cls = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( cap ) );
    B.tnt = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( cap ) );
    B.lit = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( cap ) );
    if( !B.v || !B.cls || !B.tnt || !B.lit ) {
      TreeBatchFree( B );
      return false;
    }
    B.cap = cap;
    return true;
  }
  inline void TreeVobsFree() {
    MiniMapCommon::MmFree( g_tv );
    g_tv      = 0;
    g_tvCount = 0;
    g_tvCap   = 0;
    g_tvWorld = 0;
  }
  inline int TreeVobsKey() {
    return g_genTreeShapes ^ ( g_genPlantMin << 2 ) ^ ( g_genVobMin << 14 )
         ^ ( ( g_genTexColors != 0 ? 1 : 0 ) << 30 );
  }
  inline bool TreeVobsPush( const TTreeVob& e ) {
    if( g_tvCount >= g_tvCap ) {
      const int cap = ( g_tvCap < 4096 ) ? 4096 : g_tvCap * 2;
      TTreeVob* const nt = MiniMapCommon::MmAlloc<TTreeVob>( (size_t)( cap ) );
      if( !nt ) return false;
      memcpy( nt, g_tv, (size_t)( g_tvCount ) * sizeof( nt[0] ) );
      MiniMapCommon::MmFree( g_tv );
      g_tv    = nt;
      g_tvCap = cap;
    }
    g_tv[g_tvCount++] = e;
    return true;
  }
  inline bool TreeVobIgnore( const char* name ) {
    return name && ( GenHasAnyWord( name, "CAVEWEB|SPIDERWEB|COBWEB" ) );
  }
  static const void*   g_tvIgnKey[2048] = { 0 };
  static unsigned char g_tvIgnVal[2048] = { 0 };
  inline bool TreeVobIgnoreCached( const zCVisual* vis, const char* name ) {
    const int slot = (int)( ( ( (unsigned int)vis ) >> 4 ) & 2047 );
    if( g_tvIgnKey[slot] == vis ) return g_tvIgnVal[slot] != 0;
    const bool ign = TreeVobIgnore( name );
    g_tvIgnKey[slot] = vis;
    g_tvIgnVal[slot] = ign ? 1 : 0;
    return ign;
  }
  inline void TreeVobsConsider( oCWorld* world, zCVob* vob, zCClassDef* npcDef,
                                zCClassDef* itemDef, zCClassDef* pmDef ) {
    zCVisual* const vis = vob->visual;
    if( !vis || vob->type == zVOB_TYPE_LEVEL_COMPONENT ) return;
    if( !vob->showVisual ) { g_tvSkipHidden++; return; }
    if( vob->homeWorld != (zCWorld*)world ) return;
    zCClassDef* const def = vob->_GetClassDef();
    if( npcDef && zCObject::CheckInheritance( npcDef, def ) ) return;
    if( itemDef && zCObject::CheckInheritance( itemDef, def ) ) return;
    const zTBBox3D& bb = vob->bbox3D;
    const float sx = bb.maxs[0] - bb.mins[0];
    const float sz = bb.maxs[2] - bb.mins[2];
    if( !( sx == sx && sz == sz && bb.maxs[1] == bb.maxs[1] )
        || sx > 1.0e6f || sz > 1.0e6f )
      return;
    const float big = ( sx > sz ) ? sx : sz;
    const float plantMin = ( g_genTreeShapes >= 2 ) ? TREE_PLANT_MIN : (float)g_genPlantMin;
    if( !( big >= plantMin ) ) return;
    const char* const vname = vis->GetObjectName().ToChar();
    if( TreeVobIgnoreCached( vis, vname ) ) return;
    const unsigned char cls = GenSurfCached( vis, vname, true );
    const bool plant = ( cls == SURF_PLANT );
    if( plant && TreeGiantSize( sx, sz, bb.maxs[1] - bb.mins[1], vname ) ) {
      g_tvSkipGiant++;
      return;
    }
    if( vob->ignoredByTraceRay ) {
      g_tvSkipTrace++;
      if( !plant ) return;
    }
    if( plant ) {
      const float mn = ( g_genTreeShapes >= 2 ) ? TREE_PLANT_MIN : (float)g_genPlantMin;
      if( !( big >= mn ) ) return;
    }
    else if( g_genTreeShapes < 2 || !( big >= (float)g_genVobMin ) )
      return;
    TTreeVob e;
    e.vob   = vob;
    e.pm    = ( pmDef && zCObject::CheckInheritance( pmDef, vis->_GetClassDef() ) )
            ? (zCProgMeshProto*)vis : 0;
    e.cls   = cls;
    e.plant = plant ? 1 : 0;
    if( !plant && !e.pm ) return;
    if( e.pm && g_genTreeShapes >= 2 ) {
      int t = 0;
      for( int s = 0; s < e.pm->numSubMeshes; s++ )
        t += e.pm->subMeshList[s].triList.numInArray;
      g_tvTris += t;
    }
    if( !TreeVobsPush( e ) ) return;
    if( plant ) g_tvPlants++; else g_tvOthers++;
  }
  inline bool TreeVobsCollectCore( oCWorld* world ) {
    __try {
      zCClassDef* const npcDef  = oCNpc::classDef;
      zCClassDef* const itemDef = oCItem::classDef;
      zCClassDef* const pmDef   = zCProgMeshProto::classDef;
      zCTree<zCVob>* const root = &world->globalVobTree;
      zCTree<zCVob>* node = root->firstChild;
      while( node && g_tvNodes < TREE_NODE_LIMIT ) {
        g_tvNodes++;
        if( node->data )
          TreeVobsConsider( world, node->data, npcDef, itemDef, pmDef );
        node = VobTreeNext( node, root );
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline bool TreeVobsCollect( oCWorld* world ) {
    const double t0 = DpMs();
    g_tvCount = 0;
    g_tvNodes = g_tvPlants = g_tvOthers = g_tvTris = 0;
    g_tvSkipHidden = g_tvSkipTrace = g_tvSkipGiant = 0;
    memset( g_tvIgnKey, 0, (size_t)( 2048 ) * sizeof( g_tvIgnKey[0] ) );
    g_tvWorld = world;
    g_tvKey   = TreeVobsKey();
    g_tvAt    = GetTickCount();
    const bool ok = world && TreeVobsCollectCore( world );
    g_tvCollectMs = DpMs() - t0;
    if( !ok ) {
      MM_LOG( zSTRING( "vob tree: exception while collecting (after " ) + zSTRING( g_tvNodes ) + " nodes)" );
      g_tvCount = 0;
    }
    return ok;
  }
  inline bool TreeVobsEnsure( oCWorld* world ) {
    if( g_tv && g_tvWorld == world && g_tvKey == TreeVobsKey()
        && GetTickCount() - g_tvAt < 30000 )
      return true;
    return TreeVobsCollect( world );
  }
  inline bool TreeBatchFill( TTreeBatch& B, float rx1, float rz1, float rx2, float rz2,
                             int limit ) {
    B.cnt = 0;
    if( limit > B.cap ) limit = B.cap;
    __try {
      for( ; B.vi < g_tvCount; B.vi++, B.ti = 0 ) {
        const TTreeVob& e = g_tv[B.vi];
        if( !e.pm ) continue;
        zCVob* const vob = e.vob;
        const zTBBox3D& bb = vob->bbox3D;
        if( bb.maxs[0] < rx1 || bb.mins[0] > rx2 || bb.maxs[2] < rz1 || bb.mins[2] > rz2 ) continue;
        const zMAT4& M = vob->trafoObjToWorld;
        const zCProgMeshProto* const pm = e.pm;
        const zVEC3* const pos  = pm->posList.array;
        const int          posN = pm->posList.numInArray;
        if( !pos || posN <= 0 || !pm->subMeshList ) continue;
        int flat = 0;
        for( int s = 0; s < pm->numSubMeshes; s++ ) {
          const zCProgMeshProto::zCSubMesh& sm = pm->subMeshList[s];
          const int nt = sm.triList.numInArray;
          if( flat + nt <= B.ti ) {
            flat += nt;
            continue;
          }
          unsigned char cls = e.cls;
          zCMaterial* const mtl = sm.material;
          if( !e.plant && mtl ) {
            const unsigned char mc = GenSurfCached( mtl, mtl->GetObjectName().ToChar(), false );
            if( mc != SURF_TERRAIN ) cls = mc;
          }
          const unsigned char tnt = GenTexOfMaterial( mtl, cls );
          const zTPMTriangle* const tri = sm.triList.array;
          const zTPMWedge*    const wdg = sm.wedgeList.array;
          const int wn = sm.wedgeList.numInArray;
          for( int t = ( B.ti > flat ) ? B.ti - flat : 0; t < nt; t++ ) {
            if( B.cnt >= limit ) {
              B.ti = flat + t;
              return true;
            }
            float* const o = B.v + B.cnt * 9;
            bool bad = false;
            for( int k = 0; k < 3; k++ ) {
              const int w = (int)tri[t].wedge[k];
              const int p = ( w < wn ) ? (int)wdg[w].position : posN;
              if( p >= posN ) { bad = true; break; }
              *(zVEC3*)( o + k * 3 ) = M * pos[p];
            }
            if( bad ) continue;
            const zVEC3* const v = (const zVEC3*)o;
            const zVEC3 nrm = ( v[1] - v[0] ).Cross( v[2] - v[0] );
            const float nl  = nrm.Length();
            float d = ( nl > 1.0e-6f ) ? ( nrm[0] * -0.45f + nrm[1] * 0.78f + nrm[2] * 0.44f ) / nl : 1.0f;
            if( d < 0.0f ) d = -d; else if( d > 1.0f ) d = 1.0f;
            B.lit[B.cnt] = PackLit( 0.58f + 0.42f * d );
            B.cls[B.cnt] = cls;
            B.tnt[B.cnt] = tnt;
            B.cnt++;
          }
          flat += nt;
        }
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  struct TTreeJob { TGenFastCtx* C; const TTreeBatch* B; };
  static void TreeRasterJob( void* ctx, int z0, int z1 ) {
    const TTreeJob* const J = (const TTreeJob*)ctx;
    TGenFastCtx& C = *J->C;
    const TTreeBatch& B = *J->B;
    __try {
      for( int i = 0; i < B.cnt; i++ ) {
        const float* const o = B.v + i * 9;
        const double az = ( (double)o[2] - C.z1 ) * C.sz - 0.5;
        const double bz = ( (double)o[5] - C.z1 ) * C.sz - 0.5;
        const double cz = ( (double)o[8] - C.z1 ) * C.sz - 0.5;
        double zmin = az, zmax = az;
        if( bz < zmin ) zmin = bz;
        if( bz > zmax ) zmax = bz;
        if( cz < zmin ) zmin = cz;
        if( cz > zmax ) zmax = cz;
        if( zmax < (double)z0 || zmin > (double)( z1 - 1 ) ) continue;
        GenRasterTri( C, z0, z1,
                      ( (double)o[0] - C.x1 ) * C.sx - 0.5, az, (double)o[1],
                      ( (double)o[3] - C.x1 ) * C.sx - 0.5, bz, (double)o[4],
                      ( (double)o[6] - C.x1 ) * C.sx - 0.5, cz, (double)o[7],
                      B.cls[i], C.tint ? B.tnt[i] : (unsigned char)0, B.lit[i] );
      }
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      InterlockedExchange( &C.failed, 1 );
    }
  }
  inline int TreeStampCore( TGenFastCtx& C, float rx1, float rz1, float rx2, float rz2 ) {
    int done = 0;
    __try {
      const int n = C.n;
      const double ks = ( C.sx < 0.0 ) ? -C.sx : C.sx;
      for( int i = 0; i < g_tvCount; i++ ) {
        const TTreeVob& e = g_tv[i];
        if( !e.plant || ( g_genTreeShapes >= 2 && e.pm ) ) continue;
        const zTBBox3D& bb = e.vob->bbox3D;
        if( bb.maxs[0] < rx1 || bb.mins[0] > rx2 || bb.maxs[2] < rz1 || bb.mins[2] > rz2 ) continue;
        const float sx = bb.maxs[0] - bb.mins[0], sz = bb.maxs[2] - bb.mins[2];
        double r = (double)( ( sx < sz ) ? sx : sz ) * 0.5 * ks;
        if( r < 0.75 ) r = 0.75; else if( r > 2048.0 ) r = 2048.0;
        const double gx = ( (double)( bb.mins[0] + bb.maxs[0] ) * 0.5 - C.x1 ) * C.sx - 0.5;
        const double gz = ( (double)( bb.mins[2] + bb.maxs[2] ) * 0.5 - C.z1 ) * C.sz - 0.5;
        int x0 = (int)ceil( gx - r ), x1 = (int)floor( gx + r );
        int z0 = (int)ceil( gz - r ), z1 = (int)floor( gz + r );
        if( x1 < 0 || z1 < 0 || x0 > n - 1 || z0 > n - 1 ) continue;
        if( x0 < 0 ) x0 = 0;
        if( z0 < 0 ) z0 = 0;
        if( x1 > n - 1 ) x1 = n - 1;
        if( z1 > n - 1 ) z1 = n - 1;
        const float  top = bb.maxs[1];
        const double rr  = r * r;
        for( int iz = z0; iz <= z1; iz++ )
          for( int ix = x0; ix <= x1; ix++ ) {
            const double dx = (double)ix - gx, dz = (double)iz - gz;
            if( dx * dx + dz * dz > rr ) continue;
            const int q = iz * n + ix;
            const unsigned char m = C.mat[q];
            if( !C.lit[q] || m == SURF_WATER || m == SURF_ROOF || m == SURF_STONEWORK ) continue;
            if( !( top > C.hgt[q] ) ) continue;
            C.hgt[q] = top;
            C.lit[q] = PackLit( 0.86f );
            C.mat[q] = SURF_PLANT;
            if( C.tint ) C.tint[q] = 0;
          }
        done++;
      }
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
    }
    return done;
  }
  inline void TreeLog( const char* where, int stamps, int tris, double ms ) {
    MM_LOG( zSTRING( "vob tree: " ) + zSTRING( g_tvNodes ) + " nodes, plants " + zSTRING( g_tvPlants ) + ", other " + zSTRING( g_tvOthers ) + ", triangles " + zSTRING( tris ) + ", time " + zSTRING( (int)ms ) + " ms (" + zSTRING( where ) + ", GenTreeShapes " + zSTRING( g_genTreeShapes ) + ", rings " + zSTRING( stamps ) + ", skipped hidden " + zSTRING( g_tvSkipHidden ) + " / no rays " + zSTRING( g_tvSkipTrace ) + " / giants " + zSTRING( g_tvSkipGiant ) + ", collect " + zSTRING( (int)g_tvCollectMs ) + " ms)" );
  }
  inline void DpTileRelease( int s ) {
    TDpTile& t = g_dpTile[s];
    if( t.tex ) {
      t.tex->Release();
      t.tex = 0;
    }
    t.size  = 0;
    t.ready = false;
    t.used  = 0;
  }
  inline void DpTilesDrop() {
    for( int s = 0; s < DP_TILES; s++ )
      DpTileRelease( s );
    g_dpCurSlot = -1;
  }
  inline void DetailPatchFree() {
    DpTilesDrop();
    g_dpTileLim = DP_TILES;
    g_dpSigUsed = 0;
    g_dpFmt     = -1;
    g_dpBpp     = 0;
    PkJobWait( g_dpSaveJob );
    g_dpSavePending = false;
    MiniMapCommon::MmFree( g_dpHgt );    g_dpHgt   = 0;
    MiniMapCommon::MmFree( g_dpLit );    g_dpLit   = 0;
    MiniMapCommon::MmFree( g_dpMat );    g_dpMat   = 0;
    MiniMapCommon::MmFree( g_dpAux );    g_dpAux   = 0;
    MiniMapCommon::MmFree( g_dpPx );     g_dpPx    = 0;
    MiniMapCommon::MmFree( g_dpPrev );   g_dpPrev  = 0;
    MiniMapCommon::MmFree( g_dpPolys );  g_dpPolys = 0;  g_dpPolyCap = 0;  g_dpSub = 0;
    MiniMapCommon::MmFree( g_dpHKey );   g_dpHKey  = 0;
    MiniMapCommon::MmFree( g_dpHVal );   g_dpHVal  = 0;
    MiniMapCommon::MmFree( g_dpHTint );  g_dpHTint = 0;
    MiniMapCommon::MmFree( g_dpTint );   g_dpTint  = 0;  g_dpTintN = 0;
    MiniMapCommon::MmFree( g_dpEdges );  g_dpEdges = 0;  g_dpEdgeCnt = 0;
    TreeBatchFree( g_dpTB );
    TreeVobsFree();
    g_dpBufN      = 0;
    g_dpSrc       = 0;
    g_dpSrcNum    = 0;
    g_dpFailed    = false;
    g_dpPhase     = DP_IDLE;
    g_dpMaxN      = 0;
    g_dpKeyDetail = -1;
  }
  static int g_dpPxBpp = 0;
  inline bool DpBuffers( int n ) {
    const int pb = ( g_dpBpp > 0 ) ? g_dpBpp : 4;
    if( g_dpBufN == n && g_dpPxBpp == pb && g_dpHgt && g_dpLit && g_dpMat
        && g_dpAux && g_dpPx && g_dpPrev )
      return true;
    PkJobWait( g_dpSaveJob );
    g_dpSavePending = false;
    MiniMapCommon::MmFree( g_dpHgt );  MiniMapCommon::MmFree( g_dpLit );  MiniMapCommon::MmFree( g_dpMat );
    MiniMapCommon::MmFree( g_dpAux );  MiniMapCommon::MmFree( g_dpPx );   MiniMapCommon::MmFree( g_dpPrev );
    g_dpHgt = g_dpPrev = 0;
    g_dpLit = g_dpMat = g_dpAux = g_dpPx = 0;
    if( CanAllocGrid( n, pb ) ) {
      g_dpHgt  = MiniMapCommon::MmAlloc<float>( (size_t)( n * n ) );
      g_dpLit  = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n ) );
      g_dpMat  = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n ) );
      g_dpAux  = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n ) );
      g_dpPx   = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n * pb ) );
      g_dpPrev = MiniMapCommon::MmAlloc<float>( (size_t)( n ) );
    }
    g_dpBufN = n;
    g_dpPxBpp = pb;
    if( g_dpHgt && g_dpLit && g_dpMat && g_dpAux && g_dpPx && g_dpPrev ) return true;
    MiniMapCommon::MmFree( g_dpHgt );  MiniMapCommon::MmFree( g_dpLit );  MiniMapCommon::MmFree( g_dpMat );
    MiniMapCommon::MmFree( g_dpAux );  MiniMapCommon::MmFree( g_dpPx );   MiniMapCommon::MmFree( g_dpPrev );
    g_dpHgt = g_dpPrev = 0;
    g_dpLit = g_dpMat = g_dpAux = g_dpPx = 0;
    g_dpBufN = 0;
    return false;
  }
  inline zCTexture* DpEnsureTex( int s, int n ) {
    if( s < 0 || s >= DP_TILES ) return 0;
    TDpTile& tile = g_dpTile[s];
    if( tile.tex && tile.size == n ) return tile.tex;
    DpTileRelease( s );
    zTRnd_TextureFormat fmt = zRND_TEX_FORMAT_ARGB_4444;
    int bpp = 0;
    if( !PickTexFormat( false, fmt, bpp ) ) return 0;
    zCTexture* tex = CreateTex2D( n, n, fmt, zCOLOR( 120, 128, 100, 255 ), true );
    if( !tex ) return 0;
    tile.tex = tex; tile.size = n; tile.ready = false;
    g_dpFmt = (int)fmt; g_dpBpp = bpp;
    return tex;
  }
  struct TDpSlice { TGenParFn fn; void* ctx; int base; };
  static void DpSliceJob( void* c, int a, int b ) {
    const TDpSlice* const s = (const TDpSlice*)c;
    s->fn( s->ctx, s->base + a, s->base + b );
  }
  inline void DpParallel( TGenParFn fn, void* ctx, int a, int b ) {
    if( b <= a ) return;
    if( b - a < 64 ) {
      fn( ctx, a, b );
      return;
    }
    TDpSlice s;
    s.fn   = fn;
    s.ctx  = ctx;
    s.base = a;
    int chunk = ( b - a ) / ( GenParThreads() * 2 );
    if( chunk < 1 ) chunk = 1;
    GenParallel( DpSliceJob, &s, b - a, chunk );
  }
  static void DpRasterJob( void* ctx, int z0, int z1 ) {
    TGenFastCtx* const C = (TGenFastCtx*)ctx;
    const int n = C->n;
    for( int i = z0 * n; i < z1 * n; i++ ) {
      C->hgt[i] = GEN_FAST_EMPTY;
      C->lit[i] = 0;
      C->mat[i] = 0;
    }
    if( C->tint )
      for( int i = z0 * n; i < z1 * n; i++ )
        C->tint[i] = 0;
    __try {
      GenFastRaster( *C, z0, z1 );
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      InterlockedExchange( &C->failed, 1 );
    }
  }
  static void DpPathsJob( void* ctx, int z0, int z1 ) {
    const TDpPaths& P = *(const TDpPaths*)ctx;
    const int n = P.n;
    for( int k = 0; k < P.ne; k++ ) {
      const TEdge& e = P.e[k];
      const float ex1 = ( e.ax < e.bx ) ? e.ax : e.bx;
      const float ex2 = ( e.ax < e.bx ) ? e.bx : e.ax;
      const float ez1 = ( e.az < e.bz ) ? e.az : e.bz;
      const float ez2 = ( e.az < e.bz ) ? e.bz : e.az;
      float fa = (float)( ( ex1 - P.r - P.x1 ) / P.cw ), fb = (float)( ( ex2 + P.r - P.x1 ) / P.cw );
      float ga = (float)( ( ez1 - P.r - P.z1 ) / P.ch ), gb = (float)( ( ez2 + P.r - P.z1 ) / P.ch );
      if( fa > fb ) { std::swap( fa, fb ); }
      if( ga > gb ) { std::swap( ga, gb ); }
      if( fb < 0.0f || ga > (float)( z1 - 1 ) || gb < (float)z0 || fa > (float)( n - 1 ) ) continue;
      int ix0 = (int)fa - 1, ix1 = (int)fb + 1;
      int iz0 = (int)ga - 1, iz1 = (int)gb + 1;
      if( ix0 < 0 )      ix0 = 0;
      if( ix1 > n - 1 )  ix1 = n - 1;
      if( iz0 < z0 )     iz0 = z0;
      if( iz1 > z1 - 1 ) iz1 = z1 - 1;
      const float dx = e.bx - e.ax, dz = e.bz - e.az;
      const float len2 = dx * dx + dz * dz;
      for( int iz = iz0; iz <= iz1; iz++ ) {
        const float wz = (float)( P.z1 + P.ch * ( (double)iz + 0.5 ) );
        for( int ix = ix0; ix <= ix1; ix++ ) {
          const float wx = (float)( P.x1 + P.cw * ( (double)ix + 0.5 ) );
          float t = ( len2 > 0.0001f )
                  ? ( ( wx - e.ax ) * dx + ( wz - e.az ) * dz ) / len2 : 0.0f;
          if( t < 0.0f ) t = 0.0f; else if( t > 1.0f ) t = 1.0f;
          const float ox = e.ax + t * dx - wx, oz = e.az + t * dz - wz;
          if( ox * ox + oz * oz > P.r2 ) continue;
          const int q = iz * n + ix;
          if( !P.lit[q] ) continue;
          const unsigned char c = P.mat[q];
          if( c == SURF_ROOF || c == SURF_PATH ) continue;
          if( c == SURF_WATER && !P.water ) continue;
          P.mat[q] = SURF_PATH;
        }
      }
    }
  }
  inline void DpFillFwd( int z0, int z1 ) {
    const int n = g_dpN, D = g_dpFillMax;
    float* const hgt = g_dpHgt;
    unsigned char* const lit = g_dpLit;
    unsigned char* const mat = g_dpMat;
    unsigned char* const aux = g_dpAux;
    for( int iz = z0; iz < z1; iz++ )
      for( int ix = 0; ix < n; ix++ ) {
        const int q = iz * n + ix;
        if( lit[q] != 0 ) {
          aux[q] = 0;
          continue;
        }
        int best = 255, src = -1;
        if( ix > 0 && aux[q - 1] < best ) { best = aux[q - 1]; src = q - 1; }
        if( iz > 0 && aux[q - n] < best ) { best = aux[q - n]; src = q - n; }
        if( src >= 0 && best + 1 <= D ) {
          hgt[q] = hgt[src];
          lit[q] = lit[src];
          mat[q] = mat[src];
          if( g_dpFast.tint ) g_dpFast.tint[q] = g_dpFast.tint[src];
          aux[q] = (unsigned char)( best + 1 );
        }
        else
          aux[q] = 255;
      }
  }
  inline void DpFillBwd( int z0, int z1 ) {
    const int n = g_dpN, D = g_dpFillMax;
    float* const hgt = g_dpHgt;
    unsigned char* const lit = g_dpLit;
    unsigned char* const mat = g_dpMat;
    unsigned char* const aux = g_dpAux;
    for( int iz = z1 - 1; iz >= z0; iz-- )
      for( int ix = n - 1; ix >= 0; ix-- ) {
        const int q = iz * n + ix;
        const int cur = aux[q];
        if( cur == 0 ) continue;
        int best = cur, src = -1;
        if( ix < n - 1 && aux[q + 1] + 1 < best ) { best = aux[q + 1] + 1; src = q + 1; }
        if( iz < n - 1 && aux[q + n] + 1 < best ) { best = aux[q + n] + 1; src = q + n; }
        if( src >= 0 && best <= D ) {
          hgt[q] = hgt[src];
          lit[q] = lit[src];
          mat[q] = mat[src];
          if( g_dpFast.tint ) g_dpFast.tint[q] = g_dpFast.tint[src];
          aux[q] = (unsigned char)best;
        }
      }
  }
  inline bool DpPolysCore( int from, int to ) {
    __try {
      const TMapInfo& r = g_dpNext;
      const float x1 = ( r.x1 < r.x2 ) ? r.x1 : r.x2, x2 = ( r.x1 < r.x2 ) ? r.x2 : r.x1;
      const float z1 = ( r.y1 < r.y2 ) ? r.y1 : r.y2, z2 = ( r.y1 < r.y2 ) ? r.y2 : r.y1;
      for( int p = from; p < to; p++ ) {
        zCPolygon* const poly = g_dpSrc[p];
        if( !GenFastPolyOk( poly ) ) continue;
        const int nv = (int)poly->polyNumVert;
        float ax = 1.0e30f, bx = -1.0e30f, az = 1.0e30f, bz = -1.0e30f;
        bool  bad = false;
        for( int v = 0; v < nv; v++ ) {
          const zCVertex* const vx = poly->vertex[v];
          if( !vx ) { bad = true; break; }
          const float px = vx->position[0], pz = vx->position[2];
          if( px < ax ) ax = px;
          if( px > bx ) bx = px;
          if( pz < az ) az = pz;
          if( pz > bz ) bz = pz;
        }
        if( bad || bx < x1 || ax > x2 || bz < z1 || az > z2 ) continue;
        if( g_dpSub >= g_dpPolyCap ) {
          const int cap = ( g_dpPolyCap < 8192 ) ? 8192 : g_dpPolyCap * 2;
          zCPolygon** const nl = MiniMapCommon::MmAlloc<zCPolygon*>( (size_t)( cap ) );
          if( !nl ) return false;
          memcpy( nl, g_dpPolys, (size_t)( g_dpSub ) * sizeof( nl[0] ) );
          MiniMapCommon::MmFree( g_dpPolys );
          g_dpPolys   = nl;
          g_dpPolyCap = cap;
        }
        g_dpPolys[g_dpSub++] = poly;
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline int GenInsertMatHash( zCMaterial* mtl, const void** hkey, unsigned char* hval,
                               unsigned char* htnt, unsigned int hmask, int mats, int maxMats ) {
    if( !mtl ) return 0;
    unsigned int s = GenFastHash( mtl, hmask );
    while( hkey[s] && hkey[s] != mtl ) s = ( s + 1 ) & hmask;
    if( hkey[s] ) return 0;
    if( mats >= maxMats ) return -1;
    const int grp = (int)mtl->matGroup;
    const char* const nm = mtl->GetObjectName().ToChar();
    unsigned char cls = ( grp == (int)zMAT_GROUP_WATER && !GenWaterGroupIsLand( nm ) ) ? SURF_WATER
                      : ( grp == 6 )     ? SURF_SNOW
                      : GenMatSkip( nm ) ? SURF_SKIP
                                         : GenSurfCached( mtl, nm, false );
    if( GenCrownPoly( cls, nm ) ) cls = SURF_SKIP;
    hkey[s] = mtl;
    hval[s] = cls;
    if( htnt ) htnt[s] = ( cls == SURF_SKIP ) ? 0 : GenTexOfMaterial( mtl, cls );
    return 1;
  }
  inline bool DpMatsCore() {
    const unsigned int HSIZE = 16384, hmask = HSIZE - 1;
    if( !g_dpHKey ) g_dpHKey = MiniMapCommon::MmAlloc<const void*>( (size_t)( HSIZE ) );
    if( !g_dpHVal ) g_dpHVal = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( HSIZE ) );
    if( !g_dpHKey || !g_dpHVal ) return false;
    memset( g_dpHKey, 0, HSIZE * sizeof( void* ) );
    memset( g_dpHVal, 0, HSIZE );
    if( g_genTexColors ) {
      if( !g_dpHTint ) g_dpHTint = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( HSIZE ) );
      if( g_dpHTint ) memset( g_dpHTint, 0, HSIZE );
    }
    __try {
      int mats = 0;
      for( int p = 0; p < g_dpSub; p++ )
        if( GenInsertMatHash( g_dpPolys[p]->material, g_dpHKey, g_dpHVal, g_genTexColors ? g_dpHTint : 0, hmask, mats, (int)( HSIZE / 4 * 3 ) ) > 0 )
          mats++;
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline bool DpVobsCore( oCWorld* world ) {
    g_dpS   = 1;
    g_dpNB  = 0;
    g_dpCur = 0;
    if( !g_genVobs || !world || !world->voblist || !world->bspTree.bspRoot ) return true;
    __try {
      const int n = g_dpN;
      const TMapInfo& r = g_dpNext;
      const double sx = g_dpSXd;
      const double sz = g_dpSZd;
      const float rx1 = ( r.x1 < r.x2 ) ? r.x1 : r.x2, rx2 = ( r.x1 < r.x2 ) ? r.x2 : r.x1;
      const float rz1 = ( r.y1 < r.y2 ) ? r.y1 : r.y2, rz2 = ( r.y1 < r.y2 ) ? r.y2 : r.y1;
      const float minBig = ( ( g_genVobMin < g_genPlantMin )
                           ? (float)g_genVobMin : (float)g_genPlantMin ) * DqVobScale();
      const double rayBudget = (double)kDqRays[g_detailQuality];
      (void)rayBudget;
      g_dpS  = DpRayBlock( g_dpCoreN, g_dpPCell );
      g_dpBx = DpFloorMod( g_dpOx, g_dpS );
      g_dpBz = DpFloorMod( g_dpOz, g_dpS );
      g_dpNB = ( n + ( ( g_dpBx > g_dpBz ) ? g_dpBx : g_dpBz ) + g_dpS - 1 ) / g_dpS;
      memset( g_dpAux, 0, g_dpNB * g_dpNB );
      {
        for( zCListSort<zCVob>* node = world->voblist; node; node = node->next ) {
          zCVob* const vob = node->data;
          if( !vob || vob->type == zVOB_TYPE_LEVEL_COMPONENT || !vob->GetVisual() ) continue;
          const zTBBox3D& vb = vob->bbox3D;
          if( vb.maxs[0] < rx1 || vb.mins[0] > rx2 || vb.maxs[2] < rz1 || vb.mins[2] > rz2 ) continue;
          const float bsx = vb.maxs[0] - vb.mins[0];
          const float bsz = vb.maxs[2] - vb.mins[2];
          if( !( ( ( bsx > bsz ) ? bsx : bsz ) >= minBig ) ) continue;
          double ax = ( (double)vb.mins[0] - g_dpX1d ) * sx - 0.5;
          double bx = ( (double)vb.maxs[0] - g_dpX1d ) * sx - 0.5;
          double az = ( (double)vb.mins[2] - g_dpZ1d ) * sz - 0.5;
          double bz = ( (double)vb.maxs[2] - g_dpZ1d ) * sz - 0.5;
          if( !( ax == ax && bx == bx && az == az && bz == bz ) ) continue;
          if( ax > bx ) { const double t = ax; ax = bx; bx = t; }
          if( az > bz ) { const double t = az; az = bz; bz = t; }
          ax = ceil( ax ) - 1.0;
          bx = floor( bx ) + 1.0;
          az = ceil( az ) - 1.0;
          bz = floor( bz ) + 1.0;
          if( bx < 0.0 || bz < 0.0 || ax > (double)( n - 1 ) || az > (double)( n - 1 ) ) continue;
          if( ax < 0.0 ) ax = 0.0;
          if( az < 0.0 ) az = 0.0;
          if( bx > (double)( n - 1 ) ) bx = (double)( n - 1 );
          if( bz > (double)( n - 1 ) ) bz = (double)( n - 1 );
          const int s = g_dpS, nb = g_dpNB;
          for( int jz = DpBlockFirst( (int)az, g_dpBz, s ); jz <= DpBlockFirst( (int)bz, g_dpBz, s ); jz++ )
            for( int jx = DpBlockFirst( (int)ax, g_dpBx, s ); jx <= DpBlockFirst( (int)bx, g_dpBx, s ); jx++ )
              g_dpAux[jz * nb + jx] = 1;
        }
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      g_dpNB = 0;
      return false;
    }
  }
  inline bool DpRaysCore( oCWorld* world, double until ) {
    __try {
      const int n = g_dpN, s = g_dpS, nb = g_dpNB, total = nb * nb;
      const TMapInfo& r = g_dpNext;
      (void)r;
      const float top = world->bspTree.bspRoot->bbox3D.maxs[1] + 500.0f;
      const int flagsVob = zTRACERAY_STAT_POLY | zTRACERAY_POLY_NORMAL
                         | zTRACERAY_VOB_IGNORE_CHARACTER;
      int k = 0;
      while( g_dpCur < total ) {
        if( ( ++k & 31 ) == 0 && DpMs() >= until ) break;
        const int bi = g_dpCur++;
        if( !g_dpAux[bi] ) continue;
        const int bz0 = DpBlockStart( bi / nb, g_dpBz, s ), bx0 = DpBlockStart( bi % nb, g_dpBx, s );
        if( bx0 >= n || bz0 >= n ) continue;
        int cx = bx0 + s / 2, cz = bz0 + s / 2;
        if( cx < 0 ) cx = 0;
        if( cz < 0 ) cz = 0;
        if( cx > n - 1 ) cx = n - 1;
        if( cz > n - 1 ) cz = n - 1;
        const int i   = cz * n + cx;
        if( !g_dpLit[i] ) continue;
        const float terrY = g_dpHgt[i];
        if( top <= terrY + 50.0f ) continue;
        const zVEC3 from( (float)( g_dpX1d + ( (double)cx + 0.5 ) / g_dpSXd ), top,
                          (float)( g_dpZ1d + ( (double)cz + 0.5 ) / g_dpSZd ) );
        const zVEC3 ray( 0.0f, -( top - ( terrY + 50.0f ) ), 0.0f );
        g_dpRays++;
        if( !world->TraceRayNearestHit( from, ray, (zCVob*)0, flagsVob )
            || !world->traceRayReport.foundHit
            || !world->traceRayReport.foundVob )
          continue;
        zCVob* const    vob  = world->traceRayReport.foundVob;
        const float     vobY = world->traceRayReport.foundIntersection[1];
        const zTBBox3D& vbb  = vob->bbox3D;
        const float     bsx  = vbb.maxs[0] - vbb.mins[0];
        const float     bsz  = vbb.maxs[2] - vbb.mins[2];
        const float     big  = ( bsx > bsz ) ? bsx : bsz;
        zCVisual* const vis  = vob->GetVisual();
        const char* const vnm = vis ? vis->GetObjectName().ToChar() : 0;
        const unsigned char cls = GenSurfCached( vis, vnm, true );
        const float min = ( ( cls == SURF_PLANT ) ? (float)g_genPlantMin
                                                  : (float)g_genVobMin ) * DqVobScale();
        if( !( big >= min && vobY > terrY + 50.0f ) ) continue;
        if( cls == SURF_PLANT && TreeGiantSize( bsx, bsz, vbb.maxs[1] - vbb.mins[1], vnm ) ) continue;
        for( int zz = ( bz0 > 0 ) ? bz0 : 0; zz < bz0 + s && zz < n; zz++ )
          for( int xx = ( bx0 > 0 ) ? bx0 : 0; xx < bx0 + s && xx < n; xx++ ) {
            const int q = zz * n + xx;
            if( !g_dpLit[q] ) continue;
            if( vobY > g_dpHgt[q] + 50.0f ) {
              g_dpHgt[q] = vobY;
              g_dpMat[q] = cls;
              if( g_dpFast.tint ) g_dpFast.tint[q] = 0;
            }
          }
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline bool DpPlantsCore( oCWorld* world ) {
    if( !g_genVobs || !world || !world->voblist ) return true;
    __try {
      const int n = g_dpN;
      const TMapInfo& r = g_dpNext;
      const float pc = ( g_dpPCell > 0.01f ) ? g_dpPCell : 1.0f;
      const float gc = ( g_dpGCell > 0.01f ) ? g_dpGCell : 1.0f;
      const float rx1 = ( r.x1 < r.x2 ) ? r.x1 : r.x2, rx2 = ( r.x1 < r.x2 ) ? r.x2 : r.x1;
      const float rz1 = ( r.y1 < r.y2 ) ? r.y1 : r.y2, rz2 = ( r.y1 < r.y2 ) ? r.y2 : r.y1;
      const float farW = gc * 10.0f;
      for( zCListSort<zCVob>* node = world->voblist; node; node = node->next ) {
        zCVob* const vob = node->data;
        if( !vob ) continue;
        const zTBBox3D& bb2 = vob->bbox3D;
        if( bb2.maxs[0] + farW < rx1 || bb2.mins[0] - farW > rx2
            || bb2.maxs[2] + farW < rz1 || bb2.mins[2] - farW > rz2 )
          continue;
        zCVisual* const vis = vob->GetVisual();
        if( !vis ) continue;
        const float bsx = bb2.maxs[0] - bb2.mins[0];
        const float bsz = bb2.maxs[2] - bb2.mins[2];
        const float big = ( bsx > bsz ) ? bsx : bsz;
        if( big < (float)g_genPlantMin ) continue;
        if( GenSurfCached( vis, vis->GetObjectName().ToChar(), true ) != SURF_PLANT ) continue;
        float radW = big * 0.5f;
        if( radW > gc * 10.0f ) radW = gc * 10.0f;
        if( radW < gc )         radW = gc;
        int rad = (int)( radW / pc );
        if( rad < 1 ) rad = 1; else if( rad > 512 ) rad = 512;
        const zVEC3 pos = vob->GetPositionWorld();
        const int cx = (int)floor( ( (double)pos[0] - g_dpX1d ) * g_dpSXd );
        const int cz = (int)floor( ( (double)pos[2] - g_dpZ1d ) * g_dpSZd );
        if( cx < -rad || cx >= n + rad || cz < -rad || cz >= n + rad ) continue;
        const int   rr  = rad * rad;
        const float top = bb2.maxs[1];
        for( int dz = -rad; dz <= rad; dz++ ) {
          const int zz = cz + dz;
          if( zz < 0 || zz >= n ) continue;
          for( int dx = -rad; dx <= rad; dx++ ) {
            if( dx * dx + dz * dz > rr ) continue;
            const int xx = cx + dx;
            if( xx < 0 || xx >= n ) continue;
            const int q = zz * n + xx;
            if( !g_dpLit[q] || g_dpMat[q] == SURF_WATER ) continue;
            if( g_dpMat[q] == SURF_ROOF || g_dpMat[q] == SURF_STONEWORK ) continue;
            g_dpMat[q] = SURF_PLANT;
            if( top > g_dpHgt[q] )
              g_dpHgt[q] += ( top - g_dpHgt[q] ) * 0.35f;
          }
        }
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline void DpFail( const char* why ) {
    g_dpPhase = DP_IDLE;
    g_dpFailed = true;
    MM_LOG( zSTRING( "detail: aborted (" ) + zSTRING( why ) + ")" );
  }
  inline void DpPhase( int ph ) {
    g_dpPhase = ph;
    g_dpCur   = 0;
    g_dpSlice = 4;
  }
  inline void DpAdapt( int& slice, double ms, int lo, int hi ) {
    const double want = g_dpFrameMs * 0.4;
    double k = want / ( ( ms > 0.05 ) ? ms : 0.05 );
    if( k > 2.0 ) k = 2.0;
    if( k > 0.8 && k < 1.25 ) return;
    int s = (int)( (double)slice * k );
    if( s < lo ) s = lo; else if( s > hi ) s = hi;
    slice = s;
  }
  inline void DpSetupRaster() {
    TGenFastCtx& C = g_dpFast;
    const TMapInfo& r = g_dpNext;
    C.polys   = g_dpPolys;
    C.numPoly = g_dpSub;
    C.n       = g_dpN;
    (void)r;
    C.x1      = g_dpX1d;
    C.z1      = g_dpZ1d;
    C.sx      = g_dpSXd;
    C.sz      = g_dpSZd;
    C.ceil    = 1.0e30f;
    C.hgt     = g_dpHgt;
    C.lit     = g_dpLit;
    C.mat     = g_dpMat;
    C.hkey    = g_dpHKey;
    C.hval    = g_dpHVal;
    C.hmask   = 16384 - 1;
    C.htint   = 0;
    C.tint    = 0;
    if( g_genTexColors && g_dpHTint ) {
      const int nn = g_dpN * g_dpN;
      if( !g_dpTint || g_dpTintN != nn ) {
        MiniMapCommon::MmFree( g_dpTint );
        g_dpTint  = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( nn ) );
        g_dpTintN = g_dpTint ? nn : 0;
      }
      if( g_dpTint ) {
        C.htint = g_dpHTint;
        C.tint  = g_dpTint;
      }
    }
    C.lx      = -0.45f;
    C.ly      =  0.78f;
    C.lz      =  0.44f;
    C.rows    = 0;
    C.chunks  = 0;
    C.next    = 0;
    C.failed  = 0;
  }
  inline void DpSetupPaths() {
    g_dpEdgeCnt = 0;
    if( !g_genPaths ) return;
    EnsureEdges();
    if( g_edgeCount <= 0 || !g_edges ) return;
    MiniMapCommon::MmFree( g_dpEdges );
    g_dpEdges = MiniMapCommon::MmAlloc<TEdge>( (size_t)( g_edgeCount ) );
    if( !g_dpEdges ) return;
    const TMapInfo& r = g_dpNext;
    const float gc = ( g_dpGCell > 0.01f ) ? g_dpGCell : 1.0f;
    float rad = (float)g_genPathWide * 0.5f;
    if( rad < gc ) rad = gc; else if( rad > gc * 24.0f ) rad = gc * 24.0f;
    const float rx1 = ( r.x1 < r.x2 ) ? r.x1 : r.x2, rx2 = ( r.x1 < r.x2 ) ? r.x2 : r.x1;
    const float rz1 = ( r.y1 < r.y2 ) ? r.y1 : r.y2, rz2 = ( r.y1 < r.y2 ) ? r.y2 : r.y1;
    for( int e = 0; e < g_edgeCount; e++ ) {
      const TEdge& ed = g_edges[e];
      const float ax = ( ed.ax < ed.bx ) ? ed.ax : ed.bx, bx = ( ed.ax < ed.bx ) ? ed.bx : ed.ax;
      const float az = ( ed.az < ed.bz ) ? ed.az : ed.bz, bz = ( ed.az < ed.bz ) ? ed.bz : ed.az;
      if( bx + rad < rx1 || ax - rad > rx2 || bz + rad < rz1 || az - rad > rz2 ) continue;
      g_dpEdges[g_dpEdgeCnt++] = ed;
    }
    TDpPaths& P = g_dpPaths;
    P.e     = g_dpEdges;
    P.ne    = g_dpEdgeCnt;
    P.n     = g_dpN;
    P.water = g_genPathsWater;
    P.x1    = g_dpX1d;
    P.z1    = g_dpZ1d;
    P.cw    = 1.0 / g_dpSXd;
    P.ch    = 1.0 / g_dpSZd;
    P.r     = rad;
    P.r2    = rad * rad;
    P.mat   = g_dpMat;
    P.lit   = g_dpLit;
  }
  inline void DpSetupSmooth() {
    g_dpSweepCnt = 0;
    if( g_genSmooth <= 0 ) return;
    const int n = g_dpN;
    g_dpSm.hgt  = g_dpHgt;
    g_dpSm.lit  = g_dpLit;
    g_dpSm.prev = g_dpPrev;
    g_dpSm.n    = n;
    for( int sm = 0; sm < g_genSmooth && sm < 2; sm++ ) {
      g_dpSweep[g_dpSweepCnt].fn = GenSmoothRowsJob;  g_dpSweep[g_dpSweepCnt++].ctx = &g_dpSm;
      g_dpSweep[g_dpSweepCnt].fn = GenSmoothColsJob;  g_dpSweep[g_dpSweepCnt++].ctx = &g_dpSm;
    }
  }
  inline void DpHeightScale( oCWorld* world, float& hmin, float& hmax ) {
    hmin = g_genHMin;
    hmax = g_genHMax;
    if( !g_genHKnown && world && world->bspTree.bspRoot ) {
      hmin = world->bspTree.bspRoot->bbox3D.mins[1];
      hmax = world->bspTree.bspRoot->bbox3D.maxs[1];
    }
  }
  inline void DpSetupShade( oCWorld* world ) {
    float hmin = 0.0f, hmax = 0.0f;
    DpHeightScale( world, hmin, hmax );
    float range = hmax - hmin;
    if( range < 1.0f ) range = 1.0f;
    const TMapInfo& r = g_dpNext;
    (void)r;
    float cellX = (float)( 1.0 / g_dpSXd );
    float cellZ = (float)( 1.0 / g_dpSZd );
    if( cellX > -0.01f && cellX < 0.01f ) cellX = 1.0f;
    if( cellZ > -0.01f && cellZ < 0.01f ) cellZ = 1.0f;
    DpSaveFinish( true );
    TGenShade& S  = g_dpShade;
    S.n           = g_dpN;
    S.bpp         = g_dpBpp;
    S.hgt         = g_dpHgt;
    S.lit         = g_dpLit;
    S.mat         = g_dpMat;
    S.px          = g_dpPx;
    S.hmin        = hmin;
    S.range       = range;
    S.relief      = (float)g_genRelief / 100.0f * (float)kDqReliefPct[g_detailQuality] / 100.0f;
    S.cellX       = cellX;
    S.cellZ       = cellZ;
    S.lx          = -0.45f;
    S.ly          =  0.78f;
    S.lz          =  0.44f;
    S.fmt         = g_dpFmt;
    S.edge        = ( g_genEdge + kDqEdgeAdd[g_detailQuality] > 60 ) ? 60
                  : g_genEdge + kDqEdgeAdd[g_detailQuality];
    S.contour     = g_genContour;
    S.contourDark = g_genContourDark;
    S.wall        = 0;
    S.emptyC      = 0;
    S.lowY        = -1.0e30f;
    S.sculpt      = g_genSculpt;
    S.tex         = g_genTexColors;
    S.tint        = g_dpFast.tint;
    S.ox          = g_dpOx;
    S.oz          = g_dpOz;
  }
  inline void DpTileRect( const TMapInfo& m, int n, int neq, int ix, int iz,
                          TMapInfo& r ) {
    const double sx = ( (double)m.x2 - (double)m.x1 ) * (double)n / (double)neq;
    const double sz = ( (double)m.y2 - (double)m.y1 ) * (double)n / (double)neq;
    r.x1 = (float)( (double)m.x1 + sx * (double)ix );
    r.x2 = (float)( (double)m.x1 + sx * (double)( ix + 1 ) );
    r.y1 = (float)( (double)m.y1 + sz * (double)iz );
    r.y2 = (float)( (double)m.y1 + sz * (double)( iz + 1 ) );
    r.hasCoords = true;
    r.tag       = 0;
    r.world[0]  = 0;
  }
  inline int DpFloorMod( int v, int m ) {
    const int r = v % m;
    return ( r < 0 ) ? r + m : r;
  }
  inline int DpRayBlock( int n, float cell ) {
    static const float kCm[4] = { 40.0f, 20.0f, 10.0f, 10.0f };
    const int q = ( g_detailQuality >= 0 && g_detailQuality <= 3 ) ? g_detailQuality : 0;
    int s = 1;
    if( cell > 0.01f )
      while( (float)( s * 2 ) * cell <= kCm[q] * 1.25f && s < 64 )
        s *= 2;
    const double cells = (double)n * (double)n, budget = (double)kDqRays[q];
    while( cells / ( (double)s * (double)s ) > budget * 64.0 && s < 64 )
      s *= 2;
    return s;
  }
  inline int DpPadCells( int n, float cell, int fillMax, int block ) {
    const float shadeCm = 800.0f;
    int p = ( cell > 0.01f ) ? (int)( shadeCm / cell + 0.999f ) : 64;
    if( p < 8 ) p = 8;
    p += fillMax + block + 4;
    const int hi = ( n / 4 > 24 ) ? n / 4 : 24;
    if( p > hi ) p = hi;
    return p;
  }
  inline void DpWorkOrigin( const TMapInfo& m, int neq, int ox, int oz,
                           double& x1, double& z1, double& sx, double& sz ) {
    const double cx = ( (double)m.x2 - (double)m.x1 ) / (double)neq;
    const double cz = ( (double)m.y2 - (double)m.y1 ) / (double)neq;
    x1 = (double)m.x1 + cx * (double)ox;
    z1 = (double)m.y1 + cz * (double)oz;
    sx = 1.0 / cx;
    sz = 1.0 / cz;
  }
  inline int DpBlockFirst( int c, int off, int s ) { return ( c + off ) / s; }
  inline int DpBlockStart( int j, int off, int s ) { return j * s - off; }
  inline void DpTileRectPad( const TMapInfo& m, int n, int neq, int ix, int iz,
                             int pad, TMapInfo& r ) {
    DpTileRect( m, n, neq, ix, iz, r );
    const double cx = ( (double)r.x2 - (double)r.x1 ) / (double)n;
    const double cz = ( (double)r.y2 - (double)r.y1 ) / (double)n;
    r.x1 = (float)( (double)r.x1 - cx * (double)pad );
    r.x2 = (float)( (double)r.x2 + cx * (double)pad );
    r.y1 = (float)( (double)r.y1 - cz * (double)pad );
    r.y2 = (float)( (double)r.y2 + cz * (double)pad );
  }
  inline void DpCropPx() {
    const int n = g_dpCoreN, N = g_dpN, p = g_dpPad, bpp = g_dpBpp;
    if( !g_dpPx || n <= 0 || p < 0 || N != n + 2 * p || bpp <= 0 ) return;
    const size_t row = (size_t)n * (size_t)bpp;
    for( int z = 0; z < n; z++ )
      memmove( g_dpPx + (size_t)z * row,
               g_dpPx + ( (size_t)( z + p ) * (size_t)N + (size_t)p ) * (size_t)bpp,
               row );
  }
  const double DP_HYST = 0.2;
  static int g_dpWantIx = 0x7FFFFFFF, g_dpWantOx = 0;
  static int g_dpWantIz = 0x7FFFFFFF, g_dpWantOz = 0;
  inline int DpWantSide( int i, double f, int pi, int po ) {
    if( i == pi && ( po == i - 1 || po == i + 1 ) ) {
      if( po == i - 1 ) return ( f > 0.5 + DP_HYST ) ? i + 1 : i - 1;
      return ( f < 0.5 - DP_HYST ) ? i - 1 : i + 1;
    }
    if( pi == i - 1 || pi == i + 1 ) {
      if( po == i ) return pi;
    }
    return ( f < 0.5 ) ? i - 1 : i + 1;
  }
  inline int DpWanted( const TMapInfo& m, int n, int neq, float px, float pz,
                       int lim, int* wx, int* wz ) {
    const double sx = ( (double)m.x2 - (double)m.x1 ) * (double)n / (double)neq;
    const double sz = ( (double)m.y2 - (double)m.y1 ) * (double)n / (double)neq;
    if( sx == 0.0 || sz == 0.0 ) return 0;
    const double fx = ( (double)px - (double)m.x1 ) / sx;
    const double fz = ( (double)pz - (double)m.y1 ) / sz;
    if( !( fx == fx && fz == fz ) || fx < -1.0e6 || fx > 1.0e6 || fz < -1.0e6 || fz > 1.0e6 ) return 0;
    const int ix = (int)floor( fx ), iz = (int)floor( fz );
    const int ox = DpWantSide( ix, fx - (double)ix, g_dpWantIx, g_dpWantOx );
    const int oz = DpWantSide( iz, fz - (double)iz, g_dpWantIz, g_dpWantOz );
    g_dpWantIx = ix;  g_dpWantOx = ox;
    g_dpWantIz = iz;  g_dpWantOz = oz;
    const int cx[4] = { ix, ox, ix, ox };
    const int cz[4] = { iz, iz, oz, oz };
    if( lim > 4 ) lim = 4;
    for( int k = 0; k < lim; k++ ) {
      wx[k] = cx[k];
      wz[k] = cz[k];
    }
    return lim;
  }
  inline int DpTileSig( const TMapInfo& m, int n, int neq, int gN,
                        float hmin, float hmax ) {
    const int vals[10] = {
      GenSignature( m ), DP_TILE_VERSION, n, neq, gN, g_genDetail,
      g_genTexColors, GenAlgoVersion(), GenHPack( hmin, hmax ), (int)g_worldTag
    };
    unsigned int h = 2166136261u;
    for( int i = 0; i < 10; i++ ) {
      h ^= (unsigned int)vals[i];
      h *= 16777619u;
    }
    if( g_detailQuality > 0 ) {
      h ^= 0x44510000u | (unsigned int)g_detailQuality;
      h *= 16777619u;
    }
    return (int)( h & 0x7FFFFFFF );
  }
  const char* const DP_DIR = "MiniMap\\details";
  const int DP_NAME = 160;
  inline bool DpDiskOn() {
    return g_genCache != 0 && g_genDetailCacheMB > 0 && g_world[0] != 0;
  }
  inline int DpPutInt( char* out, int at, int max, int v ) {
    if( !out || at < 0 || at >= max ) return at;
    const unsigned int u = ( v < 0 ) ? ( (unsigned int)( -( v + 1 ) ) + 1u ) : (unsigned int)v;
    const int n = snprintf( out + at, (size_t)( max - at ), "%s%u", ( v < 0 ) ? "m" : "", u );
    return ( n > 0 ) ? ( ( at + n < max ) ? ( at + n ) : ( max - 1 ) ) : at;
  }
  inline bool DpTileName( int sig, int ix, int iz, char* out ) {
    char base[NAME_MAX];
    GenCacheName( g_world, base, NAME_MAX );
    if( _strnicmp( base, "MiniMap_gen_", 12 ) != 0 ) return false;
    const char* b = base + 12;
    size_t len = strlen( b );
    if( len > 6 && _stricmp( b + len - 6, ".cache" ) == 0 ) len -= 6;
    char mid[NAME_MAX];
    strncpy_s( mid, sizeof( mid ), b, len );
    char xbuf[16], zbuf[16];
    DpPutInt( xbuf, 0, sizeof( xbuf ), ix );
    DpPutInt( zbuf, 0, sizeof( zbuf ), iz );
    const int n = snprintf( out, (size_t)DP_NAME, "%s\\%s_%08X_%s_%s.cache", DP_DIR, mid, (unsigned int)sig, xbuf, zbuf );
    return n > 0 && n < DP_NAME - 1;
  }
  inline bool DpHeadOk( const int* head, int n, int bpp, int sig, int ix, int iz ) {
    return head[0] == DP_CACHE_MAGIC && head[1] == DP_TILE_VERSION
        && head[2] == n && head[3] == g_dpFmt && head[4] == bpp
        && head[5] == sig && head[6] == ix && head[7] == iz;
  }
  inline bool DpTileLoad( int s, int ix, int iz, int n, int sig, DWORD& ms ) {
    ms = 0;
    if( !DpDiskOn() || s < 0 || s >= DP_TILES || !g_dpTile[s].tex || g_dpBpp <= 0 ) return false;
    char file[DP_NAME];
    if( !DpTileName( sig, ix, iz, file ) ) return false;
    const DWORD t0 = GetTickCount();
    HANDLE h = CreateFileA( file, GENERIC_READ | FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ, 0,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE )
      h = CreateFileA( file, GENERIC_READ, FILE_SHARE_READ, 0,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) return false;
    const int bpp = g_dpBpp;
    int   head[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    DWORD read = 0;
    if( !ReadFile( h, head, sizeof( head ), &read, 0 ) || read != sizeof( head )
        || !DpHeadOk( head, n, bpp, sig, ix, iz ) ) {
      CloseHandle( h );
      MM_LOG( zSTRING( "detail: tile " ) + zSTRING( ix ) + "," + zSTRING( iz ) + " - file out of date, recalculating" );
      return false;
    }
    const int bytes = n * n * bpp;
    unsigned char* px = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( bytes ) );
    if( !px ) {
      CloseHandle( h );
      return false;
    }
    bool packed = false;
    bool ok = PkReadAny( h, px, (DWORD)bytes, &packed );
    if( ok ) {
      FILETIME ft;
      GetSystemTimeAsFileTime( &ft );
      SetFileTime( h, 0, 0, &ft );
    }
    CloseHandle( h );
    if( ok ) ok = GenUpload( g_dpTile[s].tex, px, n, bpp );
    MiniMapCommon::MmFree( px );
    if( ok && !packed && g_genPack != 0 )
      PkRepackAsync( file, sizeof( head ), (unsigned)bytes, g_genPack );
    ms = GetTickCount() - t0;
    return ok;
  }
  struct TDpFile { char name[DP_NAME]; unsigned __int64 size; unsigned __int64 t; };
  inline void DpCachePrune( const char* keep ) {
    const unsigned __int64 limit = (unsigned __int64)g_genDetailCacheMB * 1024u * 1024u;
    const int CAP = 4096;
    TDpFile* f = MiniMapCommon::MmAlloc<TDpFile>( (size_t)( CAP ) );
    if( !f ) return;
    int cnt = 0;
    unsigned __int64 sum = 0;
    char mask[DP_NAME];
    snprintf( mask, DP_NAME, "%s\\*.cache", DP_DIR );
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( mask, &fd );
    if( fh != INVALID_HANDLE_VALUE ) {
      do {
        if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) continue;
        const unsigned __int64 sz = ( (unsigned __int64)fd.nFileSizeHigh << 32 ) | fd.nFileSizeLow;
        sum += sz;
        if( cnt >= CAP ) continue;
        const int p = snprintf( f[cnt].name, DP_NAME, "%s\\%s", DP_DIR, fd.cFileName );
        if( p <= 0 || p >= DP_NAME - 1 ) continue;
        f[cnt].size = sz;
        f[cnt].t    = ( (unsigned __int64)fd.ftLastWriteTime.dwHighDateTime << 32 )
                    | fd.ftLastWriteTime.dwLowDateTime;
        cnt++;
      } while( FindNextFileA( fh, &fd ) );
      FindClose( fh );
    }
    int removed = 0;
    while( sum > limit ) {
      int old = -1;
      for( int i = 0; i < cnt; i++ ) {
        if( !f[i].name[0] || ( keep && StrEqUp( f[i].name, keep ) ) ) continue;
        if( old < 0 || f[i].t < f[old].t ) old = i;
      }
      if( old < 0 ) break;
      if( DeleteFileA( f[old].name ) ) {
        sum -= f[old].size;
        removed++;
      }
      f[old].name[0] = 0;
    }
    MiniMapCommon::MmFree( f );
    if( removed > 0 )
      MM_LOG( zSTRING( "detail: limit GenDetailCacheMB=" ) + zSTRING( g_genDetailCacheMB ) + " - oldest tiles removed: " + zSTRING( removed ) );
  }
  const int DP_OLD_DAYS = 2;
  inline bool DpParseName( const char* name, const char* pre, char* sg ) {
    int i = (int)strlen( pre );
    if( _strnicmp( name, pre, (size_t)i ) != 0 ) return false;
    for( int j = 0; j < 8; j++, i++ ) {
      if( !HexChar( name[i] ) ) return false;
      sg[j] = UpChar( name[i] );
    }
    sg[8] = 0;
    for( int part = 0; part < 2; part++ ) {
      if( name[i] != '_' ) return false;
      i++;
      if( name[i] == 'm' ) i++;
      int d = 0;
      while( name[i] >= '0' && name[i] <= '9' ) {
        i++;
        d++;
      }
      if( !d ) return false;
    }
    return StrEqUp( name + i, ".cache" );
  }
  inline void DpTilePruneOld( int sig ) {
    char full[DP_NAME];
    if( !DpTileName( sig, 0, 0, full ) ) return;
    const char* nm = MiniMapCfg::BaseName( full );
    const int len = (int)strlen( nm ), TAIL = 19;
    if( len <= TAIL ) return;
    char pre[DP_NAME], cur[9], mask[DP_NAME];
    MiniMapCfg::CopyN( pre, DP_NAME, nm, len - TAIL + 1 );
    snprintf( cur, sizeof( cur ), "%08X", (unsigned int)sig );
    snprintf( mask, DP_NAME, "%s\\%s*.cache", DP_DIR, pre );
    WIN32_FIND_DATAA fd;
    HANDLE fh = FindFirstFileA( mask, &fd );
    if( fh == INVALID_HANDLE_VALUE ) return;
    int removed = 0;
    do {
      char sg[9];
      if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) || !DpParseName( fd.cFileName, pre, sg ) ) continue;
      if( StrEqUp( sg, cur ) ) continue;
      if( !FileAgeOld( fd.ftLastWriteTime, DP_OLD_DAYS ) ) continue;
      char f[DP_NAME];
      const int p = snprintf( f, DP_NAME, "%s\\%s", DP_DIR, fd.cFileName );
      if( p > 0 && p < DP_NAME - 1 && DeleteFileA( f ) ) removed++;
    } while( FindNextFileA( fh, &fd ) );
    FindClose( fh );
    if( removed > 0 )
      MM_LOG( zSTRING( "detail: removed tiles of old settings for this world (unused for " ) + zSTRING( DP_OLD_DAYS ) + " days): " + zSTRING( removed ) );
  }
  inline void DpSaveFinish( bool block ) {
    if( !g_dpSavePending ) return;
    if( !block && !PkJobDone( g_dpSaveJob ) ) return;
    const bool ok = PkJobWait( g_dpSaveJob );
    g_dpSavePending = false;
    if( !ok )
      MM_LOG( "detail: tile not saved to disk" );
    if( ok ) {
      DpTilePruneOld( g_dpSaveSig );
      DpCachePrune( g_dpSaveFile );
    }
  }
  inline bool DpTileSave( int ix, int iz, int n, int sig, const unsigned char* px, int bpp ) {
    if( !DpDiskOn() || !px || bpp <= 0 ) return false;
    char file[DP_NAME];
    if( !DpTileName( sig, ix, iz, file ) ) return false;
    FilesEnsure();
    DpSaveFinish( true );
    int head[8];
    head[0] = DP_CACHE_MAGIC;
    head[1] = DP_TILE_VERSION;
    head[2] = n;
    head[3] = g_dpFmt;
    head[4] = bpp;
    head[5] = sig;
    head[6] = ix;
    head[7] = iz;
    FilesDirs();
    StrCopy( g_dpSaveFile, file, DP_NAME );
    g_dpSaveSig = sig;
    if( !PkJobStart( g_dpSaveJob, g_dpSaveFile, head, sizeof( head ), px,
                     (unsigned)( n * n * bpp ), g_genPack ) )
      return false;
    g_dpSavePending = true;
    return true;
  }
  inline bool DetailPatchWant( int& n, int& neq, int& gN ) {
    if( g_genDetail < 1 || !g_texReady || g_mapIdx < 0 || !g_maps[g_mapIdx].hasCoords ) return false;
    gN = ( g_genSizeUsed > 0 ) ? g_genSizeUsed : g_genTexSize;
    if( gN <= 0 ) return false;
    const int neq0 = 8192 << g_genDetail;
    if( neq0 <= gN ) return false;
    int n0 = g_genPatchSize;
    if( g_genDetail >= 3 )
      n0 *= 2;
    if( n0 > 4096 ) n0 = 4096;
    n = n0;
    if( ProcLowMem() && n > 2048 ) n = 2048;
    const int mx = MaxTexSize();
    while( mx > 0 && n > mx && n > 256 )
      n /= 2;
    if( g_dpMaxN > 0 && n > g_dpMaxN ) n = g_dpMaxN;
    neq = (int)( (__int64)neq0 * n / n0 );
    if( neq <= gN ) return false;
    return n >= 512;
  }
  inline bool DpSlotTex( int& s, int n ) {
    for( ;; ) {
      if( s >= 0 && s < g_dpTileLim && DpEnsureTex( s, n ) ) return true;
      if( g_dpTileLim <= 1 ) return false;
      g_dpTileLim--;
      for( int k = g_dpTileLim; k < DP_TILES; k++ )
        DpTileRelease( k );
      MM_LOG( zSTRING( "detail: out of memory for tile texture - tiles at most " ) + zSTRING( g_dpTileLim ) );
      if( s >= g_dpTileLim ) s = g_dpTileLim - 1;
    }
  }
  inline bool WorldBspPolys( oCWorld* world, zCPolygon**& polys, int& num ) {
    polys = 0; num = 0;
    if( !world ) return false;
    zCMesh* const mesh = world->bspTree.mesh;
    if( mesh && mesh->numPoly > 0 && mesh->polyList ) { polys = mesh->polyList; num = mesh->numPoly; }
    else if( world->bspTree.treePolyList && world->bspTree.numPolys > 0 ) { polys = world->bspTree.treePolyList; num = world->bspTree.numPolys; }
    return polys != 0 && num > 0;
  }
  inline void DetailPatchStart( oCWorld* world, const TMapInfo& m, int n, int neq, int gN,
                                int s, int ix, int iz ) {
    const float coreCell = DpAbs( ( m.x2 - m.x1 ) / (float)neq );
    int fillMax = 16;
    {
      const float gCell = DpAbs( ( m.x2 - m.x1 ) / (float)gN );
      const float ratio = ( coreCell > 0.01f ) ? gCell / coreCell : 1.0f;
      fillMax = (int)( ratio * 16.0f );
      if( fillMax < 8 ) fillMax = 8; else if( fillMax > 250 ) fillMax = 250;
    }
    const int pad   = DpPadCells( n, coreCell, fillMax, DpRayBlock( n, coreCell ) );
    const int workN = n + 2 * pad;
    if( !DpBuffers( workN ) ) {
      for( int k = 0; k < DP_TILES; k++ )
        if( k != s )
          DpTileRelease( k );
      if( g_dpTileLim > 1 )
        MM_LOG( "detail: out of memory for buffers - one tile" );
      g_dpTileLim = 1;
      if( s != 0 ) {
        DpTileRelease( s );
        s = 0;
      }
      if( !DpEnsureTex( s, n ) || !DpBuffers( workN ) ) {
        DpTilesDrop();
        if( n / 2 >= 512 ) {
          g_dpMaxN = n / 2;
          MM_LOG( zSTRING( "detail: out of memory - tile size " ) + zSTRING( n / 2 ) );
        }
        else {
          g_dpFailed = true;
          MM_LOG( "detail: out of memory or texture - off until settings change" );
        }
        return;
      }
    }
    if( !g_dpTile[s].tex || g_dpTile[s].size != n ) {
      g_dpFailed = true;
      MM_LOG( "detail: no texture - off until settings change" );
      return;
    }
    g_dpMaxN = n;
    zCPolygon** polys = 0; int num = 0;
    if( !WorldBspPolys( world, polys, num ) ) {
      g_dpFailed = true;
      MM_LOG( "detail: no world polygon list" );
      return;
    }
    DpTileRect( m, n, neq, ix, iz, g_dpCoreRect );
    DpTileRectPad( m, n, neq, ix, iz, pad, g_dpNext );
    g_dpTile[s].ready = false;
    g_dpTile[s].ix    = ix;
    g_dpTile[s].iz    = iz;
    g_dpCurSlot = s;
    g_dpCurIx   = ix;
    g_dpCurIz   = iz;
    g_dpN       = workN;
    g_dpCoreN   = n;
    g_dpPad     = pad;
    g_dpOx      = ix * n - pad;
    g_dpOz      = iz * n - pad;
    DpWorkOrigin( m, neq, g_dpOx, g_dpOz, g_dpX1d, g_dpZ1d, g_dpSXd, g_dpSZd );
    g_dpNeqUsed = neq;
    g_dpGCell   = DpAbs( ( m.x2 - m.x1 ) / (float)gN );
    g_dpPCell   = coreCell;
    g_dpFillMax = fillMax;
    g_dpSrc     = polys;
    g_dpSrcNum  = num;
    g_dpSub     = 0;
    g_dpCur     = 0;
    g_dpStep    = 0;
    g_dpRays    = 0;
    g_dpFrames  = 0;
    g_dpWorkMs  = 0.0;
    g_dpT0      = GetTickCount();
    g_dpLastTick = g_dpT0;
    memset( g_dpPhMs, 0, (size_t)( DP_PH_COUNT ) * sizeof( g_dpPhMs[0] ) );
    g_dpPhase   = DP_POLYS;
  }
  inline void DetailPatchStep( oCWorld* world ) {
    {
      const DWORD now = GetTickCount();
      DWORD dt = now - g_dpLastTick;
      if( dt > 250 ) dt = 250;
      if( g_dpPhase > DP_IDLE && g_dpPhase < DP_PH_COUNT )
        g_dpPhMs[g_dpPhase] += dt;
      g_dpLastTick = now;
    }
    const double start = DpMs();
    const double until = start + g_dpFrameMs;
    const int    n     = g_dpN;
    g_dpFrames++;
    while( g_dpPhase != DP_IDLE ) {
      const double t0 = DpMs();
      if( t0 >= until ) break;
      const int a = g_dpCur;
      switch( g_dpPhase ) {
        case DP_POLYS: {
          int b = a + g_dpPolySlice;
          if( b > g_dpSrcNum ) b = g_dpSrcNum;
          if( !DpPolysCore( a, b ) ) { DpFail( "polygons" ); break; }
          g_dpCur = b;
          DpAdapt( g_dpPolySlice, DpMs() - t0, 2000, 1 << 20 );
          if( b >= g_dpSrcNum ) {
            if( !DpMatsCore() ) { DpFail( "materials" ); break; }
            DpSetupRaster();
            DpPhase( DP_RASTER );
          }
          break;
        }
        case DP_RASTER: {
          int b = a + g_dpSlice;
          if( b > n ) b = n;
          DpParallel( DpRasterJob, &g_dpFast, a, b );
          if( g_dpFast.failed ) { DpFail( "exception in worker" ); break; }
          g_dpCur = b;
          DpAdapt( g_dpSlice, DpMs() - t0, 4, n );
          if( b >= n ) {
            DpPhase( DP_TREES );
            g_dpTreeTris = g_dpTreeStamps = 0;
            g_dpTreeMs   = 0.0;
            g_dpTreeIn   = false;
          }
          break;
        }
        case DP_TREES: {
          if( g_genTreeShapes == 0 ) { DpPhase( DP_VOBS ); break; }
          const TMapInfo& r = g_dpNext;
          const float rx1 = ( r.x1 < r.x2 ) ? r.x1 : r.x2, rx2 = ( r.x1 < r.x2 ) ? r.x2 : r.x1;
          const float rz1 = ( r.y1 < r.y2 ) ? r.y1 : r.y2, rz2 = ( r.y1 < r.y2 ) ? r.y2 : r.y1;
          if( !g_dpTreeIn ) {
            g_dpTreeIn = true;
            TreeVobsEnsure( world );
            g_dpTreeStamps = TreeStampCore( g_dpFast, rx1, rz1, rx2, rz2 );
            if( g_genTreeShapes < 2 || g_tvCount <= 0
                || !TreeBatchAlloc( g_dpTB, 4096 ) )
              g_dpTB.vi = g_tvCount;
            g_dpTreeMs += DpMs() - t0;
            break;
          }
          bool fin = !g_dpTB.v || g_dpTB.vi >= g_tvCount;
          if( !fin ) {
            if( !TreeBatchFill( g_dpTB, rx1, rz1, rx2, rz2, g_dpTB.cap ) )
              fin = true;
            else if( g_dpTB.cnt > 0 ) {
              TTreeJob job;
              job.C = &g_dpFast;
              job.B = &g_dpTB;
              g_dpFast.failed = 0;
              DpParallel( TreeRasterJob, &job, 0, n );
              if( g_dpFast.failed ) { DpFail( "exception in worker (trees)" ); break; }
              g_dpTreeTris += g_dpTB.cnt;
            }
            if( g_dpTB.vi >= g_tvCount ) fin = true;
          }
          g_dpTreeMs += DpMs() - t0;
          if( fin ) {
            TreeLog( "detail tile", g_dpTreeStamps, g_dpTreeTris, g_dpTreeMs );
            TreeBatchFree( g_dpTB );
            DpPhase( DP_VOBS );
          }
          break;
        }
        case DP_VOBS:
          if( !DpVobsCore( world ) ) { DpFail( "vobs" ); break; }
          DpPhase( DP_RAYS );
          break;
        case DP_RAYS:
          if( !DpRaysCore( world, until ) ) { DpFail( "rays" ); break; }
          if( g_dpCur >= g_dpNB * g_dpNB )
            DpPhase( DP_PLANTS );
          break;
        case DP_PLANTS:
          if( g_genTreeShapes == 0 && !DpPlantsCore( world ) ) { DpFail( "plants" ); break; }
          DpPhase( DP_FILL_FWD );
          break;
        case DP_FILL_FWD: {
          int b = a + g_dpSlice;
          if( b > n ) b = n;
          DpFillFwd( a, b );
          g_dpCur = b;
          DpAdapt( g_dpSlice, DpMs() - t0, 4, n );
          if( b >= n ) {
            DpPhase( DP_FILL_BWD );
            g_dpCur = n;
          }
          break;
        }
        case DP_FILL_BWD: {
          int b = a - g_dpSlice;
          if( b < 0 ) b = 0;
          DpFillBwd( b, a );
          g_dpCur = b;
          DpAdapt( g_dpSlice, DpMs() - t0, 4, n );
          if( b <= 0 ) {
            DpSetupPaths();
            DpPhase( DP_PATHS );
          }
          break;
        }
        case DP_PATHS: {
          if( g_dpEdgeCnt <= 0 ) {
            DpSetupSmooth();
            DpPhase( DP_SMOOTH );
            g_dpStep = 0;
            break;
          }
          int b = a + g_dpSlice;
          if( b > n ) b = n;
          DpParallel( DpPathsJob, &g_dpPaths, a, b );
          g_dpCur = b;
          DpAdapt( g_dpSlice, DpMs() - t0, 4, n );
          if( b >= n ) g_dpEdgeCnt = 0;
          break;
        }
        case DP_SMOOTH: {
          if( g_dpStep >= g_dpSweepCnt ) {
            DpSetupShade( world );
            DpPhase( DP_SHADE );
            break;
          }
          int b = a + g_dpSlice;
          if( b > n ) b = n;
          DpParallel( g_dpSweep[g_dpStep].fn, g_dpSweep[g_dpStep].ctx, a, b );
          g_dpCur = b;
          DpAdapt( g_dpSlice, DpMs() - t0, 4, n );
          if( b >= n ) {
            g_dpCur = 0;
            g_dpStep++;
            g_dpSlice = 4;
          }
          break;
        }
        case DP_SHADE: {
          int b = a + g_dpSlice;
          if( b > n ) b = n;
          DpParallel( GenShadeJob, &g_dpShade, a, b );
          g_dpCur = b;
          DpAdapt( g_dpSlice, DpMs() - t0, 4, n );
          if( b >= n )
            DpPhase( DP_UPLOAD );
          break;
        }
        case DP_UPLOAD: {
          g_dpPhase = DP_IDLE;
          const int cs = g_dpCurSlot;
          g_dpCurSlot = -1;
          zCTexture* const ctex = ( cs >= 0 && cs < DP_TILES ) ? g_dpTile[cs].tex : 0;
          DpCropPx();
          const int coreN = g_dpCoreN;
          bool upOk = GenUpload( ctex, g_dpPx, coreN, g_dpBpp );
          if( !upOk && ctex && g_dpTileLim > 1 ) {
            for( int k = 0; k < DP_TILES; k++ )
              if( k != cs )
                DpTileRelease( k );
            g_dpTileLim = 1;
            MM_LOG( "detail: tile texture upload failed - one tile" );
            upOk = GenUpload( ctex, g_dpPx, coreN, g_dpBpp );
          }
          if( !upOk ) {
            DpTilesDrop();
            if( coreN > 512 )
              g_dpMaxN = coreN / 2;
            else
              g_dpFailed = true;
            MM_LOG( zSTRING( "detail: texture upload failed " ) + zSTRING( coreN ) + zSTRING( coreN > 512 ? " - trying smaller" : " - off" ) );
            break;
          }
          g_dpTile[cs].rect  = g_dpCoreRect;
          g_dpTile[cs].ix    = g_dpCurIx;
          g_dpTile[cs].iz    = g_dpCurIz;
          g_dpTile[cs].ready = true;
          g_dpTile[cs].used  = GetTickCount();
          memcpy( g_dpPhMsLast, g_dpPhMs, (size_t)( DP_PH_COUNT ) * sizeof( g_dpPhMsLast[0] ) );
          g_dpWorkMs += DpMs() - start;
          const DWORD sv0 = GetTickCount();
          const bool saved = DpTileSave( g_dpCurIx, g_dpCurIz, coreN, g_dpSigUsed, g_dpPx, g_dpBpp );
          const int mb = (int)( ( (double)n * (double)n * ( 11.0 + (double)g_dpBpp ) )
                                / ( 1024.0 * 1024.0 ) );
          MM_LOG( zSTRING( "detail: tile " ) + zSTRING( g_dpCurIx ) + "," + zSTRING( g_dpCurIz ) + " computed " + zSTRING( coreN ) + "x" + zSTRING( coreN ) + ", density as map " + zSTRING( g_dpNeqUsed ) + " (cell " + zSTRING( (int)( g_dpPCell * 10.0f + 0.5f ) ) + " mm, world map " + zSTRING( (int)( g_dpGCell * 10.0f + 0.5f ) ) + " mm), side " + zSTRING( (int)( DpAbs( g_dpCoreRect.x2 - g_dpCoreRect.x1 ) / 100.0f ) ) + " m, polygons " + zSTRING( g_dpSub ) + ", rays " + zSTRING( g_dpRays ) + " (block " + zSTRING( g_dpS ) + ", margin " + zSTRING( g_dpPad ) + "), memory approx. " + zSTRING( mb ) + " MB, work " + zSTRING( (int)g_dpWorkMs ) + " ms in " + zSTRING( g_dpFrames ) + " frames, total " + zSTRING( (int)( GetTickCount() - g_dpT0 ) ) + " ms, " + zSTRING( saved ? "saved (" : "not saved to disk (" ) + zSTRING( (int)( GetTickCount() - sv0 ) ) + " ms)" );
          return;
        }
        default:
          g_dpPhase = DP_IDLE;
          break;
      }
    }
    g_dpWorkMs += DpMs() - start;
  }
  inline int DpTileLimit( int n ) {
    const int bpp = ( g_dpBpp > 0 ) ? g_dpBpp : 2;
    const double mb = (double)n * (double)n * (double)bpp / ( 1024.0 * 1024.0 );
    const int budget = ProcLowMem() ? DP_TEX_BUDGET_LOW_MB : DP_TEX_BUDGET_MB;
    int lim = ( mb > 0.0 ) ? (int)( (double)budget / mb ) : DP_TILES;
    if( lim > g_dpTileLim ) lim = g_dpTileLim;
    if( lim > DP_TILES )    lim = DP_TILES;
    if( lim < 1 )           lim = 1;
    return lim;
  }
  inline bool DpAnyTex() {
    for( int s = 0; s < DP_TILES; s++ )
      if( g_dpTile[s].tex ) return true;
    return false;
  }
  inline int DpFindTile( int ix, int iz, int lim ) {
    for( int s = 0; s < lim; s++ )
      if( g_dpTile[s].ready && g_dpTile[s].ix == ix && g_dpTile[s].iz == iz ) return s;
    return -1;
  }
  inline int DpFreeSlot( int lim, const int* wx, const int* wz, int cnt ) {
    int best = -1;
    for( int s = 0; s < lim; s++ ) {
      const TDpTile& t = g_dpTile[s];
      if( !t.ready ) return s;
      bool want = false;
      for( int k = 0; k < cnt; k++ )
        if( t.ix == wx[k] && t.iz == wz[k] ) want = true;
      if( want ) continue;
      if( best < 0 || t.used < g_dpTile[best].used ) best = s;
    }
    return best;
  }
  inline void DetailPatchUpdate() {
    if( g_genDetail < 1 ) {
      if( DpAnyTex() || g_dpBufN )
        DetailPatchFree();
      return;
    }
    if( !player || !ogame ) return;
    if( g_genRunning ) return;
    if( !g_texReady ) return;
    int n = 0, neq = 0, gN = 0;
    if( !DetailPatchWant( n, neq, gN ) ) {
      g_dpPhase = DP_IDLE;
      DpTilesDrop();
      return;
    }
    oCWorld* const world = ogame->GetGameWorld();
    if( !world || !world->bspTree.bspRoot ) return;
    const TMapInfo& m = g_maps[g_mapIdx];
    const int sig = GenSignature( m );
    const int neqKey = 8192 << g_genDetail;
    if( g_dpKeyDetail != g_genDetail || g_dpKeyNeq != neqKey
        || g_dpKeyGN != gN || g_dpKeySig != sig
        || ( g_dpKeyN != g_genPatchSize ) ) {
      if( g_dpKeyDetail != g_genDetail || g_dpKeyN != g_genPatchSize ) g_dpMaxN = 0;
      g_dpKeyDetail = g_genDetail;
      g_dpKeyN      = g_genPatchSize;
      g_dpKeyNeq    = neqKey;
      g_dpKeyGN     = gN;
      g_dpKeySig    = sig;
      g_dpPhase     = DP_IDLE;
      g_dpFailed    = false;
      g_dpTileLim   = DP_TILES;
      DpTilesDrop();
      if( !DetailPatchWant( n, neq, gN ) ) return;
    }
    if( g_dpFailed ) return;
    float hmin = 0.0f, hmax = 0.0f;
    DpHeightScale( world, hmin, hmax );
    const int tsig = DpTileSig( m, n, neq, gN, hmin, hmax );
    if( tsig != g_dpSigUsed ) {
      g_dpPhase   = DP_IDLE;
      DpTilesDrop();
      g_dpSigUsed = tsig;
      const double side = DpAbs( m.x2 - m.x1 ) * (double)n / (double)neq / 100.0;
      MM_LOG( zSTRING( "detail: tile " ) + zSTRING( (int)( side + 0.5 ) ) + " m, density as map " + zSTRING( neq ) + ", n=" + zSTRING( n ) + ", in memory up to " + zSTRING( DpTileLimit( n ) ) + zSTRING( ProcLowMem() ? " (G1 without LAA)" : "" ) );
    }
    const int lim = DpTileLimit( n );
    for( int s = lim; s < DP_TILES; s++ )
      if( g_dpTile[s].tex && s != g_dpCurSlot )
        DpTileRelease( s );
    const zVEC3 pp = player->GetPositionWorld();
    const float gCell = DpAbs( ( m.x2 - m.x1 ) / (float)gN );
    const float upp   = ( g_ppuX > 0.000001f ) ? 1.0f / g_ppuX : 1.0e30f;
    const bool  zoomFull = g_full && g_fullMapped && upp <= gCell * 2.0f;
    int wx[4], wz[4];
    const int cnt = DpWanted( m, n, neq, zoomFull ? g_px : pp[0], zoomFull ? g_pz : pp[2], lim, wx, wz );
    if( cnt <= 0 ) return;
    const DWORD now = GetTickCount();
    g_dpWantCnt   = cnt;
    g_dpWantReady = 0;
    for( int k = 0; k < cnt; k++ ) {
      const int s = DpFindTile( wx[k], wz[k], lim );
      if( s >= 0 ) {
        g_dpTile[s].used = now;
        g_dpWantReady++;
      }
    }
    if( g_dpPhase != DP_IDLE ) {
      bool keep = false;
      for( int k = 0; k < cnt; k++ )
        if( wx[k] == g_dpCurIx && wz[k] == g_dpCurIz ) keep = true;
      if( g_dpCurIx >= wx[0] - 1 && g_dpCurIx <= wx[0] + 1
          && g_dpCurIz >= wz[0] - 1 && g_dpCurIz <= wz[0] + 1 )
        keep = true;
      if( !keep || g_dpCurSlot < 0 || g_dpCurSlot >= lim ) {
        MM_LOG( zSTRING( "detail: tile " ) + zSTRING( g_dpCurIx ) + "," + zSTRING( g_dpCurIz ) + " aborted - player far away" );
        g_dpPhase   = DP_IDLE;
        g_dpCurSlot = -1;
      }
    }
    if( g_dpPhase == DP_IDLE ) {
      if( upp > gCell * 2.0f ) return;
      int k = 0;
      while( k < cnt && DpFindTile( wx[k], wz[k], lim ) >= 0 )
        k++;
      if( k >= cnt ) return;
      int s = DpFreeSlot( lim, wx, wz, cnt );
      if( s < 0 ) return;
      if( !DpSlotTex( s, n ) ) {
        DpTilesDrop();
        if( n / 2 >= 512 ) {
          g_dpMaxN = n / 2;
          MM_LOG( zSTRING( "detail: out of memory for texture - tile size " ) + zSTRING( n / 2 ) );
        }
        else {
          g_dpFailed = true;
          MM_LOG( "detail: no texture - off until settings change" );
        }
        return;
      }
      g_dpTile[s].ready = false;
      DWORD ms = 0;
      if( DpTileLoad( s, wx[k], wz[k], n, tsig, ms ) ) {
        TDpTile& t = g_dpTile[s];
        DpTileRect( m, n, neq, wx[k], wz[k], t.rect );
        t.ix    = wx[k];
        t.iz    = wz[k];
        t.ready = true;
        t.used  = now;
        MM_LOG( zSTRING( "detail: tile " ) + zSTRING( wx[k] ) + "," + zSTRING( wz[k] ) + " from disk (" + zSTRING( (int)ms ) + " ms)" );
        return;
      }
      DetailPatchStart( world, m, n, neq, gN, s, wx[k], wz[k] );
      if( g_dpPhase == DP_IDLE ) return;
    }
    DetailPatchStep( world );
  }
  inline void DrawDetailPatch( int x, int y, int w, int h ) {
    if( g_genDetail < 1 || g_mapIdx < 0 ) return;
    const TMapInfo& m = g_maps[g_mapIdx];
    const int gN = ( g_genSizeUsed > 0 ) ? g_genSizeUsed : g_genTexSize;
    if( gN <= 0 || g_ppuX <= 0.000001f ) return;
    const float gCell = DpAbs( ( m.x2 - m.x1 ) / (float)gN );
    if( 1.0f / g_ppuX >= gCell ) return;
    for( int s = 0; s < DP_TILES; s++ ) {
      const TDpTile& t = g_dpTile[s];
      if( !t.ready || !t.tex ) continue;
      g_curTex   = t.tex;
      g_curRect  = &t.rect;
      g_curBlend = true;
      DrawMapTexture( x, y, w, h );
    }
    g_curTex   = 0;
    g_curRect  = 0;
    g_curBlend = false;
  }
  static int  g_layFmt = -1;
  static int  g_layBpp = 0;
  inline float LaySide() {
    float side = (float)g_zoom * 2.0f;
    if( side < 3000.0f ) side = 3000.0f; else if( side > 20000.0f ) side = 20000.0f;
    return side;
  }
  inline void LayFree() { DetailPatchFree();
    StyleFree( true );
    if( g_layTex ) {
      g_layTex->Release();
      g_layTex = 0;
    }
    g_layTexSize = 0;
    g_layFmt     = -1;
    g_layBpp     = 0;
    g_layReady   = false;
    g_layPhase   = LAY_IDLE;
    g_layCovered = false;
    g_layVote    = 0;
    g_layHold    = 0;
    g_layFade    = 0.0f;
    MiniMapCommon::MmFree( g_layPolys );  g_layPolys = 0;  g_layPolyCap = 0;
    MiniMapCommon::MmFree( g_layHgt );    g_layHgt   = 0;
    MiniMapCommon::MmFree( g_layLit );    g_layLit   = 0;
    MiniMapCommon::MmFree( g_layMat );    g_layMat   = 0;
    MiniMapCommon::MmFree( g_layTint );   g_layTint  = 0;  g_layTintN = 0;  g_layTintOn = false;
    MiniMapCommon::MmFree( g_layMask );   g_layMask  = 0;
    MiniMapCommon::MmFree( g_layPx );     g_layPx    = 0;
    g_layBufN = 0;
  }
  inline zCTexture* LayEnsureTex( int n ) {
    if( g_layTex && g_layTexSize == n ) return g_layTex;
    if( g_layTex ) { g_layTex->Release(); g_layTex = 0; g_layTexSize = 0; }
    zTRnd_TextureFormat fmt = zRND_TEX_FORMAT_ARGB_4444;
    int bpp = 0;
    if( !PickTexFormat( false, fmt, bpp ) ) return 0;
    zCTexture* tex = CreateTex2D( n, n, fmt, zCOLOR( 120, 128, 100, 255 ), false );
    if( !tex ) return 0;
    g_layTex = tex; g_layTexSize = n; g_layFmt = (int)fmt; g_layBpp = bpp;
    return tex;
  }
  inline bool LayBuffers( int n ) {
    if( g_layBufN == n && g_layHgt && g_layLit && g_layMat && g_layMask && g_layPx ) return true;
    MiniMapCommon::MmFree( g_layHgt );   MiniMapCommon::MmFree( g_layLit );  MiniMapCommon::MmFree( g_layMat );
    MiniMapCommon::MmFree( g_layMask );  MiniMapCommon::MmFree( g_layPx );
    g_layHgt = 0;
    g_layLit = g_layMat = g_layMask = g_layPx = 0;
    if( CanAllocGrid( n, 4 ) ) {
      g_layHgt  = MiniMapCommon::MmAlloc<float>( (size_t)( n * n ) );
      g_layLit  = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n ) );
      g_layMat  = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n ) );
      g_layMask = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n ) );
      g_layPx   = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n * 4 ) );
    }
    g_layBufN = n;
    if( g_layHgt && g_layLit && g_layMat && g_layMask && g_layPx ) return true;
    MiniMapCommon::MmFree( g_layHgt );   MiniMapCommon::MmFree( g_layLit );  MiniMapCommon::MmFree( g_layMat );
    MiniMapCommon::MmFree( g_layMask );  MiniMapCommon::MmFree( g_layPx );
    g_layHgt = 0;
    g_layLit = g_layMat = g_layMask = g_layPx = 0;
    g_layBufN = 0;
    return false;
  }
  struct TLayStat {
    int polys, rays, threads;
    const char* why;
  };
  inline bool LayComputeCore( oCWorld* world, int n, float ceil, TLayStat& st ) {
    const void**   hkey = 0;
    unsigned char* hval = 0;
    unsigned char* htnt = 0;
    bool ok = false;
    g_layTintOn = false;
    __try {
      for( ;; ) {
        if( !world || !world->bspTree.bspRoot ) { st.why = "no world"; break; }
        zCPolygon** polys = 0; int numPoly = 0;
        if( !WorldBspPolys( world, polys, numPoly ) ) { st.why = "no polygon list"; break; }
        const TMapInfo& m = g_layPend;
        const float x1 = m.x1, x2 = m.x2, z1 = m.y1, z2 = m.y2;
        int sub = 0;
        for( int p = 0; p < numPoly; p++ ) {
          zCPolygon* const poly = polys[p];
          if( !GenFastPolyOk( poly ) ) continue;
          const int nv = (int)poly->polyNumVert;
          float ax = 1.0e30f, bx = -1.0e30f, az = 1.0e30f, bz = -1.0e30f, ay = 1.0e30f;
          bool  bad = false;
          for( int v = 0; v < nv; v++ ) {
            const zCVertex* const vx = poly->vertex[v];
            if( !vx ) { bad = true; break; }
            const float px = vx->position[0], py = vx->position[1], pz = vx->position[2];
            if( px < ax ) ax = px;
            if( px > bx ) bx = px;
            if( pz < az ) az = pz;
            if( pz > bz ) bz = pz;
            if( py < ay ) ay = py;
          }
          if( bad || bx < x1 || ax > x2 || bz < z1 || az > z2 || ay >= ceil ) continue;
          if( sub >= g_layPolyCap ) {
            const int cap = ( g_layPolyCap < 4096 ) ? 4096 : g_layPolyCap * 2;
            zCPolygon** const nl = MiniMapCommon::MmAlloc<zCPolygon*>( (size_t)( cap ) );
            if( !nl ) { bad = true; break; }
            memcpy( nl, g_layPolys, (size_t)( sub ) * sizeof( nl[0] ) );
            MiniMapCommon::MmFree( g_layPolys );
            g_layPolys   = nl;
            g_layPolyCap = cap;
          }
          g_layPolys[sub++] = poly;
        }
        st.polys = sub;
        const unsigned int HSIZE = 16384, hmask = HSIZE - 1;
        hkey = MiniMapCommon::MmAlloc<const void*>( (size_t)( HSIZE ) );
        hval = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( HSIZE ) );
        if( !hkey || !hval ) { st.why = "out of memory"; break; }
        for( unsigned int s = 0; s < HSIZE; s++ ) { hkey[s] = 0; hval[s] = 0; }
        if( g_genTexColors ) {
          const int nn0 = n * n;
          if( !g_layTint || g_layTintN != nn0 ) {
            MiniMapCommon::MmFree( g_layTint );
            g_layTint  = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( nn0 ) );
            g_layTintN = g_layTint ? nn0 : 0;
          }
          if( g_layTint ) {
            htnt = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( HSIZE ) );
            if( htnt )
              memset( htnt, 0, (size_t)( HSIZE ) * sizeof( htnt[0] ) );
          }
        }
        int mats = 0;
        for( int p = 0; p < sub; p++ )
          if( GenInsertMatHash( g_layPolys[p]->material, hkey, hval, htnt, hmask, mats, (int)( HSIZE / 4 * 3 ) ) > 0 )
            mats++;
        const int nn = n * n;
        for( int i = 0; i < nn; i++ ) {
          g_layHgt[i]  = GEN_FAST_EMPTY;
          g_layLit[i] = 0;
          g_layMat[i]  = 0;
          g_layMask[i] = 0;
        }
        unsigned char* const ltint = ( htnt && g_layTint && g_layTintN == nn ) ? g_layTint : 0;
        if( ltint )
          memset( ltint, 0, (size_t)( nn ) * sizeof( ltint[0] ) );
        TGenFastCtx C;
        C.polys   = g_layPolys;
        C.numPoly = sub;
        C.n       = n;
        C.x1      = (double)x1;
        C.z1      = (double)z1;
        C.sx      = (double)n / ( (double)x2 - (double)x1 );
        C.sz      = (double)n / ( (double)z2 - (double)z1 );
        C.ceil    = ceil;
        C.hgt     = g_layHgt;
        C.lit     = g_layLit;
        C.mat     = g_layMat;
        C.hkey    = hkey;
        C.hval    = hval;
        C.hmask   = hmask;
        C.htint   = ltint ? htnt : 0;
        C.tint    = ltint;
        g_layTintOn = ( ltint != 0 );
        C.lx      = -0.45f;
        C.ly      =  0.78f;
        C.lz      =  0.44f;
        C.next    = 0;
        C.failed  = 0;
        const int threads = GenParThreads();
        int chunks = threads * 2;
        if( chunks > n ) chunks = n;
        C.rows   = ( n + chunks - 1 ) / chunks;
        C.chunks = ( n + C.rows - 1 ) / C.rows;
        HANDLE h[GEN_MAX_THREADS];
        int    made = 0;
        for( int t = 1; t < threads; t++ ) {
          HANDLE th = CreateThread( 0, 256 * 1024, GenFastWorker, &C,
                                    STACK_SIZE_PARAM_IS_A_RESERVATION, 0 );
          if( !th ) break;
          h[made++] = th;
        }
        GenFastWorker( &C );
        if( made > 0 ) {
          WaitForMultipleObjects( (DWORD)made, h, TRUE, INFINITE );
          for( int t = 0; t < made; t++ )
            CloseHandle( h[t] );
        }
        st.threads = made + 1;
        if( C.failed ) { st.why = "exception in worker"; break; }
        for( int i = 0; i < nn; i++ )
          if( g_layHgt[i] <= GEN_FAST_EMPTY ) {
            g_layHgt[i] = 0.0f;
            g_layLit[i] = 0;
            g_layMat[i] = 0;
          }
        if( g_genVobs ) {
          const float minBig = ( g_genVobMin < g_genPlantMin )
                             ? (float)g_genVobMin : (float)g_genPlantMin;
          int marked = 0;
          for( zCListSort<zCVob>* node = world->voblist; node; node = node->next ) {
            zCVob* const vob = node->data;
            if( !vob || vob->type == zVOB_TYPE_LEVEL_COMPONENT || !vob->GetVisual() ) continue;
            const zTBBox3D& vb = vob->bbox3D;
            if( vb.maxs[0] < x1 || vb.mins[0] > x2 || vb.maxs[2] < z1 || vb.mins[2] > z2 ) continue;
            if( vb.mins[1] >= ceil ) continue;
            const float sx = vb.maxs[0] - vb.mins[0];
            const float sz = vb.maxs[2] - vb.mins[2];
            if( !( ( ( sx > sz ) ? sx : sz ) >= minBig ) ) continue;
            int ix0 = (int)floor( ( vb.mins[0] - x1 ) * C.sx ) - 1;
            int ix1 = (int)floor( ( vb.maxs[0] - x1 ) * C.sx ) + 1;
            int iz0 = (int)floor( ( vb.mins[2] - z1 ) * C.sz ) - 1;
            int iz1 = (int)floor( ( vb.maxs[2] - z1 ) * C.sz ) + 1;
            if( ix0 < 0 ) ix0 = 0;
            if( iz0 < 0 ) iz0 = 0;
            if( ix1 > n - 1 ) ix1 = n - 1;
            if( iz1 > n - 1 ) iz1 = n - 1;
            for( int iz = iz0; iz <= iz1; iz++ )
              for( int ix = ix0; ix <= ix1; ix++ ) {
                unsigned char& mk = g_layMask[iz * n + ix];
                if( !mk ) { mk = 1; marked++; }
              }
          }
          int s = 1;
          while( marked / ( s * s ) > LAY_VOB_RAYS && s < 16 )
            s *= 2;
          const zTBBox3D& bb = world->bspTree.bspRoot->bbox3D;
          g_layS   = s;
          g_layTop = ( ceil < 1.0e29f ) ? ceil : bb.maxs[1] + 500.0f;
          g_layCur = 0;
        }
        else
          g_layS = 0;
        ok = true;
        break;
      }
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      st.why = "exception";
      ok = false;
    }
    MiniMapCommon::MmFree( hkey );
    MiniMapCommon::MmFree( hval );
    MiniMapCommon::MmFree( htnt );
    return ok;
  }
  inline void LayCompute( oCWorld* world, float floorY, float headY ) {
    const DWORD t0 = GetTickCount();
    if( g_mapIdx < 0 || !g_maps[g_mapIdx].hasCoords ) return;
    const float side = LaySide();
    int n = 1024;
    const int mx = MaxTexSize();
    while( mx > 0 && n > mx && n > 64 )
      n /= 2;
    if( !LayBuffers( n ) || !LayEnsureTex( n ) ) {
      MM_LOG( zSTRING( "player layer: out of memory/texture " ) + zSTRING( n ) );
      return;
    }
    const zVEC3 pp = player->GetPositionWorld();
    g_layPCX   = pp[0];
    g_layPCZ   = pp[2];
    g_layPSide = side;
    g_layPend.x1 = pp[0] - side * 0.5f;
    g_layPend.x2 = pp[0] + side * 0.5f;
    g_layPend.y1 = pp[2] - side * 0.5f;
    g_layPend.y2 = pp[2] + side * 0.5f;
    g_layPend.hasCoords = true;
    g_layPend.tag = 0;
    g_layPend.world[0] = 0;
    const float ceil = headY + LAY_CEIL_MARGIN;
    TLayStat st;
    st.polys = 0; st.rays = 0; st.threads = 0; st.why = 0;
    if( !LayComputeCore( world, n, ceil, st ) ) {
      MM_LOG( zSTRING( "player layer: failed (" ) + zSTRING( st.why ? st.why : "?" ) + ")" );
      g_layLast = GetTickCount();
      return;
    }
    g_layN       = n;
    g_layCeil    = ceil;
    g_layPFloor  = floorY;
    g_layPolysN  = st.polys;
    g_layRaysN   = 0;
    g_layT0      = t0;
    g_layFrames  = 1;
    g_layWorkMs  = (double)( GetTickCount() - t0 );
    g_layLongest = (int)g_layWorkMs;
    g_layPhase   = LAY_RAYS;
  }
  inline bool LayRaysCore( oCWorld* world, double until ) {
    __try {
      const int n = g_layN, s = g_layS;
      if( s <= 0 ) return true;
      const int nb = ( n + s - 1 ) / s, total = nb * nb;
      const TMapInfo& m = g_layPend;
      const float x1 = m.x1, z1 = m.y1;
      const float cw = ( m.x2 - m.x1 ) / (float)n, ch = ( m.y2 - m.y1 ) / (float)n;
      const float top = g_layTop, ceil = g_layCeil;
      const int flagsVob = zTRACERAY_STAT_POLY | zTRACERAY_POLY_NORMAL
                         | zTRACERAY_VOB_IGNORE_CHARACTER;
      unsigned char* const ltint = ( g_layTintOn && g_layTintN == n * n ) ? g_layTint : 0;
      int k = 0;
      while( g_layCur < total ) {
        if( ( ++k & 15 ) == 0 && DpMs() >= until ) break;
        const int bi  = g_layCur++;
        const int bz0 = ( bi / nb ) * s, bx0 = ( bi % nb ) * s;
        const int cx  = ( bx0 + s / 2 < n ) ? bx0 + s / 2 : bx0;
        const int cz  = ( bz0 + s / 2 < n ) ? bz0 + s / 2 : bz0;
        const int i   = cz * n + cx;
        if( !g_layMask[i] || !g_layLit[i] ) continue;
        const float terrY = g_layHgt[i];
        if( top <= terrY + 50.0f ) continue;
        const zVEC3 from( x1 + cw * ( (float)cx + 0.5f ), top,
                          z1 + ch * ( (float)cz + 0.5f ) );
        const zVEC3 ray( 0.0f, -( top - ( terrY + 50.0f ) ), 0.0f );
        g_layRaysN++;
        if( !world->TraceRayNearestHit( from, ray, (zCVob*)0, flagsVob )
            || !world->traceRayReport.foundHit
            || !world->traceRayReport.foundVob )
          continue;
        zCVob* const    vob  = world->traceRayReport.foundVob;
        const float     vobY = world->traceRayReport.foundIntersection[1];
        const zTBBox3D& vbb  = vob->bbox3D;
        const float     sx   = vbb.maxs[0] - vbb.mins[0];
        const float     sz   = vbb.maxs[2] - vbb.mins[2];
        const float     big  = ( sx > sz ) ? sx : sz;
        zCVisual* const vis  = vob->GetVisual();
        const char* const vnm = vis ? vis->GetObjectName().ToChar() : 0;
        const unsigned char cls = GenSurfCached( vis, vnm, true );
        const float min = ( cls == SURF_PLANT ) ? (float)g_genPlantMin
                                                : (float)g_genVobMin;
        if( !( big >= min && vobY > terrY + 50.0f && vobY < ceil ) ) continue;
        if( cls == SURF_PLANT && TreeGiantSize( sx, sz, vbb.maxs[1] - vbb.mins[1], vnm ) ) continue;
        for( int zz = bz0; zz < bz0 + s && zz < n; zz++ )
          for( int xx = bx0; xx < bx0 + s && xx < n; xx++ ) {
            const int q = zz * n + xx;
            if( !g_layMask[q] || !g_layLit[q] ) continue;
            if( vobY > g_layHgt[q] + 50.0f ) {
              g_layHgt[q] = vobY;
              g_layMat[q] = cls;
              if( ltint ) ltint[q] = 0;
            }
          }
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  static TGenShade g_layShade;
  inline void LaySetupShade( oCWorld* world ) {
    const int n = g_layN;
    float hmin = g_genHMin, hmax = g_genHMax;
    if( !g_genHKnown && world && world->bspTree.bspRoot ) {
      hmin = world->bspTree.bspRoot->bbox3D.mins[1];
      hmax = world->bspTree.bspRoot->bbox3D.maxs[1];
    }
    float range = hmax - hmin;
    if( range < 1.0f ) range = 1.0f;
    const int nn = n * n;
    for( int i = 0; i < nn; i++ )
      g_layMask[i] = ( g_layLit[i] != 0 && g_layHgt[i] > g_layPFloor + 180.0f ) ? 1 : 0;
    TGenShade& S  = g_layShade;
    S.n           = n;
    S.bpp         = g_layBpp;
    S.hgt         = g_layHgt;
    S.lit         = g_layLit;
    S.mat         = g_layMat;
    S.px          = g_layPx;
    S.hmin        = hmin;
    S.range       = range;
    S.relief      = (float)g_genRelief / 100.0f;
    S.cellX       = g_layPSide / (float)n;
    S.cellZ       = g_layPSide / (float)n;
    S.lx          = -0.45f;
    S.ly          =  0.78f;
    S.lz          =  0.44f;
    S.fmt         = g_layFmt;
    S.edge        = g_genEdge;
    S.contour     = g_genContour;
    S.contourDark = g_genContourDark;
    S.wall        = g_layMask;
    S.emptyC      = 0xFF16130Ful;
    S.lowY        = g_layPFloor - 400.0f;
    S.sculpt      = g_genSculpt;
    S.tex         = g_genTexColors;
    S.tint        = ( g_layTintOn && g_layTintN == nn ) ? g_layTint : 0;
    g_layCur      = 0;
  }
  inline void LayStep( oCWorld* world ) {
    if( g_layPhase == LAY_IDLE ) return;
    const double start = DpMs();
    const double until = start + g_dpFrameMs;
    g_layFrames++;
    bool done = false;
    if( g_layPhase == LAY_RAYS ) {
      if( !LayRaysCore( world, until ) ) {
        MM_LOG( "player layer: failed (rays)" );
        g_layPhase = LAY_IDLE;
        g_layLast  = GetTickCount();
        return;
      }
      const int nb = ( g_layS > 0 ) ? ( g_layN + g_layS - 1 ) / g_layS : 0;
      if( g_layCur >= nb * nb ) {
        LaySetupShade( world );
        g_layPhase = LAY_SHADE;
      }
    }
    if( g_layPhase == LAY_SHADE && DpMs() < until ) {
      const int n = g_layN;
      while( g_layCur < n && DpMs() < until ) {
        int b = g_layCur + 64;
        if( b > n ) b = n;
        DpParallel( GenShadeJob, &g_layShade, g_layCur, b );
        g_layCur = b;
      }
      if( g_layCur >= n ) {
        g_layPhase = LAY_IDLE;
        if( !GenUpload( g_layTex, g_layPx, n, g_layBpp ) ) {
          MM_LOG( "player layer: texture upload failed" );
          g_layLast = GetTickCount();
          return;
        }
        g_layRect  = g_layPend;
        g_layCX    = g_layPCX;
        g_layCZ    = g_layPCZ;
        g_laySide  = g_layPSide;
        g_layReady = true;
        g_layFloor = g_layPFloor;
        g_layLast  = GetTickCount();
        done       = true;
      }
    }
    const double ms = DpMs() - start;
    g_layWorkMs += ms;
    if( (int)ms > g_layLongest ) g_layLongest = (int)ms;
    if( done ) {
      const int total = (int)( GetTickCount() - g_layT0 );
      if( LayLogTick( g_layLongest ) )
      MM_LOG( zSTRING( "player layer: " ) + zSTRING( total ) + " ms total, work " + zSTRING( (int)g_layWorkMs ) + " ms in " + zSTRING( g_layFrames ) + " frames (longest " + zSTRING( g_layLongest ) + " ms), " + zSTRING( g_layN ) + "x" + zSTRING( g_layN ) + ", side " + zSTRING( (int)g_layPSide ) + ", polygons " + zSTRING( g_layPolysN ) + ", rays " + zSTRING( g_layRaysN ) + ", ceiling " + zSTRING( (int)g_layCeil ) + ", coverage " + zSTRING( g_layCovPct ) + "%, level " + zSTRING( g_genIndoorLvl ) );
    }
  }
  inline void LayProbeCover( oCWorld* world, const zVEC3& pp, float headY,
                             int rayLen, int& valid, int& hit ) {
    valid = 0;
    hit   = 0;
    const int flagsDown = zTRACERAY_STAT_POLY | zTRACERAY_POLY_NORMAL
                        | zTRACERAY_VOB_IGNORE_CHARACTER
                        | zTRACERAY_VOB_IGNORE_NO_CD_DYN;
    const int flagsUp   = zTRACERAY_STAT_POLY | zTRACERAY_VOB_IGNORE_CHARACTER
                        | zTRACERAY_VOB_IGNORE_NO_CD_DYN | zTRACERAY_POLY_2SIDED;
    static const float kDir[8][2] = {
      {  1.0f,     0.0f    }, {  0.7071f,  0.7071f }, {  0.0f,  1.0f    }, { -0.7071f,  0.7071f },
      { -1.0f,     0.0f    }, { -0.7071f, -0.7071f }, {  0.0f, -1.0f    }, {  0.7071f, -0.7071f }
    };
    for( int k = 0; k < 9; k++ ) {
      const float sx = pp[0] + ( ( k == 0 ) ? 0.0f : kDir[k - 1][0] * LAY_PROBE_R );
      const float sz = pp[2] + ( ( k == 0 ) ? 0.0f : kDir[k - 1][1] * LAY_PROBE_R );
      const zVEC3 dFrom( sx, headY, sz );
      const zVEC3 dRay( 0.0f, -600.0f, 0.0f );
      if( !world->TraceRayNearestHit( dFrom, dRay, (zCVob*)player, flagsDown )
          || !world->traceRayReport.foundHit )
        continue;
      const float floorY = world->traceRayReport.foundIntersection[1];
      valid++;
      const zVEC3 uFrom( sx, floorY + 160.0f, sz );
      const zVEC3 uRay( 0.0f, (float)rayLen, 0.0f );
      if( world->TraceRayNearestHit( uFrom, uRay, (zCVob*)player, flagsUp )
          && world->traceRayReport.foundHit ) {
        zCVob* const v = world->traceRayReport.foundVob;
        if( v != (zCVob*)player )
          hit++;
      }
    }
  }
  static DWORD g_packTick = 0;
  static bool  g_packDone = false;
  inline void PackTick() {
    DpSaveFinish( false );
    if( !g_genCache || g_genPack == 0 || g_packDone || g_genRunning || g_dpPhase != DP_IDLE ) return;
    const DWORD now = GetTickCount();
    if( now - g_packTick < 10000 ) return;
    g_packTick = now;
    if( PkRepackDirNext( FILES_DET_DIR, (unsigned)DP_CACHE_MAGIC, 8, 2, 4, g_genPack ) == 0
        && PkRepackDirNext( FILES_GEN_DIR, (unsigned)GEN_CACHE_MAGIC, 6, 1, 3, g_genPack ) == 0 )
      g_packDone = true;
  }
  inline void UpdatePlayerLayer() { PackTick(); DetailPatchUpdate();
    if( g_genRunning ) return;
    if( !g_texReady ) return;
    if( !g_genIndoor || !player || !ogame || g_mapIdx < 0 ) {
      g_layReady   = false;
      g_layCovered = false;
      g_layVote    = 0;
      g_layHold    = 0;
      return;
    }
    oCWorld* const world = ogame->GetGameWorld();
    if( !world ) return;
    const DWORD now = GetTickCount();
    const zTBBox3D& pb = player->bbox3D;
    const float floorY = pb.mins[1];
    const float headY  = pb.maxs[1];
    const zVEC3 pp     = player->GetPositionWorld();
    if( now >= g_layNext ) {
      g_layNext = now + 250;
      const TLayLevel& lv = kLayLevels[ ( g_genIndoorLvl < 1 ) ? 1
                                      : ( ( g_genIndoorLvl > 3 ) ? 3 : g_genIndoorLvl ) ];
      int valid = 0, hit = 0;
      LayProbeCover( world, pp, headY, lv.rayLen, valid, hit );
      if( valid >= 4 ) {
        g_layCovPct = hit * 100 / valid;
        const bool want = g_layCovered ? ( g_layCovPct >= lv.offPct )
                                       : ( g_layCovPct >= lv.onPct );
        if( want == g_layCovered || ( !want && g_layHold > 0 ) )
          g_layVote = 0;
        else if( ++g_layVote >= ( want ? lv.kOn : lv.kOff ) ) {
          g_layCovered = want;
          g_layVote    = 0;
          g_layHold    = want ? lv.hold : 0;
          MM_LOG( zSTRING( "indoor layer: " ) + zSTRING( want ? "on" : "off" ) + ", coverage " + zSTRING( g_layCovPct ) + "%, level " + zSTRING( g_genIndoorLvl ) );
        }
      }
      if( g_layHold > 0 )
        g_layHold--;
    }
    if( !g_layCovered ) {
      g_layReady = false;
      g_layPhase = LAY_IDLE;
      return;
    }
    if( g_layPhase != LAY_IDLE ) {
      LayStep( world );
      return;
    }
    bool need = !g_layReady;
    if( !need ) {
      const float dx = pp[0] - g_layCX, dz = pp[2] - g_layCZ;
      const float q  = g_laySide * 0.25f;
      if( dx > q || dx < -q || dz > q || dz < -q ) need = true;
      if( floorY - g_layFloor > LAY_FLOOR_STEP || g_layFloor - floorY > LAY_FLOOR_STEP ) need = true;
      const float want = LaySide();
      if( want > g_laySide * 1.5f || want < g_laySide * 0.5f ) need = true;
      if( need && now - g_layLast < 300 ) need = false;
    }
    if( need )
      LayCompute( world, floorY, headY );
  }
  inline void DrawPlayerLayer( int x, int y, int w, int h ) {
    const DWORD now = GetTickCount();
    const float dt  = ( g_layFadeT && now - g_layFadeT < 500 )
                    ? (float)( now - g_layFadeT ) * 0.001f : 0.0f;
    g_layFadeT = now;
    const float target = g_layReady ? 1.0f : 0.0f;
    const float step   = dt / 0.35f;
    if( g_layFade < target ) {
      g_layFade += step;
      if( g_layFade > target ) g_layFade = target;
    }
    else if( g_layFade > target ) {
      g_layFade -= step;
      if( g_layFade < target ) g_layFade = target;
    }
    if( g_layFade <= 0.0f || !g_layTex ) return;
    int a = (int)( (float)g_alpha * (float)g_genIndoorAlpha * 0.01f * g_layFade + 0.5f );
    if( a < 1 ) a = 1; else if( a > 255 ) a = 255;
    g_curTex   = g_layTex;
    g_curRect  = &g_layRect;
    g_curBlend = true;
    g_curAlpha = a;
    DrawMapTexture( x, y, w, h );
    g_curAlpha = -1;
    g_curTex   = 0;
    g_curRect  = 0;
    g_curBlend = false;
  }
  enum {
    GEN_PH_NONE = 0, GEN_PH_MATS, GEN_PH_RASTER, GEN_PH_TREES, GEN_PH_HITS, GEN_PH_MASK,
    GEN_PH_RAYS, GEN_PH_STATS, GEN_PH_PLANTS, GEN_PH_OUTSIDE, GEN_PH_FILL,
    GEN_PH_SEA, GEN_PH_PATHS, GEN_PH_SMOOTH, GEN_PH_LAND, GEN_PH_SHADE,
    GEN_PH_UPLOAD, GEN_PH_SAVE, GEN_PH_N
  };
  const double GEN_FRAME_MS       = 500.0;
  const DWORD  GEN_START_DELAY_MS = 300;
  static bool           g_genRedo    = false;
  static oCWorld*       g_gmWorld    = 0;
  static char           g_gmWorldName[NAME_MAX] = "";
  static TMapInfo       g_gmRect;
  static int            g_gmSig      = 0;
  static int            g_gmN        = 0;
  static int            g_gmBpp      = 4;
  static DWORD          g_gmStartAt  = 0;
  static DWORD          g_gmT0       = 0;
  static DWORD          g_gmLastTick = 0;
  static float*         g_gmHgt      = 0;
  static unsigned char* g_gmLit       = 0;
  static unsigned char* g_gmMat      = 0;
  static unsigned char* g_gmTint     = 0;
  static unsigned char* g_gmPx       = 0;
  static unsigned char* g_gmOuts     = 0;
  static unsigned char* g_gmMask     = 0;
  static float*         g_gmPrev     = 0;
  static const void**   g_gmHKey     = 0;
  static unsigned char* g_gmHVal     = 0;
  static unsigned char* g_gmHTnt     = 0;
  static int*           g_gmWRow     = 0;
  static int*           g_gmWCol     = 0;
  static TGenFastCtx    g_gmFast;
  static TGenShade      g_gmShade;
  static TGenSmooth     g_gmSm;
  static bool           g_gmEntered  = false;
  static int            g_gmCur      = 0;
  static int            g_gmPass     = 0;
  static int            g_gmSub      = 0;
  static int            g_gmCount    = 0;
  static int            g_gmSeq      = 16;
  static int            g_gmPar      = 8;
  static int            g_gmThreads  = 1;
  static float          g_gmTop      = 0.0f;
  static float          g_gmCw = 0.0f, g_gmCh = 0.0f;
  static int            g_gmWide     = 1;
  static float          g_gmHMin = 0.0f, g_gmHMax = 0.0f;
  static int            g_gmHits = 0, g_gmWater = 0, g_gmVobs = 0, g_gmPlants = 0;
  static int            g_gmHoles = 0, g_gmSea = 0, g_gmPaths = 0;
  static int            g_gmPolys = 0, g_gmMats = 0, g_gmRays = 0;
  static int            g_gmTreeTris = 0, g_gmTreeStamps = 0;
  static double         g_gmTreeMs   = 0.0;
  static int            g_gmFrames   = 0;
  static double         g_gmWorkMs   = 0.0;
  static double         g_gmMaxMs    = 0.0;
  static DWORD          g_gmPhMs[GEN_PH_N];
  static DWORD          g_gmPhMsLast[GEN_PH_N];
  inline void GenFreeBufs() {
    MiniMapCommon::MmFree( g_gmHgt );   g_gmHgt  = 0;
    MiniMapCommon::MmFree( g_gmLit );   g_gmLit  = 0;
    MiniMapCommon::MmFree( g_gmMat );   g_gmMat  = 0;
    MiniMapCommon::MmFree( g_gmTint );  g_gmTint = 0;
    MiniMapCommon::MmFree( g_gmPx );    g_gmPx   = 0;
    MiniMapCommon::MmFree( g_gmOuts );  g_gmOuts = 0;
    MiniMapCommon::MmFree( g_gmMask );  g_gmMask = 0;
    MiniMapCommon::MmFree( g_gmPrev );  g_gmPrev = 0;
    MiniMapCommon::MmFree( g_gmHKey );  g_gmHKey = 0;
    MiniMapCommon::MmFree( g_gmHVal );  g_gmHVal = 0;
    MiniMapCommon::MmFree( g_gmHTnt );  g_gmHTnt = 0;
    MiniMapCommon::MmFree( g_gmWRow );  g_gmWRow = 0;
    MiniMapCommon::MmFree( g_gmWCol );  g_gmWCol = 0;
    TreeBatchFree( g_gmTB );
  }
  inline void GenStop() {
    GenFreeBufs();
    g_genRunning = false;
    g_genHeld    = false;
    g_genPhase   = GEN_PH_NONE;
    g_genFrac    = 0.0f;
    g_genEtaMs   = -1;
    g_gmWorld    = 0;
  }
  inline void GenAbort( const char* why = "aborted" ) {
    if( !g_genRunning && !g_gmHgt && !g_gmPx ) return;
    MM_LOG( zSTRING( "terrain map: calculation aborted (" ) + zSTRING( why ) + ") in phase " + zSTRING( g_genPhase ) );
    if( g_genRunning ) g_genRedo = true;
    GenStop();
  }
  inline void GenFail( const char* why ) {
    MM_LOG( zSTRING( "terrain map: calculation failed (" ) + zSTRING( why ) + ")" );
    GenSurfReset();
    GenStop();
  }
  inline void GenPhase( int ph ) {
    g_genPhase  = ph;
    g_gmEntered = false;
    g_gmCur     = 0;
    g_gmPass    = 0;
    g_gmSub     = 0;
    g_gmCount   = 0;
    g_gmSeq     = 16;
    int par = g_gmN / ( g_gmThreads * 32 );
    if( par < 4 ) par = 4;
    g_gmPar = par;
  }
  inline void GenAdapt( int& v, double ms, int lo, int hi ) {
    const double want = GEN_FRAME_MS * 0.3;
    double k = want / ( ( ms > 0.05 ) ? ms : 0.05 );
    if( k > 2.0 ) k = 2.0;
    if( k > 0.8 && k < 1.25 ) return;
    int s = (int)( (double)v * k );
    if( k > 1.0 && s <= v ) s = v + 1;
    if( s < lo ) s = lo; else if( s > hi ) s = hi;
    v = s;
  }
  inline const char* GenMatsCore( oCWorld* world ) {
    const char* why = 0;
    __try {
      for( ;; ) {
        const TMapInfo& m = g_gmRect;
        if( !world || !world->bspTree.bspRoot ) { why = "no world"; break; }
        if( m.x2 == m.x1 || m.y2 == m.y1 )       { why = "empty map area"; break; }
        zCPolygon** polys = 0; int numPoly = 0;
        if( !WorldBspPolys( world, polys, numPoly ) ) { why = "no polygon list"; break; }
        g_gmPolys = numPoly;
        const unsigned int HSIZE = 65536;
        const unsigned int hmask = HSIZE - 1;
        g_gmHKey = MiniMapCommon::MmAlloc<const void*>( (size_t)( HSIZE ) );
        g_gmHVal = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( HSIZE ) );
        if( !g_gmHKey || !g_gmHVal ) { why = "out of memory for materials"; break; }
        memset( g_gmHKey, 0, (size_t)( HSIZE ) * sizeof( g_gmHKey[0] ) );
    memset( g_gmHVal, 0, (size_t)( HSIZE ) * sizeof( g_gmHVal[0] ) );
        if( g_gmTint ) {
          g_gmHTnt = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( HSIZE ) );
          if( g_gmHTnt )
            memset( g_gmHTnt, 0, (size_t)( HSIZE ) * sizeof( g_gmHTnt[0] ) );
        }
        int mats = 0;
        bool full = false;
        for( int p = 0; p < numPoly && !full; p++ ) {
          const zCPolygon* const poly = polys[p];
          if( !GenFastPolyOk( poly ) || !poly->material ) continue;
          const int r = GenInsertMatHash( poly->material, g_gmHKey, g_gmHVal, g_gmHTnt, hmask, mats, (int)( HSIZE / 4 * 3 ) );
          if( r > 0 ) mats++; else if( r < 0 ) full = true;
        }
        if( full ) { why = "too many materials"; break; }
        g_gmMats = mats;
        TGenFastCtx& C = g_gmFast;
        C.polys   = polys;
        C.numPoly = numPoly;
        C.n       = g_gmN;
        C.x1      = (double)m.x1;
        C.z1      = (double)m.y1;
        C.sx      = (double)g_gmN / ( (double)m.x2 - (double)m.x1 );
        C.sz      = (double)g_gmN / ( (double)m.y2 - (double)m.y1 );
        C.ceil    = 1.0e30f;
        C.hgt     = g_gmHgt;
        C.lit     = g_gmLit;
        C.mat     = g_gmMat;
        C.hkey    = g_gmHKey;
        C.hval    = g_gmHVal;
        C.hmask   = hmask;
        C.htint   = g_gmHTnt;
        C.tint    = g_gmHTnt ? g_gmTint : 0;
        C.lx      = -0.45f;
        C.ly      =  0.78f;
        C.lz      =  0.44f;
        C.rows    = 0;
        C.chunks  = 0;
        C.next    = 0;
        C.failed  = 0;
        break;
      }
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      why = "exception";
    }
    return why;
  }
  inline bool GenMaskCore( oCWorld* world ) {
    __try {
      const int n  = g_gmN;
      const int nn = n * n;
      const TGenFastCtx& C = g_gmFast;
      const zTBBox3D& bb = world->bspTree.bspRoot->bbox3D;
      g_gmTop = bb.maxs[1] + 500.0f;
      const float minBig = ( g_genVobMin < g_genPlantMin )
                         ? (float)g_genVobMin : (float)g_genPlantMin;
      const int maskBytes = ( nn + 7 ) / 8;
      g_gmMask = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( maskBytes ) );
      if( !g_gmMask ) return true;
      unsigned char* const mask = g_gmMask;
      memset( mask, 0, (size_t)( maskBytes ) * sizeof( mask[0] ) );
      for( zCListSort<zCVob>* node = world->voblist; node; node = node->next ) {
        zCVob* const vob = node->data;
        if( !vob || !vob->GetVisual() ) continue;
        if( vob->type == zVOB_TYPE_LEVEL_COMPONENT ) continue;
        const zTBBox3D& vb = vob->bbox3D;
        const float sx = vb.maxs[0] - vb.mins[0];
        const float sz = vb.maxs[2] - vb.mins[2];
        const float big = ( sx > sz ) ? sx : sz;
        if( !( big >= minBig ) ) continue;
        double ax = ( (double)vb.mins[0] - C.x1 ) * C.sx - 0.5;
        double bx = ( (double)vb.maxs[0] - C.x1 ) * C.sx - 0.5;
        double az = ( (double)vb.mins[2] - C.z1 ) * C.sz - 0.5;
        double bz = ( (double)vb.maxs[2] - C.z1 ) * C.sz - 0.5;
        if( ax > bx ) { const double t = ax; ax = bx; bx = t; }
        if( az > bz ) { const double t = az; az = bz; bz = t; }
        ax = ceil( ax ) - 1.0;
        bx = floor( bx ) + 1.0;
        az = ceil( az ) - 1.0;
        bz = floor( bz ) + 1.0;
        if( bx < 0.0 || bz < 0.0 || ax > (double)( n - 1 ) || az > (double)( n - 1 ) ) continue;
        if( ax < 0.0 ) ax = 0.0;
        if( az < 0.0 ) az = 0.0;
        if( bx > (double)( n - 1 ) ) bx = (double)( n - 1 );
        if( bz > (double)( n - 1 ) ) bz = (double)( n - 1 );
        const int ix0 = (int)ax, ix1 = (int)bx;
        const int iz0 = (int)az, iz1 = (int)bz;
        for( int iz = iz0; iz <= iz1; iz++ )
          for( int ix = ix0; ix <= ix1; ix++ ) {
            const int i = iz * n + ix;
            mask[i >> 3] |= (unsigned char)( 1 << ( i & 7 ) );
          }
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline bool GenRaysCore( oCWorld* world, double until ) {
    __try {
      const TMapInfo& m  = g_gmRect;
      const int       n  = g_gmN;
      const int       nn = n * n;
      const float     top = g_gmTop;
      const int       flagsVob = zTRACERAY_STAT_POLY | zTRACERAY_POLY_NORMAL
                               | zTRACERAY_VOB_IGNORE_CHARACTER;
      unsigned char* const mask = g_gmMask;
      float*         const hgt  = g_gmHgt;
      const unsigned char* const lit = g_gmLit;
      unsigned char* const mat  = g_gmMat;
      unsigned char* const tint = g_gmTint;
      int k = 0, kr = 0;
      while( g_gmCur < nn ) {
        if( ( ++k & 8191 ) == 0 && DpMs() >= until ) break;
        const int i = g_gmCur;
        if( !lit[i] || ( mask && !( mask[i >> 3] & ( 1 << ( i & 7 ) ) ) ) ) {
          g_gmCur++;
          continue;
        }
        if( ( ++kr & 15 ) == 0 && DpMs() >= until ) break;
        g_gmCur++;
        const int   iz = i / n;
        const int   ix = i - iz * n;
        const float wz = m.y1 + ( m.y2 - m.y1 ) * ( (float)iz + 0.5f ) / (float)n;
        const float wx = m.x1 + ( m.x2 - m.x1 ) * ( (float)ix + 0.5f ) / (float)n;
        const zVEC3 from( wx, top, wz );
        g_gmRays++;
        const float terrY = hgt[i];
        const zVEC3 ray( 0.0f, -( top - ( terrY + 50.0f ) ), 0.0f );
        if( world->TraceRayNearestHit( from, ray, (zCVob*)0, flagsVob )
            && world->traceRayReport.foundHit
            && world->traceRayReport.foundVob ) {
          zCVob* const    vob  = world->traceRayReport.foundVob;
          const float     vobY = world->traceRayReport.foundIntersection[1];
          const zTBBox3D& vbb  = vob->bbox3D;
          const float     sx   = vbb.maxs[0] - vbb.mins[0];
          const float     sz   = vbb.maxs[2] - vbb.mins[2];
          const float     big  = ( sx > sz ) ? sx : sz;
          zCVisual* const     vis = vob->GetVisual();
          const char* const   vnm = vis ? vis->GetObjectName().ToChar() : 0;
          const unsigned char cls = GenSurfCached( vis, vnm, true );
          const float         min = ( cls == SURF_PLANT ) ? (float)g_genPlantMin
                                                          : (float)g_genVobMin;
          const bool giant = ( cls == SURF_PLANT )
                          && TreeGiantSize( sx, sz, vbb.maxs[1] - vbb.mins[1], vnm );
          if( big >= min && vobY > terrY + 50.0f && !giant ) {
            hgt[i] = vobY;
            mat[i] = cls;
            if( tint ) tint[i] = 0;
            g_gmVobs++;
          }
        }
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline void GenPlantsStep( oCWorld* world ) {
    if( !g_genVobs || !world->voblist ) return;
    const TMapInfo& m = g_gmRect;
    const int n = g_gmN;
    float* const         hgt = g_gmHgt;
    const unsigned char* const lit = g_gmLit;
    unsigned char* const mat = g_gmMat;
    const float cellW = ( ( m.x2 - m.x1 ) < 0.0f ? -( m.x2 - m.x1 )
                                                 :  ( m.x2 - m.x1 ) ) / (float)n;
    for( zCListSort<zCVob>* node = world->voblist; node; node = node->next ) {
      zCVob* const vob = node->data;
      if( !vob ) continue;
      zCVisual* const vis = vob->GetVisual();
      if( !vis ) continue;
      if( GenSurfCached( vis, vis->GetObjectName().ToChar(), true ) != SURF_PLANT ) continue;
      const zTBBox3D& bb2 = vob->bbox3D;
      const float sx = bb2.maxs[0] - bb2.mins[0];
      const float sz = bb2.maxs[2] - bb2.mins[2];
      const float big = ( sx > sz ) ? sx : sz;
      if( big < (float)g_genPlantMin ) continue;
      const zVEC3 pos = vob->GetPositionWorld();
      const float fx  = ( pos[0] - m.x1 ) / ( m.x2 - m.x1 ) * (float)n;
      const float fz  = ( pos[2] - m.y1 ) / ( m.y2 - m.y1 ) * (float)n;
      const int   cx  = (int)fx;
      const int   cz  = (int)fz;
      if( cx < 0 || cx >= n || cz < 0 || cz >= n ) continue;
      int rad = (int)( big * 0.5f / ( cellW > 0.1f ? cellW : 1.0f ) );
      if( rad < 1 ) rad = 1; else if( rad > 10 ) rad = 10;
      const int rr = rad * rad;
      const float top = bb2.maxs[1];
      for( int dz = -rad; dz <= rad; dz++ ) {
        const int zz = cz + dz;
        if( zz < 0 || zz >= n ) continue;
        for( int dx = -rad; dx <= rad; dx++ ) {
          if( dx * dx + dz * dz > rr ) continue;
          const int xx = cx + dx;
          if( xx < 0 || xx >= n ) continue;
          const int q = zz * n + xx;
          if( !lit[q] || mat[q] == SURF_WATER ) continue;
          if( mat[q] == SURF_ROOF || mat[q] == SURF_STONEWORK ) continue;
          mat[q] = SURF_PLANT;
          if( top > hgt[q] )
            hgt[q] += ( top - hgt[q] ) * 0.35f;
        }
      }
      g_gmPlants++;
    }
  }
  #define MM_OUT_GET( i )  ( ( outs[(i) >> 3] >> ( (i) & 7 ) ) & 1 )
  #define MM_OUT_SET( i )  ( outs[(i) >> 3] |= (unsigned char)( 1 << ( (i) & 7 ) ) )
  inline void GenOutRows( int k0, int k1, bool back ) {
    const int n = g_gmN;
    const unsigned char* const lit = g_gmLit;
    unsigned char* const outs = g_gmOuts;
    int changed = 0;
    for( int k = k0; k < k1; k++ ) {
      if( !back ) {
        const int iz = 1 + k;
        for( int ix = 1; ix < n - 1; ix++ ) {
          const int q = iz * n + ix;
          if( lit[q] != 0 || MM_OUT_GET( q ) ) continue;
          if( MM_OUT_GET( q - 1 ) || MM_OUT_GET( q - n ) ) {
            MM_OUT_SET( q );
            changed++;
          }
        }
      }
      else {
        const int iz = n - 2 - k;
        for( int ix = n - 2; ix > 0; ix-- ) {
          const int q = iz * n + ix;
          if( lit[q] != 0 || MM_OUT_GET( q ) ) continue;
          if( MM_OUT_GET( q + 1 ) || MM_OUT_GET( q + n ) ) {
            MM_OUT_SET( q );
            changed++;
          }
        }
      }
    }
    g_gmCount += changed;
  }
  inline void GenFillRows( int z0, int z1, int round ) {
    const int n = g_gmN;
    float* const         hgt  = g_gmHgt;
    unsigned char* const lit = g_gmLit;
    unsigned char* const mat  = g_gmMat;
    unsigned char* const tint = g_gmTint;
    unsigned char* const outs = g_gmOuts;
    for( int iz = z0; iz < z1; iz++ )
      for( int ix = 0; ix < n; ix++ ) {
        const int q = iz * n + ix;
        if( lit[q] != 0 || MM_OUT_GET( q ) ) continue;
        float sumH = 0.0f;
        int   sumL = 0;
        int   cnt = 0, best = SURF_TERRAIN, bestT = 0;
        for( int dz = -1; dz <= 1; dz++ )
          for( int dx = -1; dx <= 1; dx++ ) {
            const int zz = iz + dz, xx = ix + dx;
            if( zz < 0 || zz >= n || xx < 0 || xx >= n ) continue;
            const int s = zz * n + xx;
            if( !lit[s] ) continue;
            sumH += hgt[s];
            sumL += (int)lit[s];
            best  = mat[s];
            if( tint ) bestT = tint[s];
            cnt++;
          }
        if( cnt < ( ( round < 24 ) ? 2 : 1 ) ) continue;
        hgt[q] = sumH / (float)cnt;
        lit[q] = (unsigned char)( ( sumL + cnt / 2 ) / cnt );
        mat[q] = (unsigned char)best;
        if( tint ) tint[q] = (unsigned char)bestT;
        g_gmCount++;
        g_gmHoles++;
      }
  }
  inline void GenSeaRows( int k0, int k1, bool back ) {
    const int n = g_gmN;
    float* const         hgt  = g_gmHgt;
    unsigned char* const lit = g_gmLit;
    unsigned char* const mat  = g_gmMat;
    unsigned char* const tint = g_gmTint;
    unsigned char* const outs = g_gmOuts;
    if( g_genOutside == 3 ) {
      for( int k = k0; k < k1; k++ ) {
        if( !back ) {
          const int iz = k;
          for( int ix = 0; ix < n; ix++ ) {
            const int q = iz * n + ix;
            if( lit[q] != 0 ) continue;
            int src = -1;
            if( ix > 0 && lit[q - 1] != 0 )      src = q - 1;
            else if( iz > 0 && lit[q - n] != 0 ) src = q - n;
            if( src < 0 ) continue;
            hgt[q] = hgt[src];
            lit[q] = lit[src];
            mat[q] = mat[src];
            if( tint ) tint[q] = tint[src];
            g_gmSea++;
          }
        }
        else {
          const int iz = n - 1 - k;
          for( int ix = n - 1; ix >= 0; ix-- ) {
            const int q = iz * n + ix;
            if( lit[q] != 0 ) continue;
            int src = -1;
            if( ix < n - 1 && lit[q + 1] != 0 )      src = q + 1;
            else if( iz < n - 1 && lit[q + n] != 0 ) src = q + n;
            if( src < 0 ) continue;
            hgt[q] = hgt[src];
            lit[q] = lit[src];
            mat[q] = mat[src];
            if( tint ) tint[q] = tint[src];
            g_gmSea++;
          }
        }
      }
      return;
    }
    const float hmin = g_gmHMin, hmax = g_gmHMax;
    const float range2 = ( hmax - hmin > 1.0f ) ? ( hmax - hmin ) : 1000.0f;
    for( int q = k0 * n; q < k1 * n; q++ ) {
      if( lit[q] != 0 || !MM_OUT_GET( q ) ) continue;
      if( g_genOutside == 1 ) {
        hgt[q] = hmin;
        lit[q] = PackLit( 1.0f );
        mat[q] = SURF_WATER;
      }
      else {
        const int   ix2 = q % n, iz2 = q / n;
        const float bump = GenNoise( ix2 >> 3, iz2 >> 3 ) * 0.55f
                         + GenNoise( ix2 >> 1, iz2 >> 1 ) * 0.45f;
        hgt[q] = hmin + range2 * ( 0.72f + bump * 0.35f );
        lit[q] = PackLit( 0.9f );
        mat[q] = SURF_ROCK;
      }
      g_gmSea++;
    }
  }
  #undef MM_OUT_GET
  #undef MM_OUT_SET
  inline void GenPathEdge( int e ) {
    const TMapInfo& m = g_gmRect;
    const int n = g_gmN, wide = g_gmWide;
    const unsigned char* const lit = g_gmLit;
    unsigned char* const mat = g_gmMat;
    const TEdge& ed = g_edges[e];
    const float fx1 = ( ed.ax - m.x1 ) / g_gmCw;
    const float fz1 = ( ed.az - m.y1 ) / g_gmCh;
    const float fx2 = ( ed.bx - m.x1 ) / g_gmCw;
    const float fz2 = ( ed.bz - m.y1 ) / g_gmCh;
    float dx = fx2 - fx1, dz = fz2 - fz1;
    float len = (float)sqrt( dx * dx + dz * dz );
    if( len < 0.001f ) return;
    int steps = (int)( len * 2.0f ) + 1;
    if( steps > 4096 ) steps = 4096;
    for( int t = 0; t <= steps; t++ ) {
      const float f  = (float)t / (float)steps;
      const int   cx = (int)( fx1 + dx * f );
      const int   cz = (int)( fz1 + dz * f );
      for( int oz = -wide; oz <= wide; oz++ )
        for( int ox = -wide; ox <= wide; ox++ ) {
          if( ox * ox + oz * oz > wide * wide ) continue;
          const int xx = cx + ox, zz = cz + oz;
          if( xx < 0 || xx >= n || zz < 0 || zz >= n ) continue;
          const int q = zz * n + xx;
          if( !lit[q] ) continue;
          if( mat[q] == SURF_ROOF ) continue;
          if( mat[q] == SURF_WATER && !g_genPathsWater ) continue;
          if( mat[q] == SURF_PATH ) continue;
          mat[q] = SURF_PATH;
          g_gmPaths++;
        }
    }
  }
  inline void GenParRange( TGenParFn fn, void* ctx, int a, int b, int chunk ) {
    if( b <= a ) return;
    TDpSlice s;
    s.fn   = fn;
    s.ctx  = ctx;
    s.base = a;
    GenParallel( DpSliceJob, &s, b - a, ( chunk > 0 ) ? chunk : 1 );
  }
  inline int GenSubChunk( int par ) {
    const int c = par / 4;
    return ( c > 0 ) ? c : 1;
  }
  static const int kGenPhW[GEN_PH_N] = {
    0, 100, 3000, 1000, 300, 100, 1500, 300, 100, 2500, 2500, 800, 1500, 600, 300, 4000, 300, 800
  };
  inline float GenPhaseFrac() {
    const int n  = ( g_gmN > 0 ) ? g_gmN : 1;
    float f = 0.0f;
    switch( g_genPhase ) {
      case GEN_PH_RASTER: case GEN_PH_HITS: case GEN_PH_STATS:
      case GEN_PH_LAND:   case GEN_PH_SHADE:
        f = (float)g_gmCur / (float)n;
        break;
      case GEN_PH_RAYS:
        f = (float)( (double)g_gmCur / ( (double)n * (double)n ) );
        break;
      case GEN_PH_TREES:
        f = ( g_tvCount > 0 ) ? (float)g_gmTB.vi / (float)g_tvCount : 0.0f;
        break;
      case GEN_PH_OUTSIDE:
        f = ( (float)( g_gmPass * 2 + g_gmSub ) + (float)g_gmCur / (float)n ) / 12.0f;
        break;
      case GEN_PH_FILL:
        f = ( (float)g_gmPass + (float)g_gmCur / (float)n ) / 8.0f;
        break;
      case GEN_PH_SEA:
        f = ( g_genOutside == 3 )
          ? ( (float)( g_gmPass * 2 + g_gmSub ) + (float)g_gmCur / (float)n ) / 12.0f
          : (float)g_gmCur / (float)n;
        break;
      case GEN_PH_PATHS:
        f = ( g_edgeCount > 0 ) ? (float)g_gmCur / (float)g_edgeCount : 0.0f;
        break;
      case GEN_PH_SMOOTH: {
        const int sw = ( g_genSmooth > 0 ) ? g_genSmooth * 2 : 1;
        f = ( (float)g_gmPass + (float)g_gmCur / (float)n ) / (float)sw;
        break;
      }
      default:
        f = 0.0f;
    }
    if( f < 0.0f ) f = 0.0f; else if( f > 0.95f ) f = 0.95f;
    return f;
  }
  inline void GenProgressUpdate() {
    DWORD lastSum = 0;
    for( int i = 1; i < GEN_PH_N; i++ )
      lastSum += g_gmPhMsLast[i];
    const bool useLast = ( lastSum >= 1000 );
    double total = 0.0, done = 0.0;
    for( int i = 1; i < GEN_PH_N; i++ ) {
      const double w = useLast ? (double)g_gmPhMsLast[i] : (double)kGenPhW[i];
      total += w;
      if( i < g_genPhase )
        done += w;
      else if( i == g_genPhase )
        done += w * (double)GenPhaseFrac();
    }
    float f = ( total > 0.0 && g_genPhase > GEN_PH_NONE ) ? (float)( done / total ) : 0.0f;
    if( f > 0.99f ) f = 0.99f;
    g_genFrac = f;
    const int gi  = GenMsIdx( g_gmN );
    const int all = useLast ? (int)lastSum
                  : ( ( gi >= 0 && g_genMs[gi] > 0 ) ? g_genMs[gi] : -1 );
    if( g_genPhase == GEN_PH_NONE ) {
      const int wait = (int)( g_gmStartAt - GetTickCount() );
      g_genEtaMs = ( all > 0 ) ? all + ( wait > 0 ? wait : 0 ) : -1;
      return;
    }
    const DWORD el = GetTickCount() - g_gmT0;
    if( all > 0 )
      g_genEtaMs = (int)( (double)all * ( 1.0 - (double)f ) );
    else if( f >= 0.10f && el >= 3000 )
      g_genEtaMs = (int)( (double)el * ( 1.0 - (double)f ) / (double)f );
    else
      g_genEtaMs = -1;
  }
  inline bool GenStart( oCWorld* world ) {
    const TMapInfo& m = g_gmRect;
    int n = g_gmN;
    if( ( g_genTex && g_genTexSize != n ) || ( !g_genTex && g_genBpp <= 0 ) ) {
      GenFail( "no texture" );
      return false;
    }
    LayFree();
    if( ( !g_texReady || ProcLowMem() ) && g_genTex ) {
      if( g_texReady && ProcLowMem() )
        MM_LOG( zSTRING( "terrain map: low memory (no LAA) - old texture freed before grid " ) + zSTRING( n ) );
      DeferGenTexture();
    }
    for( ;; ) {
      if( !CanAllocGrid( n, g_genBpp ? g_genBpp : 4 ) && g_genTex ) {
        MM_LOG( zSTRING( "terrain map: freeing previous texture before grid " ) + zSTRING( n ) );
        DeferGenTexture();
      }
      if( CanAllocGrid( n, g_genBpp ? g_genBpp : 4 ) ) {
        g_gmHgt = MiniMapCommon::MmAlloc<float>( (size_t)( n * n ) );
        g_gmLit = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n ) );
        g_gmMat = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n ) );
      }
      if( g_gmHgt && g_gmLit && g_gmMat ) break;
      GenFreeBufs();
      if( n <= 256 ) {
        MM_LOG( zSTRING( "terrain map: out of memory even for grid " ) + zSTRING( n ) + "x" + zSTRING( n ) );
        GenStop();
        return false;
      }
      MM_LOG( zSTRING( "terrain map: out of memory for grid " ) + zSTRING( n ) + "x" + zSTRING( n ) + " - trying " + zSTRING( n / 2 ) );
      if( g_genOomSize == 0 || n < g_genOomSize ) g_genOomSize = n;
      n /= 2;
      zCTexture* const tex = GenEnsureTexture( n );
      if( !tex ) {
        GenStop();
        return false;
      }
      if( g_genTexSize > 0 && g_genTexSize != n ) n = g_genTexSize;
      g_genSizeUsed = n;
      g_gmN = n;
      const int bppNow = g_genBpp ? g_genBpp : 4;
      if( GenCacheLoad( g_world, m, n, bppNow, tex ) ) {
        StrCopy( g_genWorld, g_world, NAME_MAX );
        GenStop();
        ApplyTexture();
        return false;
      }
      DeferGenTexture();
    }
    g_gmN   = n;
    g_gmBpp = g_genBpp ? g_genBpp : 4;
    GenSurfReset();
    g_gmTint = g_genTexColors ? MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n ) ) : 0;
    g_gmThreads = GenParThreads();
    g_gmHMin = g_gmHMax = 0.0f;
    g_gmHits = g_gmWater = g_gmVobs = g_gmPlants = 0;
    g_gmHoles = g_gmSea = g_gmPaths = 0;
    g_gmPolys = g_gmMats = g_gmRays = 0;
    g_gmFrames = 0;
    g_gmWorkMs = 0.0;
    g_gmMaxMs  = 0.0;
    g_gmT0       = GetTickCount();
    g_gmLastTick = g_gmT0;
    memset( g_gmPhMs, 0, (size_t)( GEN_PH_N ) * sizeof( g_gmPhMs[0] ) );
    GenPhase( GEN_PH_MATS );
    MM_LOG( zSTRING( "terrain map: calculating in steps " ) + zSTRING( n ) + "x" + zSTRING( n ) + ", up to " + zSTRING( (int)GEN_FRAME_MS ) + " ms per frame" );
    return true;
  }
  inline void GenFinish() {
    const int n = g_gmN;
    const DWORD all = GetTickCount() - g_gmT0;
    const int gi = GenMsIdx( n );
    if( gi >= 0 ) {
      g_genMs[gi] = (int)( GetTickCount() - g_gmT0 ) + 1;
      RequestSave();
    }
    memcpy( g_gmPhMsLast, g_gmPhMs, (size_t)( GEN_PH_N ) * sizeof( g_gmPhMsLast[0] ) );
    MM_LOG( zSTRING( "terrain map: " ) + zSTRING( n ) + "x" + zSTRING( n ) + ", hits " + zSTRING( g_gmHits ) + ", heights " + zSTRING( (int)g_gmHMin ) + ".." + zSTRING( (int)g_gmHMax ) + ", buildings " + zSTRING( g_gmVobs ) + ", plants " + zSTRING( g_gmPlants ) + ", holes " + zSTRING( g_gmHoles ) + ", background " + zSTRING( g_gmSea ) + ", paths " + zSTRING( g_gmPaths ) + ", water " + zSTRING( g_gmWater ) + ", time " + zSTRING( (int)all ) + " ms" );
    {
      static const char* kPh[GEN_PH_N] = {
        "-", "materials", "raster", "trees", "hits", "mask", "rays",
        "classes", "plants", "outside", "holes", "background", "paths",
        "smoothing", "land", "colours", "texture", "cache"
      };
      zSTRING s = zSTRING( "terrain map: work " ) + zSTRING( (int)g_gmWorkMs ) + " ms in " + zSTRING( g_gmFrames ) + " frames (longest " + zSTRING( (int)g_gmMaxMs ) + " ms), phases [ms]:";
      for( int i = 1; i < GEN_PH_N; i++ )
        s = s + " " + zSTRING( kPh[i] ) + " " + zSTRING( (int)g_gmPhMs[i] );
      MM_LOG( s );
    }
    for( int u = 0; u < g_surfUnkCount; u++ )
      if( SurfUnkLogOnce( g_surfUnk[u] ) )
        MM_LOG( zSTRING( "material without class: " ) + zSTRING( g_surfUnk[u] ) );
    {
      static const char* kName[15] = {
        "terrain", "water", "snow", "buildings", "path", "meadow", "forest",
        "rock", "plant", "roof", "dressed stone", "field", "swamp",
        "beach", "interior"
      };
      for( int c = 0; c < SURF_COUNT; c++ ) {
        if( !g_surfHits[c] ) continue;
        const __int64 hits64 = (__int64)g_surfHits[c];
        const __int64 area64 = (__int64)n * (__int64)n;
        const int     pct    = (int)( hits64 * 100 / area64 );
        MM_LOG( zSTRING( "surface " ) + zSTRING( kName[c] ) + ": " + zSTRING( pct ) + "%, e.g. " + zSTRING( g_surfFirst[c][0] ? g_surfFirst[c] : "-" ) );
      }
    }
    GenStop();
  }
  inline void GenStep( oCWorld* world ) {
    {
      const DWORD now = GetTickCount();
      DWORD dt = now - g_gmLastTick;
      if( dt > 2000 ) dt = 2000;
      if( g_genPhase > GEN_PH_NONE && g_genPhase < GEN_PH_N )
        g_gmPhMs[g_genPhase] += dt;
      g_gmLastTick = now;
    }
    const double start = DpMs();
    const double until = start + GEN_FRAME_MS;
    const int    n     = g_gmN;
    const int    nn    = n * n;
    g_gmFrames++;
    while( g_genRunning && DpMs() < until ) {
      const double t0 = DpMs();
      const int    a  = g_gmCur;
      if( t0 - start > GEN_FRAME_MS * 0.2
          && ( g_genPhase == GEN_PH_MATS || g_genPhase == GEN_PH_MASK
               || g_genPhase == GEN_PH_PLANTS || g_genPhase == GEN_PH_UPLOAD
               || g_genPhase == GEN_PH_SAVE ) )
        break;
      switch( g_genPhase ) {
        case GEN_PH_MATS: {
          const char* why = GenMatsCore( world );
          if( why ) { GenFail( why ); break; }
          if( g_gmTint && !g_gmHTnt )
            memset( g_gmTint, 0, nn );
          GenPhase( GEN_PH_RASTER );
          break;
        }
        case GEN_PH_RASTER: {
          int b = a + g_gmPar * g_gmThreads;
          if( b > n ) b = n;
          GenParRange( DpRasterJob, &g_gmFast, a, b, GenSubChunk( g_gmPar ) );
          if( g_gmFast.failed ) { GenFail( "exception in worker" ); break; }
          g_gmCur = b;
          GenAdapt( g_gmPar, DpMs() - t0, 1, n );
          if( b >= n ) {
            MiniMapCommon::MmFree( g_gmHKey );  g_gmHKey = 0;
            MiniMapCommon::MmFree( g_gmHVal );  g_gmHVal = 0;
            MiniMapCommon::MmFree( g_gmHTnt );  g_gmHTnt = 0;
            GenPhase( GEN_PH_TREES );
          }
          break;
        }
        case GEN_PH_TREES: {
          if( !g_gmEntered ) {
            g_gmEntered = true;
            g_gmTreeTris = g_gmTreeStamps = 0;
            g_gmTreeMs   = 0.0;
            if( g_genTreeShapes == 0 ) { GenPhase( GEN_PH_HITS ); break; }
            TreeVobsCollect( world );
            if( g_tvCount <= 0 ) {
              TreeLog( "world map", 0, 0, DpMs() - t0 );
              GenPhase( GEN_PH_HITS );
              break;
            }
            const TMapInfo& m = g_gmRect;
            g_gmTreeStamps = TreeStampCore( g_gmFast,
                                ( m.x1 < m.x2 ) ? m.x1 : m.x2, ( m.y1 < m.y2 ) ? m.y1 : m.y2,
                                ( m.x1 < m.x2 ) ? m.x2 : m.x1, ( m.y1 < m.y2 ) ? m.y2 : m.y1 );
            if( g_genTreeShapes >= 2
                && !TreeBatchAlloc( g_gmTB, ProcLowMem() ? 16384 : 65536 ) )
              MM_LOG( "vob tree: out of memory for triangle batch - shapes skipped" );
            g_gmTreeMs += DpMs() - t0;
            break;
          }
          bool fin = !g_gmTB.v;
          if( !fin ) {
            const TMapInfo& m = g_gmRect;
            if( !TreeBatchFill( g_gmTB, ( m.x1 < m.x2 ) ? m.x1 : m.x2, ( m.y1 < m.y2 ) ? m.y1 : m.y2,
                                ( m.x1 < m.x2 ) ? m.x2 : m.x1, ( m.y1 < m.y2 ) ? m.y2 : m.y1,
                                g_gmTB.cap ) ) {
              MM_LOG( "vob tree: exception while reading meshes - phase ended" );
              fin = true;
            }
            else if( g_gmTB.cnt > 0 ) {
              TTreeJob job;
              job.C = &g_gmFast;
              job.B = &g_gmTB;
              g_gmFast.failed = 0;
              GenParRange( TreeRasterJob, &job, 0, n, GenSubChunk( n / g_gmThreads ) );
              if( g_gmFast.failed ) { GenFail( "exception in worker (trees)" ); break; }
              g_gmTreeTris += g_gmTB.cnt;
            }
            if( g_gmTB.vi >= g_tvCount ) fin = true;
          }
          g_gmTreeMs += DpMs() - t0;
          if( fin ) {
            TreeLog( "world map", g_gmTreeStamps, g_gmTreeTris, g_gmTreeMs );
            TreeBatchFree( g_gmTB );
            GenPhase( GEN_PH_HITS );
          }
          break;
        }
        case GEN_PH_HITS: {
          int b = a + g_gmSeq;
          if( b > n ) b = n;
          float* const hgt = g_gmHgt;
          unsigned char* const lit = g_gmLit;
          unsigned char* const mat = g_gmMat;
          for( int i = a * n; i < b * n; i++ ) {
            if( hgt[i] <= GEN_FAST_EMPTY ) {
              hgt[i] = 0.0f;
              lit[i] = 0;
              mat[i] = 0;
              continue;
            }
            const float y = hgt[i];
            if( g_gmHits == 0 || y < g_gmHMin ) g_gmHMin = y;
            if( g_gmHits == 0 || y > g_gmHMax ) g_gmHMax = y;
            g_gmHits++;
            if( mat[i] == SURF_WATER )
              g_gmWater++;
          }
          g_gmCur = b;
          GenAdapt( g_gmSeq, DpMs() - t0, 1, n );
          if( b >= n ) {
            if( g_gmHits < nn / 20 ) { GenFail( "almost no hits" ); break; }
            GenPhase( g_genVobs ? GEN_PH_MASK : GEN_PH_STATS );
          }
          break;
        }
        case GEN_PH_MASK:
          if( !GenMaskCore( world ) ) { GenFail( "exception" ); break; }
          GenPhase( GEN_PH_RAYS );
          break;
        case GEN_PH_RAYS:
          if( !GenRaysCore( world, until ) ) { GenFail( "exception" ); break; }
          if( g_gmCur >= nn ) {
            MiniMapCommon::MmFree( g_gmMask );  g_gmMask = 0;
            GenPhase( GEN_PH_STATS );
          }
          break;
        case GEN_PH_STATS: {
          int b = a + g_gmSeq;
          if( b > n ) b = n;
          for( int i = a * n; i < b * n; i++ )
            if( g_gmLit[i] != 0 && g_gmMat[i] < SURF_COUNT )
              g_surfHits[g_gmMat[i]]++;
          g_gmCur = b;
          GenAdapt( g_gmSeq, DpMs() - t0, 1, n );
          if( b >= n ) {
            MM_LOG( zSTRING( "terrain map: fast pass, polygons " ) + zSTRING( g_gmPolys ) + ", materials " + zSTRING( g_gmMats ) + ", vob rays " + zSTRING( g_gmRays ) + ", time " + zSTRING( (int)( GetTickCount() - g_gmT0 ) ) + " ms" );
            GenPhase( GEN_PH_PLANTS );
          }
          break;
        }
        case GEN_PH_PLANTS:
          if( g_genTreeShapes == 0 )
            GenPlantsStep( world );
          GenPhase( GEN_PH_OUTSIDE );
          break;
        case GEN_PH_OUTSIDE: {
          if( !g_gmEntered ) {
            g_gmEntered = true;
            if( !g_genFill && !g_genOutside ) { GenPhase( GEN_PH_PATHS ); break; }
            const int obytes = ( nn + 7 ) / 8;
            g_gmOuts = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( obytes ) );
            if( !g_gmOuts ) { GenPhase( GEN_PH_PATHS ); break; }
            unsigned char* const outs = g_gmOuts;
            const unsigned char* const lit = g_gmLit;
            memset( outs, 0, (size_t)( obytes ) * sizeof( outs[0] ) );
            for( int i = 0; i < n; i++ ) {
              if( !lit[i] )                 outs[i >> 3] |= (unsigned char)( 1 << ( i & 7 ) );
              const int j = ( n - 1 ) * n + i;
              if( !lit[j] )                 outs[j >> 3] |= (unsigned char)( 1 << ( j & 7 ) );
              const int k = i * n;
              if( !lit[k] )                 outs[k >> 3] |= (unsigned char)( 1 << ( k & 7 ) );
              const int l = i * n + n - 1;
              if( !lit[l] )                 outs[l >> 3] |= (unsigned char)( 1 << ( l & 7 ) );
            }
            break;
          }
          const int K = n - 2;
          int b = a + g_gmSeq;
          if( b > K ) b = K;
          GenOutRows( a, b, g_gmSub != 0 );
          g_gmCur = b;
          GenAdapt( g_gmSeq, DpMs() - t0, 1, n );
          if( b >= K ) {
            g_gmCur = 0;
            if( g_gmSub == 0 )
              g_gmSub = 1;
            else {
              g_gmSub = 0;
              if( !g_gmCount || ++g_gmPass >= 24 ) {
                GenPhase( GEN_PH_FILL );
                break;
              }
              g_gmCount = 0;
            }
          }
          break;
        }
        case GEN_PH_FILL: {
          if( !g_gmEntered ) {
            g_gmEntered = true;
            if( !g_genFill ) { GenPhase( GEN_PH_SEA ); break; }
          }
          int b = a + g_gmSeq;
          if( b > n ) b = n;
          GenFillRows( a, b, g_gmPass );
          g_gmCur = b;
          GenAdapt( g_gmSeq, DpMs() - t0, 1, n );
          if( b >= n ) {
            g_gmCur = 0;
            if( !g_gmCount || ++g_gmPass >= 64 ) {
              GenPhase( GEN_PH_SEA );
              break;
            }
            g_gmCount = 0;
          }
          break;
        }
        case GEN_PH_SEA: {
          if( !g_gmEntered ) {
            g_gmEntered = true;
            if( !g_genOutside ) {
              MiniMapCommon::MmFree( g_gmOuts );  g_gmOuts = 0;
              GenPhase( GEN_PH_PATHS );
              break;
            }
          }
          int b = a + g_gmSeq;
          if( b > n ) b = n;
          GenSeaRows( a, b, g_gmSub != 0 );
          g_gmCur = b;
          GenAdapt( g_gmSeq, DpMs() - t0, 1, n );
          if( b >= n ) {
            g_gmCur = 0;
            bool done = true;
            if( g_genOutside == 3 ) {
              done = false;
              if( g_gmSub == 0 )
                g_gmSub = 1;
              else {
                g_gmSub = 0;
                if( ++g_gmPass >= 6 ) done = true;
              }
            }
            if( done ) {
              MiniMapCommon::MmFree( g_gmOuts );  g_gmOuts = 0;
              GenPhase( GEN_PH_PATHS );
            }
          }
          break;
        }
        case GEN_PH_PATHS: {
          if( !g_gmEntered ) {
            g_gmEntered = true;
            if( !g_genPaths ) { GenPhase( GEN_PH_SMOOTH ); break; }
            EnsureEdges();
            const TMapInfo& m = g_gmRect;
            g_gmCw = ( m.x2 - m.x1 ) / (float)n;
            g_gmCh = ( m.y2 - m.y1 ) / (float)n;
            const float cell = ( g_gmCw < 0.0f ? -g_gmCw : g_gmCw );
            int wide = (int)( (float)g_genPathWide / ( cell > 0.1f ? cell : 1.0f ) * 0.5f );
            if( wide < 1 ) wide = 1; else if( wide > 24 ) wide = 24;
            g_gmWide = wide;
            break;
          }
          while( g_gmCur < g_edgeCount && g_edges ) {
            GenPathEdge( g_gmCur++ );
            if( DpMs() >= until ) break;
          }
          if( g_gmCur >= g_edgeCount || !g_edges )
            GenPhase( GEN_PH_SMOOTH );
          break;
        }
        case GEN_PH_SMOOTH: {
          if( !g_gmEntered ) {
            g_gmEntered = true;
            if( g_genSmooth <= 0 ) { GenPhase( GEN_PH_LAND ); break; }
            g_gmPrev = MiniMapCommon::MmAlloc<float>( (size_t)( n ) );
            if( !g_gmPrev ) { GenPhase( GEN_PH_LAND ); break; }
            g_gmSm.hgt  = g_gmHgt;
            g_gmSm.lit  = g_gmLit;
            g_gmSm.prev = g_gmPrev;
            g_gmSm.n    = n;
            break;
          }
          const bool cols = ( g_gmPass & 1 ) != 0;
          int chunk = g_gmPar;
          if( cols ) {
            chunk = ( chunk + 15 ) & ~15;
            if( chunk < 16 ) chunk = 16;
          }
          int b = a + chunk * g_gmThreads;
          if( b > n ) b = n;
          GenParRange( cols ? GenSmoothColsJob : GenSmoothRowsJob, &g_gmSm, a, b, chunk );
          g_gmCur = b;
          GenAdapt( g_gmPar, DpMs() - t0, 1, n );
          if( b >= n ) {
            g_gmCur = 0;
            int par = n / ( g_gmThreads * 32 );
            g_gmPar = ( par < 4 ) ? 4 : par;
            if( ++g_gmPass >= g_genSmooth * 2 ) {
              MiniMapCommon::MmFree( g_gmPrev );  g_gmPrev = 0;
              GenPhase( GEN_PH_LAND );
            }
          }
          break;
        }
        case GEN_PH_LAND: {
          if( !g_gmEntered ) {
            g_gmEntered = true;
            if( g_gmHits < nn / 20 ) {
              MM_LOG( zSTRING( "terrain map: too few hits (" ) + zSTRING( g_gmHits ) + " of " + zSTRING( nn ) + ") - giving up" );
              GenStop();
              break;
            }
            float range = g_gmHMax - g_gmHMin;
            if( range < 1.0f ) range = 1.0f;
            g_genHMin   = g_gmHMin;
            g_genHMax   = g_gmHMin + range;
            g_genHKnown = true;
            g_gmWRow = MiniMapCommon::MmAlloc<int>( (size_t)( n ) );
            g_gmWCol = MiniMapCommon::MmAlloc<int>( (size_t)( n ) );
            if( !g_gmWRow || !g_gmWCol ) {
              MiniMapCommon::MmFree( g_gmWRow );  g_gmWRow = 0;
              MiniMapCommon::MmFree( g_gmWCol );  g_gmWCol = 0;
              GenPhase( GEN_PH_SHADE );
              break;
            }
            memset( g_gmWRow, 0, (size_t)( n ) * sizeof( g_gmWRow[0] ) );
    memset( g_gmWCol, 0, (size_t)( n ) * sizeof( g_gmWCol[0] ) );
            break;
          }
          int b = a + g_gmSeq;
          if( b > n ) b = n;
          for( int iz = a; iz < b; iz++ )
            for( int ix = 0; ix < n; ix++ ) {
              const int q = iz * n + ix;
              if( !g_gmLit[q] || g_gmMat[q] == SURF_WATER ) continue;
              g_gmWRow[iz]++;
              g_gmWCol[ix]++;
            }
          g_gmCur = b;
          GenAdapt( g_gmSeq, DpMs() - t0, 1, n );
          if( b < n ) break;
          {
            const int* const wRow = g_gmWRow;
            const int* const wCol = g_gmWCol;
            const TMapInfo& m = g_gmRect;
            int prog = n / 50;
            if( prog < 1 ) prog = 1;
            int r1 = -1, r2 = -1, c1 = -1, c2 = -1;
            for( int i = 0; i < n; i++ ) {
              if( wRow[i] >= prog ) { if( r1 < 0 ) r1 = i; r2 = i; }
              if( wCol[i] >= prog ) { if( c1 < 0 ) c1 = i; c2 = i; }
            }
            if( r1 >= 0 && c1 >= 0 && r2 > r1 && c2 > c1 ) {
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
              MM_LOG( zSTRING( "land area from map: " ) + zSTRING( (int)lx1 ) + " " + zSTRING( (int)lz1 ) + " .. " + zSTRING( (int)lx2 ) + " " + zSTRING( (int)lz2 ) );
              SaveMaps();
            }
          }
          MiniMapCommon::MmFree( g_gmWRow );  g_gmWRow = 0;
          MiniMapCommon::MmFree( g_gmWCol );  g_gmWCol = 0;
          GenPhase( GEN_PH_SHADE );
          break;
        }
        case GEN_PH_SHADE: {
          if( !g_gmEntered ) {
            g_gmEntered = true;
            const TMapInfo& m = g_gmRect;
            const int bpp = g_gmBpp;
            g_gmPx = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( nn * bpp ) );
            if( !g_gmPx && g_gmTint ) {
              MiniMapCommon::MmFree( g_gmTint );
              g_gmTint = 0;
              g_gmPx = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( nn * bpp ) );
              MM_LOG( "terrain map: low memory - no texture colours" );
            }
            if( !g_gmPx ) { GenFail( "out of memory for pixels" ); break; }
            float range = g_gmHMax - g_gmHMin;
            if( range < 1.0f ) range = 1.0f;
            float cellX = ( m.x2 - m.x1 ) / (float)n;
            float cellZ = ( m.y2 - m.y1 ) / (float)n;
            if( cellX > -0.01f && cellX < 0.01f ) cellX = 1.0f;
            if( cellZ > -0.01f && cellZ < 0.01f ) cellZ = 1.0f;
            const float lx = -0.45f, ly = 0.78f, lz = 0.44f;
            TGenShade& S  = g_gmShade;
            S.n           = n;
            S.bpp         = bpp;
            S.hgt         = g_gmHgt;
            S.lit         = g_gmLit;
            S.mat         = g_gmMat;
            S.px          = g_gmPx;
            S.hmin        = g_gmHMin;
            S.range       = range;
            S.relief      = (float)g_genRelief / 100.0f;
            S.cellX       = cellX;
            S.cellZ       = cellZ;
            S.lx          = lx;
            S.ly          = ly;
            S.lz          = lz;
            S.fmt         = (int)g_genFmt;
            S.edge        = g_genEdge;
            S.contour     = g_genContour;
            S.contourDark = g_genContourDark;
            S.wall        = 0;
            S.emptyC      = 0;
            S.lowY        = -1.0e30f;
            S.sculpt      = g_genSculpt;
            S.tex         = g_genTexColors;
            S.tint        = g_gmTint;
            break;
          }
          int b = a + g_gmPar * g_gmThreads;
          if( b > n ) b = n;
          GenParRange( GenShadeJob, &g_gmShade, a, b, GenSubChunk( g_gmPar ) );
          g_gmCur = b;
          GenAdapt( g_gmPar, DpMs() - t0, 1, n );
          if( b >= n ) {
            MiniMapCommon::MmFree( g_gmHgt );   g_gmHgt  = 0;
            MiniMapCommon::MmFree( g_gmLit );   g_gmLit  = 0;
            MiniMapCommon::MmFree( g_gmMat );   g_gmMat  = 0;
            MiniMapCommon::MmFree( g_gmTint );  g_gmTint = 0;
            GenPhase( GEN_PH_UPLOAD );
          }
          break;
        }
        case GEN_PH_UPLOAD: {
          if( !g_genTex )
            GenEnsureTexture( n );
          const bool ok = g_genTex && g_genTexSize == n
                       && GenUpload( g_genTex, g_gmPx, n, g_gmBpp );
          if( !ok ) {
            MM_LOG( zSTRING( "terrain map: " ) + zSTRING( n ) + "x" + zSTRING( n ) + " - texture upload FAILED" );
            GenStop();
            break;
          }
          StrCopy( g_genWorld, g_world, NAME_MAX );
          g_genRedo = false;
          ApplyTexture();
          GenPhase( GEN_PH_SAVE );
          g_gmCur = 0;
          {
            const double ms = DpMs() - start;
            g_gmWorkMs += ms;
            if( ms > g_gmMaxMs ) g_gmMaxMs = ms;
          }
          GenProgressUpdate();
          return;
        }
        case GEN_PH_SAVE: {
          GenCacheSave( g_world, g_gmRect, n, g_gmBpp, g_gmPx );
          g_gmPhMs[GEN_PH_SAVE] += GetTickCount() - g_gmLastTick;
          {
            const double ms = DpMs() - start;
            g_gmWorkMs += ms;
            if( ms > g_gmMaxMs ) g_gmMaxMs = ms;
          }
          GenFinish();
          return;
        }
        default:
          GenStop();
          break;
      }
    }
    if( g_genRunning ) {
      const double ms = DpMs() - start;
      g_gmWorkMs += ms;
      if( ms > g_gmMaxMs ) g_gmMaxMs = ms;
      GenProgressUpdate();
    }
  }
  inline bool GenerateMap( oCWorld* world, const TMapInfo& m, DWORD delayMs = 0 ) {
    if( g_genRunning )
      GenAbort( "new request" );
    if( !world || !zrenderer || !world->bspTree.bspRoot ) return false;
    int n = g_genSize;
    while( g_genOomSize > 0 && n >= g_genOomSize && n > 256 )
      n /= 2;
    while( n > 256 && !CanAllocGrid( n, 2 ) && GenCacheProbe( g_world, m, 2 ) < n && GenCacheProbe( g_world, m, 4 ) < n ) {
      MM_LOG( zSTRING( "terrain map: out of memory for grid " ) + zSTRING( n ) + "x" + zSTRING( n ) + " - trying " + zSTRING( n / 2 ) );
      if( g_genOomSize == 0 || n < g_genOomSize ) g_genOomSize = n;
      n /= 2;
    }
    zCTexture* tex = GenEnsureTexture( n );
    while( !tex && !g_genTex && n > 64 ) {
      n /= 2;
      MM_LOG( zSTRING( "terrain map: grid too large, trying " ) + zSTRING( n ) + "x" + zSTRING( n ) );
      tex = GenEnsureTexture( n );
    }
    if( !tex ) return false;
    if( g_genTexSize > 0 && g_genTexSize != n ) {
      MM_LOG( zSTRING( "terrain map: grid matched to texture, " ) + zSTRING( n ) + " -> " + zSTRING( g_genTexSize ) );
      n = g_genTexSize;
    }
    g_genSizeUsed = n;
    {
      const int bppNow = g_genBpp ? g_genBpp : 4;
      for( ;; ) {
        if( GenCacheLoad( g_world, m, n, bppNow, tex ) ) {
          StrCopy( g_genWorld, g_world, NAME_MAX );
          g_genRedo = false;
          return true;
        }
        if( !g_texReady || ProcLowMem() ) { DeferGenTexture(); tex = 0; }
        if( n <= 256 || CanAllocGrid( n, bppNow ) ) break;
        if( g_genTex ) {
          DeferGenTexture(); tex = 0;
          if( CanAllocGrid( n, bppNow ) ) break;
        }
        MM_LOG( zSTRING( "terrain map: out of memory for grid " ) + zSTRING( n ) + "x" + zSTRING( n ) + " - trying " + zSTRING( n / 2 ) );
        if( g_genOomSize == 0 || n < g_genOomSize ) g_genOomSize = n;
        n /= 2;
        tex = GenEnsureTexture( n );
        if( !tex ) return false;
        if( g_genTexSize > 0 && g_genTexSize != n ) n = g_genTexSize;
        g_genSizeUsed = n;
      }
    }
    if( ProcLowMem() ) {
      const int bppNow = g_genBpp ? g_genBpp : 4;
      const int k = GenCacheProbe( g_world, m, bppNow );
      if( k > 0 && k < n ) {
        zCTexture* const t2 = GenEnsureTexture( k );
        if( t2 && g_genTexSize == k ) {
          g_genSizeUsed = k;
          if( GenCacheLoad( g_world, m, k, bppNow, t2 ) ) {
            StrCopy( g_genWorld, g_world, NAME_MAX );
            g_genRedo = false;
            MM_LOG( zSTRING( "terrain map: ready map " ) + zSTRING( k ) + " from disk (low memory, no recalculation)" );
            return true;
          }
          tex = GenEnsureTexture( n );
          if( !tex ) return false;
          if( g_genTexSize > 0 && g_genTexSize != n ) n = g_genTexSize;
          g_genSizeUsed = n;
          DeferGenTexture();
        }
      }
    }
    g_gmWorld   = world;
    StrCopy( g_gmWorldName, g_world, NAME_MAX );
    g_gmRect    = m;
    g_gmSig     = GenSignature( m );
    g_gmN       = n;
    g_gmStartAt = GetTickCount() + delayMs;
    g_genRedo   = false;
    g_genRunning = true;
    g_barKeysLogged = false;
    g_genPhase   = GEN_PH_NONE;
    g_genFrac    = 0.0f;
    GenProgressUpdate();
    MM_LOG( zSTRING( "terrain map: calculating " ) + zSTRING( n ) + "x" + zSTRING( n ) + " in " + zSTRING( (int)delayMs ) + " ms (in steps)" );
    return false;
  }
  inline void GenUpdate() {
    if( !g_genRunning ) return;
    oCWorld* const world = ogame ? ogame->GetGameWorld() : 0;
    if( !world || world != g_gmWorld || g_mapIdx < 0 || !StrEqUp( g_gmWorldName, g_world ) ) {
      GenAbort( "world change" );
      return;
    }
    if( GenSignature( g_gmRect ) != g_gmSig ) {
      GenAbort( "options changed" );
      GenerateMap( world, g_maps[g_mapIdx], 0 );
      ApplyTexture();
      return;
    }
    if( g_genHeld ) return;
    if( g_genPhase == GEN_PH_NONE ) {
      if( (int)( GetTickCount() - g_gmStartAt ) < 0 ) {
        GenProgressUpdate();
        return;
      }
      if( !GenStart( world ) ) return;
    }
    GenStep( world );
  }
  inline void FillDisc( int x, int y, int w, int h, const zCOLOR& c ) {
    if( !screen || w <= 0 || h <= 0 ) return;
    const float r  = DiscR();
    const float cx = (float)x + (float)w * 0.5f;
    const float cy = (float)y + (float)h * 0.5f;
    for( int row = 0; row <= h; row++ ) {
      const float py = (float)( y + row );
      const float dy = py - cy;
      const float t  = r * r - dy * dy;
      if( t <= 0.0f ) continue;
      const float half = sqrtf( t );
      screen->Line( VX( cx - half ), VY( py ), VX( cx + half ), VY( py ), c );
    }
  }
  inline void DrawWaynet( bool overMap ) {
    EnsureEdges();
    if( !g_edges || g_edgeCount <= 0 ) return;
    const zCOLOR c    = overMap ? zCOLOR( 246, 226, 168, 230 ) : ColWay();
    const zCOLOR dark = zCOLOR( 28, 22, 16, 170 );
    for( int i = 0; i < g_edgeCount; i++ ) {
      const TEdge& e = g_edges[i];
      float ax, ay, bx, by;
      WorldToPx( e.ax, e.az, ax, ay );
      WorldToPx( e.bx, e.bz, bx, by );
      if( ax < g_clipX0 && bx < g_clipX0 ) continue;
      if( ax > g_clipX1 && bx > g_clipX1 ) continue;
      if( ay < g_clipY0 && by < g_clipY0 ) continue;
      if( ay > g_clipY1 && by > g_clipY1 ) continue;
      if( !overMap ) {
        PLine( ax, ay, bx, by, c );
        continue;
      }
      const float ddx = bx - ax, ddy = by - ay;
      const bool  flat = ( ddx * ddx >= ddy * ddy );
      const float ox = flat ? 0.0f : 1.0f;
      const float oy = flat ? 1.0f : 0.0f;
      PLine( ax - ox,        ay - oy,        bx - ox,        by - oy,        dark );
      PLine( ax + 2.0f * ox, ay + 2.0f * oy, bx + 2.0f * ox, by + 2.0f * oy, dark );
      PLine( ax,             ay,             bx,             by,             c );
      PLine( ax + ox,        ay + oy,        bx + ox,        by + oy,        c );
    }
  }
}
