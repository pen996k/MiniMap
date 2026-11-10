#pragma once
namespace MiniMapIcons {
  enum {
    KEY_LEN    = 48,
    FILE_LEN   = 112,
    GROUP_LEN  = 24,
    MAX_GROUPS = 32,
    MAX_ENTRIES = 8192,
    MAX_IMAGES = 200
  };
  enum EKind { K_NAME = 0, K_BODY = 1, K_MDS = 2, K_GUILD = 3, K_ITEM = 4, K_COUNT = 5 };
  enum EFlag { F_KEEPCOLORS = 1, F_NOOUTLINE = 2, F_COLOR = 4 };
  struct TEntry {
    char  key[KEY_LEN];
    short image;
    short grp;
    unsigned char kind, flags;
    short src;
    int   line;
    int   guild;
    char  groupName[GROUP_LEN];
  };
  struct TGroup {
    char  name[GROUP_LEN];
    short image;
    short src;
    int   line;
  };
  struct TImage {
    char  file[FILE_LEN];
    unsigned char flags;
    unsigned char rgb[3];
    short src;
    int   line;
  };
  typedef void ( *TMsgFn )( void* ctx, int list, int line, const char* msg );
  struct TLists {
    TEntry* e;
    int     n, cap;
    TGroup  g[MAX_GROUPS];
    int     ng;
    TImage  img[MAX_IMAGES];
    int     nimg;
    int     errors, notes;
    bool    full;
    int*    hash[K_COUNT];
    int     hcap;
  };
  inline void Init( TLists& L ) { memset( &L, 0, sizeof( L ) ); }
  inline void Free( TLists& L ) {
    MiniMapCommon::MmFree( L.e );
    for( int k = 0; k < K_COUNT; k++ )
      MiniMapCommon::MmFree( L.hash[k] );
    memset( &L, 0, sizeof( L ) );
  }
  using MiniMapCfg::Copy;
  using MiniMapCfg::Cat;
  using MiniMapCfg::Up;
  using MiniMapCfg::IsSpace;
  using MiniMapCfg::TrimSpan;
  using MiniMapCfg::TrimStr;
  using MiniMapCfg::EqNoCase;
  using MiniMapCfg::StartsNoCase;
  using MiniMapCfg::EndsNoCase;
  using MiniMapCfg::HasNoCase;
  using MiniMapCfg::Fnv;
  inline bool NormKey( const char* s, int n, int kind, char* out ) {
    TrimSpan( s, n );
    if( n <= 0 ) return false;
    if( kind == K_BODY && ( EndsNoCase( s, n, ".MDM" ) || EndsNoCase( s, n, ".ASC" ) || EndsNoCase( s, n, ".MMB" ) ) )
      n -= 4;
    if( kind == K_MDS && !EndsNoCase( s, n, ".MDS" ) ) {
      if( n + 4 >= KEY_LEN ) return false;
      for( int i = 0; i < n; i++ ) out[i] = Up( s[i] );
      memcpy( out + n, ".MDS", 5 );
      return true;
    }
    if( n >= KEY_LEN || n <= 0 ) return false;
    for( int i = 0; i < n; i++ ) out[i] = Up( s[i] );
    out[n] = 0;
    return true;
  }
  inline bool KeyCharsOk( const char* k ) {
    for( ; *k; k++ ) {
      const char c = *k;
      if( !( ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' ) || c == '_' || c == '.' || c == '-' || c == '$' || c == '@' ) ) return false;
    }
    return true;
  }
  inline bool GroupCharsOk( const char* k ) {
    if( !*k ) return false;
    for( ; *k; k++ ) {
      const char c = *k;
      if( !( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' ) || c == '_' || c == '-' ) ) return false;
    }
    return true;
  }
  inline const char* MakeFile( const char* dir, const char* f, int n, char* out ) {
    TrimSpan( f, n );
    if( n <= 0 ) return "missing icon file name after '='";
    if( EndsNoCase( f, n, ".jpg" ) || EndsNoCase( f, n, ".jpeg" ) ) return "JPG has no transparency - save the icon as PNG or TGA";
    if( !EndsNoCase( f, n, ".png" ) && !EndsNoCase( f, n, ".tga" ) ) return "icon file must be .png or .tga";
    for( int i = 0; i < n; i++ ) {
      const char c = f[i];
      if( c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|' || (unsigned char)c < 32 ) return "icon file name has a character that is not allowed (: * ? \" < > |)";
      if( c == '.' && i + 1 < n && f[i + 1] == '.' ) return "icon file must stay inside the MiniMap\\Icons folder (no \"..\")";
    }
    if( f[0] == '\\' || f[0] == '/' ) return "icon file must be relative to the list (no leading \\ or /)";
    const int dn = dir ? (int)strlen( dir ) : 0;
    if( dn + 1 + n >= FILE_LEN ) return "icon file path is too long";
    int k = 0;
    for( int i = 0; i < dn; i++ ) out[k++] = ( dir[i] == '/' ) ? '\\' : dir[i];
    if( dn > 0 && out[k - 1] != '\\' ) out[k++] = '\\';
    for( int i = 0; i < n; i++ ) out[k++] = ( f[i] == '/' ) ? '\\' : f[i];
    out[k] = 0;
    return 0;
  }
  inline int ImageAdd( TLists& L, const char* file, unsigned char flags, int src, int line,
                       TMsgFn msg, void* ctx, const unsigned char* rgb = 0 ) {
    for( int i = 0; i < L.nimg; i++ )
      if( EqNoCase( L.img[i].file, file ) ) return i;
    if( L.nimg >= MAX_IMAGES ) {
      if( !L.full && msg )
        msg( ctx, src, line, "too many different icon files (limit 200) - the rest is ignored" );
      L.full = true;
      L.errors++;
      return -1;
    }
    TImage& m = L.img[L.nimg];
    Copy( m.file, FILE_LEN, file );
    m.flags = flags;
    m.rgb[0] = rgb ? rgb[0] : 255;
    m.rgb[1] = rgb ? rgb[1] : 255;
    m.rgb[2] = rgb ? rgb[2] : 255;
    m.src   = (short)src;
    m.line  = line;
    return L.nimg++;
  }
  inline bool ParseColorOpt( const char* s, unsigned char rgb[3] ) {
    if( !StartsNoCase( s, "color" ) && !StartsNoCase( s, "colour" ) ) return false;
    s += StartsNoCase( s, "colour" ) ? 6 : 5;
    int v[3], n = 0;
    while( n < 3 ) {
      while( IsSpace( *s ) || *s == '=' || *s == ':' ) s++;
      if( *s < '0' || *s > '9' ) return false;
      int r = 0, d = 0;
      while( *s >= '0' && *s <= '9' && d < 4 ) { r = r * 10 + ( *s - '0' ); s++; d++; }
      if( d >= 4 || r > 255 ) return false;
      v[n++] = r;
    }
    while( IsSpace( *s ) ) s++;
    if( *s ) return false;
    rgb[0] = (unsigned char)v[0]; rgb[1] = (unsigned char)v[1]; rgb[2] = (unsigned char)v[2];
    return true;
  }
  inline int GroupFind( const TLists& L, const char* name ) {
    for( int i = 0; i < L.ng; i++ )
      if( EqNoCase( L.g[i].name, name ) ) return i;
    return -1;
  }
  inline int GroupGet( TLists& L, const char* name, int src, int line, TMsgFn msg, void* ctx ) {
    const int i = GroupFind( L, name );
    if( i >= 0 ) return i;
    if( L.ng >= MAX_GROUPS ) {
      if( msg )
        msg( ctx, src, line, "too many groups (limit 32) - this group is ignored" );
      L.errors++;
      return -1;
    }
    TGroup& g = L.g[L.ng];
    Copy( g.name, GROUP_LEN, name );
    g.image = -1;
    g.src   = (short)src;
    g.line  = line;
    return L.ng++;
  }
  inline bool ParseInt( const char* s, int& v ) {
    if( !*s ) return false;
    int r = 0;
    for( ; *s; s++ ) {
      if( *s < '0' || *s > '9' ) return false;
      r = r * 10 + ( *s - '0' );
      if( r > 100000 ) return false;
    }
    v = r;
    return true;
  }
  inline void Note( TLists& L, TMsgFn msg, void* ctx, int src, int line, const char* m, bool err ) {
    if( err ) L.errors++; else L.notes++;
    if( msg ) msg( ctx, src, line, m );
  }
  inline int Parse( TLists& L, const char* text, int len, const char* dir, int src,
                    TMsgFn msg, void* ctx ) {
    if( !text || len <= 0 ) return 0;
    if( len >= 3 && (unsigned char)text[0] == 0xEF && (unsigned char)text[1] == 0xBB && (unsigned char)text[2] == 0xBF ) {
      text += 3;
      len  -= 3;
    }
    int section = 1;
    int added   = 0;
    int lineNo  = 0;
    int p = 0;
    char buf[512];
    while( p < len ) {
      int e = p;
      while( e < len && text[e] != '\n' && text[e] != '\r' ) e++;
      lineNo++;
      int n = e - p;
      const bool tooLong = n >= (int)sizeof( buf );
      if( tooLong ) n = (int)sizeof( buf ) - 1;
      memcpy( buf, text + p, n );
      buf[n] = 0;
      p = e + 1;
      if( e < len && text[e] == '\r' && e + 1 < len && text[e + 1] == '\n' )
        p++;
      if( tooLong ) {
        Note( L, msg, ctx, src, lineNo, "line is longer than 511 characters - skipped", true );
        continue;
      }
      for( int i = 0; buf[i]; i++ ) {
        if( buf[i] == '#' || buf[i] == ';' || ( buf[i] == '/' && buf[i + 1] == '/' ) ) {
          buf[i] = 0;
          break;
        }
      }
      char* s = buf;
      const int sl = TrimStr( s );
      if( !sl ) continue;
      if( s[0] == '[' ) {
        if( s[sl - 1] != ']' ) {
          Note( L, msg, ctx, src, lineNo, "section name must end with ']'", true );
          section = -1;
          continue;
        }
        s[sl - 1] = 0;
        char* nm = s + 1;
        TrimStr( nm );
        if( EqNoCase( nm, "groups" ) )         section = 0;
        else if( EqNoCase( nm, "creatures" ) ) section = 1;
        else if( EqNoCase( nm, "items" ) )     section = 2;
        else {
          Note( L, msg, ctx, src, lineNo, "unknown section (use [creatures], [groups] or [items]) - lines below are skipped", true );
          section = -1;
        }
        continue;
      }
      if( section < 0 ) continue;
      char* eq = strchr( s, '=' );
      if( !eq ) {
        Note( L, msg, ctx, src, lineNo, "missing '=' (expected: NAME = icon.png)", true );
        continue;
      }
      *eq = 0;
      char* val = eq + 1;
      char* tok[8];
      int   nt = 0;
      {
        char* q = val;
        tok[nt++] = q;
        while( *q && nt < 8 ) {
          if( *q == ',' ) { *q = 0; tok[nt++] = q + 1; }
          q++;
        }
        if( *q && strchr( q, ',' ) ) {
          Note( L, msg, ctx, src, lineNo, "too many values after '=' (max 7) - extra ones ignored", false );
        }
      }
      for( int t = 0; t < nt; t++ ) TrimStr( tok[t] );
      unsigned char flags = 0;
      unsigned char rgb[3] = { 255, 255, 255 };
      const char* grpName = 0;
      bool bad = false;
      for( int t = 1; t < nt; t++ ) {
        if( !tok[t][0] ) continue;
        if( EqNoCase( tok[t], "keepcolors" ) || EqNoCase( tok[t], "keepcolours" ) ) flags |= F_KEEPCOLORS;
        else if( EqNoCase( tok[t], "nooutline" ) ) flags |= F_NOOUTLINE;
        else if( ParseColorOpt( tok[t], rgb ) ) flags |= F_COLOR;
        else if( StartsNoCase( tok[t], "color" ) || StartsNoCase( tok[t], "colour" ) ) {
          Note( L, msg, ctx, src, lineNo, "write the color as: color R G B (0-255, e.g. color 255 140 0)", true );
          bad = true;
        }
        else if( section == 1 && !grpName ) {
          const char* g = tok[t];
          if( StartsNoCase( g, "group:" ) ) g += 6;
          while( IsSpace( *g ) ) g++;
          if( !GroupCharsOk( g ) || (int)strlen( g ) >= GROUP_LEN ) {
            Note( L, msg, ctx, src, lineNo, "group name may only use letters, digits, '_' and '-' (max 23)", true );
            bad = true;
          }
          else
            grpName = g;
        }
        else if( section == 2 ) {
          Note( L, msg, ctx, src, lineNo, "unknown option after ',' in [items] (use keepcolors, nooutline or color R G B - items have no groups)", true );
          bad = true;
        }
        else {
          Note( L, msg, ctx, src, lineNo, "unknown option after ',' (use a group name, keepcolors, nooutline or color R G B)", true );
          bad = true;
        }
      }
      if( bad ) continue;
      char* key = s;
      const int kl = TrimStr( key );
      if( !kl ) {
        Note( L, msg, ctx, src, lineNo, "missing name before '='", true );
        continue;
      }
      if( section == 0 ) {
        if( !GroupCharsOk( key ) || kl >= GROUP_LEN ) {
          Note( L, msg, ctx, src, lineNo, "group name may only use letters, digits, '_' and '-' (max 23)", true );
          continue;
        }
        char file[FILE_LEN];
        const char* err = MakeFile( dir, tok[0], (int)strlen( tok[0] ), file );
        if( err ) {
          Note( L, msg, ctx, src, lineNo, err, true );
          continue;
        }
        const int gi = GroupGet( L, key, src, lineNo, msg, ctx );
        if( gi < 0 ) continue;
        if( L.g[gi].image >= 0 )
          Note( L, msg, ctx, src, lineNo, "group defined again - this line wins", false );
        const int im = ImageAdd( L, file, flags, src, lineNo, msg, ctx, rgb );
        if( im < 0 ) continue;
        L.g[gi].image = (short)im;
        L.g[gi].src   = (short)src;
        L.g[gi].line  = lineNo;
        added++;
        continue;
      }
      int kind = -1;
      const char* k = key;
      if( section == 2 )                     kind = K_ITEM;
      else if( StartsNoCase( k, "npc:" ) )   { kind = K_NAME;  k += 4; }
      else if( StartsNoCase( k, "body:" ) )  { kind = K_BODY;  k += 5; }
      else if( StartsNoCase( k, "mds:" ) )   { kind = K_MDS;   k += 4; }
      else if( StartsNoCase( k, "guild:" ) ) { kind = K_GUILD; k += 6; }
      while( IsSpace( *k ) ) k++;
      const int kn = (int)strlen( k );
      if( kind < 0 ) {
        int dummy;
        if( EndsNoCase( k, kn, ".MDS" ) )                    kind = K_MDS;
        else if( StartsNoCase( k, "GIL_" ) || ParseInt( k, dummy ) ) kind = K_GUILD;
        else                                                  kind = K_NAME;
      }
      TEntry ent;
      memset( &ent, 0, sizeof( ent ) );
      if( !NormKey( k, kn, kind, ent.key ) ) {
        Note( L, msg, ctx, src, lineNo, "name before '=' is empty or longer than 47 characters", true );
        continue;
      }
      if( !KeyCharsOk( ent.key ) ) {
        Note( L, msg, ctx, src, lineNo, "name before '=' may only use letters, digits and _ . - $ @", true );
        continue;
      }
      ent.kind  = (unsigned char)kind;
      ent.flags = flags;
      ent.src   = (short)src;
      ent.line  = lineNo;
      ent.guild = -1;
      ent.image = -1;
      ent.grp   = -1;
      if( kind == K_GUILD ) {
        int gv;
        if( ParseInt( ent.key, gv ) ) ent.guild = gv;
      }
      if( !( tok[0][0] == '-' && tok[0][1] == 0 ) ) {
        char file[FILE_LEN];
        const char* err = MakeFile( dir, tok[0], (int)strlen( tok[0] ), file );
        if( err ) {
          Note( L, msg, ctx, src, lineNo, err, true );
          continue;
        }
        ent.image = (short)ImageAdd( L, file, flags, src, lineNo, msg, ctx, rgb );
        if( ent.image < 0 ) continue;
      }
      else if( kind == K_ITEM ) {
      }
      else if( !grpName ) {
        Note( L, msg, ctx, src, lineNo, "'-' (no own icon) needs a group after ','", true );
        continue;
      }
      if( grpName ) {
        Copy( ent.groupName, GROUP_LEN, grpName );
        ent.grp = (short)GroupGet( L, grpName, src, lineNo, msg, ctx );
      }
      if( L.n >= L.cap ) {
        if( L.cap >= MAX_ENTRIES ) {
          if( !L.full && msg )
            msg( ctx, src, lineNo, "too many lines in icon lists (limit 8192) - the rest is ignored" );
          L.full = true;
          L.errors++;
          continue;
        }
        int nc = L.cap ? L.cap * 2 : 256;
        if( nc > MAX_ENTRIES ) nc = MAX_ENTRIES;
        TEntry* ne = MiniMapCommon::MmAlloc<TEntry>( (size_t)( nc ) );
        if( !ne ) {
          Note( L, msg, ctx, src, lineNo, "not enough memory for icon lists", true );
          return added;
        }
        if( L.n ) memcpy( ne, L.e, sizeof( TEntry ) * L.n );
        MiniMapCommon::MmFree( L.e );
        L.e   = ne;
        L.cap = nc;
      }
      L.e[L.n++] = ent;
      added++;
    }
    return added;
  }
  typedef int ( *TGuildFn )( void* ctx, const char* name );
  inline void Build( TLists& L, TMsgFn msg, void* ctx, TGuildFn guild, void* gctx ) {
    for( int k = 0; k < K_COUNT; k++ ) {
      MiniMapCommon::MmFree( L.hash[k] );
      L.hash[k] = 0;
    }
    L.hcap = 0;
    int cap = 16;
    while( cap < L.n * 2 + 2 ) cap <<= 1;
    for( int k = 0; k < K_COUNT; k++ ) {
      L.hash[k] = MiniMapCommon::MmAlloc<int>( (size_t)( cap ) );
      if( !L.hash[k] ) {
        for( int j = 0; j < k; j++ ) { MiniMapCommon::MmFree( L.hash[j] ); L.hash[j] = 0; }
        Note( L, msg, ctx, 0, 0, "not enough memory for icon index", true );
        return;
      }
      memset( L.hash[k], 0, sizeof( int ) * cap );
    }
    L.hcap = cap;
    for( int g = 0; g < L.ng; g++ ) {
      if( L.g[g].image < 0 ) {
        char m[96];
        snprintf( m, sizeof( m ), "group '%s' is used but not defined in any [groups]", L.g[g].name );
        Note( L, msg, ctx, L.g[g].src, L.g[g].line, m, true );
      }
    }
    for( int i = 0; i < L.n; i++ ) {
      TEntry& e = L.e[i];
      if( e.kind == K_GUILD && e.guild < 0 ) {
        const int v = guild ? guild( gctx, e.key ) : -1;
        if( v < 0 ) {
          Note( L, msg, ctx, e.src, e.line, "unknown guild name in this game's scripts - line ignored", false );
          continue;
        }
        e.guild = v;
      }
      char gk[16];
      const char* hk = e.key;
      if( e.kind == K_GUILD ) { snprintf( gk, sizeof( gk ), "%d", e.guild ); hk = gk; }
      int* const H = L.hash[e.kind];
      unsigned int h = Fnv( hk ) & ( cap - 1 );
      for( int probe = 0; probe < cap; probe++, h = ( h + 1 ) & ( cap - 1 ) ) {
        if( !H[h] ) { H[h] = i + 1; break; }
        TEntry& o = L.e[H[h] - 1];
        const char* ok2 = o.key;
        char ogk[16];
        if( o.kind == K_GUILD ) { snprintf( ogk, sizeof( ogk ), "%d", o.guild ); ok2 = ogk; }
        if( !strcmp( ok2, hk ) ) {
          char m[160];
          snprintf( m, sizeof( m ), "'%s' is also on an earlier line - this line wins", e.key );
          Note( L, msg, ctx, e.src, e.line, m, false );
          H[h] = i + 1;
          break;
        }
      }
    }
  }
  inline const TEntry* FindStr( const TLists& L, int kind, const char* key ) {
    if( !L.hcap || !L.hash[kind] || !key || !*key ) return 0;
    const int* const H = L.hash[kind];
    const int cap = L.hcap;
    unsigned int h = Fnv( key ) & ( cap - 1 );
    for( int probe = 0; probe < cap; probe++, h = ( h + 1 ) & ( cap - 1 ) ) {
      if( !H[h] ) return 0;
      const TEntry& e = L.e[H[h] - 1];
      if( !strcmp( e.key, key ) ) return &e;
    }
    return 0;
  }
  inline const TEntry* FindGuild( const TLists& L, int guild ) {
    if( guild < 0 ) return 0;
    char gk[16];
    snprintf( gk, sizeof( gk ), "%d", guild );
    if( !L.hcap || !L.hash[K_GUILD] ) return 0;
    const int* const H = L.hash[K_GUILD];
    unsigned int h = Fnv( gk ) & ( L.hcap - 1 );
    for( int probe = 0; probe < L.hcap; probe++, h = ( h + 1 ) & ( L.hcap - 1 ) ) {
      if( !H[h] ) return 0;
      const TEntry& e = L.e[H[h] - 1];
      if( e.guild == guild ) return &e;
    }
    return 0;
  }
  inline const TEntry* Resolve( const TLists& L, const char* inst, const char* body,
                                const char* mds, int guild, int* which = 0 ) {
    char k[KEY_LEN];
    const TEntry* e = 0;
    int w = -1;
    if( inst && *inst && NormKey( inst, (int)strlen( inst ), K_NAME, k ) ) e = FindStr( L, K_NAME, k ), w = e ? 0 : -1;
    if( !e && body && *body && NormKey( body, (int)strlen( body ), K_BODY, k ) ) {
      e = FindStr( L, K_BODY, k );
      if( !e ) e = FindStr( L, K_NAME, k );
      if( e ) w = 1;
    }
    if( !e && mds && *mds && NormKey( mds, (int)strlen( mds ), K_MDS, k ) ) {
      e = FindStr( L, K_MDS, k );
      if( e ) w = 2;
    }
    if( !e ) {
      e = FindGuild( L, guild );
      if( e ) w = 3;
    }
    if( which ) *which = w;
    return e;
  }
  enum EItemClass {
    IC_GOLD = 0, IC_MELEE_1H, IC_MELEE_2H, IC_BOW, IC_CROSSBOW, IC_AMMO, IC_ARMOR,
    IC_POTION, IC_PLANT, IC_FOOD, IC_SCROLL, IC_RUNE, IC_JEWELRY, IC_DOCS, IC_KEY,
    IC_TORCH, IC_TROPHY, IC_OTHER, IC_COUNT
  };
  static const char* const kItemClassKey[IC_COUNT] = {
    "gold", "melee_1h", "melee_2h", "bow", "crossbow", "ammo", "armor",
    "potion", "plant", "food", "scroll", "rune", "jewelry", "docs", "key",
    "torch", "trophy", "other"
  };
  using namespace Gothic_II_Addon;
  enum { FL_BELT = 1 << 24 };
  inline int ItemClassOf( const char* inst, unsigned int mf, unsigned int fl ) {
    const char* n = inst ? inst : "";
    if( mf & ITM_CAT_NF )     return ( fl & ( ITM_FLAG_2HD_SWD | ITM_FLAG_2HD_AXE ) ) ? IC_MELEE_2H : IC_MELEE_1H;
    if( mf & ITM_CAT_FF )     return ( fl & ITM_FLAG_CROSSBOW ) ? IC_CROSSBOW : IC_BOW;
    if( mf & ITM_CAT_MUN )    return IC_AMMO;
    if( mf & ITM_CAT_ARMOR )  return IC_ARMOR;
    if( mf & ITM_CAT_RUNE )   return ( StartsNoCase( n, "ITSC" ) || StartsNoCase( n, "ITARSCROLL" ) ) ? IC_SCROLL : IC_RUNE;
    if( mf & ITM_CAT_POTION ) return IC_POTION;
    if( mf & ITM_CAT_FOOD ) {
      if( StartsNoCase( n, "ITFO_POTION" ) ) return IC_POTION;
      if( StartsNoCase( n, "ITPL" ) || StartsNoCase( n, "ITFO_PLANTS" ) || HasNoCase( n, "HERB" ) || HasNoCase( n, "MUSHROOM" ) ) return IC_PLANT;
      return IC_FOOD;
    }
    if( mf & ITM_CAT_DOCS )   return IC_DOCS;
    if( ( mf & (unsigned int)ITM_CAT_MAGIC ) || ( fl & ( ITM_FLAG_RING | ITM_FLAG_AMULET | FL_BELT ) )
        || StartsNoCase( n, "ITRI_" ) || StartsNoCase( n, "ITAM_" ) || StartsNoCase( n, "ITBE_" ) )
      return IC_JEWELRY;
    if( ( mf & ITM_CAT_LIGHT ) || StartsNoCase( n, "ITLS" ) ) return IC_TORCH;
    if( StartsNoCase( n, "ITKE" ) )                           return IC_KEY;
    if( StartsNoCase( n, "ITMI_GOLD" ) || StartsNoCase( n, "ITMINUGGET" ) || StartsNoCase( n, "ITMI_NUGGET" )
        || StartsNoCase( n, "ITMI_ORE" ) || HasNoCase( n, "NUGGET" ) )
      return IC_GOLD;
    if( StartsNoCase( n, "ITAT" ) )                           return IC_TROPHY;
    return IC_OTHER;
  }
  inline const TEntry* ResolveItem( const TLists& L, const char* inst, int cls ) {
    char k[KEY_LEN];
    if( inst && *inst && NormKey( inst, (int)strlen( inst ), K_ITEM, k ) ) {
      const TEntry* e = FindStr( L, K_ITEM, k );
      if( e ) return e;
    }
    if( cls < 0 || cls >= IC_COUNT ) return 0;
    NormKey( kItemClassKey[cls], (int)strlen( kItemClassKey[cls] ), K_ITEM, k );
    return FindStr( L, K_ITEM, k );
  }
  inline int ImageFor( const TLists& L, const TEntry* e, int level, const unsigned char* ok ) {
    if( !e || level <= 0 ) return -1;
    const int own = ( e->image >= 0 && ( !ok || ok[e->image] ) ) ? e->image : -1;
    int grp = -1;
    if( e->grp >= 0 && e->grp < L.ng ) {
      const int gi = L.g[e->grp].image;
      if( gi >= 0 && ( !ok || ok[gi] ) ) grp = gi;
    }
    if( level == 1 ) return ( grp >= 0 ) ? grp : own;
    return ( own >= 0 ) ? own : grp;
  }
}
