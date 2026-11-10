#pragma once
namespace MiniMap {
  enum { MA_IDLE = 0, MA_WAIT, MA_NPCS, MA_PLACES, MA_TEX, MA_TEXTS, MA_ITEMS, MA_DONE };
  static const DWORD MA_WAIT_MS   = 8000;
  static const int   MA_SYM_TICK  = 4000;
  static const int   MA_ITEM_TICK = 24;
  static const int   MA_LIST_MAX  = 8000;
  static int   g_maState = MA_IDLE;
  static DWORD g_maT0    = 0;
  static char  g_maWorld[NAME_MAX] = "";
  static int   g_maSym = 0, g_maNSym = 0;
  static bool  g_maSetsOn = false;
  static MiniMapCfg::TListSet g_maKinds, g_maNames;
  inline void ModAutoFreeSets() {
    if( g_maSetsOn ) {
      MiniMapCfg::ListFree( g_maKinds );
      MiniMapCfg::ListFree( g_maNames );
    }
    g_maSetsOn = false;
  }
  inline void ModAutoFree() {
    ModAutoFreeSets();
    g_maState  = MA_IDLE;
    g_maWorld[0] = 0;
  }
  inline bool ModAutoPath( char* out, const char* name ) {
    char dir[MAX_PATH];
    if( !g_modName[0] || !FilePath( dir, MAX_PATH, CFG_MODS_DIR, g_modName ) ) return false;
    return FilePath( out, MAX_PATH, dir, name );
  }
  static const char* const MA_OLD_HEAD = "# Written once by MiniMap. Every entry starts with '#' and does nothing.";
  inline bool ModAutoOldUntouched( const char* path ) {
    int len = 0;
    unsigned char* b = CfgReadFile( path, len, 4 * 1024 * 1024 );
    if( !b ) return false;
    bool ours = strstr( (const char*)b, MA_OLD_HEAD ) != 0;
    for( const char* l = (const char*)b; ours && *l; ) {
      const char* e = strchr( l, '\n' );
      const int n = e ? (int)( e - l ) : (int)strlen( l );
      int i = 0;
      while( i < n && ( l[i] == ' ' || l[i] == '\t' ) ) i++;
      if( i < n && l[i] != '#' && l[i] != '[' && l[i] != '\r' && memchr( l + i, '=', n - i ) ) ours = false;
      l = e ? e + 1 : l + n;
    }
    MiniMapCommon::MmFree( b );
    return ours;
  }
  static int g_maOldMoved = 0;
  inline bool ModAutoExists( const char* name ) {
    char p[MAX_PATH];
    if( !ModAutoPath( p, name ) ) return true;
    if( GetFileAttributesA( p ) == INVALID_FILE_ATTRIBUTES ) return false;
    if( !ModAutoOldUntouched( p ) ) return true;
    char dir[MAX_PATH], bdir[MAX_PATH], bp[MAX_PATH];
    if( !FilePath( dir, MAX_PATH, CFG_MODS_DIR, g_modName ) || !FilePath( bdir, MAX_PATH, dir, "backup" )
        || !FilePath( bp, MAX_PATH, bdir, name ) )
      return true;
    CreateDirectoryA( bdir, 0 );
    if( !MoveFileExA( p, bp, MOVEFILE_REPLACE_EXISTING ) ) return true;
    g_maOldMoved++;
    MM_LOG( zSTRING( "mod lists: old list " ) + zSTRING( name ) + " (not changed by you) moved to backup - writing a new one" );
    return false;
  }
  inline void ModAutoName( char* out, int cap, const char* what, bool world ) {
    char w[NAME_MAX];
    int wn = 0;
    if( world ) for( const char* p = g_world; *p && wn < NAME_MAX - 1; p++ )
      w[wn++] = ( isalnum( (unsigned char)*p ) || *p == '_' || *p == '-' ) ? *p : '_';
    w[wn] = 0;
    snprintf( out, (size_t)cap, "auto_%s%s%s.txt", what ? what : "", world ? "_" : "", w );
  }
  inline void ModAutoHead( MiniMapCfg::TTextBuf& B, const char* about, const char* world, const char* label = "world" ) {
    MiniMapCfg::BufPut( B, "# MiniMap - " );
    MiniMapCfg::BufPut( B, about );
    if( world && world[0] ) {
      MiniMapCfg::BufPut( B, " (" ); MiniMapCfg::BufPut( B, label ); MiniMapCfg::BufPut( B, " " );
      MiniMapCfg::BufPut( B, world ); MiniMapCfg::BufPut( B, ")" );
    }
    MiniMapCfg::BufPut( B, ".\r\n"
      "# Lines with # do nothing. To use one: remove the #, change the value, load a save.\r\n" );
  }
  inline bool ModAutoSave( const char* name, const MiniMapCfg::TTextBuf& B ) {
    char p[MAX_PATH];
    if( B.bad || !B.p || B.n <= 0 || !ModAutoPath( p, name ) ) return false;
    char dir[MAX_PATH];
    if( FilePath( dir, MAX_PATH, CFG_MODS_DIR, g_modName ) ) {
      CreateDirectoryA( FILES_DIR, 0 );
      CreateDirectoryA( CFG_DIR, 0 );
      CreateDirectoryA( CFG_MODS_DIR, 0 );
      CreateDirectoryA( dir, 0 );
    }
    HANDLE h = CreateFileA( p, GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0 );
    if( h == INVALID_HANDLE_VALUE ) return false;
    DWORD wr = 0;
    const BOOL ok = WriteFile( h, B.p, (DWORD)B.n, &wr, 0 );
    CloseHandle( h );
    return ok && wr == (DWORD)B.n;
  }
  inline int ModAutoSection( MiniMapCfg::TTextBuf& B, const char* sec, const char* help,
                             MiniMapCfg::TListSet& S, bool cntCmt ) {
    for( int i = 0; i < S.n; i++ ) S.e[i].off = 1;
    MiniMapCfg::ListSort( S );
    MiniMapCfg::BufPut( B, "\r\n[" );
    MiniMapCfg::BufPut( B, sec );
    MiniMapCfg::BufPut( B, "]\r\n# Format: " );
    MiniMapCfg::BufPut( B, help );
    MiniMapCfg::BufPut( B, "\r\n" );
    if( S.n == 0 )
      MiniMapCfg::BufPut( B, "# (nothing found)\r\n" );
    int skipped = MiniMapCfg::ListWrite( S, B, cntCmt );
    if( S.over ) {
      MiniMapCfg::BufPut( B, "# ... and " );
      MiniMapCfg::BufInt( B, S.over );
      MiniMapCfg::BufPut( B, " more (list limit)\r\n" );
    }
    return skipped + S.bad;
  }
  inline void ModAutoLog( const char* file, bool ok, int entries, int skipped ) {
    if( ok )
      MM_LOG( zSTRING( "mod lists: wrote " ) + zSTRING( g_modName ) + "\\" + zSTRING( file ) + ", entries " + zSTRING( entries ) + ", names not usable as keys " + zSTRING( skipped ) );
    else
      MM_LOG( zSTRING( "mod lists: could not write " ) + zSTRING( file ) );
  }
  inline const char* ModAutoTexWord( int cls ) {
    if( cls == SURF_TERRAIN ) return "terrain";
    for( int k = 0; k < 14; k++ )
      if( kTxSurf[k] == cls ) return kTerrainKey[k];
    return 0;
  }
  inline const char* ModAutoPlaceWord( int kind ) {
    for( int k = 0; k < (int)( sizeof( kPlaceWordKind ) / sizeof( kPlaceWordKind[0] ) ); k++ )
      if( kPlaceWordKind[k].kind == kind ) return kPlaceWordKind[k].key;
    return "other";
  }
  inline void ModAutoPlaceWordsAll( char* out, int cap ) {
    out[0] = 0;
    for( int k = 0; k < (int)( sizeof( kPlaceWordKind ) / sizeof( kPlaceWordKind[0] ) ); k++ ) {
      const int l = (int)strlen( out );
      if( l ) MiniMapCfg::Copy( out + l, cap - l, " " );
      const int l2 = (int)strlen( out );
      MiniMapCfg::Copy( out + l2, cap - l2, kPlaceWordKind[k].key );
    }
  }
  inline bool ModAutoHasName( const char* nm, const char* key ) {
    return nm && nm[0] && !NameIsKey( nm, key );
  }
  inline void ModAutoKeyUp( char* out, int cap, const char* key ) {
    MiniMapCfg::Copy( out, cap, key );
    if( out && cap > 0 ) _strupr_s( out, (size_t)cap );
  }
  inline bool ModAutoNpcsCore( oCWorld* world, MiniMapCfg::TListSet& names, MiniMapCfg::TListSet& trad ) {
    __try {
      char key[MiniMapCfg::KEY_LEN];
      int guard = 0;
      for( zCListSort<oCNpc>* node = world->voblist_npcs; node && guard < 200000; node = node->next, guard++ ) {
        oCNpc* const npc = node->data;
        if( !npc || npc == player || !HideSymName( npc->instanz, key, (int)sizeof( key ) ) ) continue;
        const char* nm = npc->name[0].ToChar();
        const bool has = ModAutoHasName( nm, key );
        MiniMapCfg::ListAdd( names, key, has ? nm : "?", has ? 0 : "no name in game" );
        if( TraderIs( npc ) )
          MiniMapCfg::ListAdd( trad, key, "no", "trader now" );
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline void ModAutoNpcs( oCWorld* world ) {
    char file[96];
    ModAutoName( file, (int)sizeof( file ), "npcs", true );
    if( ModAutoExists( file ) ) return;
    MiniMapCfg::TListSet names, trad;
    MiniMapCfg::ListInit( names, MA_LIST_MAX );
    MiniMapCfg::ListInit( trad, MA_LIST_MAX );
    TraderEnsure();
    const bool ok = ModAutoNpcsCore( world, names, trad );
    if( ok && names.n > 0 ) {
      MiniMapCfg::TTextBuf B;
      MiniMapCfg::BufInit( B );
      ModAutoHead( B, "characters and creatures of this mod", g_world );
      const int n = names.n;
      int sk = ModAutoSection( B, "names", "CHARACTER = name in the tooltip and search (2x = two of them)", names, true );
      sk += ModAutoSection( B, "traders", "CHARACTER = no (not a trader) or yes (a trader)", trad, false );
      MiniMapCfg::BufPut( B, "\r\n[hide]\r\n# Format: CHARACTER = yes (never shown on the map)\r\n" );
      ModAutoLog( file, ModAutoSave( file, B ), n, sk );
      MiniMapCfg::BufFree( B );
    }
    else if( !ok )
      MM_LOG( "mod lists: exception reading characters - list skipped" );
    MiniMapCfg::ListFree( names );
    MiniMapCfg::ListFree( trad );
  }
  inline void ModAutoPlaceOne( zCVob* v, MiniMapCfg::TListSet& names, MiniMapCfg::TListSet& words ) {
    zCClassDef* const def = v->_GetClassDef();
    if( !SwitchIs( oCMobInter::classDef, def ) || SwitchIs( oCMobContainer::classDef, def )
        || SwitchIs( oCMobLadder::classDef, def ) || SwitchKindOf( v ) >= 0 )
      return;
    oCMobInter* const mi = (oCMobInter*)v;
    const char* const focus = mi->name.ToChar();
    if( !focus || !focus[0] ) return;
    const int kind = PlaceKindOf( v );
    if( kind >= 0 ) {
      zSTRING nm = mi->GetName();
      const bool has = ModAutoHasName( nm.ToChar(), focus );
      char key[MiniMapCfg::KEY_LEN];
      ModAutoKeyUp( key, (int)sizeof( key ), focus );
      char cmt[48];
      snprintf( cmt, sizeof( cmt ), "marker: %s%s", ModAutoPlaceWord( kind ), has ? "" : ", no name in game" );
      MiniMapCfg::ListAdd( names, key, has ? nm.ToChar() : PlaceKindName( kind ), cmt );
    }
    else if( kind == -1 && !SwitchIs( oCMobDoor::classDef, def ) && strlen( focus ) >= 3 ) {
      char key[MiniMapCfg::KEY_LEN];
      ModAutoKeyUp( key, (int)sizeof( key ), focus );
      MiniMapCfg::ListAdd( words, key, "other", "no marker now" );
    }
  }
  inline bool ModAutoPlacesCore( oCWorld* world, MiniMapCfg::TListSet& names, MiniMapCfg::TListSet& words ) {
    if( !oCMobInter::classDef ) return true;
    __try {
      int guard = 0;
      for( zCListSort<zCVob>* node = world->voblist; node && guard < 2000000; node = node->next, guard++ ) {
        zCVob* const v = node->data;
        if( v && v->homeWorld == (zCWorld*)world )
          ModAutoPlaceOne( v, names, words );
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline void ModAutoPlaces( oCWorld* world ) {
    char file[96];
    ModAutoName( file, (int)sizeof( file ), "places", true );
    if( ModAutoExists( file ) ) return;
    MiniMapCfg::TListSet names, words;
    MiniMapCfg::ListInit( names, MA_LIST_MAX );
    MiniMapCfg::ListInit( words, MA_LIST_MAX );
    const bool ok = ModAutoPlacesCore( world, names, words );
    if( ok && names.n + words.n > 0 ) {
      MiniMapCfg::TTextBuf B;
      MiniMapCfg::BufInit( B );
      ModAutoHead( B, "places (mobs) of this mod", g_world );
      const int n = names.n + words.n;
      int sk = ModAutoSection( B, "names", "MOB = name in the tooltip (no name in game: the kind is shown)", names, false );
      char kinds[256], help[400];
      ModAutoPlaceWordsAll( kinds, (int)sizeof( kinds ) );
      snprintf( help, sizeof( help ), "MOB = kind, for mobs without a marker (%s)", kinds );
      sk += ModAutoSection( B, "place_words", help, words, false );
      ModAutoLog( file, ModAutoSave( file, B ), n, sk );
      MiniMapCfg::BufFree( B );
    }
    else if( !ok )
      MM_LOG( "mod lists: exception reading places - list skipped" );
    MiniMapCfg::ListFree( names );
    MiniMapCfg::ListFree( words );
  }
  inline bool ModAutoTexCore( oCWorld* world, MiniMapCfg::TListSet& tex ) {
    __try {
      zCPolygon** polys = 0;
      int numPoly = 0;
      zCMesh* const mesh = world->bspTree.mesh;
      if( mesh && mesh->numPoly > 0 && mesh->polyList ) { polys = mesh->polyList; numPoly = mesh->numPoly; }
      else if( world->bspTree.treePolyList && world->bspTree.numPolys > 0 ) {
        polys = world->bspTree.treePolyList; numPoly = world->bspTree.numPolys;
      }
      for( int p = 0; p < numPoly; p++ ) {
        const zCPolygon* const poly = polys[p];
        if( !poly || !poly->material ) continue;
        zCMaterial* const mtl = poly->material;
        const char* const nm = mtl->GetObjectName().ToChar();
        if( !nm || !nm[0] || !MiniMapCfg::ListKeyOk( nm ) ) continue;
        MiniMapCfg::TListEnt* e = MiniMapCfg::ListFind( tex, nm );
        if( e ) { e->cnt++; continue; }
        const int grp = (int)mtl->matGroup;
        int cls;
        if( grp == (int)zMAT_GROUP_WATER && !GenWaterGroupIsLand( nm ) ) cls = SURF_WATER;
        else if( grp == 6 )        cls = SURF_SNOW;
        else if( GenMatSkip( nm ) ) continue;
        else {
          const int t = TxSurfOf( nm );
          cls = ( t >= 0 ) ? t : (int)GenClassName( nm );
        }
        const char* w = ModAutoTexWord( cls );
        if( w )
          MiniMapCfg::ListAdd( tex, nm, w );
      }
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      return false;
    }
  }
  inline void ModAutoTextures( oCWorld* world ) {
    char file[96];
    ModAutoName( file, (int)sizeof( file ), "textures", true );
    if( ModAutoExists( file ) ) return;
    MiniMapCfg::TListSet tex;
    MiniMapCfg::ListInit( tex, MA_LIST_MAX );
    const bool ok = ModAutoTexCore( world, tex );
    if( ok && tex.n > 0 ) {
      MiniMapCfg::TTextBuf B;
      MiniMapCfg::BufInit( B );
      ModAutoHead( B, "textures (materials) of this mod", g_world );
      const int n = tex.n;
      const int sk = ModAutoSection( B, "textures", "TEXTURE = kind of ground (2x = used twice). A change draws the map again once.", tex, true );
      ModAutoLog( file, ModAutoSave( file, B ), n, sk );
      MiniMapCfg::BufFree( B );
    }
    else if( !ok )
      MM_LOG( "mod lists: exception reading textures - list skipped" );
    MiniMapCfg::ListFree( tex );
  }
  inline void ModAutoTexts() {
    const int lang = ( g_lang >= 0 && g_lang < LANG_COUNT ) ? g_lang : LANG_PL;
    char what[16];
    snprintf( what, sizeof( what ), "texts_%s", kLangSuffix[lang] );
    char file[64];
    ModAutoName( file, (int)sizeof( file ), what, false );
    if( ModAutoExists( file ) ) return;
    MiniMapCfg::TListSet tx, mn;
    MiniMapCfg::ListInit( tx, TX_COUNT + 8 );
    MiniMapCfg::ListInit( mn, MENU_TR_COUNT * 2 + 8 );
    for( int i = 0; i < TX_COUNT; i++ ) {
      const char* s = TEXT[i][lang];
      if( s && s[0] )
        MiniMapCfg::ListAdd( tx, TEXT_KEY[i], s );
    }
    for( int i = 0; i < MENU_TR_COUNT; i++ ) {
      if( MenuLocked( i ) ) continue;
      const char* t0 = MENU_TR[i].text0.t[lang];
      const char* t1 = MENU_TR[i].text1.t[lang];
      if( lang == LANG_PL ) {
        t0 = MENU_TR_PL[i][0] ? MENU_TR_PL[i][0] : ( g_menuPLHave[i] ? g_menuPL0[i] : 0 );
        t1 = MENU_TR_PL[i][1] ? MENU_TR_PL[i][1] : ( g_menuPLHave[i] ? g_menuPL1[i] : 0 );
      }
      if( t0 && t0[0] )
        MiniMapCfg::ListAdd( mn, MENU_TR[i].item, t0 );
      if( t1 && t1[0] ) {
        char k[MiniMapCfg::KEY_LEN];
        snprintf( k, sizeof( k ), "%s.info", MENU_TR[i].item );
        MiniMapCfg::ListAdd( mn, k, t1 );
      }
    }
    MiniMapCfg::TTextBuf B;
    MiniMapCfg::BufInit( B );
    ModAutoHead( B, "texts of MiniMap", LANG_NAME[lang], "language" );
    char sec[16];
    snprintf( sec, sizeof( sec ), "text_%s", kLangSuffix[lang] );
    const int n = tx.n + mn.n;
    int sk = ModAutoSection( B, sec, "NAME = text on the map (tooltips, search bar, notes)", tx, false );
    snprintf( sec, sizeof( sec ), "menu_%s", kLangSuffix[lang] );
    sk += ModAutoSection( B, sec, "NAME = menu line, NAME.info = text at the bottom (keep the number of | signs)", mn, false );
    ModAutoLog( file, ModAutoSave( file, B ), n, sk );
    MiniMapCfg::BufFree( B );
    MiniMapCfg::ListFree( tx );
    MiniMapCfg::ListFree( mn );
  }
  inline const char* ModAutoKindWord( oCItem* it ) {
    switch( SearchCatOf( it ) ) {
      case SCAT_MELEE:  return ItemKindIcon( it ) == ICON_ITEM_2H ? "melee_2h" : "melee_1h";
      case SCAT_RANGED: return "ranged";
      case SCAT_ARMOR:  return "armor";
      case SCAT_POTION: return "potion";
      case SCAT_FOOD:   return "food";
      case SCAT_MAGIC:  return "magic";
      case SCAT_DOCS:   return "docs";
      case SCAT_GOLD:   return "gold";
    }
    return "other";
  }
  inline bool ModAutoItemOne( int s ) {
    if( !InvIsItemInstance( s ) ) return false;
    char key[MiniMapCfg::KEY_LEN];
    if( !HideSymName( s, key, (int)sizeof( key ) ) ) return false;
    if( strchr( key, '.' ) ) return false;
    if( !MiniMapCfg::ListKeyOk( key ) ) { g_maKinds.bad++; return false; }
    oCItem* it = InvCreateRaw( s );
    if( !it ) return true;
    const zSTRING& nm = ( it->name.Length() > 0 ) ? it->name : it->description;
    const char* const ns = const_cast<zSTRING&>( nm ).ToChar();
    const char* const kw = ModAutoKindWord( it );
    MiniMapCfg::ListAdd( g_maKinds, key, kw, ( ns && ns[0] ) ? ns : 0 );
    if( ns && ns[0] )
      MiniMapCfg::ListAdd( g_maNames, key, ns );
    it->Release();
    return true;
  }
  inline bool ModAutoItemsCore( int& made ) {
    __try {
      for( int k = 0; k < MA_SYM_TICK && g_maSym < g_maNSym && made < MA_ITEM_TICK && !g_invCreateOff; k++ )
        if( ModAutoItemOne( g_maSym++ ) )
          made++;
      return true;
    }
    __except( EXCEPTION_EXECUTE_HANDLER ) {
      g_maSym++;
      return false;
    }
  }
  inline bool ModAutoItemsStep() {
    char file[64];
    ModAutoName( file, (int)sizeof( file ), "items", false );
    if( !g_maSetsOn ) {
      if( ModAutoExists( file ) ) return true;
      if( g_invCreateOff || !zfactory ) {
        MM_LOG( "mod lists: items not listed (item creation off)" );
        return true;
      }
      MiniMapCfg::ListInit( g_maKinds, MA_LIST_MAX );
      MiniMapCfg::ListInit( g_maNames, MA_LIST_MAX );
      g_maSetsOn = true;
      g_maSym    = 0;
      g_maNSym   = HideSymCount();
      return false;
    }
    int made = 0;
    static int s_exc = 0;
    if( !ModAutoItemsCore( made ) && s_exc++ < 3 )
      MM_LOG( zSTRING( "mod lists: exception at script symbol " ) + zSTRING( g_maSym - 1 ) + " - skipped" );
    if( g_maSym < g_maNSym && !g_invCreateOff ) return false;
    if( g_maKinds.n > 0 ) {
      MiniMapCfg::TTextBuf B;
      MiniMapCfg::BufInit( B );
      ModAutoHead( B, "items of this mod", 0 );
      if( g_invCreateOff )
        MiniMapCfg::BufPut( B, "# (list not complete: the game stopped item creation)\r\n" );
      const int n = g_maKinds.n;
      int sk = ModAutoSection( B, "item_kinds", "ITEM = kind (melee_1h melee_2h ranged armor potion food magic docs gold other)", g_maKinds, false );
      sk += ModAutoSection( B, "names", "ITEM = name in the tooltip, search and item list", g_maNames, false );
      MiniMapCfg::BufPut( B, "\r\n[hide]\r\n# Format: ITEM = yes (never shown on the map)\r\n" );
      ModAutoLog( file, ModAutoSave( file, B ), n, sk );
      MiniMapCfg::BufFree( B );
    }
    ModAutoFreeSets();
    return true;
  }
  inline void ModAutoTick() {
    if( !g_modFiles || !g_modAuto || !g_modName[0] || !g_world[0] ) return;
    if( !StrEqUp( g_maWorld, g_world ) ) {
      ModAutoFreeSets();
      StrCopy( g_maWorld, g_world, NAME_MAX );
      g_maState = MA_WAIT;
      g_maT0    = GetTickCount();
      return;
    }
    if( g_maState == MA_DONE || g_maState == MA_IDLE ) return;
    if( g_maState == MA_WAIT ) {
      if( (DWORD)( GetTickCount() - g_maT0 ) < MA_WAIT_MS || g_genRunning ) return;
      g_maState = MA_NPCS;
    }
    oCWorld* const world = ogame ? ogame->GetGameWorld() : 0;
    if( !world || !parser || !player ) return;
    switch( g_maState ) {
      case MA_NPCS:   ModAutoNpcs( world );     g_maState = MA_PLACES; break;
      case MA_PLACES: ModAutoPlaces( world );   g_maState = MA_TEX;    break;
      case MA_TEX:    ModAutoTextures( world ); g_maState = MA_TEXTS;  break;
      case MA_TEXTS:  ModAutoTexts();           g_maState = MA_ITEMS;  break;
      case MA_ITEMS:  if( ModAutoItemsStep() )  g_maState = MA_DONE;   break;
      default:        g_maState = MA_DONE;      break;
    }
  }
}
