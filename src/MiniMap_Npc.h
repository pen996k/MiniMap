#pragma once
namespace MiniMap {
  inline void TexRect( float x, float y, float w, float h, const zCOLOR& c );
  inline void MarkBar( float x0, float y, float x1, const zCOLOR& c ) {
    if( x1 < x0 ) {
      std::swap( x0, x1 );
    }
    if( x1 - x0 < 0.5f ) x1 = x0 + 1.0f;
    if( x1 < g_clipX0 || x0 > g_clipX1 || y < g_clipY0 || y > g_clipY1 ) return;
    if( x0 < g_clipX0 ) x0 = g_clipX0;
    if( x1 > g_clipX1 ) x1 = g_clipX1;
    TexRect( x0, y, x1 - x0, 1.0f, c );
  }
  inline void MarkDisc( float px, float py, float r, const zCOLOR& c ) {
    for( float k = -r; k <= r; k += 1.0f ) {
      const float t = r * r - k * k;
      if( t <= 0.0f ) continue;
      const float half = sqrtf( t );
      MarkBar( px - half, py + k, px + half, c );
    }
  }
  inline void MarkHuman( float px, float py, float r, const zCOLOR& c ) {
    MarkDisc( px, py - r * 0.55f, r * 0.55f, c );
    for( float k = 0.0f; k <= r * 1.05f; k += 1.0f ) {
      const float w = r * 0.85f * ( 1.0f - k / ( r * 2.2f ) );
      MarkBar( px - w, py + k * 0.6f, px + w, c );
    }
  }
  inline void MarkBeast( float px, float py, float r, const zCOLOR& c ) {
    for( float k = -r * 0.7f; k <= r * 0.7f; k += 1.0f ) {
      const float t = 1.0f - ( k / ( r * 0.75f ) ) * ( k / ( r * 0.75f ) );
      if( t <= 0.0f ) continue;
      const float half = r * 1.35f * sqrtf( t );
      MarkBar( px - half, py + k, px + half, c );
    }
  }
  inline void MarkMonster( float px, float py, float r, const zCOLOR& c ) {
    for( float k = -r; k <= r; k += 1.0f ) {
      const float d    = ( k < 0.0f ) ? -k : k;
      const float half = r - d;
      if( half <= 0.0f ) continue;
      MarkBar( px - half, py + k, px + half, c );
    }
    MarkBar( px - r * 1.4f, py, px + r * 1.4f, c );
    for( float t2 = -r * 1.4f; t2 <= r * 1.4f; t2 += 1.0f )
      MarkBar( px - 0.5f, py + t2, px + 0.5f, c );
  }
  inline void DrawMark( int kind, float px, float py, float r, const zCOLOR& c ) {
    if( px < g_clipX0 - r * 2.0f || px > g_clipX1 + r * 2.0f ) return;
    if( py < g_clipY0 - r * 2.0f || py > g_clipY1 + r * 2.0f ) return;
    const zCOLOR dark( 18, 16, 15, 255 );
    for( int pass = 0; pass < 2; pass++ ) {
      const float  rr = ( pass == 0 ) ? r + 1.0f : r;
      const zCOLOR cc = ( pass == 0 ) ? dark : c;
      switch( kind ) {
        case 1:  MarkBeast( px, py, rr, cc );    break;
        case 2:  MarkMonster( px, py, rr, cc );  break;
        default: MarkHuman( px, py, rr, cc );    break;
      }
    }
  }
  inline bool MarkerVisible( float px, float py ) {
    return InDisc( px, py );
  }
  static const DWORD NPC_CACHE_MS = 750;
  static oCNpc**  g_npcCache     = 0;
  static int      g_npcCount     = 0;
  static int      g_npcCap       = 0;
  static DWORD    g_npcNext      = 0;
  static oCWorld* g_npcWorld     = 0;
  static int      g_npcLastShown = 0;
  static bool*    g_npcMatch     = 0;
  static int      g_npcMatchCap  = 0;
  static int      g_npcMatchN    = 0;
  static bool*    g_corpseMatch    = 0;
  static int      g_corpseMatchCap = 0;
  static int      g_corpseMatchN   = 0;
  static int      g_corpseFound    = 0;
  inline void SearchMatchCorpses();
  inline void ChestMark( int kind, float px, float py, float s );
  inline void CorpseMark( float px, float py, float s );
  inline void SearchMatchNpcs();
  inline void SearchSuggestBuild();
  inline bool SearchFilterOn();
  inline void SearchRing( float px, float py, float s );
  inline void SearchDimMark( float px, float py, float s );
  static unsigned char* g_npcFlag    = 0;
  static int            g_npcFlagCap = 0;
  static int            g_npcFlagN   = 0;
  struct NpcSpawnPos { oCNpc* npc; float x, z; };
  static NpcSpawnPos* g_npcSpawn  = 0;
  static int          g_npcSpawnN = 0;
  struct NpcWpPos { float x, z; unsigned char has; };
  static NpcWpPos* g_npcWp    = 0;
  static int       g_npcWpCap = 0;
  static int       g_npcWpN   = 0;
  struct NpcWpMemo { oCNpc* npc; unsigned int h; float x, z; unsigned char has; };
  enum { NPC_WP_MEMO = 4096 };
  static NpcWpMemo* g_npcWpMemo  = 0;
  static int        g_npcWpMemoN = 0;
  static int        g_npcWpBudget = 0;
  inline void NpcCacheFree( bool release = true ) {
    if( g_npcCache && release ) {
      for( int i = 0; i < g_npcCount; i++ )
        if( g_npcCache[i] )
          g_npcCache[i]->Release();
    }
    MiniMapCommon::MmFree( g_npcCache );
    g_npcCache     = 0;
    g_npcCount     = 0;
    g_npcCap       = 0;
    g_npcNext      = 0;
    g_npcWorld     = 0;
    g_npcMatchN    = 0;
    g_corpseMatchN = 0;
    g_corpseFound  = 0;
    g_npcFlagN     = 0;
    MiniMapCommon::MmFree( g_npcFlag );
    g_npcFlag      = 0;
    g_npcFlagCap   = 0;
    MiniMapCommon::MmFree( g_npcSpawn );
    g_npcSpawn     = 0;
    g_npcSpawnN    = 0;
    MiniMapCommon::MmFree( g_npcWp );
    g_npcWp        = 0;
    g_npcWpCap     = 0;
    g_npcWpN       = 0;
    MiniMapCommon::MmFree( g_npcWpMemo );
    g_npcWpMemo    = 0;
    g_npcWpMemoN   = 0;
    g_npcLastShown = 0;
  }
  inline int NpcPtrCmp( const void* a, const void* b ) {
    const unsigned long x = (unsigned long)*(oCNpc* const*)a;
    const unsigned long y = (unsigned long)*(oCNpc* const*)b;
    return ( x < y ) ? -1 : ( x > y ) ? 1 : 0;
  }
  inline int NpcSpawnCmp( const void* a, const void* b ) {
    const unsigned long x = (unsigned long)( (const NpcSpawnPos*)a )->npc;
    const unsigned long y = (unsigned long)( (const NpcSpawnPos*)b )->npc;
    return ( x < y ) ? -1 : ( x > y ) ? 1 : 0;
  }
  inline void NpcSpawnBuild() {
    MiniMapCommon::MmFree( g_npcSpawn );
    g_npcSpawn  = 0;
    g_npcSpawnN = 0;
    oCSpawnManager* const sm = ogame ? ogame->spawnman : 0;
    if( !sm ) return;
    const int cnt = sm->spawnList.numInArray;
    if( cnt <= 0 || cnt > 100000 || !sm->spawnList.parray ) return;
    NpcSpawnPos* const arr = MiniMapCommon::MmAlloc<NpcSpawnPos>( (size_t)( cnt ) );
    if( !arr ) return;
    int n = 0;
    for( int i = 0; i < cnt; i++ ) {
      oCSpawnManager::oTSpawnNode* const nd = sm->spawnList.parray[i];
      if( !nd || !nd->npc ) continue;
      arr[n].npc = nd->npc;
      arr[n].x   = nd->spawnPos[0];
      arr[n].z   = nd->spawnPos[2];
      n++;
    }
    if( n > 1 )
      qsort( arr, n, sizeof( NpcSpawnPos ), NpcSpawnCmp );
    g_npcSpawn  = arr;
    g_npcSpawnN = n;
  }
  inline const NpcSpawnPos* NpcSpawnFind( oCNpc* npc ) {
    if( !g_npcSpawn || g_npcSpawnN <= 0 ) return 0;
    NpcSpawnPos key;
    key.npc = npc;
    return (const NpcSpawnPos*)bsearch( &key, g_npcSpawn, g_npcSpawnN,
                                        sizeof( NpcSpawnPos ), NpcSpawnCmp );
  }
  inline void NpcSpawnDiag() {
    int offN = 0, posN = 0;
    for( int i = 0; i < g_npcCount; i++ ) {
      oCNpc* const npc = g_npcCache[i];
      if( npc && !npc->homeWorld ) {
        offN++;
        if( NpcSpawnFind( npc ) )
          posN++;
      }
    }
    static int loggedOff = -1, loggedPos = -1, skipped = 0;
    static DWORD lastT = 0;
    if( offN != loggedOff || posN != loggedPos ) {
      const DWORD now  = GetTickCount();
      const int   dOff = ( offN > loggedOff ) ? offN - loggedOff : loggedOff - offN;
      if( loggedOff >= 0 && dOff < 50 && (DWORD)( now - lastT ) < 30000u ) {
        skipped++;
        return;
      }
      lastT     = now;
      loggedOff = offN;
      loggedPos = posN;
      MM_LOG( zSTRING( "characters: removed by spawn manager " ) + zSTRING( offN ) + ", with respawn point " + zSTRING( posN ) );
      if( skipped > 0 )
        MM_LOG( zSTRING( "characters: changes skipped since last entry " ) + zSTRING( skipped ) );
      skipped = 0;
    }
  }
  inline bool NpcListPush( oCNpc**& arr, int& n, int& cap, oCNpc* npc ) {
    if( n >= cap ) {
      const int ncap = ( cap < 64 ) ? 256 : cap * 2;
      oCNpc** na = MiniMapCommon::MmAlloc<oCNpc*>( (size_t)( ncap ) );
      if( !na ) return false;
      memcpy( na, arr, (size_t)( n ) * sizeof( na[0] ) );
      MiniMapCommon::MmFree( arr );
      arr = na;
      cap = ncap;
    }
    arr[n++] = npc;
    return true;
  }
  enum { NPCF_TRADER = 1, NPCF_GONE = 2, NPCF_LOOT = 4 };
  static const int NPC_GRP_TRADER = 3;
  inline bool CorpseLootable( oCItem* it );
  inline int  SearchCatOf( oCItem* it );
  inline int  ItemKindIcon( oCItem* it );
  inline bool NpcFlagAt( int i, int f ) {
    return g_npcFlag && g_npcFlagN == g_npcCount && i >= 0 && i < g_npcFlagN
        && ( g_npcFlag[i] & f ) != 0;
  }
  static int*  g_traderInst = 0;
  static int   g_traderN    = 0;
  static void* g_traderSrc  = 0;
  static int   g_traderGen  = -1;
  static int*  g_traderOff  = 0;
  static int   g_traderOffN = 0;
  inline void TraderFree() {
    MiniMapCommon::MmFree( g_traderInst );
    MiniMapCommon::MmFree( g_traderOff );
    g_traderInst = 0;
    g_traderN    = 0;
    g_traderOff  = 0;
    g_traderOffN = 0;
    g_traderSrc  = 0;
    g_traderGen  = -1;
  }
  inline int IntCmp( const void* a, const void* b ) {
    const int x = *(const int*)a, y = *(const int*)b;
    return ( x < y ) ? -1 : ( x > y ) ? 1 : 0;
  }
  inline int IntSortUnique( int* a, int n ) {
    if( n > 1 )
      qsort( a, n, sizeof( int ), IntCmp );
    int u = 0;
    for( int j = 0; j < n; j++ )
      if( u == 0 || a[u - 1] != a[j] )
        a[u++] = a[j];
    return u;
  }
  inline int TraderSymIndex( const char* name ) {
    if( !parser || !name || !*name ) return -1;
    const int idx = parser->GetIndex( zSTRING( name ) );
    if( idx < 0 ) return -1;
    zCPar_Symbol* const s = parser->GetSymbol( idx );
    return ( s && s->type == zPAR_TYPE_INSTANCE ) ? idx : -1;
  }
  inline void TraderEnsure() {
    oCInfoManager* const im = ogame ? ogame->infoman : 0;
    if( !im || ( (void*)im == g_traderSrc && g_traderGen == g_cfgGen ) ) return;
    TraderFree();
    g_traderSrc = (void*)im;
    g_traderGen = g_cfgGen;
    const int cfgSec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "traders" ) : -1;
    int cfgYes = 0, cfgNo = 0, cfgBad = 0;
    for( int i = MiniMapCfg::Next( g_cfg, cfgSec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, cfgSec, i ) ) {
      const int b = MiniMapCfg::Bool( g_cfg.e[i].val );
      if( b == 1 ) cfgYes++;
      else if( b == 0 ) cfgNo++;
    }
    int n = 0, infos = 0;
    for( zCListSort<oCInfo>* e = im->infoList.next; e && infos < 50000; e = e->next, infos++ )
      if( e->data && e->data->pd.trade )
        n++;
    if( n + cfgYes > 0 ) g_traderInst = MiniMapCommon::MmAlloc<int>( (size_t)( n + cfgYes ) );
    if( cfgNo > 0 ) g_traderOff = MiniMapCommon::MmAlloc<int>( (size_t)( cfgNo ) );
    int fromCfg = 0, offCfg = 0;
    if( g_traderInst ) {
      int k = 0, guard = 0;
      for( zCListSort<oCInfo>* e = im->infoList.next; e && k < n && guard < 50000; e = e->next, guard++ )
        if( e->data && e->data->pd.trade )
          g_traderInst[k++] = e->data->pd.npc;
      for( int i = MiniMapCfg::Next( g_cfg, cfgSec, -1 ); i >= 0 && k < n + cfgYes; i = MiniMapCfg::Next( g_cfg, cfgSec, i ) ) {
        const MiniMapCfg::TItem& t = g_cfg.e[i];
        const int b = MiniMapCfg::Bool( t.val );
        if( b != 1 ) continue;
        const int idx = TraderSymIndex( t.key );
        if( idx < 0 ) {
          CfgMsg( 0, t.src, t.line, "[traders]: no character instance with this name in this game - ignored" );
          cfgBad++;
          continue;
        }
        g_traderInst[k++] = idx;
        fromCfg++;
      }
      g_traderN = IntSortUnique( g_traderInst, k );
    }
    if( g_traderOff ) {
      int k = 0;
      for( int i = MiniMapCfg::Next( g_cfg, cfgSec, -1 ); i >= 0 && k < cfgNo; i = MiniMapCfg::Next( g_cfg, cfgSec, i ) ) {
        const MiniMapCfg::TItem& t = g_cfg.e[i];
        if( MiniMapCfg::Bool( t.val ) != 0 ) continue;
        const int idx = TraderSymIndex( t.key );
        if( idx < 0 ) {
          CfgMsg( 0, t.src, t.line, "[traders]: no character instance with this name in this game - ignored" );
          cfgBad++;
          continue;
        }
        g_traderOff[k++] = idx;
      }
      g_traderOffN = offCfg = IntSortUnique( g_traderOff, k );
    }
    for( int i = MiniMapCfg::Next( g_cfg, cfgSec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, cfgSec, i ) )
      if( MiniMapCfg::Bool( g_cfg.e[i].val ) < 0 ) {
        CfgMsg( 0, g_cfg.e[i].src, g_cfg.e[i].line, "[traders]: value must be yes or no - ignored" );
        cfgBad++;
      }
    MM_LOG( zSTRING( "traders: " ) + zSTRING( g_traderN ) + " characters with trade dialogue (dialogues " + zSTRING( infos ) + ")" + ( ( fromCfg || offCfg || cfgBad )
                ? zSTRING( ", from txt files: +" ) + zSTRING( fromCfg ) + " / -" + zSTRING( offCfg ) + ", bad lines " + zSTRING( cfgBad )
                : zSTRING( "" ) ) );
  }
  inline bool TraderIs( oCNpc* npc ) {
    if( !npc ) return false;
    TraderEnsure();
    if( g_traderN <= 0 || !g_traderInst ) return false;
    const int inst = npc->instanz;
    if( g_traderOffN > 0 && bsearch( &inst, g_traderOff, g_traderOffN, sizeof( int ), IntCmp ) ) return false;
    return bsearch( &inst, g_traderInst, g_traderN, sizeof( int ), IntCmp ) != 0;
  }
  static int*  g_hideInst   = 0;
  static int   g_hideN      = 0;
  static int   g_hideGen    = -1;
  static void* g_hideParser = 0;
  inline void HideFree() {
    MiniMapCommon::MmFree( g_hideInst );
    g_hideInst   = 0;
    g_hideN      = 0;
    g_hideGen    = -1;
    g_hideParser = 0;
  }
  inline bool HideSymName( int i, char* out, int cap ) {
    out[0] = 0;
    __try {
      zCPar_Symbol* const s = parser->GetSymbol( i );
      if( !s || s->type != zPAR_TYPE_INSTANCE || s->name.Length() <= 0 ) return false;
      StrCopy( out, s->name.ToChar(), cap );
      return out[0] != 0;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline int HideSymCount() {
    __try {
      return parser ? parser->symtab.GetNumInList() : 0;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return 0;
    }
  }
  inline void HideEnsure() {
    if( g_hideGen == g_cfgGen && g_hideParser == (void*)parser ) return;
    HideFree();
    g_hideGen    = g_cfgGen;
    g_hideParser = (void*)parser;
    const int sec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "hide" ) : -1;
    if( sec < 0 || !parser ) return;
    int nYes = 0, nPat = 0, bad = 0;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      const int b = MiniMapCfg::Bool( t.val );
      const int pre = MiniMapCfg::PatPrefix( t.key );
      if( b < 0 ) { CfgMsg( 0, t.src, t.line, "[hide]: value must be yes or no - ignored" ); bad++; continue; }
      if( pre < 0 ) { CfgMsg( 0, t.src, t.line, "[hide]: '*' only at the end, after at least 3 letters (e.g. TEST_*) - ignored" ); bad++; continue; }
      if( b == 1 ) { if( pre > 0 ) nPat++; else nYes++; }
    }
    if( nYes + nPat == 0 ) {
      if( bad )
        MM_LOG( zSTRING( "hide: no usable lines, bad lines " ) + zSTRING( bad ) );
      return;
    }
    const int nsym = nPat ? HideSymCount() : 0;
    enum { HIDE_MAX = 65536 };
    int cap = nYes + ( nPat ? nsym : 0 );
    if( cap > HIDE_MAX ) cap = HIDE_MAX;
    int* a = MiniMapCommon::MmAlloc<int>( (size_t)( cap > 0 ? cap : 1 ) );
    if( !a ) return;
    int k = 0, unknown = 0;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0 && k < cap; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      if( MiniMapCfg::Bool( t.val ) != 1 || MiniMapCfg::PatPrefix( t.key ) != 0 ) continue;
      const int idx = TraderSymIndex( t.key );
      if( idx < 0 ) {
        CfgMsg( 0, t.src, t.line, "[hide]: no instance with this name in this game - ignored" );
        unknown++;
        continue;
      }
      a[k++] = idx;
    }
    int byPat = 0;
    if( nPat ) {
      char nm[MiniMapCfg::KEY_LEN];
      for( int s = 0; s < nsym && k < cap; s++ ) {
        if( !HideSymName( s, nm, (int)sizeof( nm ) ) ) continue;
        for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
          const MiniMapCfg::TItem& t = g_cfg.e[i];
          if( MiniMapCfg::PatPrefix( t.key ) > 0 && MiniMapCfg::Bool( t.val ) == 1 && MiniMapCfg::PatMatch( t.key, nm ) ) {
            a[k++] = s;
            byPat++;
            break;
          }
        }
      }
    }
    k = IntSortUnique( a, k );
    int unhid = 0;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      if( MiniMapCfg::Bool( t.val ) != 0 || MiniMapCfg::PatPrefix( t.key ) != 0 ) continue;
      const int idx = TraderSymIndex( t.key );
      for( int j = 0; idx >= 0 && j < k; j++ )
        if( a[j] == idx ) {
          a[j] = a[--k];
          unhid++;
          break;
        }
    }
    k = IntSortUnique( a, k );
    g_hideInst = a;
    g_hideN    = k;
    MM_LOG( zSTRING( "hide: instances hidden " ) + zSTRING( k ) + " (by name " + zSTRING( nYes - unknown ) + ", by prefix " + zSTRING( byPat ) + ", shown again " + zSTRING( unhid ) + "), unknown names " + zSTRING( unknown ) + ", bad lines " + zSTRING( bad ) );
  }
  inline bool HiddenInst( int inst ) {
    HideEnsure();
    return g_hideN > 0 && bsearch( &inst, g_hideInst, g_hideN, sizeof( int ), IntCmp ) != 0;
  }
  enum { IK_NONE = -1, IK_MELEE_1H = 0, IK_MELEE_2H, IK_RANGED, IK_ARMOR, IK_POTION,
         IK_FOOD, IK_MAGIC, IK_DOCS, IK_GOLD, IK_OTHER, IK_COUNT };
  static const char* const kItemKindKey[IK_COUNT] = {
    "melee_1h", "melee_2h", "ranged", "armor", "potion", "food", "magic", "docs", "gold", "other"
  };
  struct TItemKindOv { int inst; int kind; };
  static TItemKindOv* g_ikOv     = 0;
  static int          g_ikN      = 0;
  static int          g_ikGen    = -1;
  static void*        g_ikParser = 0;
  inline void ItemKindsFree() {
    MiniMapCommon::MmFree( g_ikOv );
    g_ikOv     = 0;
    g_ikN      = 0;
    g_ikGen    = -1;
    g_ikParser = 0;
  }
  inline int IkCmp( const void* a, const void* b ) {
    const int x = ( (const TItemKindOv*)a )->inst, y = ( (const TItemKindOv*)b )->inst;
    return ( x < y ) ? -1 : ( x > y ) ? 1 : 0;
  }
  inline int ItemKindKeyIndex( const char* v ) {
    for( int k = 0; k < IK_COUNT; k++ )
      if( MiniMapCfg::EqNoCase( kItemKindKey[k], v ) ) return k;
    return IK_NONE;
  }
  inline void ItemKindsEnsure() {
    if( g_ikGen == g_cfgGen && g_ikParser == (void*)parser ) return;
    ItemKindsFree();
    g_ikGen    = g_cfgGen;
    g_ikParser = (void*)parser;
    const int sec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "item_kinds" ) : -1;
    if( sec < 0 || !parser ) return;
    int nName = 0, nPat = 0, bad = 0;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      const int pre = MiniMapCfg::PatPrefix( t.key );
      if( ItemKindKeyIndex( t.val ) == IK_NONE ) {
        CfgMsg( 0, t.src, t.line, "[item_kinds]: unknown kind (melee_1h melee_2h ranged armor potion food magic docs gold other) - ignored" );
        bad++;
        continue;
      }
      if( pre < 0 ) {
        CfgMsg( 0, t.src, t.line, "[item_kinds]: '*' only at the end, after at least 3 letters - ignored" );
        bad++;
        continue;
      }
      if( pre > 0 ) nPat++; else nName++;
    }
    if( nName + nPat == 0 ) {
      if( bad )
        MM_LOG( zSTRING( "item kinds: no usable lines, bad lines " ) + zSTRING( bad ) );
      return;
    }
    const int nsym = nPat ? HideSymCount() : 0;
    enum { IK_MAX = 65536 };
    int cap = nName + ( nPat ? nsym : 0 );
    if( cap > IK_MAX ) cap = IK_MAX;
    TItemKindOv* a = MiniMapCommon::MmAlloc<TItemKindOv>( (size_t)( cap > 0 ? cap : 1 ) );
    if( !a ) return;
    int k = 0, unknown = 0, byPat = 0;
    if( nPat ) {
      char nm[MiniMapCfg::KEY_LEN];
      for( int s = 0; s < nsym && k < cap; s++ ) {
        if( !HideSymName( s, nm, (int)sizeof( nm ) ) ) continue;
        int kind = IK_NONE;
        for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
          const MiniMapCfg::TItem& t = g_cfg.e[i];
          if( MiniMapCfg::PatPrefix( t.key ) > 0 && MiniMapCfg::PatMatch( t.key, nm ) ) {
            const int kval = ItemKindKeyIndex( t.val );
            if( kval != IK_NONE ) kind = kval;
          }
        }
        if( kind != IK_NONE ) {
          a[k].inst = s;
          a[k].kind = kind;
          k++;
          byPat++;
        }
      }
    }
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      const int kind = ItemKindKeyIndex( t.val );
      if( kind == IK_NONE || MiniMapCfg::PatPrefix( t.key ) != 0 ) continue;
      const int idx = TraderSymIndex( t.key );
      if( idx < 0 ) {
        CfgMsg( 0, t.src, t.line, "[item_kinds]: no item instance with this name in this game - ignored" );
        unknown++;
        continue;
      }
      bool done = false;
      for( int j = 0; j < k && !done; j++ )
        if( a[j].inst == idx ) { a[j].kind = kind; done = true; }
      if( !done && k < cap ) {
        a[k].inst = idx;
        a[k].kind = kind;
        k++;
      }
    }
    if( k > 1 )
      qsort( a, k, sizeof( TItemKindOv ), IkCmp );
    g_ikOv = a;
    g_ikN  = k;
    MM_LOG( zSTRING( "item kinds: from txt files " ) + zSTRING( k ) + " instances (by prefix " + zSTRING( byPat ) + "), unknown names " + zSTRING( unknown ) + ", bad lines " + zSTRING( bad ) );
  }
  inline int ItemKindTxt( oCItem* it ) {
    if( !it ) return IK_NONE;
    ItemKindsEnsure();
    if( g_ikN <= 0 ) return IK_NONE;
    TItemKindOv key;
    key.inst = it->instanz;
    key.kind = 0;
    const TItemKindOv* f = (const TItemKindOv*)bsearch( &key, g_ikOv, g_ikN, sizeof( TItemKindOv ), IkCmp );
    return f ? f->kind : IK_NONE;
  }
  struct TNameOv { int inst; int item; };
  static TNameOv* g_nmOv     = 0;
  static int      g_nmN      = 0;
  static int      g_nmGen    = -1;
  static void*    g_nmParser = 0;
  inline void NamesFree() {
    MiniMapCommon::MmFree( g_nmOv );
    g_nmOv     = 0;
    g_nmN      = 0;
    g_nmGen    = -1;
    g_nmParser = 0;
  }
  inline int NmCmp( const void* a, const void* b ) {
    const int x = ( (const TNameOv*)a )->inst, y = ( (const TNameOv*)b )->inst;
    return ( x < y ) ? -1 : ( x > y ) ? 1 : 0;
  }
  inline void NamesEnsure() {
    if( g_nmGen == g_cfgGen && g_nmParser == (void*)parser ) return;
    NamesFree();
    g_nmGen    = g_cfgGen;
    g_nmParser = (void*)parser;
    const int sec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "names" ) : -1;
    if( sec < 0 || !parser ) return;
    const int cnt = MiniMapCfg::Count( g_cfg, "names" );
    if( cnt <= 0 ) return;
    TNameOv* a = MiniMapCommon::MmAlloc<TNameOv>( (size_t)( cnt ) );
    if( !a ) return;
    int k = 0, mobs = 0, empty = 0;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0 && k < cnt; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      if( !t.val[0] ) {
        CfgMsg( 0, t.src, t.line, "[names]: empty name - ignored" );
        empty++;
        continue;
      }
      const int idx = TraderSymIndex( t.key );
      if( idx < 0 ) {
        mobs++;
        continue;
      }
      a[k].inst = idx;
      a[k].item = i;
      k++;
    }
    if( k > 1 )
      qsort( a, k, sizeof( TNameOv ), NmCmp );
    g_nmOv = a;
    g_nmN  = k;
    MM_LOG( zSTRING( "names: from txt files " ) + zSTRING( k ) + " characters/items, " + zSTRING( mobs ) + " other keys (mob names), empty " + zSTRING( empty ) );
  }
  inline const char* NameTxtInst( int inst ) {
    NamesEnsure();
    if( g_nmN <= 0 ) return 0;
    TNameOv key;
    key.inst = inst;
    key.item = 0;
    const TNameOv* f = (const TNameOv*)bsearch( &key, g_nmOv, g_nmN, sizeof( TNameOv ), NmCmp );
    return ( f && f->item >= 0 && f->item < g_cfg.n ) ? g_cfg.e[f->item].val : 0;
  }
  inline const char* NameTxtMob( const char* focus ) {
    if( !focus || !*focus || !g_modFiles ) return 0;
    const MiniMapCfg::TItem* t = CfgFind( "names", focus );
    return ( t && t->val[0] ) ? t->val : 0;
  }
  static int   g_totState = 0;
  static float g_totX = 0.0f, g_totY = 0.0f, g_totZ = 0.0f;
  static const float TOT_R = 800.0f;
  inline void TotEnsure( oCWorld* world ) {
    if( g_totState != 0 || !world ) return;
    g_totState = 2;
    zCWayNet* const wn = world->wayNet;
    zCWaypoint* const wp = wn ? wn->GetWaypoint( zSTRING( "TOT" ) ) : 0;
    if( wp ) {
      g_totX     = wp->pos[0];
      g_totY     = wp->pos[1];
      g_totZ     = wp->pos[2];
      g_totState = 1;
      MM_LOG( zSTRING( "TOT point: " ) + zSTRING( (int)g_totX ) + ", " + zSTRING( (int)g_totZ ) + " - characters there hidden" );
    }
  }
  inline bool TotNear( float x, float y, float z, bool fullY ) {
    if( g_totState != 1 ) return false;
    const float dx = x - g_totX, dy = y - g_totY, dz = z - g_totZ;
    if( dx * dx + dz * dz >= TOT_R * TOT_R ) return false;
    return !fullY || dy * dy < 2000.0f * 2000.0f;
  }
  static const int INV_LISTS = zSwitch( (int)INV_MAX, (int)INV_MAX, 1, 1 );
  template<typename TNpc = oCNpc>
  inline zCListSort<oCItem>* InvFirst( TNpc* npc, int c ) {
    if constexpr( ENGINE <= Engine_G1A ) return npc->inventory2.inventory[c].next;
    else                                 return npc->inventory2.inventory.next;
  }
  template<typename TNpc = oCNpc>
  inline const zSTRING& InvPacked( TNpc* npc, int c ) {
    if constexpr( ENGINE <= Engine_G1A ) return npc->inventory2.packString[c];
    else                                 return npc->inventory2.packString;
  }
  static const int INVN_KEY  = 40;
  static const int INVN_NAME = 44;
  struct TPackIt {
    const char* p;
    char        key[INVN_KEY];
    int         amount;
  };
  inline void PackStart( TPackIt& t, const zSTRING& s ) {
    t.p      = ( s.Length() > 0 ) ? const_cast<zSTRING&>( s ).ToChar() : 0;
    t.key[0] = 0;
    t.amount = 0;
  }
  inline bool PackNext( TPackIt& t ) {
    while( t.p && *t.p ) {
      const char* s = t.p;
      while( *s == ' ' )
        s++;
      int  k       = 0;
      bool tooLong = false;
      for( ; *s && *s != ':' && *s != ','; s++ ) {
        if( k < INVN_KEY - 1 ) {
          char c = *s;
          if( c >= 'a' && c <= 'z' ) c = (char)( c - 32 );
          t.key[k++] = c;
        }
        else
          tooLong = true;
      }
      while( k > 0 && t.key[k - 1] == ' ' )
        k--;
      t.key[k] = 0;
      int am = 1;
      if( *s == ':' ) {
        s++;
        int v = 0, d = 0;
        for( ; *s >= '0' && *s <= '9'; s++, d++ )
          if( v < 10000000 ) v = v * 10 + ( *s - '0' );
        if( d > 0 && v > 0 ) am = v;
        while( *s && *s != ',' )
          s++;
      }
      if( *s == ',' )
        s++;
      t.p = s;
      if( k > 0 && !tooLong ) {
        t.amount = am;
        return true;
      }
    }
    return false;
  }
  struct TInvName {
    char        key[INVN_KEY];
    char        name[INVN_NAME];
    signed char state;
    signed char cat;
    signed char queued;
    signed char icon;
  };
  static const int INVN_SIZE    = 4096;
  static const int INV_PEND_MAX = 512;
  static const int INV_TICK     = 4;
  static TInvName  g_invName[INVN_SIZE];
  static int       g_invNameUsed  = 0;
  static int       g_invPend[INV_PEND_MAX];
  static int       g_invPendN     = 0;
  static bool      g_invCreateOff = false;
  static zCPar_Symbol* g_invItemCls = 0;
  static bool      g_invItemClsTried = false;
  inline TInvName* InvNameFind( const char* key ) {
    unsigned long h = 2166136261UL;
    for( const char* q = key; *q; q++ )
      h = ( h ^ (unsigned char)*q ) * 16777619UL;
    for( int k = 0; k < INVN_SIZE; k++ ) {
      TInvName& e = g_invName[( h + k ) & ( INVN_SIZE - 1 )];
      if( !e.key[0] ) {
        if( g_invNameUsed >= INVN_SIZE * 3 / 4 ) return 0;
        StrCopy( e.key, key, INVN_KEY );
        e.name[0] = 0;
        e.state   = 0;
        e.cat     = 0;
        e.queued  = 0;
        e.icon    = -1;
        g_invNameUsed++;
        return &e;
      }
      if( strcmp( e.key, key ) == 0 ) return &e;
    }
    return 0;
  }
  inline const TInvName* InvNameGet( const char* key ) {
    TInvName* const e = InvNameFind( key );
    if( e && e->state == 0 && !e->queued && g_invPendN < INV_PEND_MAX ) {
      e->queued = 1;
      g_invPend[g_invPendN++] = (int)( e - g_invName );
    }
    return e;
  }
  inline bool InvIsItemInstance( int idx ) {
    if( !parser || idx < 0 ) return false;
    if( !g_invItemClsTried ) {
      g_invItemClsTried = true;
      g_invItemCls = parser->GetSymbol( zSTRING( "C_ITEM" ) );
    }
    zCPar_Symbol* const s = parser->GetSymbol( idx );
    if( !g_invItemCls || !s || s->type != zPAR_TYPE_INSTANCE ) return false;
    zCPar_Symbol* c = s->parent;
    for( int g = 0; c && g < 8 && c != g_invItemCls; g++ )
      c = c->parent;
    return c == g_invItemCls;
  }
  inline oCItem* InvCreateRaw( int idx ) {
    oCItem* it = 0;
    const int oldIgn = zCVob::s_ignoreVisuals;
    zCVob::s_ignoreVisuals = 1;
    __try {
      it = zfactory ? zfactory->CreateItem( idx ) : 0;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      it = 0;
      g_invCreateOff = true;
    }
    zCVob::s_ignoreVisuals = oldIgn;
    return it;
  }
  inline void InvNameTick() {
    static int s_failLogged = 0;
    for( int done = 0; g_invPendN > 0 && done < INV_TICK; ) {
      TInvName& e = g_invName[g_invPend[--g_invPendN]];
      e.queued = 0;
      if( e.state != 0 ) continue;
      done++;
      oCItem* it = 0;
      if( parser && zfactory && !g_invCreateOff ) {
        const int idx = parser->GetIndex( zSTRING( e.key ) );
        if( InvIsItemInstance( idx ) ) it = InvCreateRaw( idx );
        if( g_invCreateOff )
          MM_LOG( zSTRING( "inventory: exception creating item " ) + zSTRING( e.key ) + " - packed goods names off until game exit" );
      }
      if( !it ) {
        e.state = 2;
        if( s_failLogged < 5 && !g_invCreateOff ) {
          s_failLogged++;
          MM_LOG( zSTRING( "inventory: no item for instance " ) + zSTRING( e.key ) );
        }
        continue;
      }
      const zSTRING& nm = ( it->name.Length() > 0 ) ? it->name : it->description;
      StrCopy( e.name, const_cast<zSTRING&>( nm ).ToChar(), INVN_NAME );
      e.name[0] = InvCapFirst( e.name[0] );
      e.cat     = (signed char)SearchCatOf( it );
      e.icon    = (signed char)ItemKindIcon( it );
      e.state   = e.name[0] ? 1 : 2;
      it->Release();
    }
  }
  enum { INV_ALL = 0, INV_LOOT = 1, INV_GOODS = 2 };
  inline bool InvItemCounts( oCItem* it, int mode ) {
    if( mode == INV_LOOT ) return CorpseLootable( it );
    if( mode == INV_GOODS ) return !( it->flags & ITM_FLAG_ACTIVE );
    return true;
  }
  inline bool NpcHasLoot( oCNpc* npc ) {
    for( int c = 0; c < INV_LISTS; c++ ) {
      if( InvPacked( npc, c ).Length() > 0 ) return true;
      int guard = 0;
      for( zCListSort<oCItem>* e = InvFirst( npc, c ); e && guard < 4096; e = e->next, guard++ )
        if( e->data && CorpseLootable( e->data ) ) return true;
    }
    return false;
  }
  struct TInvRow { char name[INVN_NAME]; int amount; signed char icon; };
  inline void InvRowAdd( TInvRow* rows, int& nr, int maxr, int& other, const char* name, int am,
                         int icon ) {
    for( int k = 0; k < nr; k++ )
      if( strcmp( rows[k].name, name ) == 0 ) {
        rows[k].amount += am;
        return;
      }
    if( nr < maxr ) {
      StrCopy( rows[nr].name, name, INVN_NAME );
      rows[nr].amount = am;
      rows[nr].icon   = (signed char)icon;
      nr++;
    }
    else
      other++;
  }
  inline int InvCollect( oCNpc* npc, int mode, TInvRow* rows, int maxr, int& other ) {
    int nr = 0;
    other = 0;
    for( int c = 0; c < INV_LISTS; c++ ) {
      int guard = 0;
      for( zCListSort<oCItem>* e = InvFirst( npc, c ); e && guard < 4096; e = e->next, guard++ ) {
        oCItem* const it = e->data;
        if( !it || !InvItemCounts( it, mode ) ) continue;
        const zSTRING& nm = ( it->name.Length() > 0 ) ? it->name : it->description;
        char buf[INVN_NAME];
        StrCopy( buf, const_cast<zSTRING&>( nm ).ToChar(), INVN_NAME );
        if( !buf[0] )
          StrCopy( buf, "?", INVN_NAME );
        buf[0] = InvCapFirst( buf[0] );
        InvRowAdd( rows, nr, maxr, other, buf, ( it->amount > 0 ) ? it->amount : 1, ItemKindIcon( it ) );
      }
      TPackIt t;
      PackStart( t, InvPacked( npc, c ) );
      while( PackNext( t ) ) {
        const TInvName* const en = InvNameGet( t.key );
        const char* const nm = ( en && en->state == 1 ) ? en->name
                             : ( en && en->state == 2 ) ? t.key : "...";
        InvRowAdd( rows, nr, maxr, other, nm, t.amount, en ? en->icon : -1 );
      }
    }
    return nr;
  }
  inline void NpcFlagsBuild( oCWorld* world ) {
    g_npcFlagN = 0;
    if( !g_npcCache || g_npcCount <= 0 ) return;
    if( g_npcFlagCap < g_npcCount ) {
      unsigned char* const a = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( g_npcCount + 64 ) );
      if( !a ) return;
      MiniMapCommon::MmFree( g_npcFlag );
      g_npcFlag    = a;
      g_npcFlagCap = g_npcCount + 64;
    }
    TotEnsure( world );
    int traders = 0, gone = 0, hiddenTxt = 0;
    for( int i = 0; i < g_npcCount; i++ ) {
      oCNpc* const npc = g_npcCache[i];
      unsigned char f = 0;
      if( npc && npc != player ) {
        if( TraderIs( npc ) ) {
          f |= NPCF_TRADER;
          traders++;
        }
        if( g_totState == 1 ) {
          zVEC3 q = npc->GetPositionWorld();
          bool  fullY = true;
          const NpcSpawnPos* const sb = npc->homeWorld ? 0 : NpcSpawnFind( npc );
          if( sb ) {
            q[0]  = sb->x;
            q[2]  = sb->z;
            fullY = false;
          }
          if( TotNear( q[0], q[1], q[2], fullY ) ) {
            f |= NPCF_GONE;
            gone++;
          }
        }
        if( npc->attribute[NPC_ATR_HITPOINTS] <= 0 && NpcHasLoot( npc ) )
          f |= NPCF_LOOT;
        if( !( f & NPCF_GONE ) && HiddenInst( npc->instanz ) ) {
          f |= NPCF_GONE;
          hiddenTxt++;
        }
      }
      g_npcFlag[i] = f;
    }
    g_npcFlagN = g_npcCount;
    static int s_lt = -1, s_lg = -1, s_lhid = -1;
    if( traders != s_lt || gone != s_lg || hiddenTxt != s_lhid ) {
      s_lt = traders;
      s_lg = gone;
      s_lhid = hiddenTxt;
      MM_LOG( zSTRING( "characters: traders in world " ) + zSTRING( traders ) + ", at TOT point (hidden) " + zSTRING( gone ) + ( hiddenTxt ? zSTRING( ", hidden by [hide] " ) + zSTRING( hiddenTxt ) : zSTRING( "" ) ) );
    }
  }
  inline zCWaypoint* NpcWpFind( oCWorld* world, const zSTRING& name ) {
    return world->wayNet->GetWaypoint( name );
  }
  inline unsigned int NpcWpHash( const zSTRING& a, const zSTRING& b ) {
    unsigned int h = 2166136261u;
    for( const char* p = a.ToChar(); p && *p; p++ ) { h ^= (unsigned char)*p; h *= 16777619u; }
    h ^= 0x7Cu; h *= 16777619u;
    for( const char* p = b.ToChar(); p && *p; p++ ) { h ^= (unsigned char)*p; h *= 16777619u; }
    return h;
  }
  inline int NpcWpCore( oCWorld* world, oCNpc* npc, float& x, float& z ) {
    __try {
      const unsigned int h = NpcWpHash( npc->wpname, npc->spawnPoint );
      NpcWpMemo* slot = 0;
      if( g_npcWpMemo ) {
        const int k0 = (int)( ( (unsigned long)npc >> 4 ) % NPC_WP_MEMO );
        for( int t = 0; t < 8; t++ ) {
          NpcWpMemo& e = g_npcWpMemo[( k0 + t ) % NPC_WP_MEMO];
          if( e.npc == npc && e.h == h ) {
            x = e.x; z = e.z;
            return e.has ? 1 : 0;
          }
          if( !e.npc || e.npc == npc ) { slot = &e; break; }
        }
      }
      if( g_npcWpBudget <= 0 ) return -1;
      g_npcWpBudget--;
      zCWaypoint* w = 0;
      if( npc->wpname.Length() > 0 ) w = NpcWpFind( world, npc->wpname );
      if( !w && npc->spawnPoint.Length() > 0 ) w = NpcWpFind( world, npc->spawnPoint );
      if( w ) { x = w->pos[0]; z = w->pos[2]; }
      if( slot ) {
        if( !slot->npc ) g_npcWpMemoN++;
        slot->npc = npc; slot->h = h;
        slot->x = w ? x : 0.0f; slot->z = w ? z : 0.0f;
        slot->has = w ? 1 : 0;
      }
      return w ? 1 : 0;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return 0;
    }
  }
  inline void NpcWpBuild( oCWorld* world ) {
    g_npcWpN = 0;
    if( !g_npcCache || g_npcCount <= 0 || !world || !world->wayNet ) return;
    if( g_npcWpCap < g_npcCount ) {
      NpcWpPos* const a = MiniMapCommon::MmAlloc<NpcWpPos>( (size_t)( g_npcCount + 64 ) );
      if( !a ) return;
      MiniMapCommon::MmFree( g_npcWp );
      g_npcWp    = a;
      g_npcWpCap = g_npcCount + 64;
    }
    int found = 0, need = 0, later = 0;
    if( !g_npcWpMemo ) {
      g_npcWpMemo = MiniMapCommon::MmAlloc<NpcWpMemo>( (size_t)( NPC_WP_MEMO ) );
      if( g_npcWpMemo )
        memset( g_npcWpMemo, 0, sizeof( NpcWpMemo ) * NPC_WP_MEMO );
      g_npcWpMemoN = 0;
    }
    if( g_npcWpMemo && g_npcWpMemoN > NPC_WP_MEMO * 3 / 4 ) {
      memset( g_npcWpMemo, 0, sizeof( NpcWpMemo ) * NPC_WP_MEMO );
      g_npcWpMemoN = 0;
    }
    g_npcWpBudget = 48;
    for( int i = 0; i < g_npcCount; i++ ) {
      NpcWpPos& e = g_npcWp[i];
      e.has = 0;
      oCNpc* const npc = g_npcCache[i];
      if( !npc || npc->homeWorld || NpcSpawnFind( npc ) ) continue;
      need++;
      const int r = NpcWpCore( world, npc, e.x, e.z );
      if( r > 0 ) {
        e.has = 1;
        found++;
      }
      else if( r < 0 )
        later++;
    }
    g_npcWpN = g_npcCount;
    static int s_need = -1, s_found = -1;
    if( later == 0 && ( need != s_need || found != s_found ) ) {
      s_need = need; s_found = found;
      MM_LOG( zSTRING( "characters: removed without respawn point " ) + zSTRING( need ) + ", placed at routine waypoint " + zSTRING( found ) );
    }
  }
  inline bool NpcWpAt( int i, float& x, float& z ) {
    if( !g_npcWp || g_npcWpN != g_npcCount || i < 0 || i >= g_npcWpN || !g_npcWp[i].has ) return false;
    x = g_npcWp[i].x;
    z = g_npcWp[i].z;
    return true;
  }
  inline void NpcCacheRefresh( oCWorld* world ) {
    oCNpc** arr = 0;
    int     n   = 0;
    int     cap = 0;
    for( zCListSort<oCNpc>* node = world->voblist_npcs; node; node = node->next ) {
      oCNpc* const npc = node->data;
      if( npc && !NpcListPush( arr, n, cap, npc ) ) break;
    }
    if( n > 1 )
      qsort( arr, n, sizeof( oCNpc* ), NpcPtrCmp );
    const int sorted = n;
    zCClassDef* const npcDef = oCNpc::classDef;
    if( npcDef ) {
      for( zCListSort<zCVob>* node = world->voblist; node; node = node->next ) {
        zCVob* const vob = node->data;
        if( !vob ) continue;
        if( !zCObject::CheckInheritance( npcDef, vob->_GetClassDef() ) ) continue;
        oCNpc* npc = (oCNpc*)vob;
        if( sorted > 0 && bsearch( &npc, arr, sorted, sizeof( oCNpc* ), NpcPtrCmp ) ) continue;
        if( !NpcListPush( arr, n, cap, npc ) ) break;
      }
    }
    for( int i = 0; i < n; i++ )
      arr[i]->AddRef();
    oCNpc** const old  = g_npcCache;
    const int     oldN = g_npcCount;
    g_npcCache = arr;
    g_npcCount = n;
    g_npcCap   = cap;
    g_npcWorld = world;
    NpcSpawnBuild();
    NpcSpawnDiag();
    for( int i = 0; i < oldN; i++ )
      if( old[i] )
        old[i]->Release();
    MiniMapCommon::MmFree( old );
    NpcFlagsBuild( world );
    NpcWpBuild( world );
    NpcIconsBuild();
    SearchMatchNpcs();
    SearchMatchCorpses();
    SearchSuggestBuild();
    static int loggedN = -1;
    if( loggedN < 0 || ( n - loggedN ) * ( n - loggedN ) > 400 ) {
      loggedN = n;
      MM_LOG( zSTRING( "characters: in world " ) + zSTRING( n ) + " (npc list " + zSTRING( sorted ) + ", extra from vob list " + zSTRING( n - sorted ) + ")" );
    }
  }
  static int g_partyIdx = -2;
  static const int NPC_AIVARS = zSwitch( 50, 50, 70, 100 );
  inline int PartyAivarIdx() {
    if( g_partyIdx == -2 ) {
      g_partyIdx = -1;
      zCPar_Symbol* const s = parser ? parser->GetSymbol( zSTRING( "AIV_PARTYMEMBER" ) ) : 0;
      if( s ) {
        int v = -1;
        s->GetValue( v, 0 );
        if( v >= 0 && v < NPC_AIVARS ) g_partyIdx = v;
      }
    }
    return g_partyIdx;
  }
  inline bool NpcSummonedAlly( oCNpc* npc ) {
    if( !npc->isSummoned || npc->IsHuman() ) return false;
    if constexpr( ENGINE >= Engine_G2 ) {
      const int g = npc->guild;
      if( g == Gothic_II_Addon::NPC_GIL_SUMMONED_GOBBO_SKELETON || g == Gothic_II_Addon::NPC_GIL_SUMMONED_WOLF
          || g == Gothic_II_Addon::NPC_GIL_SUMMONED_SKELETON || g == Gothic_II_Addon::NPC_GIL_SUMMONED_GOLEM
          || g == Gothic_II_Addon::NPC_GIL_SUMMONED_DEMON )
        return true;
      if constexpr( ENGINE == Engine_G2A )
        return g == Gothic_II_Addon::NPC_GIL_SUMMONED_GUARDIAN || g == Gothic_II_Addon::NPC_GIL_SUMMONED_ZOMBIE;
      return false;
    }
    return true;
  }
  inline int NpcGroup( oCNpc* npc ) {
    const int pi = PartyAivarIdx();
    if( pi >= 0 && npc->aiscriptvars[pi] != 0 ) return 2;
    if( npc->IsUnconscious() ) return 1;
    const int att = npc->GetAttitude( player );
    switch( att ) {
      case NPC_ATT_HOSTILE:
      case NPC_ATT_ANGRY:    return 0;
      case NPC_ATT_FRIENDLY: return 2;
      default:               return NpcSummonedAlly( npc ) ? 2 : 1;
    }
  }
  inline zCOLOR ColNpcGroup( int g ) {
    return ( g == 0 ) ? ColFoe() : ( g == 2 ) ? ColFriend() : ColNeutral();
  }
  static zCTexture* g_iconTex   = 0;
  static bool       g_iconTried = false;
  inline void IconFree( bool release = true ) {
    if( g_iconTex && release )
      g_iconTex->Release();
    g_iconTex   = 0;
    g_iconTried = false;
    MarkAtlasFree( release );
    IconPackFree( release );
  }
  inline zCOLOR AtlasColor( int icon ) {
    static const signed char kCol[ICON_SLOTS] = {
      COLK_ENEMY, COLK_NEUTRAL, COLK_FRIEND, COLK_ITEM, COLK_ITEM_1H, COLK_ITEM_2H, COLK_ITEM_RANGED,
      COLK_ITEM_ARMOR, COLK_ITEM_POTION, COLK_ITEM_PLANT, COLK_ITEM_MAGIC, COLK_CHEST_OPEN, COLK_CHEST_KEY,
      COLK_CHEST_LOCKPICK, COLK_CHEST_BOTH, COLK_CHEST_SEALED, COLK_SWITCH, COLK_SWITCH, COLK_SWITCH_TARGET,
      COLK_SHRINE, COLK_BOARD, COLK_FORGE, COLK_ORE, COLK_TREASURE, COLK_OTHER_PLACE, COLK_OTHER_PLACE,
      COLK_OTHER_PLACE, COLK_OTHER_PLACE, COLK_OTHER_PLACE, COLK_EXIT, COLK_TRADER, COLK_CORPSE, -1,
      COLK_NEUTRAL, COLK_ALCHEMY, COLK_KITCHEN, COLK_WORKSHOP, COLK_OTHER_PLACE, COLK_RUNE, COLK_SCROLL,
      COLK_DEMON, COLK_OTHER_PLACE, COLK_PLAYER
    };
    if( icon == ICON_TELEPORT ) return zCOLOR( 190, 120, 255, 255 );
    if( icon == PLACE_OTHER || icon == PLACE_REST ) return ColPlaceOther();
    return ColPick( ( icon >= 0 && icon < ICON_SLOTS ) ? kCol[icon] : COLK_NEUTRAL );
  }
  static unsigned char g_iconFromFile[ICON_SLOTS];
  inline void BakeSrcCell( const MiniMapImage::TSrc& s, unsigned char* px, int n, int bpp, int fmt,
                           int ox, int oy, int S, unsigned bf, bool grey, int cr, int cg, int cb ) {
    if( S > MiniMapImage::CELL_MAX || ox < 0 || oy < 0 || ox + S > n || oy + S > n ) return;
    static unsigned long cellPx[MiniMapImage::CELL_MAX * MiniMapImage::CELL_MAX];
    MiniMapImage::Bake( s, S, bf, cellPx, S );
    if( grey ) MiniMapImage::Tint( cellPx, S * S, cr, cg, cb );
    for( int y = 0; y < S; y++ )
      for( int x = 0; x < S; x++ ) {
        const unsigned long argb = cellPx[y * S + x];
        const int q = ( oy + y ) * n + ox + x;
        if( bpp == 4 ) ( (unsigned long*)px )[q] = argb;
        else           ( (unsigned short*)px )[q] = GenPack16( argb, fmt, 0.0f );
      }
  }
  inline bool IconFileOne( unsigned char* px, int n, int bpp, int fmt, int slot, const char* file,
                           unsigned flags, const MiniMapCfg::TItem& t, const unsigned char* rgb ) {
    char path[MAX_PATH];
    if( !FilePath( path, MAX_PATH, CFG_ICONS_DIR, file ) ) return false;
    int len = 0;
    unsigned char* data = CfgReadFile( path, len, MiniMapImage::IMG_MAX_BYTES );
    if( !data ) {
      CfgMsg( 0, t.src, t.line, len == -2 ? "[icons]: file is larger than 8 MB - built-in icon"
                                          : "[icons]: file not found in MiniMap\\Icons - built-in icon" );
      return false;
    }
    unsigned char* rgba = 0;
    int w = 0, h = 0;
    const char* err = MiniMapImage::Decode( data, len, rgba, w, h );
    MiniMapCommon::MmFree( data );
    if( err ) {
      char m[160];
      snprintf( m, sizeof( m ), "[icons]: %s", err );
      CfgMsg( 0, t.src, t.line, m );
      return false;
    }
    MiniMapImage::TSrc s;
    if( !MiniMapImage::Prep( rgba, w, h, s ) ) {
      CfgMsg( 0, t.src, t.line, "[icons]: image is fully transparent - built-in icon" );
      MiniMapImage::Free( rgba );
      return false;
    }
    const bool grey = ( flags & ( MiniMapCfg::ICOV_TINT | MiniMapCfg::ICOV_COLOR ) ) != 0;
    unsigned bf = grey ? 0u : (unsigned)MiniMapImage::KEEPCOLORS;
    if( flags & MiniMapCfg::ICOV_NOOUTLINE ) bf |= MiniMapImage::NOOUTLINE;
    int cr = 255, cg = 255, cb = 255;
    if( flags & MiniMapCfg::ICOV_COLOR ) {
      cr = rgb[0]; cg = rgb[1]; cb = rgb[2];
    }
    else if( flags & MiniMapCfg::ICOV_TINT ) {
      const zCOLOR col = AtlasColor( slot );
      cr = col.r; cg = col.g; cb = col.b;
    }
    for( int cell = 0; cell < ICON_CELLS; cell++ ) {
      int ox, oy, S;
      IconSlotXY( cell, slot, ox, oy, S );
      BakeSrcCell( s, px, n, bpp, fmt, ox, oy, S, bf, grey, cr, cg, cb );
    }
    MiniMapImage::Free( rgba );
    return true;
  }
  inline const char* IconGameName();
  inline bool MarkStylePath( char* out, int cap, const char* name, int style, const char* game );
  inline void IconCustomFill();
  inline void IconPackInstall();
  inline bool IconKnownFile( const char* rel, const unsigned char* d, int n );
  inline void MarkStyleColor( int slot, unsigned char* rgb ) {
    const zCOLOR c = AtlasColor( slot );
    rgb[0] = c.r; rgb[1] = c.g; rgb[2] = c.b;
  }
  static void ( *g_markColorFn )( int slot, unsigned char* rgb ) = MarkStyleColor;
  static const char* g_markGameUsed = 0;
  inline bool MarkFileColored( const char* path ) {
    int len = 0;
    unsigned char* data = CfgReadFile( path, len, MiniMapImage::IMG_MAX_BYTES );
    if( !data ) return false;
    unsigned char* rgba = 0;
    int w = 0, h = 0;
    const char* err = MiniMapImage::Decode( data, len, rgba, w, h );
    MiniMapCommon::MmFree( data );
    if( err ) return false;
    const bool c = MiniMapImage::IsColored( rgba, w, h );
    MiniMapImage::Free( rgba );
    return c;
  }
  inline bool SimpleStyleBakeOne( unsigned char* px, int n, int bpp, int fmt, int slot, const char* rel ) {
    char path[MAX_PATH];
    if( !FilePath( path, MAX_PATH, CFG_ICONS_DIR, rel ) ) return false;
    int len = 0;
    unsigned char* data = CfgReadFile( path, len, MiniMapImage::IMG_MAX_BYTES );
    if( !data ) return false;
    const bool untouched = IconKnownFile( rel, data, len );
    MiniMapCommon::MmFree( data );
    if( untouched ) return false;
    unsigned char rgb[3] = { 255, 255, 255 };
    g_markColorFn( slot, rgb );
    MiniMapCfg::TItem none;
    memset( &none, 0, sizeof( none ) );
    none.src = -1;
    const unsigned fl = MarkFileColored( path ) ? 0u : (unsigned)MiniMapCfg::ICOV_COLOR;
    if( !IconFileOne( px, n, bpp, fmt, slot, rel, fl, none, rgb ) ) return false;
    g_iconFromFile[slot] = 1;
    return true;
  }
  inline int MarkStyleBake( unsigned char* px, int n, int bpp, int fmt ) {
    g_markGameUsed = 0;
    if( g_iconStyle < 0 ) return 0;
    IconPackInstall();
    const char* const game = IconGameName();
    g_markGameUsed = game;
    if( g_iconStyle == 0 ) {
      static const TIconKey kSimpleNpc[4] = {
        { "enemy", 0 }, { "neutral", 1 }, { "friend", 2 }, { "animal", ICON_ANIMAL }
      };
      int done0 = 0;
      char rel[MAX_PATH];
      for( int k = 0; k < 4; k++ ) {
        snprintf( rel, sizeof( rel ), "default\\simple\\%s.png", kSimpleNpc[k].key );
        if( SimpleStyleBakeOne( px, n, bpp, fmt, kSimpleNpc[k].slot, rel ) )
          done0++;
      }
      for( int k = 0; k < kIconFileKeyN; k++ ) {
        const int slot = kIconFileKey[k].slot;
        const bool isItem = ( slot == ICON_ITEM_OTHER || ( slot >= ICON_ITEM_1H && slot <= ICON_ITEM_MAGIC ) );
        snprintf( rel, sizeof( rel ), "default\\simple\\%s%s.png", isItem ? "" : "mark_", kIconFileKey[k].key );
        if( SimpleStyleBakeOne( px, n, bpp, fmt, slot, rel ) )
          done0++;
      }
      return done0;
    }
    if( g_iconStyle == 3 )
      IconCustomFill();
    MiniMapCfg::TItem none;
    memset( &none, 0, sizeof( none ) );
    none.src = -1;
    int done = 0;
    for( int k = 0; k < kIconFileKeyN; k++ ) {
      const int slot = kIconFileKey[k].slot;
      if( slot < 0 || slot >= ICON_SLOTS || slot == ICON_ITEM_OTHER || ( slot >= ICON_ITEM_1H && slot <= ICON_ITEM_MAGIC ) ) continue;
      char path[MAX_PATH];
      if( !MarkStylePath( path, MAX_PATH, kIconFileKey[k].key, g_iconStyle, game ) ) continue;
      unsigned char rgb[3] = { 255, 255, 255 };
      g_markColorFn( slot, rgb );
      const char* rel = path;
      const size_t dl = strlen( CFG_ICONS_DIR );
      if( !_strnicmp( path, CFG_ICONS_DIR, dl ) && path[dl] == '\\' ) rel = path + dl + 1;
      const unsigned fl = ( g_iconStyle == 3 && MarkFileColored( path ) ) ? 0u : (unsigned)MiniMapCfg::ICOV_COLOR;
      if( IconFileOne( px, n, bpp, fmt, slot, rel, fl, none, rgb ) ) {
        if( !g_iconFromFile[slot] ) done++;
        g_iconFromFile[slot] = 1;
      }
    }
    return done;
  }
  inline int IconFilesBake( unsigned char* px, int n, int bpp, int fmt ) {
    memset( g_iconFromFile, 0, sizeof( g_iconFromFile ) );
    if( !px || n <= 0 || ( bpp != 4 && bpp != 2 ) ) return 0;
    int done = MarkStyleBake( px, n, bpp, fmt );
    const int sec = g_modFiles ? MiniMapCfg::SecFind( g_cfg, "icons" ) : -1;
    if( sec < 0 ) return done;
    for( int i = MiniMapCfg::Next( g_cfg, sec, -1 ); i >= 0; i = MiniMapCfg::Next( g_cfg, sec, i ) ) {
      const MiniMapCfg::TItem& t = g_cfg.e[i];
      const int slot = IconFileSlot( t.key );
      if( slot < 0 || slot >= ICON_SLOTS ) {
        CfgMsg( 0, t.src, t.line, "[icons]: unknown icon name (list: examples\\02_icons.txt) - ignored" );
        continue;
      }
      char file[MAX_PATH];
      unsigned flags = 0;
      unsigned char rgb[3] = { 255, 255, 255 };
      const char* err = 0;
      if( !MiniMapCfg::ParseIconVal( t.val, file, MAX_PATH, flags, &err, rgb ) ) {
        CfgMsg( 0, t.src, t.line, err ? err : "[icons]: bad value" );
        continue;
      }
      if( IconFileOne( px, n, bpp, fmt, slot, file, flags, t, rgb ) ) {
        if( !g_iconFromFile[slot] ) done++;
        g_iconFromFile[slot] = 1;
      }
    }
    return done;
  }
  inline void PackAtlas16InPlace( unsigned long* px, int count, int bpp, int fmt ) {
    if( bpp != 2 ) return;
    unsigned short* const p16 = (unsigned short*)px;
    for( int i = 0; i < count; i++ ) p16[i] = GenPack16( px[i], fmt, 0.0f );
  }
  inline void AtlasBakeCell( unsigned char* px, int n, int bpp, int fmt, int ox, int oy, int S,
                             int icon, const zCOLOR& col, bool useHole,
                             bool (*hullFn)( int, float, float ), bool (*holeFn)( int, float, float ),
                             const float* rot = 0, int ss = 4 ) {
    const float dk[3]  = { 18.0f, 16.0f, 15.0f };
    const float cc[3]  = { (float)col.r, (float)col.g, (float)col.b };
    const float tpu    = ( (float)S * 0.5f - 1.6f ) / 0.9f;
    const float border = 1.1f / tpu;
    const float step   = 1.0f / (float)ss, norm = 1.0f / (float)( ss * ss );
    for( int ty = 0; ty < S; ty++ ) {
      for( int tx = 0; tx < S; tx++ ) {
        float fill = 0.0f, dark = 0.0f, shade = 0.0f;
        for( int sy = 0; sy < ss; sy++ ) {
          for( int sx = 0; sx < ss; sx++ ) {
            const float su = ( (float)tx + ( (float)sx + 0.5f ) * step - (float)S * 0.5f ) / tpu;
            const float sv = ( (float)ty + ( (float)sy + 0.5f ) * step - (float)S * 0.5f ) / tpu;
            const float u  = rot ? ( -rot[1] * su + rot[0] * sv ) : su;
            const float v  = rot ? ( -rot[0] * su - rot[1] * sv ) : sv;
            const bool in  = hullFn( icon, u, v );
            if( in && !( useHole && holeFn( icon, u, v ) ) ) {
              fill += 1.0f; shade += 1.08f - 0.16f * sv; continue;
            }
            bool edge = in;
            for( int d = 0; d < 16 && !edge; d++ )
              edge = hullFn( icon, u + MiniMapImage::kDir16[d][0] * border, v + MiniMapImage::kDir16[d][1] * border )
                  || hullFn( icon, u + MiniMapImage::kDir16[d][0] * border * 0.5f, v + MiniMapImage::kDir16[d][1] * border * 0.5f );
            if( edge ) dark += 1.0f;
          }
        }
        const float a = ( fill + dark ) * norm;
        if( a <= 0.0f ) continue;
        const float k = ( fill > 0.0f ) ? shade / fill : 1.0f;
        float rgb[3];
        for( int ch = 0; ch < 3; ch++ ) {
          float f = cc[ch] * k;
          if( f > 255.0f ) f = 255.0f;
          rgb[ch] = ( f * fill + dk[ch] * dark ) / ( fill + dark );
        }
        const unsigned long argb = ( (unsigned long)( a * 255.0f + 0.5f ) << 24 )
          | ( (unsigned long)rgb[0] << 16 ) | ( (unsigned long)rgb[1] << 8 ) | (unsigned long)rgb[2];
        const int q = ( oy + ty ) * n + ox + tx;
        if( bpp == 4 ) ( (unsigned long*)px )[q] = argb;
        else           ( (unsigned short*)px )[q] = GenPack16( argb, fmt, 0.0f );
      }
    }
  }
  inline zCTexture* IconAtlas() {
    if( g_iconTex || g_iconTried ) return g_iconTex;
    g_iconTried = true;
    zTRnd_TextureFormat tfmt = zRND_TEX_FORMAT_ARGB_4444;
    int bpp = 0;
    if( !PickTexFormat( true, tfmt, bpp ) ) {
      if( zrenderer ) MM_LOG( "icons: no texture format with alpha, using fallback shapes" );
      return 0;
    }
    const int fmt = (int)tfmt, n = ICON_ATLAS;
    unsigned char* px = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n * bpp ) );
    if( !px ) return 0;
    memset( px, 0, n * n * bpp );
    for( int cell = 0; cell < ICON_CELLS; cell++ ) {
      int cx0, cy0, S;
      IconCell( cell, cx0, cy0, S );
      for( int slot = 0; slot < ICON_SLOTS; slot++ ) {
        if( slot >= ICON_COUNT && slot != ICON_PLAYER ) continue;
        int ox, oy, S2;
        IconSlotXY( cell, slot, ox, oy, S2 );
        AtlasBakeCell( px, n, bpp, fmt, ox, oy, S, IconShapeOf( slot ), AtlasColor( slot ), S >= 16, &IconHull, &IconHole );
      }
    }
    const int fromFiles = IconFilesBake( px, n, bpp, fmt );
    g_iconTex = CreateAndUploadTex( n, n, tfmt, zCOLOR( 128, 128, 128, 128 ), px, bpp );
    MiniMapCommon::MmFree( px );
    MM_LOG( zSTRING( g_iconTex ? "icons: atlas ready, format " : "icons: atlas failed, format " ) + zSTRING( fmt ) + ( fromFiles ? zSTRING( ", icons from files " ) + zSTRING( fromFiles ) : zSTRING( "" ) ) );
    return g_iconTex;
  }
  inline float IconScaleK() {
    return IconSizeK( g_sizeNpc );
  }
  inline float MarkSizePx() {
    const float ppm = g_ppuX * 100.0f;
    float s = 7.5f + 2.0f * ( ( ppm > 0.001f ) ? logf( ppm ) * 1.4427f : -10.0f );
    const float top = g_iconZoom ? 36.0f : 16.0f;
    if( g_iconZoom && s > 16.0f ) s = 16.0f + ( s - 16.0f ) * 2.0f;
    if( !g_full ) {
      const float bw = g_clipX1 - g_clipX0;
      const float bh = g_clipY1 - g_clipY0;
      const float cap = ( ( bw < bh ) ? bw : bh ) * 0.07f;
      if( s > cap ) s = cap;
    }
    if( s < 6.0f ) s = 6.0f; else if( s > top ) s = top;
    return s;
  }
  inline float IconBasePx() {
    const float s = MarkSizePx();
    float lo = 12.0f * UiScale();
    if( !g_full ) {
      const float bw = g_clipX1 - g_clipX0, bh = g_clipY1 - g_clipY0;
      const float cap = ( ( bw < bh ) ? bw : bh ) * 0.07f;
      if( lo > cap ) lo = cap;
    }
    return ( s < lo ) ? lo : s;
  }
  static const float MARK_SET_K = 1.25f / 0.85f;
  inline float NpcSizePx()   { return IconBasePx() * IconSizeK( g_sizeNpc ); }
  inline float ItemSizePx()  { return IconBasePx() * IconSizeK( g_sizeItem ); }
  inline float MarkSizePxK() { return IconBasePx() * IconSizeK( g_sizeMark ) * ( g_iconSet ? MARK_SET_K : 1.0f ); }
  inline void IconUV( int icon, float s, float& u0, float& v0, float& u1, float& v1 ) {
    int cx0, cy0, S;
    IconSlotXY( IconCellFor( s ), icon, cx0, cy0, S );
    const float inv = 1.0f / (float)ICON_ATLAS;
    u0 = (float)cx0 * inv;       v0 = (float)cy0 * inv;
    u1 = u0 + (float)S * inv;    v1 = v0 + (float)S * inv;
  }
  inline bool DrawClippedTile( zCTexture* tex, float px, float py, float s,
                               float u0, float v0, float u1, float v1,
                               const zCOLOR& col = zCOLOR( 255, 255, 255, 255 ) ) {
    if( !tex || !zrenderer || s <= 0.5f ) return false;
    const float h = s * 0.5f;
    float x0 = px - h, y0 = py - h, x1 = px + h, y1 = py + h;
    if( x1 <= g_clipX0 || x0 >= g_clipX1 || y1 <= g_clipY0 || y0 >= g_clipY1 ) return true;
    const float du = ( u1 - u0 ) / s, dv = ( v1 - v0 ) / s;
    if( x0 < g_clipX0 ) { u0 += ( g_clipX0 - x0 ) * du; x0 = g_clipX0; }
    if( y0 < g_clipY0 ) { v0 += ( g_clipY0 - y0 ) * dv; y0 = g_clipY0; }
    if( x1 > g_clipX1 ) { u1 -= ( x1 - g_clipX1 ) * du; x1 = g_clipX1; }
    if( y1 > g_clipY1 ) { v1 -= ( y1 - g_clipY1 ) * dv; y1 = g_clipY1; }
    tex->CacheIn( -1.0f );
    TAlphaBlend blend;
    TIconFilter filt;
    zrenderer->DrawTile( tex, zVEC2( x0, y0 ), zVEC2( x1, y1 ), MINIMAP_Z, zVEC2( u0, v0 ), zVEC2( u1, v1 ), col );
    return true;
  }
  inline bool DrawIcon( int icon, float px, float py, float s ) {
    zCTexture* const tex = IconAtlas();
    if( !tex ) return false;
    float u0, v0, u1, v1;
    IconUV( icon, s, u0, v0, u1, v1 );
    return DrawClippedTile( tex, px, py, s, u0, v0, u1, v1 );
  }
  inline bool DrawIconRot( int icon, float px, float py, float s, float fx, float fy ) {
    zCTexture* const tex = IconAtlas();
    if( !tex || !zrenderer ) return false;
    float u0, v0, u1, v1;
    IconUV( icon, s, u0, v0, u1, v1 );
    const float h = s * 0.5f;
    const float rx = -fy * h, ry = fx * h, ux = fx * h, uy = fy * h;
    const float qx[4] = { px - rx + ux, px + rx + ux, px + rx - ux, px - rx - ux };
    const float qy[4] = { py - ry + uy, py + ry + uy, py + ry - uy, py - ry - uy };
    const float qu[4] = { u0, u1, u1, u0 };
    const float qv[4] = { v0, v0, v1, v1 };
    zTRndSimpleVertex vert[4];
    for( int k = 0; k < 4; k++ ) {
      vert[k].pos   = zVEC2( qx[k], qy[k] );
      vert[k].z     = MINIMAP_Z;
      vert[k].uv    = zVEC2( qu[k], qv[k] );
      vert[k].color = zCOLOR( 255, 255, 255, 255 );
    }
    tex->CacheIn( -1.0f );
    TAlphaBlend blend;
    TIconFilter filt;
    zrenderer->DrawPolySimple( tex, vert, 4 );
    return true;
  }
  inline void NpcMarkOld( int kind, float px, float py, float r, const zCOLOR& c,
                          bool cheap );
  inline bool NpcFileMark( int i, int grp, float px, float py, float s );
  inline void NpcMark( int grp, int kind, float px, float py, float s,
                       bool cheap, int npcIdx = -1 ) {
    if( g_npcIcons && npcIdx >= 0 && NpcFileMark( npcIdx, grp, px, py, s ) ) return;
    const int icon = ( grp == NPC_GRP_TRADER ) ? ICON_TRADER : ( grp == 0 ) ? 0 : ( grp == 2 ) ? 2
                   : ( kind != 0 ) ? ICON_ANIMAL : 1;
    if( DrawIcon( icon, px, py, g_iconSet ? s * 1.25f : s ) ) return;
    const zCOLOR c = ( grp == NPC_GRP_TRADER ) ? ColTrader() : ColNpcGroup( grp );
    const float  r = s * 0.35f;
    NpcMarkOld( kind, px, py, r, c, cheap );
  }
  inline void NpcMarkOld( int kind, float px, float py, float r, const zCOLOR& c,
                          bool cheap ) {
    if( !cheap ) {
      DrawMark( kind, px, py, r, c );
      return;
    }
    float x0 = px - r - 1.0f, y0 = py - r - 1.0f;
    float x1 = px + r + 1.0f, y1 = py + r + 1.0f;
    if( x0 < g_clipX0 ) x0 = g_clipX0;
    if( y0 < g_clipY0 ) y0 = g_clipY0;
    if( x1 > g_clipX1 ) x1 = g_clipX1;
    if( y1 > g_clipY1 ) y1 = g_clipY1;
    if( x1 <= x0 || y1 <= y0 ) return;
    TexRect( x0, y0, x1 - x0, y1 - y0, zCOLOR( 18, 16, 15, 255 ) );
    x0 += 1.0f; y0 += 1.0f; x1 -= 1.0f; y1 -= 1.0f;
    if( x1 > x0 && y1 > y0 )
      TexRect( x0, y0, x1 - x0, y1 - y0, c );
  }
  struct TSeen {
    const void* p;
    DWORD       next;
    DWORD       seenAt;
  };
  static const int SEEN_SIZE = 4096;
  static TSeen     g_seen[SEEN_SIZE];
  static int       g_seenUsed  = 0;
  static oCWorld*  g_seenWorld = 0;
  inline void SeenClear() {
    memset( g_seen, 0, sizeof( g_seen ) );
    g_seenUsed  = 0;
    g_seenWorld = 0;
  }
  inline TSeen* SeenSlot( const void* p );
  inline void SeenPurge() {
    static TSeen keep[SEEN_SIZE];
    const DWORD now = GetTickCount();
    int n = 0;
    for( int i = 0; i < SEEN_SIZE; i++ ) {
      const TSeen& s = g_seen[i];
      if( !s.p ) continue;
      const bool fresh = ( s.seenAt && (DWORD)( now - s.seenAt ) < 10000 )
                      || ( s.next && (long)( s.next - now ) > -10000 );
      if( fresh )
        keep[n++] = s;
    }
    oCWorld* const w = g_seenWorld;
    SeenClear();
    g_seenWorld = w;
    for( int i = 0; i < n; i++ ) {
      TSeen* d = SeenSlot( keep[i].p );
      if( !d ) break;
      d->next   = keep[i].next;
      d->seenAt = keep[i].seenAt;
    }
  }
  inline TSeen* SeenSlot( const void* p ) {
    const unsigned long h = ( (unsigned long)p >> 3 ) * 2654435761UL;
    for( int k = 0; k < SEEN_SIZE; k++ ) {
      TSeen& s = g_seen[( h + k ) & ( SEEN_SIZE - 1 )];
      if( s.p == p ) return &s;
      if( !s.p ) {
        if( g_seenUsed >= SEEN_SIZE * 3 / 4 ) return 0;
        s.p      = p;
        s.next   = 0;
        s.seenAt = 0;
        g_seenUsed++;
        return &s;
      }
    }
    return 0;
  }
  static float g_sightLastGood = 0.0f;
  inline float SightRange( oCWorld* world, int fallback ) {
    float r   = (float)g_sightRange;
    int   src = 0;
    if( r <= 0.0f ) {
      const float eng = world ? world->vobFarClipZ * world->vobFarClipZScalability : 0.0f;
      if( eng > 0.0f && eng <= 1.0e6f ) {
        r = eng;
        g_sightLastGood = eng;
        src = 1;
      } else if( g_sightLastGood > 0.0f ) {
        r = g_sightLastGood;
        src = 2;
      } else {
        r = (float)fallback;
        src = 3;
      }
    }
    if( r < 1000.0f ) r = 1000.0f; else if( r > 30000.0f ) r = 30000.0f;
    static int loggedKey = -1, loggedSrc = -1;
    const int key = ( src == 3 ) ? -2 : (int)r;
    if( key != loggedKey || src != loggedSrc ) {
      loggedKey = key;
      loggedSrc = src;
      const char* srcName = ( src == 0 ) ? "ini SightRange"
                          : ( src == 1 ) ? "game"
                          : ( src == 2 ) ? "last valid game value (game value invalid, e.g. DX11 renderer)"
                          : "fallback NpcRange/ItemRange (game value invalid)";
      MM_LOG( zSTRING( "sight range: " ) + zSTRING( (int)r ) + " cm, source: " + zSTRING( srcName ) + " (game view distance " + zSTRING( world ? (int)world->vobFarClipZ : 0 ) + " x scale " + zSTRING( world ? (int)( world->vobFarClipZScalability * 100.0f ) : 0 ) + "%)" );
    }
    return r;
  }
  inline bool InSight( oCWorld* world, zCVob* vob, float d2, bool item ) {
    const float nr = (float)g_sightNear;
    if( d2 <= nr * nr ) return true;
    if( !player || !vob ) return false;
    if( g_seenWorld != world ) {
      SeenClear();
      g_seenWorld = world;
    }
    TSeen* s = SeenSlot( vob );
    if( !s ) {
      SeenPurge();
      s = SeenSlot( vob );
      if( !s ) {
        SeenClear();
        g_seenWorld = world;
        s = SeenSlot( vob );
      }
      if( !s ) return false;
    }
    const DWORD now = GetTickCount();
    if( ( !s->next || (long)( now - s->next ) >= 0 ) && g_rayBudget > 0 ) {
      g_rayBudget--;
      LARGE_INTEGER t0, t1;
      QueryPerformanceCounter( &t0 );
      int freeLine;
      if( item ) {
        const zTBBox3D& b = vob->bbox3D;
        zVEC3 c( ( b.mins[0] + b.maxs[0] ) * 0.5f,
                 ( b.mins[1] + b.maxs[1] ) * 0.5f + 30.0f,
                 ( b.mins[2] + b.maxs[2] ) * 0.5f );
        freeLine = player->FreeLineOfSight( c, vob );
      }
      else
        freeLine = player->FreeLineOfSight( vob );
      QueryPerformanceCounter( &t1 );
      static LONGLONG sumTicks = 0;
      static int      rays     = 0;
      static DWORD    logAt    = 0;
      sumTicks += t1.QuadPart - t0.QuadPart;
      rays++;
      if( (long)( now - logAt ) >= 0 ) {
        LARGE_INTEGER f;
        QueryPerformanceFrequency( &f );
        if( logAt && rays > 0 && f.QuadPart > 0 )
          MM_LOG( zSTRING( "line of sight: " ) + zSTRING( rays ) + " rays / 10 s, avg " + zSTRING( (int)( sumTicks * 1000000 / f.QuadPart / rays ) ) + " us" );
        logAt    = now + 10000;
        sumTicks = 0;
        rays     = 0;
      }
      if( freeLine ) s->seenAt = now ? now : 1;
      s->next = now + 250 + ( ( (unsigned long)vob >> 4 ) & 127 );
      if( !s->next ) s->next = 1;
    }
    return s->seenAt && (DWORD)( now - s->seenAt ) < 1200;
  }
  inline void DrawNpcs() {
    if( !ogame || !player ) return;
    oCWorld* world = ogame->GetGameWorld();
    if( !world ) return;
    if( g_npcCache && world != g_npcWorld )
      NpcCacheFree( false );
    const DWORD now = GetTickCount();
    if( !g_npcCache || (long)( now - g_npcNext ) >= 0 ) {
      g_npcNext = now + NPC_CACHE_MS;
      NpcCacheRefresh( world );
    }
    if( !g_npcCache || g_npcCount <= 0 ) return;
    const bool filt = SearchFilterOn() && g_npcMatch && g_npcMatchN == g_npcCount;
    const bool corpses = SearchFilterOn() && g_corpseMatch && g_corpseMatchN == g_npcCount;
    const bool traders = g_showTraders && g_npcFlag && g_npcFlagN == g_npcCount;
    const bool loot    = g_showCorpses && g_npcFlag && g_npcFlagN == g_npcCount;
    if( !filt && !corpses && !g_showNpc && !traders && !loot ) return;
    const bool  sight = ( g_showNpc == SHOW_SIGHT ) && g_sightRays && !filt;
    const float range = sight ? SightRange( world, g_npcRange ) : (float)g_npcRange;
    const float r2    = range * range;
    const float size = NpcSizePx();
    const int  limit = ( g_showNpc >= 2 || filt ) ? 8000 : g_npcLimit;
    static bool s_cheapNpc = false;
    if( g_npcLastShown > 250 )      s_cheapNpc = true;
    else if( g_npcLastShown < 200 ) s_cheapNpc = false;
    const bool cheap = s_cheapNpc;
    float ins = oCSpawnManager::GetInsertRange();
    if( !( ins > 500.0f && ins < 50000.0f ) ) ins = 4500.0f;
    const float stale2 = ins * ins;
    int drawn = 0;
    const zVEC3 pp = player->GetPositionWorld();
    g_rayBudget = g_rayBudgetNpc;
    static int s_start = 0;
    if( s_start >= g_npcCount ) s_start = 0;
    const int start = sight ? s_start : 0;
    if( sight )
      s_start += ( g_sightRaysPerFrame > 1 ) ? g_sightRaysPerFrame : 1;
    for( int k = 0; k < g_npcCount && drawn < limit; k++ ) {
      const int i = ( start + k ) % g_npcCount;
      oCNpc* const npc = g_npcCache[i];
      if( !npc || npc == player ) continue;
      if( NpcFlagAt( i, NPCF_GONE ) ) continue;
      if( npc->attribute[NPC_ATR_HITPOINTS] <= 0 ) {
        const bool chit  = corpses && g_corpseMatch[i];
        const bool cshow = !corpses && loot && NpcFlagAt( i, NPCF_LOOT );
        if( ( chit || cshow ) && npc->homeWorld == (zCWorld*)world ) {
          const zVEC3 cp = npc->GetPositionWorld();
          const float cdx = cp[0] - pp[0], cdz = cp[2] - pp[2];
          if( chit || g_showCorpses >= 2
              || cdx * cdx + cdz * cdz <= (float)g_npcRange * (float)g_npcRange ) {
            float cx, cy;
            WorldToPx( cp[0], cp[2], cx, cy );
            if( cx >= g_clipX0 && cx <= g_clipX1 && cy >= g_clipY0 && cy <= g_clipY1
                && MarkerVisible( cx, cy ) ) {
              const float csz = MarkSizePxK() * 0.85f;
              CorpseMark( cx, cy, csz );
              if( chit )
                SearchRing( cx, cy, csz );
              HoverConsider( npc, cx, cy, size, true );
              if( chit )
                drawn++;
            }
          }
        }
        continue;
      }
      const bool trader  = traders && NpcFlagAt( i, NPCF_TRADER );
      if( !filt && !g_showNpc && !trader ) continue;
      zCWorld* const home = npc->homeWorld;
      if( home && home != (zCWorld*)world ) continue;
      const bool hit = filt && g_npcMatch[i];
      if( filt && !hit && ( !g_searchOthers || !g_showNpc ) ) continue;
      if( g_showNpcKind && !hit ) {
        const bool mon = npc->IsMonster() != 0 || npc->IsOrc() != 0;
        if( ( g_showNpcKind == 1 ) == mon && NpcGroup( npc ) != 2 && !trader ) continue;
      }
      const bool whole0 = hit || g_showNpc >= 2;
      const bool tradeOnly = trader && !hit && !filt
                          && ( !g_showNpc || ( g_showTraders >= 2 && !whole0 ) );
      const bool whole = hit || g_showNpc >= 2 || ( tradeOnly && g_showTraders >= 2 );
      zVEC3 p = npc->GetPositionWorld();
      bool fromSpawn = false;
      if( !home && whole ) {
        if( const NpcSpawnPos* sp = NpcSpawnFind( npc ) ) {
          p[0] = sp->x;
          p[2] = sp->z;
          fromSpawn = true;
        }
        else if( NpcWpAt( i, p[0], p[2] ) )
          fromSpawn = true;
      }
      const float dx = p[0] - pp[0];
      const float dz = p[2] - pp[2];
      const float d2 = dx * dx + dz * dz;
      if( !home && !fromSpawn && ( !whole || d2 < stale2 ) ) continue;
      if( !whole && d2 > ( tradeOnly ? (float)g_npcRange * (float)g_npcRange : r2 ) ) continue;
      float mx, my;
      WorldToPx( p[0], p[2], mx, my );
      if( mx < g_clipX0 || mx > g_clipX1 || my < g_clipY0 || my > g_clipY1 ) continue;
      if( !MarkerVisible( mx, my ) ) continue;
      if( sight && !whole && !InSight( world, npc, d2, false ) ) continue;
      if( filt && !hit ) {
        SearchDimMark( mx, my, size );
        drawn++;
        continue;
      }
      int grp = NpcGroup( npc );
      if( trader && grp != 0 ) grp = NPC_GRP_TRADER;
      int kind = 0;
      if( !npc->IsHuman() ) kind = npc->IsMonster() ? 2 : 1;
      NpcMark( grp, kind, mx, my, size, cheap, i );
      if( hit )
        SearchRing( mx, my, size );
      HoverConsider( npc, mx, my, size, true );
      drawn++;
    }
    g_npcLastShown = drawn;
    g_rayBudgetNpc = g_rayBudget;
  }
  inline zCOLOR ColMarkIcon( int icon ) {
    switch( icon ) {
      case 0:  return zCOLOR( 214,  84,  70, 255 );
      case 1:  return zCOLOR( 226, 184,  80, 255 );
      case 2:  return zCOLOR( 232, 148,  56, 255 );
      case 3:  return zCOLOR( 240, 226, 140, 255 );
      case 5: case 6: case 7: case 8: case 9:
        return IconColor( icon );
      default: return zCOLOR( 176, 200, 226, 255 );
    }
  }
  inline void DrawUserMarkShape( int icon, float px, float py, float r,
                                 const zCOLOR& c ) {
    switch( icon ) {
      case 1:
        for( float k = -r * 0.7f; k <= r * 0.7f; k += 1.0f )
          MarkBar( px - r, py + k, px + r, c );
        break;
      case 2:
        for( float k = -r; k <= r; k += 1.0f ) {
          const float w = ( k + r ) * 0.5f;
          MarkBar( px - w, py + k, px + w, c );
        }
        break;
      case 3:
        for( float k = -r; k <= r; k += 1.0f ) {
          const float d = ( k < 0.0f ) ? -k : k;
          const float w = ( r - d ) * 0.55f;
          MarkBar( px - w, py + k, px + w, c );
        }
        MarkBar( px - r, py, px + r, c );
        break;
      case 0:
        MarkDisc( px, py, r, c );
        MarkBar( px - r * 1.5f, py, px + r * 1.5f, c );
        for( float t3 = -r * 1.5f; t3 <= r * 1.5f; t3 += 1.0f )
          MarkBar( px - 0.5f, py + t3, px + 0.5f, c );
        break;
      default:
        MarkDisc( px, py, r * 0.8f, c );
        break;
    }
  }
  inline void FillTriTex( float ax, float ay, float bx, float by,
                          float cx, float cy, const zCOLOR& col );
  inline zCOLOR ColMarkDark() { return zCOLOR( 16, 14, 18, 235 ); }
  inline zCOLOR ColMarkPane() { return zCOLOR( 18, 16, 20, 215 ); }
  inline zCOLOR ColMarkText() { return zCOLOR( 202, 196, 184 ); }
  inline zCOLOR ColMarkLight(){ return zCOLOR( 240, 232, 210, 255 ); }
  static const unsigned char kMarkRgb[10][3] = {
    { 208,  84,  76 },
    { 214, 178, 106 },
    { 214, 120,  60 },
    { 236, 224, 140 },
    { 132, 176, 206 },
    { 204, 208, 218 },
    { 190, 138,  92 },
    { 160, 150, 136 },
    { 112, 188,  92 },
    { 180, 138, 224 }
  };
  inline zCOLOR IconColor( int icon ) {
    if( icon < 0 || icon > 9 ) icon = 0;
    return zCOLOR( kMarkRgb[icon][0], kMarkRgb[icon][1], kMarkRgb[icon][2], 255 );
  }
  inline int IconTx( int icon ) {
    switch( icon ) {
      case 1:  return TX_ICON_TREASURE;
      case 2:  return TX_ICON_WARNING;
      case 3:  return TX_ICON_STAR;
      case 4:  return TX_ICON_POINT;
      case 5:  return TX_ICON_FIGHT;
      case 6:  return TX_ICON_HOUSE;
      case 7:  return TX_ICON_CAVE;
      case 8:  return TX_ICON_HERB;
      case 9:  return TX_ICON_QUEST;
      default: return TX_ICON_TARGET;
    }
  }
  static char g_markName[10][48];
  static char g_markNameFold[10][48];
  static bool g_markNamesRead = false;
  inline void MarkNamesLoad();
  inline const char* IconName( int icon ) {
    MarkNamesLoad();
    if( icon >= 0 && icon < 10 && g_markName[icon][0] ) return g_textFontPL == 0 ? g_markNameFold[icon] : g_markName[icon];
    return T( IconTx( icon ) );
  }
  inline const char* IconNameEn( int icon ) {
    return TEXT[IconTx( icon )][LANG_EN];
  }
  static zCTexture* g_markTex   = 0;
  static bool       g_markTried = false;
  inline void MarkAtlasFree( bool release ) {
    if( g_markTex && release )
      g_markTex->Release();
    g_markTex   = 0;
    g_hovVob    = 0;
    g_markTried = false;
    g_markNamesRead = false;
  }
  static const char* const MARKS_DIR  = "MiniMap\\icons\\marks";
  static const char* const MARKS_LIST = "MiniMap\\icons\\marks\\marks.txt";
  static const char* const MARKS_SECTIONS = "marks names";
  static int g_markFromFile = 0;
  static int g_markNamesN   = 0;
  inline void MarkFileMsg( void*, int, int line, const char* m ) {
    MM_LOG( zSTRING( "marks: icons\\marks\\marks.txt line " ) + zSTRING( line ) + ": " + zSTRING( m ) );
  }
  inline void MarkNameUtf8( char* s, int cap ) {
    bool high = false;
    for( int i = 0; s[i]; i++ )
      if( (unsigned char)s[i] >= 0x80 ) { high = true; break; }
    if( !high ) return;
    wchar_t w[64];
    const int n = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, s, -1, w, 64 );
    if( n <= 0 ) return;
    char a[64];
    const int m = WideCharToMultiByte( 1250, 0, w, -1, a, (int)sizeof( a ), "?", 0 );
    if( m > 0 )
      MiniMapCfg::Copy( s, cap, a );
  }
  inline void MarkNamesLoad() {
    if( g_markNamesRead ) return;
    g_markNamesRead = true;
    g_markNamesN = 0;
    for( int i = 0; i < 10; i++ )
      g_markName[i][0] = g_markNameFold[i][0] = 0;
    if( !g_modFiles ) return;
    int len = 0;
    unsigned char* txt = CfgReadFile( MARKS_LIST, len, 256 * 1024 );
    if( !txt ) return;
    static MiniMapCfg::TCfg C;
    MiniMapCfg::Init( C, MARKS_SECTIONS );
    MiniMapCfg::Parse( C, (const char*)txt, len, 0, 0, 0 );
    MiniMapCommon::MmFree( txt );
    const int sec = MiniMapCfg::SecFind( C, "names" );
    for( int i = MiniMapCfg::Next( C, sec, -1 ); i >= 0; i = MiniMapCfg::Next( C, sec, i ) ) {
      const MiniMapCfg::TItem& t = C.e[i];
      int nr = 0;
      if( !MiniMapCfg::Int( t.key, 1, MARK_ICONS, nr ) ) {
        MarkFileMsg( 0, 0, t.line, "[names]: unknown marker number (use 1 to 10) - ignored" );
        continue;
      }
      if( !t.val[0] || !strcmp( t.val, "yes" ) ) {
        MarkFileMsg( 0, 0, t.line, "[names]: no name after = - built-in name" );
        continue;
      }
      if( !g_markName[nr - 1][0] )
        g_markNamesN++;
      MiniMapCfg::Copy( g_markName[nr - 1], 48, t.val );
      MarkNameUtf8( g_markName[nr - 1], 48 );
      MiniMapCfg::Copy( g_markNameFold[nr - 1], 48, g_markName[nr - 1] );
      MenuFoldPL( g_markNameFold[nr - 1] );
    }
    MiniMapCfg::Free( C );
    if( g_markNamesN )
      MM_LOG( zSTRING( "marks: names from icons\\marks\\marks.txt " ) + zSTRING( g_markNamesN ) + "/10" );
  }
  inline int MarkFilesBake( unsigned char* px, int n, int bpp, int fmt ) {
    g_markFromFile = 0;
    if( !px || n < MARK_ATLAS || ( bpp != 4 && bpp != 2 ) || !g_modFiles ) return 0;
    IconPackInstall();
    int len = 0;
    unsigned char* txt = CfgReadFile( MARKS_LIST, len, 256 * 1024 );
    if( !txt ) return 0;
    static MiniMapCfg::TCfg C;
    MiniMapCfg::Init( C, MARKS_SECTIONS );
    MiniMapCfg::Parse( C, (const char*)txt, len, 0, MarkFileMsg, 0 );
    MiniMapCommon::MmFree( txt );
    const int sec = MiniMapCfg::SecFind( C, "marks" );
    int done = 0;
    for( int i = MiniMapCfg::Next( C, sec, -1 ); i >= 0; i = MiniMapCfg::Next( C, sec, i ) ) {
      const MiniMapCfg::TItem& t = C.e[i];
      int nr = 0;
      if( !MiniMapCfg::Int( t.key, 1, MARK_ICONS, nr ) ) {
        MarkFileMsg( 0, 0, t.line, "unknown marker number (use 1 to 10) - ignored" );
        continue;
      }
      const int icon = nr - 1;
      char file[MAX_PATH], path[MAX_PATH];
      unsigned flags = 0;
      unsigned char rgb[3] = { 255, 255, 255 };
      const char* err = 0;
      if( !MiniMapCfg::ParseIconVal( t.val, file, MAX_PATH, flags, &err, rgb ) ) {
        MarkFileMsg( 0, 0, t.line, err ? err : "bad value" );
        continue;
      }
      if( !FilePath( path, MAX_PATH, MARKS_DIR, file ) ) continue;
      int fl = 0;
      unsigned char* data = CfgReadFile( path, fl, MiniMapImage::IMG_MAX_BYTES );
      if( !data ) {
        MarkFileMsg( 0, 0, t.line, fl == -2 ? "picture is larger than 8 MB - built-in picture"
                                            : "picture not found in MiniMap\\icons\\marks - built-in picture" );
        continue;
      }
      unsigned char* rgba = 0;
      int w = 0, h = 0;
      const char* derr = MiniMapImage::Decode( data, fl, rgba, w, h );
      MiniMapCommon::MmFree( data );
      if( derr ) {
        MarkFileMsg( 0, 0, t.line, derr );
        continue;
      }
      MiniMapImage::TSrc s;
      if( !MiniMapImage::Prep( rgba, w, h, s ) ) {
        MarkFileMsg( 0, 0, t.line, "picture is fully transparent - built-in picture" );
        MiniMapImage::Free( rgba );
        continue;
      }
      s.x0 = 0; s.y0 = 0; s.cw = w; s.ch = h;
      const bool grey = ( flags & ( MiniMapCfg::ICOV_TINT | MiniMapCfg::ICOV_COLOR ) ) != 0;
      unsigned bf = grey ? 0u : (unsigned)MiniMapImage::KEEPCOLORS;
      if( flags & MiniMapCfg::ICOV_NOOUTLINE ) bf |= MiniMapImage::NOOUTLINE;
      const bool own = ( flags & MiniMapCfg::ICOV_COLOR ) != 0;
      const int cr = own ? rgb[0] : kMarkRgb[icon][0];
      const int cg = own ? rgb[1] : kMarkRgb[icon][1];
      const int cb = own ? rgb[2] : kMarkRgb[icon][2];
      for( int cell = 0; cell < 3; cell++ ) {
        int ox, oy, S;
        MarkCell( cell, icon, ox, oy, S );
        BakeSrcCell( s, px, n, bpp, fmt, ox, oy, S, bf, grey, cr, cg, cb );
      }
      MiniMapImage::Free( rgba );
      done++;
    }
    MiniMapCfg::Free( C );
    g_markFromFile = done;
    MM_LOG( zSTRING( "marks: pictures from icons\\marks " ) + zSTRING( done ) + "/10" );
    return done;
  }
  inline zCTexture* MarkAtlas() {
    if( g_markTex || g_markTried ) return g_markTex;
    g_markTried = true;
    zTRnd_TextureFormat tfmt = zRND_TEX_FORMAT_ARGB_4444;
    int bpp = 0;
    if( !PickTexFormat( true, tfmt, bpp ) ) return 0;
    const int fmt = (int)tfmt, n = MARK_ATLAS;
    const DWORD tAtlas = GetTickCount();
    static unsigned char* s_px    = 0;
    static int            s_pxFmt = -1;
    const bool cached = ( s_px && s_pxFmt == fmt );
    if( !cached ) {
      MiniMapCommon::MmFree( s_px );
      s_px    = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n * bpp ) );
      s_pxFmt = s_px ? fmt : -1;
      if( !s_px ) return 0;
      memset( s_px, 0, n * n * bpp );
      for( int cell = 0; cell < 3; cell++ )
        for( int icon = 0; icon < MARK_ICONS; icon++ ) {
          int ox, oy, S;
          MarkCell( cell, icon, ox, oy, S );
          AtlasBakeCell( s_px, n, bpp, fmt, ox, oy, S, icon, IconColor( icon ), true, &MarkHull, &MarkHole );
        }
    }
    unsigned char* up = s_px;
    unsigned char* withFiles = MiniMapCommon::MmAlloc<unsigned char>( (size_t)( n * n * bpp ) );
    if( withFiles ) {
      memcpy( withFiles, s_px, n * n * bpp );
      if( MarkFilesBake( withFiles, n, bpp, fmt ) > 0 ) up = withFiles;
    }
    g_markTex = CreateAndUploadTex( n, n, tfmt, zCOLOR( 128, 128, 128, 128 ), up, bpp );
    MiniMapCommon::MmFree( withFiles );
    MM_LOG( zSTRING( g_markTex ? "marks: atlas ready, format " : "marks: atlas failed, format " ) + zSTRING( fmt ) + ( cached ? ", from memory, " : ", " ) + zSTRING( (int)( GetTickCount() - tAtlas ) ) + " ms" );
    return g_markTex;
  }
  inline bool DrawMarkAtlas( int icon, float px, float py, float s ) {
    if( icon < 0 || icon >= MARK_ICONS ) icon = 0;
    zCTexture* const tex = MarkAtlas();
    if( !tex ) return false;
    int cx0, cy0, S;
    MarkCell( ( s > 32.25f ) ? 0 : ( s > 16.25f ) ? 1 : 2, icon, cx0, cy0, S );
    const float inv = 1.0f / (float)MARK_ATLAS;
    const float u0 = (float)cx0 * inv, v0 = (float)cy0 * inv;
    return DrawClippedTile( tex, px, py, s, u0, v0, u0 + (float)S * inv, v0 + (float)S * inv );
  }
  inline float MarkPxFull() {
    const float m = (float)g_markSize / 16.0f;
    float s = MarkSizePx() * 1.25f * m;
    const float smax = 24.0f * m * IconScaleK();
    if( s > smax )  s = smax;
    if( s < 8.0f )  s = 8.0f;
    return s;
  }
  inline float MarkPxMini() {
    float s = MarkSizePx() * 1.25f * (float)g_markSize / 16.0f;
    if( s < 7.0f )  s = 7.0f;
    if( s > 32.0f * IconScaleK() ) s = 32.0f * IconScaleK();
    return s;
  }
  inline float CursorPx() {
    return (float)g_cursorSize * UiScale();
  }
  inline void QuadTex( float ax, float ay, float bx, float by,
                       float cx, float cy, float dx, float dy,
                       const zCOLOR& col ) {
    FillTriTex( ax, ay, bx, by, cx, cy, col );
    FillTriTex( ax, ay, cx, cy, dx, dy, col );
  }
  inline void BarTex( float x0, float y0, float x1, float y1,
                      float thick, const zCOLOR& col ) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    const float len = sqrtf( dx * dx + dy * dy );
    if( len < 0.001f ) return;
    dx /= len;
    dy /= len;
    const float nx = -dy * thick * 0.5f;
    const float ny =  dx * thick * 0.5f;
    QuadTex( x0 + nx, y0 + ny, x1 + nx, y1 + ny,
             x1 - nx, y1 - ny, x0 - nx, y0 - ny, col );
  }
  inline void FillCircleTex( float cx, float cy, float r, const zCOLOR& col ) {
    if( r < 0.5f ) return;
    for( float dy = -r; dy <= r; dy += 1.0f ) {
      const float k = r * r - dy * dy;
      if( k <= 0.0f ) continue;
      const float w = sqrtf( k );
      TexRect( cx - w, cy + dy, w * 2.0f, 1.0f, col );
    }
  }
  inline void StarTex( float cx, float cy, float r, const zCOLOR& col ) {
    float px[10], py[10];
    int   i;
    for( i = 0; i < 10; i++ ) {
      const float a  = -1.5707963f + (float)i * 0.62831853f;
      const float rr = ( i & 1 ) ? r * 0.42f : r;
      px[i] = cx + cosf( a ) * rr;
      py[i] = cy + sinf( a ) * rr;
    }
    for( i = 0; i < 10; i++ ) {
      const int j = ( i + 1 ) % 10;
      FillTriTex( cx, cy, px[i], py[i], px[j], py[j], col );
    }
  }
  inline void IconShape( float cx, float cy, float s, int icon,
                         const zCOLOR& col ) {
    const float h = s * 0.5f;
    switch( icon ) {
      case 1:
        TexRect( cx - h, cy - h * 0.15f, s, h * 1.15f, col );
        TexRect( cx - h, cy - h,         s, h * 0.75f, col );
        break;
      case 2:
        FillTriTex( cx, cy - h, cx + h, cy + h * 0.8f,
                    cx - h, cy + h * 0.8f, col );
        break;
      case 3:
        StarTex( cx, cy, h, col );
        break;
      case 5:
        BarTex( cx - h, cy - h, cx + h, cy + h, s * 0.2f, col );
        BarTex( cx + h, cy - h, cx - h, cy + h, s * 0.2f, col );
        break;
      case 6:
        FillTriTex( cx, cy - h, cx + h, cy - h * 0.1f, cx - h, cy - h * 0.1f, col );
        TexRect( cx - h * 0.7f, cy - h * 0.15f, h * 1.4f, h * 1.05f, col );
        break;
      case 7:
        FillTriTex( cx, cy - h, cx + h, cy + h * 0.8f, cx - h, cy + h * 0.8f, col );
        FillCircleTex( cx, cy + h * 0.1f, h * 0.6f, col );
        break;
      case 8:
        FillCircleTex( cx - h * 0.35f, cy, h * 0.42f, col );
        FillCircleTex( cx + h * 0.35f, cy, h * 0.42f, col );
        FillCircleTex( cx, cy - h * 0.45f, h * 0.42f, col );
        TexRect( cx - h * 0.1f, cy, h * 0.2f, h, col );
        break;
      case 9:
        FillCircleTex( cx, cy, h * 0.9f, col );
        break;
      case 4:
        FillCircleTex( cx, cy, h * 0.85f, col );
        break;
      default:
        FillCircleTex( cx, cy, h * 0.78f, col );
        BarTex( cx - h * 1.2f, cy, cx + h * 1.2f, cy, s * 0.17f, col );
        BarTex( cx, cy - h * 1.2f, cx, cy + h * 1.2f, s * 0.17f, col );
        break;
    }
  }
  inline void IconDetail( float cx, float cy, float s, int icon ) {
    const float  h    = s * 0.5f;
    const zCOLOR dark = ColMarkDark();
    switch( icon ) {
      case 1: {
        const float w = h * 0.34f;
        TexRect( cx - h, cy - h * 0.24f, s, h * 0.2f, dark );
        TexRect( cx - w * 0.5f, cy - h * 0.42f, w, h * 0.62f, dark );
        break;
      }
      case 2: {
        const float w = h * 0.2f;
        TexRect( cx - w * 0.5f, cy - h * 0.25f, w, h * 0.62f, dark );
        TexRect( cx - w * 0.5f, cy + h * 0.5f,  w, w,         dark );
        break;
      }
      case 6:
        TexRect( cx - h * 0.18f, cy + h * 0.3f, h * 0.36f, h * 0.6f, dark );
        break;
      case 7:
        FillCircleTex( cx, cy + h * 0.35f, h * 0.3f, dark );
        break;
      case 9: {
        const float w = h * 0.22f;
        TexRect( cx - w * 0.5f, cy - h * 0.5f, w, h * 0.55f, dark );
        TexRect( cx - w * 0.5f, cy + h * 0.25f, w, w, dark );
        break;
      }
      case 5:
      case 8:
      case 3:
        break;
      case 4:
        FillCircleTex( cx, cy, h * 0.4f, dark );
        break;
      default:
        FillCircleTex( cx, cy, h * 0.46f, dark );
        FillCircleTex( cx, cy, h * 0.2f,  IconColor( 0 ) );
        break;
    }
  }
  inline void DrawMarkIcon( float cx, float cy, float s, int icon ) {
    if( DrawMarkAtlas( icon, cx, cy, s * 1.12f ) ) return;
    IconShape( cx, cy, s + 3.0f, icon, ColMarkDark() );
    IconShape( cx, cy, s,        icon, IconColor( icon ) );
    IconDetail( cx, cy, s, icon );
  }
  inline void CycleIcon( int delta ) {
    g_markIcon += delta;
    while( g_markIcon < 0 )  g_markIcon += MARK_ICONS;
    while( g_markIcon >= MARK_ICONS ) g_markIcon -= MARK_ICONS;
    MM_LOG( zSTRING( "mark icon: " ) + zSTRING( IconNameEn( g_markIcon ) ) );
  }
  inline float TextWidthPx( const char* text ) {
    const float vx = ( g_vsx > 0.0001f ) ? g_vsx : 1.0f;
    return (float)TextWidth( text ) / vx;
  }
  inline float LineHPx() {
    if( !screen ) return 12.0f;
    const float vy = ( g_vsy > 0.0001f ) ? g_vsy : 1.0f;
    float h = (float)screen->FontY() / vy;
    if( h < 6.0f ) h = 6.0f; else if( h > 64.0f ) h = 64.0f;
    return h;
  }
  inline void MarkText( float x, float y, const char* s, const zCOLOR& col ) {
    if( !screen || !s || !s[0] ) return;
    screen->SetFontColor( col );
    screen->Print( VX( x ), VY( y ), zSTRING( s ) );
  }
  inline void Pane( float x, float y, float w, float h ) {
    TexRect( x, y, w, h, ColMarkPane() );
    TexRect( x, y, w, 1.0f, ColGold() );
    TexRect( x, y + h - 1.0f, w, 1.0f, ColGold() );
    TexRect( x, y, 1.0f, h, ColGold() );
    TexRect( x + w - 1.0f, y, 1.0f, h, ColGold() );
  }
  inline void Bubble( float x, float y, const char* text ) {
    if( !text || !text[0] ) return;
    const float pad = 4.0f;
    const float bw  = TextWidthPx( text ) + pad * 2.0f;
    const float bh  = LineHPx() + pad * 2.0f;
    if( x + bw > g_clipX1 ) x = g_clipX1 - bw;
    if( y + bh > g_clipY1 ) y = g_clipY1 - bh;
    if( x < g_clipX0 ) x = g_clipX0;
    if( y < g_clipY0 ) y = g_clipY0;
    Pane( x, y, bw, bh );
    MarkText( x + pad, y + pad, text, ColMarkText() );
  }
  static int   g_edit      = -1;
  static char  g_editBuf[48] = "";
  static int   g_editLen   = 0;
  static char  g_lastChar  = 0;
  static DWORD g_lastTime  = 0;
  static DWORD g_nextTime  = 0;
  inline bool RawMouseWritable( const void* p ) {
    if( !p ) return false;
    MEMORY_BASIC_INFORMATION mbi;
    if( !VirtualQuery( p, &mbi, sizeof( mbi ) ) || mbi.State != MEM_COMMIT ) return false;
    const DWORD rw = PAGE_READWRITE | PAGE_WRITECOPY
                   | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
    return ( mbi.Protect & rw ) != 0 && ( mbi.Protect & PAGE_GUARD ) == 0;
  }
  const unsigned int MINIMAP_KEYTABLE    = zSwitch( 0x0086CCC8u, 0x008B27C0u, 0x008C3020u, 0x008D1678u );
  const unsigned int MINIMAP_TOGGLETABLE = zSwitch( 0x0086CED0u, 0x008B29D4u, 0x008C3260u, 0x008D18B8u );
  inline bool EngineCodeHooked( const unsigned char* c ) { return MiniMapClash::CodeHooked( c ); }
  inline bool KeyTableWritable( const unsigned char* t ) {
    return t && RawMouseWritable( t ) && RawMouseWritable( t + 255 );
  }
  static unsigned char* g_keyTable = 0;
  static bool           g_keyTried = false;
  static unsigned char* g_toggleTable = 0;
  static bool           g_toggleTried = false;
  inline unsigned char* ResolveKeyTable( unsigned char*& tbl, bool& tried, unsigned int fnAddr,
                                         unsigned char b1, unsigned char b6, unsigned int fallback,
                                         const char* label, const char* fnName, const char* failMsg ) {
    if( tried ) return tbl;
    tried = true;
    const unsigned char* c = (const unsigned char*)fnAddr;
    const unsigned char pat[7] = { 0x8B, b1, 0x24, 0x04, 0x0F, 0xB6, b6 };
    if( memcmp( c, pat, 7 ) == 0 ) {
      tbl = *(unsigned char**)( c + 7 );
      MM_LOG( zSTRING( label ) + " found" );
    } else if( fallback && KeyTableWritable( (const unsigned char*)fallback ) ) {
      tbl = (unsigned char*)fallback;
      MM_LOG( zSTRING( label ) + ": " + ( EngineCodeHooked( c ) ? ( zSTRING( fnName ) + " hooked (G1Fixes?) - using known address" ) : zSTRING( "pattern not recognised - using known address" ) ) );
    } else
      MM_LOG( failMsg );
    return tbl;
  }
  inline unsigned char* ToggleTable() {
    return ResolveKeyTable( g_toggleTable, g_toggleTried, ADDR_KeyToggledW32, 0x4C, 0x81, MINIMAP_TOGGLETABLE,
                            "toggle key table", "KeyToggled", "toggle key table not recognised - TAB may reach the game" );
  }
  inline unsigned char* KeyTable() {
    return ResolveKeyTable( g_keyTable, g_keyTried, ADDR_KeyPressedW32, 0x44, 0x80, MINIMAP_KEYTABLE,
                            "key table", "KeyPressed", "key table not recognised - typing without key block" );
  }
  inline bool ConsoleOpen();
  inline bool DialogNow();
  inline bool TalkActive();
  inline void EatKeys() {
    unsigned char* pressed = KeyTable();
    unsigned char* toggled = ToggleTable();
    for( int i = 0; i < 256; i++ ) {
      if( i == KEY_ESCAPE ) continue;
      if( pressed ) pressed[i] = 0;
      if( toggled ) toggled[i] = 0;
    }
  }
  static unsigned char g_preChars[64];
  static int           g_preCount = 0;
  static int           g_preHead  = 0;
  static bool          g_charsExact = false;
  static unsigned char g_preTog[256];
  static bool          g_preTogSet = false;
  static unsigned char g_preKeys[256];
  static bool          g_preKeysSet = false;
  inline unsigned char FetchChar() {
    if( g_preHead < g_preCount ) return g_preChars[g_preHead++];
    g_preHead = g_preCount = 0;
    if( g_charsExact ) return 0;
    return zinput ? (unsigned char)zinput->GetChar() : 0;
  }
  inline bool GameHasFocus() {
    HWND f = GetForegroundWindow();
    if( !f ) return false;
    DWORD pid = 0;
    GetWindowThreadProcessId( f, &pid );
    return pid == GetCurrentProcessId();
  }
  inline bool DikHeld( int dik ) {
    if( dik <= 0 || dik >= 256 || !GameHasFocus() ) return false;
    const UINT sc = ( dik & 0x80 ) ? ( 0xE000u | (UINT)( dik & 0x7F ) ) : (UINT)dik;
    const UINT vk = MapVirtualKeyExA( sc, 3 , GetKeyboardLayout( 0 ) );
    return vk != 0 && ( GetAsyncKeyState( (int)vk ) & 0x8000 ) != 0;
  }
  inline bool BtnHeld( bool right ) {
    if( !GameHasFocus() ) return false;
    return ( GetAsyncKeyState( right ? VK_RBUTTON : VK_LBUTTON ) & 0x8000 ) != 0;
  }
  inline unsigned char DikToChar( int dik, const unsigned char* down ) {
    switch( dik ) {
      case KEY_RETURN: case KEY_NUMPADENTER: return 13;
      case KEY_BACK:   return 8;
      case KEY_ESCAPE: return 27;
      case KEY_DIVIDE: return '/';
      case KEY_MULTIPLY: return '*';
      case 0x4A: return '-';  case 0x4E: return '+';
      case 0x47: return '7';  case 0x48: return '8';  case 0x49: return '9';
      case 0x4B: return '4';  case 0x4C: return '5';  case 0x4D: return '6';
      case 0x4F: return '1';  case 0x50: return '2';  case 0x51: return '3';
      case 0x52: return '0';
    }
    if( dik <= 0 || dik >= 0x80 ) return 0;
    if( dik == 0x1D || dik == 0x2A || dik == 0x36 || dik == 0x38 || dik == 0x0F
        || dik == 0x3A || dik == 0x45 || dik == 0x46
        || ( dik >= 0x3B && dik <= 0x44 ) || dik == 0x57 || dik == 0x58 )
      return 0;
    HKL hkl = GetKeyboardLayout( 0 );
    const UINT vk = MapVirtualKeyExA( (UINT)dik, 3 , hkl );
    if( !vk ) return 0;
    BYTE ks[256];
    memset( ks, 0, sizeof( ks ) );
    const bool fg  = GameHasFocus();
    const bool shL = down[0x2A] || ( fg && ( GetAsyncKeyState( VK_LSHIFT ) & 0x8000 ) );
    const bool shR = down[0x36] || ( fg && ( GetAsyncKeyState( VK_RSHIFT ) & 0x8000 ) );
    if( shL || shR ) {
      ks[VK_SHIFT] = 0x80;
      ks[shL ? VK_LSHIFT : VK_RSHIFT] = 0x80;
    }
    if( down[0xB8] || ( fg && ( GetAsyncKeyState( VK_RMENU ) & 0x8000 ) ) ) {
      ks[VK_CONTROL] = ks[VK_LCONTROL] = 0x80;
      ks[VK_MENU]    = ks[VK_RMENU]    = 0x80;
    }
    if( GetKeyState( VK_CAPITAL ) & 1 ) ks[VK_CAPITAL] = 0x01;
    WCHAR w[4] = { 0, 0, 0, 0 };
    const int n = ToUnicodeEx( vk, (UINT)dik, ks, w, 4, 4, hkl );
    if( n != 1 || w[0] < 32 ) return 0;
    char out = 0;
    const UINT cp = 1250;
    BOOL usedDef = FALSE;
    if( WideCharToMultiByte( cp, 0, w, 1, &out, 1, 0, &usedDef ) != 1 || usedDef ) return 0;
    return (unsigned char)out;
  }
  inline void PreCharPush( unsigned char c ) {
    if( c && g_preCount < 64 )
      g_preChars[g_preCount++] = c;
  }
  inline bool SearchCharOk( unsigned char c ) {
    return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' )
        || c == ' ' || c == '-' || c == '\'' || c == '.' || c >= 0x80;
  }
  inline void FullZoomBy( int step );
  inline bool SearchTyping();
  inline void PreKeyPush( int k ) {
    const unsigned char c = DikToChar( k, g_preKeys );
    if( SearchTyping() && !( c && ( c < 32 || SearchCharOk( c ) ) ) ) {
      if( k == g_keyZoomIn )  { FullZoomBy( 1 );  return; }
      if( k == g_keyZoomOut ) { FullZoomBy( -1 ); return; }
      if( k == g_keySearch )  { PreCharPush( 13 ); return; }
    }
    PreCharPush( c );
  }
  inline void FetchCharsEarly() {
    if( g_preHead >= g_preCount ) g_preHead = g_preCount = 0;
    if( !g_preTogSet || !g_preKeysSet ) {
      g_charsExact = false;
      int zeros = 0;
      for( int guard = 0; guard < 32 && g_preCount < 64; guard++ ) {
        const unsigned char c = (unsigned char)zinput->GetChar();
        if( c == 0 ) {
          if( ++zeros >= 3 ) break;
          continue;
        }
        zeros = 0;
        g_preChars[g_preCount++] = c;
      }
      return;
    }
    g_charsExact = true;
    int q[32];
    int qn = 0;
    unsigned char inQ[256];
    memset( inQ, 0, sizeof( inQ ) );
    for( int guard = 0; guard < 32; guard++ ) {
      const int k = (int)zinput->GetKey( 0, 0 );
      if( k <= 0 ) break;
      if( k < 256 ) {
        q[qn++] = k;
        inQ[k] = 1;
      }
    }
    static bool  bsWas  = false;
    static DWORD bsNext = 0;
    const DWORD now = GetTickCount();
    const bool bsDown = GameHasFocus() && ( GetAsyncKeyState( VK_BACK ) & 0x8000 ) != 0;
    bool bsRep = false;
    if( bsDown && !bsWas )
      bsNext = now + 400;
    else if( bsDown && (LONG)( now - bsNext ) >= 0 ) {
      bsRep  = true;
      bsNext = now + 60;
    }
    bsWas = bsDown;
    for( int k = 1; k < 256; k++ )
      if( g_preTog[k] && !inQ[k] && k != KEY_BACK )
        PreKeyPush( k );
    bool bsDone = false;
    if( g_preTog[KEY_BACK] && !inQ[KEY_BACK] ) {
      PreCharPush( 8 );
      bsDone = true;
    }
    for( int i = 0; i < qn; i++ ) {
      if( q[i] == KEY_BACK ) bsDone = true;
      PreKeyPush( q[i] );
    }
    if( bsRep && !bsDone )
      PreCharPush( 8 );
  }
  inline void EndEdit( bool keep ) {
    if( g_edit >= 0 && g_edit < g_ownCount ) {
      if( keep )
        StrCopy( g_ownMarks[g_edit].note, g_editBuf, 48 );
      SaveOwnMarks();
    }
    g_edit     = -1;
    g_editLen  = 0;
    g_editBuf[0] = 0;
  }
  inline void BeginEdit( int idx ) {
    g_edit = idx;
    StrCopy( g_editBuf, ( idx >= 0 ) ? g_ownMarks[idx].note : "", 48 );
    g_editLen = (int)strlen( g_editBuf );
  }
  inline void ReadNote() {
    if( g_edit < 0 || !zinput ) return;
    if( ConsoleOpen() ) return;
    EatKeys();
    const DWORD REPEAT_FIRST = 350;
    const DWORD REPEAT_NEXT  = 45;
    const DWORD REPEAT_GAP   = 120;
    const DWORD now = GetTickCount();
    if( now - g_lastTime > REPEAT_GAP ) g_lastChar = 0;
    unsigned char c = 0;
    for( int guard = 0; guard < 64; guard++ ) {
      c = FetchChar();
      if( c == 0 ) break;
      if( !g_charsExact ) {
        g_lastTime = now;
        if( (char)c == g_lastChar && now < g_nextTime ) continue;
        g_nextTime = now + ( ( (char)c == g_lastChar ) ? REPEAT_NEXT : REPEAT_FIRST );
        g_lastChar = (char)c;
      }
      if( c == 13 ) {
        EndEdit( true );
        return;
      }
      if( c == 27 ) {
        EndEdit( false );
        return;
      }
      if( c == 8 ) {
        if( g_editLen > 0 ) g_editBuf[--g_editLen] = 0;
        continue;
      }
      if( c < 32 || g_editLen >= 46 ) continue;
      g_editBuf[g_editLen++] = (char)c;
      g_editBuf[g_editLen]   = 0;
    }
  }
  inline void DrawNoteBar() {
    if( !g_full || !screen ) return;
    int x, y, w, h;
    FullRect( x, y, w, h );
    const float lh  = LineHPx();
    const float pad = 4.0f;
    const float bh  = lh + pad * 2.0f;
    int sw = 0, sh = 0;
    ScreenSizePx( sw, sh );
    float by = (float)( y + h ) - bh - 4.0f;
    if( by + bh > (float)sh - 6.0f ) by = (float)sh - 6.0f - bh;
    if( by < (float)y + 4.0f )       by = (float)y + 4.0f;
    if( g_edit >= 0 ) {
      const bool caret = ( ( GetTickCount() / 400 ) & 1 ) == 0;
      zSTRING line = zSTRING( T( TX_NOTE_PREFIX ) ) + zSTRING( g_editBuf );
      if( caret ) line = line + "_";
      Pane( (float)x + 8.0f, by, (float)w - 16.0f, bh );
      MarkText( (float)x + 8.0f + pad, by + pad, line.ToChar(), ColMarkLight() );
      const char* tip  = T( TX_NOTE_TIP );
      const float tipw = TextWidthPx( tip );
      MarkText( (float)( x + w ) - 8.0f - pad - tipw, by + pad, tip, ColDim() );
      return;
    }
    if( !g_markHint || !g_marks ) return;
    zSTRING hint = zSTRING( T( g_wheelMarkIcon ? TX_MARK_HINT : TX_MARK_HINT_KEYS ) ) + zSTRING( IconName( g_markIcon ) );
    const float ico = lh;
    const float tw  = TextWidthPx( hint.ToChar() );
    const float bw  = tw + ico + pad * 5.0f;
    float       bx  = (float)x + (float)w * 0.5f - bw * 0.5f;
    if( bx < (float)x + 8.0f ) bx = (float)x + 8.0f;
    Pane( bx, by, bw, bh );
    DrawMarkIcon( bx + pad * 1.5f + ico * 0.5f, by + pad + lh * 0.5f,
                  ico * 0.8f, g_markIcon );
    MarkText( bx + pad * 2.5f + ico, by + pad, hint.ToChar(), ColMarkLight() );
  }
  static int*  g_rawMouse[3] = { 0, 0, 0 };
  static int   g_preMouse[3] = { 0, 0, 0 };
  static bool  g_rawTried    = false;
  static bool  g_preBtnL     = false;
  static bool  g_preBtnR     = false;
  static bool  g_preBtnSet   = false;
  inline bool MouseBtn( bool right ) {
    if( g_preBtnSet ) return right ? g_preBtnR : g_preBtnL;
    if( !zinput ) return false;
    return ( right ? zinput->GetMouseButtonPressedRight()
                   : zinput->GetMouseButtonPressedLeft() ) != 0;
  }
  const unsigned int MINIMAP_RAWMOUSE_X = zSwitch( 0x0086CCACu, 0x008B27A8u, 0x008C3004u, 0x008D165Cu );
  const unsigned int MINIMAP_RAWMOUSE_Y = zSwitch( 0x0086CCB0u, 0x008B27ACu, 0x008C3008u, 0x008D1660u );
  const unsigned int MINIMAP_RAWMOUSE_Z = zSwitch( 0x0086CCB4u, 0x008B27B0u, 0x008C300Cu, 0x008D1664u );
  inline bool RawMouse() {
    if( g_rawTried ) return g_rawMouse[0] != 0;
    g_rawTried = true;
    const unsigned char* code = (const unsigned char*)ADDR_GetMousePosW32;
    const bool hooked = EngineCodeHooked( code );
    int found = 0;
    for( int i = 0; !hooked && i < 96 && found < 3; i++ ) {
      if( code[i] == 0xDB && code[i + 1] == 0x05 ) {
        g_rawMouse[found++] = *(int**)( code + i + 2 );
        i += 5;
      }
    }
    if( found < 3 && MINIMAP_RAWMOUSE_X ) {
      g_rawMouse[0] = (int*)MINIMAP_RAWMOUSE_X;
      g_rawMouse[1] = (int*)MINIMAP_RAWMOUSE_Y;
      g_rawMouse[2] = (int*)MINIMAP_RAWMOUSE_Z;
      if( RawMouseWritable( g_rawMouse[0] ) && RawMouseWritable( g_rawMouse[2] ) ) {
        MM_LOG( zSTRING( hooked
          ? "mouse counters: GetMousePos hooked (GRawInput?) - using known addresses"
          : "mouse counters: scan failed - using known addresses" ) );
        return true;
      }
    }
    if( found < 3 ) {
      g_rawMouse[0] = 0;
      MM_LOG( "mouse counters not recognised - cursor without camera block" );
      return false;
    }
    MM_LOG( "mouse counters found" );
    return true;
  }
  static const unsigned int kMouseAccAt[2]  = { zSwitch( 0x004C7FEBu, 0u, 0u, 0x004D3E50u ), zSwitch( 0x004C7FF7u, 0u, 0u, 0x004D3E5Cu ) };
  static const unsigned int kMouseAccCnt[2] = { zSwitch( 0x0086CCACu, 0x008B27A8u, 0x008C3004u, 0x008D165Cu ), zSwitch( 0x0086CCB0u, 0x008B27ACu, 0x008C3008u, 0x008D1660u ) };
  static const unsigned int kSmoothOn = zSwitch( 0u, 0u, 0u, 0x008D1B04u ), kSmoothSt = zSwitch( 0u, 0u, 0u, 0x008D1AFCu ), kSmoothInit = zSwitch( 0u, 0u, 0u, 0x008D1D94u );
  inline bool PatchByte( unsigned char* p, unsigned char b ) {
    DWORD old = 0;
    if( !VirtualProtect( p, 1, PAGE_EXECUTE_READWRITE, &old ) ) return false;
    *p = b;
    VirtualProtect( p, 1, old, &old );
    FlushInstructionCache( GetCurrentProcess(), p, 1 );
    return true;
  }
  static int  g_mouseAccOn  = 0;
  static bool g_mouseAccBad = false;
  static int  g_smoothKeep  = -1;
  inline void MouseSmoothSet( bool off ) {
    if( !kSmoothOn ) return;
    int* const on = (int*)kSmoothOn;
    if( off ) {
      if( g_smoothKeep >= 0 || !RawMouseWritable( on ) || !RawMouseWritable( (void*)kSmoothSt )
          || IsBadReadPtr( (void*)kSmoothInit, 4 ) || *(const int*)kSmoothInit != 1 )
        return;
      g_smoothKeep = *on;
      *on = 0;
    }
    else if( g_smoothKeep >= 0 ) {
      *on = g_smoothKeep;
      *(float*)kSmoothSt = 0.0f;
      g_smoothKeep = -1;
    }
  }
  static int g_mouseAccForeign = 0;
  inline bool MouseAccForeign() {
    static const unsigned char kModrm[2] = { 0x0D, 0x15 };
    for( int i = 0; i < 2; i++ ) {
      const unsigned char* c = (const unsigned char*)kMouseAccAt[i];
      if( IsBadReadPtr( c, 6 ) || c[0] != 0x01 || c[1] != kModrm[i]
          || *(const unsigned int*)( c + 2 ) != kMouseAccCnt[i] )
        return false;
    }
    return true;
  }
  inline bool MouseAccSet( bool on ) {
    if( !kMouseAccAt[0] || ( on ? 1 : 0 ) == g_mouseAccOn ) return false;
    if( g_mouseAccForeign ) {
      g_mouseAccOn = on ? 1 : 0;
      MouseSmoothSet( on );
      return true;
    }
    if( g_mouseAccBad ) return false;
    static const unsigned char kModrm[2] = { 0x0D, 0x15 };
    const unsigned char now = on ? 0x89 : 0x01;
    if( on && MouseAccForeign() ) {
      g_mouseAccForeign = 1;
      g_mouseAccOn = 1;
      MouseSmoothSet( true );
      MM_LOG( zSTRING( "mouse sum: already set by another plugin (GRawInput?) - "
                       "only X smoothing off while the full map is open" ) );
      return true;
    }
    for( int i = 0; i < 2; i++ ) {
      const unsigned char* c = (const unsigned char*)kMouseAccAt[i];
      if( IsBadReadPtr( c, 6 ) ) {
        g_mouseAccBad = true;
        return false;
      }
      if( c[0] != now || c[1] != kModrm[i] || *(const unsigned int*)( c + 2 ) != kMouseAccCnt[i] ) {
        g_mouseAccBad = true;
        if( c[0] == 0x90 && c[1] == 0x90 )
          MM_LOG( "mouse sum: engine mouse loop removed by Union mouse wrapper (zMouseFix?)" );
        MM_LOG( zSTRING( "mouse sum: engine bytes differ - left as is (" ) + zSTRING( (int)c[0] ) + " " + zSTRING( (int)c[1] ) + ")" );
        return false;
      }
    }
    for( int i = 0; i < 2; i++ )
      if( !PatchByte( (unsigned char*)kMouseAccAt[i], on ? 0x01 : 0x89 ) ) { g_mouseAccBad = true; return false; }
    g_mouseAccOn = on ? 1 : 0;
    MouseSmoothSet( on );
    static bool logged = false;
    if( on && !logged ) { logged = true; MM_LOG( "mouse sum: full map adds all mouse events of a frame" ); }
    return true;
  }
  inline const unsigned char* MouseFixImage( HMODULE& mod, int& size ) {
    mod = GetModuleHandleA( "zMouseFix.dll" );
    if( !mod ) return 0;
    MODULEINFO mi = {};
    if( !GetModuleInformation( GetCurrentProcess(), mod, &mi, sizeof( mi ) ) || mi.SizeOfImage <= 0 || mi.SizeOfImage > 0x02000000u || IsBadReadPtr( mod, mi.SizeOfImage ) ) return 0;
    size = (int)mi.SizeOfImage;
    return (const unsigned char*)mod;
  }
  static const MouseUnionWrap::TPattern kWrapPat = { reinterpret_cast<unsigned int>( &player ), zSwitch( 0x0086CCACu, 0x008B27A8u, 0x008C3004u, 0x008D165Cu ) };
  static unsigned char* g_wrapSkip  = 0;
  static HMODULE        g_wrapMod   = 0;
  static bool           g_wrapTried = false;
  static int            g_wrapOn    = 0;
  inline unsigned char* MouseWrapFind() {
    if( g_wrapTried ) return g_wrapSkip;
    g_wrapTried = true;
    if( !kWrapPat.counterX ) return 0;
    HMODULE m = 0; int size = 0;
    const unsigned char* img = MouseFixImage( m, size );
    if( !img ) {
      if( m ) MM_LOG( "mouse wrapper (zMouseFix): image not readable - left as is" );
      return 0;
    }
    bool ours = false;
    const int at = MouseUnionWrap::FindSkip( img, size, kWrapPat, &ours );
    if( at < 0 ) {
      MM_LOG( "mouse wrapper (zMouseFix): other version, X axis code not found - left as is" );
      return 0;
    }
    g_wrapSkip = (unsigned char*)( img + at );
    g_wrapMod  = m;
    MM_LOG( "mouse wrapper (zMouseFix) found - full map uses raw X like Y" );
    return g_wrapSkip;
  }
  inline void MouseWrapSet( bool on ) {
    if( ( on ? 1 : 0 ) == g_wrapOn ) return;
    unsigned char* c = MouseWrapFind();
    if( !c || GetModuleHandleA( "zMouseFix.dll" ) != g_wrapMod ) return;
    if( *c != MouseUnionWrap::SkipByte( !on ) || !PatchByte( c, MouseUnionWrap::SkipByte( on ) ) ) return;
    g_wrapOn = on ? 1 : 0;
    MouseSmoothSet( on );
  }
  static const unsigned int kEvtCounterX = zSwitch( 0x0086CCACu, 0x008B27A8u, 0x008C3004u, 0x008D165Cu );
  static MouseUnionWrap::TEventStores g_evt;
  static HMODULE g_evtMod   = 0;
  static int     g_evtTried = 0;
  static int     g_evtOn    = 0;
  inline bool MouseEvtFind() {
    if( g_evtTried ) return g_evtTried > 0;
    HMODULE m = 0; int size = 0;
    const unsigned char* img = MouseFixImage( m, size );
    if( !m ) return false;
    g_evtTried = -1;
    if( !img ) return false;
    bool ours = false;
    if( !MouseUnionWrap::FindEventStores( img, size, (unsigned int)img, kEvtCounterX, &g_evt, &ours ) ) {
      MM_LOG( "mouse events (zMouseFix): DirectInput loop not recognised - left as is" );
      return false;
    }
    g_evtMod   = m;
    g_evtTried = 1;
    MM_LOG( zSTRING( "mouse events (zMouseFix): found " ) + zSTRING( g_evt.n ) + " copies - full map adds all mouse events of a frame" );
    return true;
  }
  inline void MouseEvtSet( bool on ) {
    if( ( on ? 1 : 0 ) == g_evtOn ) return;
    if( !MouseEvtFind() || GetModuleHandleA( "zMouseFix.dll" ) != g_evtMod ) return;
    unsigned char* const img = (unsigned char*)g_evtMod;
    const unsigned char want = MouseUnionWrap::StoreByte( on );
    if( !MouseUnionWrap::EventStoresReady( img, g_evt, on ) ) return;
    for( int k = 0; k < g_evt.n; k++ ) {
      PatchByte( img + g_evt.x[k], want );
      PatchByte( img + g_evt.y[k], want );
    }
    g_evtOn = on ? 1 : 0;
    MouseSmoothSet( on );
  }
  inline void ClashIniLine( const char* file, const char* sec, const char* key, zSTRING& out ) {
    char v[64] = "";
    GetPrivateProfileStringA( sec, key, "", v, 64, file );
    if( v[0] ) out = out + " " + zSTRING( key ) + "=" + zSTRING( v );
  }
  inline HMODULE ClashModAt( const void* addr ) {
    return addr ? reinterpret_cast<HMODULE>( Union::Dll::FindNearestModule( const_cast<void*>( addr ) ) ) : 0;
  }
  inline HMODULE ClashSelf() { return ClashModAt( (const void*)&ClashSelf ); }
}
#include "MiniMap_NpcIcons.h"
namespace MiniMap {
  inline void ClashHook( const char* name, unsigned int addr, zSTRING& out, int& n ) {
    const unsigned char* c = (const unsigned char*)addr;
    if( !addr || IsBadReadPtr( c, 8 ) || !MiniMapClash::CodeHooked( c ) ) return;
    const unsigned int to = MiniMapClash::HookTarget( c, addr );
    char mod[MAX_PATH] = "";
    HMODULE hm = ClashModAt( (const void*)to );
    if( hm ) GetModuleFileNameA( hm, mod, MAX_PATH );
    const bool own = ( hm && hm == ClashSelf() );
    out = out + zSTRING( n ? ", " : " " ) + zSTRING( name ) + " -> " + zSTRING( own ? "MiniMap (own)" : ( mod[0] ? MiniMapClash::BaseName( mod ) : "unknown" ) );
    n++;
  }
  inline void ClashReport() {
    static bool done = false;
    if( done || !g_log ) return;
    done = true;
    ProcLowMem();
    char winDir[MAX_PATH] = "";
    GetWindowsDirectoryA( winDir, MAX_PATH );
    HMODULE mods[512];
    DWORD need = 0;
    if( K32EnumProcessModules( GetCurrentProcess(), mods, sizeof( mods ), &need ) ) {
      const int cnt = (int)( ( need < sizeof( mods ) ? need : sizeof( mods ) ) / sizeof( HMODULE ) );
      zSTRING line( "conflicts: modules outside Windows:" );
      int n = 0;
      for( int i = 0; i < cnt; i++ ) {
        char path[MAX_PATH];
        if( !GetModuleFileNameA( mods[i], path, MAX_PATH ) || MiniMapClash::IsSystemModule( path, winDir ) ) continue;
        const char* base = MiniMapClash::BaseName( path );
        const char* what = MiniMapClash::KnownKind( base );
        line = line + zSTRING( n ? ", " : " " ) + zSTRING( base );
        if( what ) line = line + " (" + zSTRING( what ) + ")";
        n++;
        if( line.Length() > 900 ) { MM_LOG( line ); line = zSTRING( "conflicts: modules (cont.):" ); n = 0; }
      }
      if( n || line.Length() > 40 ) MM_LOG( line );
    }
    zSTRING hooks( "conflicts: engine functions hooked by other plugins:" );
    int nh = 0;
    ClashHook( "GetMousePos", ADDR_GetMousePosW32, hooks, nh );
    ClashHook( "KeyPressed", ADDR_KeyPressedW32, hooks, nh );
    ClashHook( "zCView::Blit", ADDR_ViewBlit, hooks, nh );
    ClashHook( "ShowVobDebugInfo", ADDR_ShowVobDebugInfo, hooks, nh );
    if( !nh ) hooks = hooks + " none";
    MM_LOG( hooks );
    const char* sysDir = zoptions ? zoptions->GetDirString( DIR_SYSTEM ).ToChar() : "";
    char gIni[MAX_PATH + 32], sIni[MAX_PATH + 32];
    snprintf( gIni, sizeof( gIni ), "%sGothic.ini", ( sysDir && sysDir[0] ) ? sysDir : ".\\" );
    snprintf( sIni, sizeof( sIni ), "%sSystemPack.ini", ( sysDir && sysDir[0] ) ? sysDir : ".\\" );
    zSTRING mouse( "conflicts: mouse settings:" );
    ClashIniLine( gIni, "GAME", "enableMouse", mouse );
    ClashIniLine( gIni, "ENGINE", "zSmoothMouse", mouse );
    ClashIniLine( gIni, "ENGINE", "zMouseRotationScale", mouse );
    ClashIniLine( sIni, "MOUSE", "EnableWrapper", mouse );
    ClashIniLine( sIni, "MOUSE", "NoDirectXAxisMode", mouse );
    ClashIniLine( sIni, "MOUSE", "SpeedMultiplierX", mouse );
    ClashIniLine( sIni, "MOUSE", "SpeedMultiplierY", mouse );
    ClashIniLine( sIni, "MOUSE", "ForceMouseControl", mouse );
    MM_LOG( mouse );
    int acc[3] = { 0, 0, 0 }, speed = 0;
    SystemParametersInfoA( SPI_GETMOUSE, 0, acc, 0 );
    SystemParametersInfoA( SPI_GETMOUSESPEED, 0, &speed, 0 );
    MM_LOG( zSTRING( "conflicts: Windows mouse speed " ) + zSTRING( speed ) + "/20, enhance pointer precision " + zSTRING( acc[2] ? "on" : "off" ) );
    LogFlush();
  }
  inline void MouseAccSync() {
    const bool want = g_full && g_enabled;
    MouseAccSet( want );
    if( want && g_mouseAccOn && g_smoothKeep < 0 )
      MouseSmoothSet( true );
    MouseWrapSet( want );
    MouseEvtSet( want );
  }
  inline void ReadMouse() {
    g_mdx = g_mdy = g_mdz = 0.0f;
    if( !zinput ) return;
    if( !g_full && g_wheelZoom < 2 ) return;
    if( g_full && RawMouse() ) {
      g_mdx =  (float)( *g_rawMouse[0] + g_preMouse[0] );
      g_mdy = -(float)( *g_rawMouse[1] + g_preMouse[1] );
      g_mdz =  (float)( *g_rawMouse[2] + g_preMouse[2] );
      *g_rawMouse[0] = 0;
      *g_rawMouse[1] = 0;
      *g_rawMouse[2] = 0;
      g_preMouse[0] = g_preMouse[1] = g_preMouse[2] = 0;
    }
    else
      zinput->GetMousePos( g_mdx, g_mdy, g_mdz );
    if( g_cursorFlipX ) g_mdx = -g_mdx;
    if( g_cursorFlipY ) g_mdy = -g_mdy;
  }
  inline void MoveCursor() {
    if( !zinput || !g_full ) return;
    int x, y, w, h;
    FullRect( x, y, w, h );
    if( !g_curReady ) {
      g_curX     = (float)x + (float)w * 0.5f;
      g_curY     = (float)y + (float)h * 0.5f;
      g_curReady = true;
    }
    const float k = (float)g_cursorSpeed * 0.01f;
    g_curX += g_mdx * k;
    g_curY -= g_mdy * k;
    float x0 = (float)x, y0 = (float)y, y1 = (float)( y + h );
    if( g_ilPanW > 0.0f ) {
      x0 = g_ilPanX;
      if( g_ilPanY < y0 )             y0 = g_ilPanY;
      if( g_ilPanY + g_ilPanH > y1 )  y1 = g_ilPanY + g_ilPanH;
    }
    if( g_curX < x0 )               g_curX = x0;
    if( g_curY < y0 )               g_curY = y0;
    if( g_curX > (float)( x + w ) ) g_curX = (float)( x + w );
    if( g_curY > y1 )               g_curY = y1;
  }
  inline float MarkHitR() {
    float r = MarkPxFull() * 0.65f;
    const float rmin = 14.0f * UiScale();
    if( r < rmin ) r = rmin;
    if( r < 8.0f ) r = 8.0f;
    return r;
  }
  inline int OwnMarkAtCursor() {
    int   best = -1;
    float bestD = MarkHitR() * MarkHitR();
    for( int i = 0; i < g_ownCount; i++ ) {
      if( !MarkHere( g_ownMarks[i] ) ) continue;
      float px, py;
      WorldToPx( g_ownMarks[i].x, g_ownMarks[i].z, px, py );
      const float dx = px - g_curX;
      const float dy = py - g_curY;
      const float d  = dx * dx + dy * dy;
      if( d < bestD ) {
        bestD = d;
        best  = i;
      }
    }
    return best;
  }
  inline void AddOwnMark() {
    if( g_ownCount >= USER_MAX || !g_world[0] ) return;
    float wx, wz;
    PxToWorld( g_curX, g_curY, wx, wz );
    TUserMark& u = g_ownMarks[g_ownCount];
    StrCopy( u.world, g_world, NAME_MAX );
    u.x       = wx;
    u.z       = wz;
    u.icon    = g_markIcon;
    u.note[0] = 0;
    u.tag     = g_worldTag;
    g_ownCount++;
    g_guide = g_ownCount - 1;
    SaveOwnMarks();
    BeginEdit( g_ownCount - 1 );
    MM_LOG( zSTRING( "mark placed: " ) + zSTRING( (int)wx ) + " " + zSTRING( (int)wz ) + ", icon " + zSTRING( u.icon ) );
  }
  inline void RemoveOwnMark( int idx ) {
    if( idx < 0 || idx >= g_ownCount ) return;
    GuideOnRemove( idx );
    for( int i = idx; i < g_ownCount - 1; i++ )
      g_ownMarks[i] = g_ownMarks[i + 1];
    g_ownCount--;
    SaveOwnMarks();
    MM_LOG( "mark deleted" );
  }
  inline void DrawCursor() {
    if( !g_full || !g_curReady ) return;
    const float r = CursorPx();
    for( int pass = 0; pass < 2; pass++ ) {
      const float  e = ( pass == 0 ) ? 3.0f : 0.0f;
      const zCOLOR c = ( pass == 0 ) ? zCOLOR( 16, 14, 13, 255 )
                                     : zCOLOR( 246, 236, 214, 255 );
      const float tipX = g_curX;
      const float tipY = g_curY;
      const float baX  = g_curX + r * 0.58f;
      const float baY  = g_curY + r * 0.86f;
      const float bbX  = g_curX + r * 0.16f;
      const float bbY  = g_curY + r * 0.70f;
      const float bcX  = g_curX + r * 0.02f;
      const float bcY  = g_curY + r * 1.06f;
      FillTriTex( tipX - e * 0.3f, tipY - e,
                  baX + e, baY + e * 0.4f,
                  bbX, bbY, c );
      FillTriTex( tipX - e * 0.3f, tipY - e,
                  bbX, bbY,
                  bcX - e * 0.4f, bcY + e, c );
    }
  }
  inline void DrawModPoints() {
    if( g_ptsN <= 0 || !g_pts || !g_world[0] ) return;
    const float sm = MarkPxMini();
    const float s  = g_full ? MarkPxFull() : sm;
    const float r  = sm * 0.3f;
    int   under  = -1;
    float underD = MarkHitR() * MarkHitR();
    for( int i = 0; i < g_ptsN; i++ ) {
      const MiniMapCfg::TPoint& p = g_pts[i];
      float mx, my;
      WorldToPx( p.x, p.z, mx, my );
      if( !InDisc( mx, my ) ) continue;
      if( mx < g_clipX0 - s || mx > g_clipX1 + s || my < g_clipY0 - s || my > g_clipY1 + s ) continue;
      const int icon = ( p.icon >= 0 && p.icon < MARK_ICONS ) ? p.icon : 0;
      if( g_full )
        DrawMarkIcon( mx, my, s, icon );
      else if( !DrawMarkAtlas( icon, mx, my, s ) ) {
        DrawUserMarkShape( icon, mx, my, r + 1.0f, zCOLOR( 18, 16, 15, 255 ) );
        DrawUserMarkShape( icon, mx, my, r, ColMarkIcon( icon ) );
      }
      if( g_full && g_curReady && p.name[0] ) {
        const float dx = mx - g_curX, dy = my - g_curY;
        const float d  = dx * dx + dy * dy;
        if( d < underD ) { underD = d; under = i; }
      }
    }
    if( under >= 0 && g_edit < 0 && !( g_marks && OwnMarkAtCursor() >= 0 ) ) {
      float nx = 0.0f, ny = 0.0f;
      WorldToPx( g_pts[under].x, g_pts[under].z, nx, ny );
      Bubble( nx + s * 0.6f, ny + s * 0.4f, g_pts[under].name );
    }
  }
}
