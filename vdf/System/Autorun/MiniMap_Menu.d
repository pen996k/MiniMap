META
{
  Parser = Menu;
  After  = MiniMap_Union.d;
};

const int MM_TEXT_X   = 1200;
const int MM_TEXT_DX  = 3600;
const int MM_TEXT_DY  = 320;
const int MM_VAL_X    = 4900;
const int MM_VAL_DX   = 2300;
const int MM_VAL_DY   = 320;
const int MM_TITLE_Y  = 800;
const int MM_START_Y  = 1350;
const int MM_DY       = 380;

const string MM_BACK_PIC   = "MENU_INGAME.TGA";
const string MM_ITEM_PIC   = "";
const string MM_CHOICE_PIC = "MENU_CHOICE_BACK.TGA";

instance C_MM_LABEL(C_MENU_ITEM_DEF)
{
  backpic        = MM_ITEM_PIC;
  posx           = MM_TEXT_X;
  posy           = MM_START_Y;
  dimx           = MM_TEXT_DX;
  dimy           = MM_TEXT_DY;
  flags          = flags | IT_EFFECTS_NEXT;
  fontname       = MENU_FONT_SMALL;
  onselaction[0] = SEL_ACTION_UNDEF;
};

instance C_MM_CHOICE(C_MENU_ITEM_DEF)
{
  backpic  = MM_CHOICE_PIC;
  type     = MENU_ITEM_CHOICEBOX;
  fontname = MENU_FONT_SMALL;
  posx     = MM_VAL_X;
  posy     = MM_START_Y;
  dimx     = MM_VAL_DX;
  dimy     = MM_VAL_DY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
};

instance C_MM_INPUT(C_MENU_ITEM_DEF)
{
  backpic               = MM_CHOICE_PIC;
  type                  = MENU_ITEM_INPUT;
  fontname              = MENU_FONT_SMALL;
  posx                  = MM_VAL_X;
  posy                  = MM_START_Y;
  dimx                  = MM_VAL_DX;
  dimy                  = MM_VAL_DY;
  flags                 = flags & ~IT_SELECTABLE;
  flags                 = flags | IT_TXT_CENTER;
  onchgsetoptionsection = "MINIMAP";
};

instance C_MM_KEYLABEL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  flags = flags & ~IT_EFFECTS_NEXT;
};

instance C_MM_LINK(C_MENU_ITEM_DEF)
{
  backpic  = MM_ITEM_PIC;
  posx     = MM_TEXT_X;
  posy     = MM_START_Y;
  dimx     = 5800;
  dimy     = MM_TEXT_DY;
  fontname = MENU_FONT_SMALL;
  flags    = flags | IT_TXT_CENTER;
};

instance MENU_OPT_MINIMAP(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_TITLE";
  items[1]       = "MM_ENABLED";
  items[2]       = "MM_ENABLED_VAL";
  items[3]       = "MM_GOTO_LOOK";
  items[4]       = "MM_GOTO_FULL";
  items[5]       = "MM_GOTO_SHOW";
  items[6]       = "MM_GOTO_ICONS";
  items[7]       = "MM_GOTO_EXTRA";
  items[8]       = "MM_GOTO_GEN";
  items[9]       = "MM_GOTO_KEYS";
  items[10]      = "MM_GOTO_EXP";
  items[11]      = "MM_LANG";
  items[12]      = "MM_LANG_VAL";
  items[13]      = "MM_LOG";
  items[14]      = "MM_LOG_VAL";
  items[15]      = "MM_AUTHOR";
  items[16]      = "MM_VERSION";
  items[17]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MENU_OPT_MINIMAP_EXTRA(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_X_TITLE";
  items[1]       = "MM_HOVER";
  items[2]       = "MM_HOVER_VAL";
  items[3]       = "MM_HOVERDET";
  items[4]       = "MM_HOVERDET_VAL";
  items[5]       = "MM_HOVERINV";
  items[6]       = "MM_HOVERINV_VAL";
  items[7]       = "MM_COMBO";
  items[8]       = "MM_COMBO_VAL";
  items[9]       = "MM_SEARCHOTH";
  items[10]      = "MM_SEARCHOTH_VAL";
  items[11]      = "MM_SEARCHINV";
  items[12]      = "MM_SEARCHINV_VAL";
  items[13]      = "MM_ITEMLIST";
  items[14]      = "MM_ITEMLIST_VAL";
  items[15]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MENU_OPT_MINIMAP_EXPORT(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_E_TITLE";
  items[1]       = "MM_EXPZOOM";
  items[2]       = "MM_EXPZOOM_VAL";
  items[3]       = "MM_EXPFMT";
  items[4]       = "MM_EXPFMT_VAL";
  items[5]       = "MM_EXPQ";
  items[6]       = "MM_EXPQ_VAL";
  items[7]       = "MM_GOTO_EXPDET";
  items[8]       = "MM_EXP_GO";
  items[9]       = "MM_E_HINT";
  items[10]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MENU_OPT_MINIMAP_EXPDET(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_D_TITLE";
  items[1]      = "MM_ED_CHESTS";
  items[2]      = "MM_ED_CHESTS_VAL";
  items[3]      = "MM_ED_ITEMS";
  items[4]      = "MM_ED_ITEMS_VAL";
  items[5]      = "MM_ED_PLANTS";
  items[6]      = "MM_ED_PLANTS_VAL";
  items[7]      = "MM_ED_NPCS";
  items[8]      = "MM_ED_NPCS_VAL";
  items[9]      = "MM_ED_FOES";
  items[10]     = "MM_ED_FOES_VAL";
  items[11]     = "MM_ED_SWITCHES";
  items[12]     = "MM_ED_SWITCHES_VAL";
  items[13]     = "MM_ED_PLACES";
  items[14]     = "MM_ED_PLACES_VAL";
  items[15]     = "MM_ED_EXITS";
  items[16]     = "MM_ED_EXITS_VAL";
  items[17]     = "MM_ED_TELEPORTS";
  items[18]     = "MM_ED_TELEPORTS_VAL";
  items[19]     = "MM_ED_MARKS";
  items[20]     = "MM_ED_MARKS_VAL";
  items[21]     = "MM_ED_PLAYER";
  items[22]     = "MM_ED_PLAYER_VAL";
  items[23]     = "MM_ED_FRIENDS";
  items[24]     = "MM_ED_FRIENDS_VAL";
  items[25]     = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MM_X_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - PODPOWIEDZI I SZUKANIE";
};

instance MM_E_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - EKSPORT MAPY";
};

instance MM_D_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - DETALE EKSPORTU";
};

instance MM_GOTO_EXTRA(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 6;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_EXTRA";
  text[0]           = "Podpowiedzi i szukanie ...";
  text[1]           = "Opis pod kursorem, szukanie i tabela przedmiotów.";
};

instance MM_GOTO_EXP(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 9;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_EXPORT";
  text[0]           = "Eksport mapy ...";
  text[1]           = "Zapis mapy œwiata do pliku obrazu (JPG, PNG, BMP).";
};

instance MM_EXPZOOM(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "Zoom mapy";
  text[1] = "Wiêkszy zoom = wiêcej pikseli i wiêkszy plik.";
};
instance MM_EXPZOOM_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "ExportZoom";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "1x|2x|4x";
};
instance MM_EXPFMT(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "Format pliku";
  text[1] = "JPG najmniejszy, PNG i BMP bez strat (BMP ogromny).";
};
instance MM_EXPFMT_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "ExportFormat";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "BMP|PNG|JPG";
};
instance MM_EXPQ(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Jakoœæ JPG";
  text[1] = "Wy¿sza = ostrzejsze ikony i wiêkszy plik.";
};
instance MM_EXPQ_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "ExportQuality";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "70%|80%|90%|95%";
};
instance MM_GOTO_EXPDET(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 4;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_EXPDET";
  text[0]           = "Detale na mapie ...";
  text[1]           = "Co zapisaæ z map¹: skrzynie, postacie, przeciwnicy...";
};
instance MM_ED_CHESTS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "Skrzynie";
  text[1] = "Skrzynie i pojemniki w kolorze zamka.";
};
instance MM_ED_CHESTS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "ExpChests";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_ITEMS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "Przedmioty";
  text[1] = "Przedmioty le¿¹ce w œwiecie (bez roœlin).";
};
instance MM_ED_ITEMS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "ExpItems";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_PLANTS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Roœliny";
  text[1] = "Zio³a i jedzenie le¿¹ce w œwiecie.";
};
instance MM_ED_PLANTS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "ExpPlants";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_NPCS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 4;
  text[0] = "Postacie neutralne";
  text[1] = "Neutralne postacie (bia³a g³owa) i handlarze (waga).";
};
instance MM_ED_NPCS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 4;
  onchgsetoption        = "ExpNpcs";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_FOES(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 5;
  text[0] = "Postacie wrogie";
  text[1] = "Wrogowie i potwory (czaszka), stan z chwili eksportu.";
};
instance MM_ED_FOES_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 5;
  onchgsetoption        = "ExpFoes";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_SWITCHES(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 6;
  text[0] = "Prze³¹czniki";
  text[1] = "DŸwignie, ko³owroty, p³yty.";
};
instance MM_ED_SWITCHES_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 6;
  onchgsetoption        = "ExpSwitches";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_PLACES(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 7;
  text[0] = "Miejsca";
  text[1] = "Kapliczki, rzemios³o, z³o¿a, skarby.";
};
instance MM_ED_PLACES_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 7;
  onchgsetoption        = "ExpPlaces";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_EXITS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 8;
  text[0] = "Przejœcia";
  text[1] = "Przejœcia do innych œwiatów.";
};
instance MM_ED_EXITS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 8;
  onchgsetoption        = "ExpExits";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_TELEPORTS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 9;
  text[0] = "Cele teleportu";
  text[1] = "Cele run z ekwipunku i teleporty w œwiecie.";
};
instance MM_ED_TELEPORTS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 9;
  onchgsetoption        = "ExpTeleports";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_MARKS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 10;
  text[0] = "W³asne znaczniki";
  text[1] = "Znaczniki postawione na du¿ej mapie.";
};
instance MM_ED_MARKS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 10;
  onchgsetoption        = "ExpMarks";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_PLAYER(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 11;
  text[0] = "Bohater";
  text[1] = "Strza³ka bohatera.";
};
instance MM_ED_PLAYER_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 11;
  onchgsetoption        = "ExpPlayer";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_ED_FRIENDS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 12;
  text[0] = "Postacie przyjazne";
  text[1] = "Przyjaciele i towarzysze (dwie zielone g³owy).";
};
instance MM_ED_FRIENDS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 12;
  onchgsetoption        = "ExpFriends";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_EXP_GO(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 6;
  onselaction[0]    = SEL_ACTION_UNDEF;
  text[0]           = "PotwierdŸ eksport";
  text[1]           = "Enter, potem wyjdŸ z menu. Esc w trakcie przerywa.";
};
instance MM_E_HINT(C_MENU_ITEM_DEF)
{
  type     = MENU_ITEM_TEXT;
  posx     = 0;
  posy     = MM_START_Y + MM_DY * 8;
  dimx     = 8100;
  dimy     = MM_TEXT_DY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
  fontname = MENU_FONT_SMALL;
  text[0]  = "Plik w folderze gry: MiniMap/export";
};

instance MM_ITEMLIST(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 7;
  text[0] = "Tabela przedmiotów";
  text[1] = "Du¿a mapa: lista po lewej, A/D kategoria, W/S wybór.";
};
instance MM_ITEMLIST_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 7;
  onchgsetoption        = "ItemList";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryta|Widoczna";
};

instance MM_TELEPORTS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 12;
  text[0] = "Teleporty";
  text[1] = "Cele teleportów na mapie.";
};
instance MM_TELEPORTS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 12;
  onchgsetoption        = "ShowTeleports";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryte|Widoczne";
};

instance MM_FOG(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 12;
  text[0] = "Mg³a na mapie";
  text[1] = "Mapa odkrywa siê tylko tam, gdzie ju¿ by³eœ.";
};
instance MM_FOG_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 12;
  onchgsetoption        = "Fog";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MENU_OPT_MINIMAP_LOOK(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_L_TITLE";
  items[1]       = "MM_SHAPE";
  items[2]       = "MM_SHAPE_VAL";
  items[3]       = "MM_SIZE";
  items[4]       = "MM_SIZE_VAL";
  items[5]       = "MM_CORNER";
  items[6]       = "MM_CORNER_VAL";
  items[7]       = "MM_MINISTYLE";
  items[8]       = "MM_MINISTYLE_VAL";
  items[9]       = "MM_ROTATE";
  items[10]      = "MM_ROTATE_VAL";
  items[11]      = "MM_ALIGN";
  items[12]      = "MM_ALIGN_VAL";
  items[13]      = "MM_WHEEL";
  items[14]      = "MM_WHEEL_VAL";
  items[15]      = "MM_SCALE";
  items[16]      = "MM_SCALE_VAL";
  items[17]      = "MM_DBAR";
  items[18]      = "MM_DBAR_VAL";
  items[19]      = "MM_GUIDE";
  items[20]      = "MM_GUIDE_VAL";
  items[21]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MENU_OPT_MINIMAP_SHOW(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_S_TITLE";
  items[1]       = "MM_NPC";
  items[2]       = "MM_NPC_VAL";
  items[3]       = "MM_NPCKIND";
  items[4]       = "MM_NPCKIND_VAL";
  items[5]       = "MM_TRADERS";
  items[6]       = "MM_TRADERS_VAL";
  items[7]       = "MM_CORPSES";
  items[8]       = "MM_CORPSES_VAL";
  items[9]       = "MM_ITEMS";
  items[10]      = "MM_ITEMS_VAL";
  items[11]      = "MM_CHESTS";
  items[12]      = "MM_CHESTS_VAL";
  items[13]      = "MM_HIDEEMPTY";
  items[14]      = "MM_HIDEEMPTY_VAL";
  items[15]      = "MM_SWITCHES";
  items[16]      = "MM_SWITCHES_VAL";
  items[17]      = "MM_PLACES";
  items[18]      = "MM_PLACES_VAL";
  items[19]      = "MM_GOTO_PLACES";
  items[20]      = "MM_EXITS";
  items[21]      = "MM_EXITS_VAL";
  items[22]      = "MM_TELEPORTS";
  items[23]      = "MM_TELEPORTS_VAL";
  items[24]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MENU_OPT_MINIMAP_PLACES(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_P_TITLE";
  items[1]       = "MM_FURNITURE";
  items[2]       = "MM_FURNITURE_VAL";
  items[3]       = "MM_CRAFT";
  items[4]       = "MM_CRAFT_VAL";
  items[5]       = "MM_ALCHEMY";
  items[6]       = "MM_ALCHEMY_VAL";
  items[7]       = "MM_RUNES";
  items[8]       = "MM_RUNES_VAL";
  items[9]       = "MM_SCROLLS";
  items[10]      = "MM_SCROLLS_VAL";
  items[11]      = "MM_DEMON";
  items[12]      = "MM_DEMON_VAL";
  items[13]      = "MM_KITCHEN";
  items[14]      = "MM_KITCHEN_VAL";
  items[15]      = "MM_WORKSHOP";
  items[16]      = "MM_WORKSHOP_VAL";
  items[17]      = "MM_FIRE";
  items[18]      = "MM_FIRE_VAL";
  items[19]      = "MM_CLUTTER";
  items[20]      = "MM_CLUTTER_VAL";
  items[21]      = "MM_LADDERS";
  items[22]      = "MM_LADDERS_VAL";
  items[23]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MENU_OPT_MINIMAP_FULL(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_F_TITLE";
  items[1]       = "MM_FULLMAP";
  items[2]       = "MM_FULLMAP_VAL";
  items[3]       = "MM_FULLSIZE";
  items[4]       = "MM_FULLSIZE_VAL";
  items[5]       = "MM_FULLSCR";
  items[6]       = "MM_FULLSCR_VAL";
  items[7]       = "MM_FULLSTYLE";
  items[8]       = "MM_FULLSTYLE_VAL";
  items[9]       = "MM_FALIGN";
  items[10]      = "MM_FALIGN_VAL";
  items[11]      = "MM_FULLCENTER";
  items[12]      = "MM_FULLCENTER_VAL";
  items[13]      = "MM_FULLPAUSE";
  items[14]      = "MM_FULLPAUSE_VAL";
  items[15]      = "MM_WHEELFULL";
  items[16]      = "MM_WHEELFULL_VAL";
  items[17]      = "MM_CURSPEED";
  items[18]      = "MM_CURSPEED_VAL";
  items[19]      = "MM_EDGESCROLL";
  items[20]      = "MM_EDGESCROLL_VAL";
  items[21]      = "MM_DRAGMAP";
  items[22]      = "MM_DRAGMAP_VAL";
  items[23]      = "MM_MARKS";
  items[24]      = "MM_MARKS_VAL";
  items[25]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MM_L_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - MINIMAPA";
};

instance MM_S_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - CO WIDAÆ NA MAPIE";
};

instance MM_P_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - MIEJSCA NA MAPIE";
};

instance MM_GOTO_PLACES(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 10;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_PLACES";
  text[0]           = "Miejsca na mapie ...";
  text[1]           = "Meble, kuŸnie, alchemia, kuchnie, warsztaty, ogieñ.";
};

instance MM_F_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - DU¯A MAPA";
};

instance MM_GOTO_LOOK(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 2;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_LOOK";
  text[0]           = "Minimapa ...";
  text[1]           = "Kszta³t, rozmiar, po³o¿enie, styl i obrót minimapy.";
};

instance MM_GOTO_SHOW(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 4;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_SHOW";
  text[0]           = "Co widaæ na mapie ...";
  text[1]           = "Postacie, przedmioty, skrzynie, miejsca, teleporty.";
};

instance MM_GOTO_FULL(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 3;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_FULL";
  text[0]           = "Du¿a mapa ...";
  text[1]           = "Wielkoœæ, styl, obrót, mysz i w³asne znaczniki.";
};

instance MM_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - USTAWIENIA";
};

instance MM_ENABLED(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "MiniMap";
  text[1] = "Czy chcesz w³¹czyæ ca³y plugin minimapy?";
};
instance MM_ENABLED_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "Enabled";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_SHAPE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "Kszta³t minimapy";
  text[1] = "Kwadrat pokazuje trochê wiêcej terenu ni¿ ko³o.";
};
instance MM_SHAPE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "Shape";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Kwadrat|Ko³o";
};

instance MM_SIZE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "Rozmiar minimapy";
  text[1] = "Bok minimapy jako procent wysokoœci ekranu.";
};
instance MM_SIZE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "SizeLevel";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "10%|12%|14%|16%|18%|20%|22%|25%|28%|32%|36%|40%";
};

instance MM_CORNER(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Po³o¿enie minimapy";
  text[1] = "W którym rogu ekranu jest minimapa.";
};
instance MM_CORNER_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "Corner";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Prawy górny|Lewy górny|Prawy dolny|Lewy dolny";
};

instance MM_ROTATE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 5;
  text[0] = "Obrót minimapy";
  text[1] = "Za kamer¹: minimapa obraca siê razem z bohaterem.";
};
instance MM_ROTATE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 5;
  onchgsetoption        = "Rotate";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Pó³noc u góry|Za kamer¹";
};

instance MM_NPC(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "Postacie na mapie";
  text[1] = "Wykrywanie postaci na mapie.";
};
instance MM_NPC_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "ShowNpc";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryte|W zasiêgu|Max";
};

instance MM_NPCKIND(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "Rodzaj postaci";
  text[1] = "Zasiêg widzenia ludzi albo potwory (tak¿e orkowie).";
};
instance MM_NPCKIND_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "ShowNpcKind";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Max|Ludzie|Potwory|Nikt";
};

instance MM_ITEMS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 5;
  text[0] = "Wykrywanie przedmiotów";
  text[1] = "Wykrywanie przedmiotów na ziemi.";
};
instance MM_ITEMS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 5;
  onchgsetoption        = "ShowItems";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryte|W zasiêgu|Max";
};

instance MM_SCALE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 8;
  text[0] = "Skala pod minimap¹";
  text[1] = "Ile metrów widaæ na minimapie (np. 60 m).";
};
instance MM_SCALE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 8;
  onchgsetoption        = "ShowScale";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryta|Zawsze|Przy zmianie";
};

instance MM_ALIGN(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 6;
  text[0] = "Obrót obrazu mapy";
  text[1] = "Obraca obraz mapy.";
};
instance MM_ALIGN_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 6;
  onchgsetoption        = "AlignIdx";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Auto|Pó³noc|90 Stopni|180 Stopni|270 Stopni";
};

instance MM_FALIGN(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 5;
  text[0] = "Obrót du¿ej mapy";
  text[1] = "Tak samo: du¿a mapa obrócona jak minimapa.";
};
instance MM_FALIGN_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 5;
  onchgsetoption        = "FullAlignIdx";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Tak samo|Pó³noc|90 Stopni|180 Stopni|270 Stopni";
};

instance MM_MINISTYLE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 4;
  text[0] = "Styl minimapy";
  text[1] = "Ramka minimapy: klasyczna, pergamin, ¿elazo albo z³oto.";
};
instance MM_MINISTYLE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 4;
  onchgsetoption        = "MiniStyle";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Klasyczny|Pergamin|¯elazo|Z³oto";
};

instance MM_FULLSIZE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "Wielkoœæ du¿ej mapy";
  text[1] = "Ile ekranu zajmuje du¿a mapa (100% = domyœlnie).";
};
instance MM_FULLSIZE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "FullSize";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "60%|70%|80%|90%|100%";
};

instance MM_FULLSTYLE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 4;
  text[0] = "Styl du¿ej mapy";
  text[1] = "Ramka i tabliczka: klasyczna, pergamin, ¿elazo, z³oto.";
};
instance MM_FULLSTYLE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 4;
  onchgsetoption        = "FullStyle";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Klasyczny|Pergamin|¯elazo|Z³oto";
};

instance MM_FULLSCR(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Du¿a mapa na ca³y ekran";
  text[1] = "Du¿a mapa zajmuje ca³y ekran lub w menu pod klawiszem.";
};
instance MM_FULLSCR_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "FullScreen";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_GOTO_GEN(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 7;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_GEN";
  text[0]           = "Wygl¹d terenu ...";
  text[1]           = "Ostroœæ, cienie, kolory, drzewa, jaskinie i mg³a.";
};

instance MM_GOTO_KEYS(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 8;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_KEYS";
  text[0]           = "Klawisze ...";
  text[1]           = "Klawisze obu map: otwieranie, przybli¿anie, szukanie.";
};

instance MM_ITEMCOL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 5;
  text[0] = "Kolory przedmiotów";
  text[1] = "Rodzaj przedmiotu kolorem: broñ, mikstury, roœliny...";
};
instance MM_ITEMCOL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 5;
  onchgsetoption        = "ItemColors";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Jednolite|Kolorowe";
};

instance MM_GOTO_COLORS(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 7;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_COLORS";
  text[0]           = "Kolory na mapie ...";
  text[1]           = "Kolory postaci i przedmiotów na mapie.";
};

instance MM_GOTO_ICONS(C_MENU_ITEM_DEF)
{
  C_MM_LINK();
  posy             += MM_DY * 5;
  onselaction[0]    = SEL_ACTION_STARTMENU;
  onselaction_s[0]  = "MENU_OPT_MINIMAP_ICONS";
  text[0]           = "Ikony i kolory ...";
  text[1]           = "Wygl¹d, wielkoœæ i kolory ikon na obu mapach.";
};

instance MENU_OPT_MINIMAP_ICONS(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_I_TITLE";
  items[1]       = "MM_ICONSET";
  items[2]       = "MM_ICONSET_VAL";
  items[3]       = "MM_SIZENPC";
  items[4]       = "MM_SIZENPC_VAL";
  items[5]       = "MM_SIZEITEM";
  items[6]       = "MM_SIZEITEM_VAL";
  items[7]       = "MM_SIZEMARK";
  items[8]       = "MM_SIZEMARK_VAL";
  items[9]       = "MM_ITEMCOL";
  items[10]      = "MM_ITEMCOL_VAL";
  items[11]      = "MM_PRING";
  items[12]      = "MM_PRING_VAL";
  items[13]      = "MM_GOTO_COLORS";
  items[14]      = "MM_I_HINT";
  items[15]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MM_I_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - IKONY I KOLORY";
};
instance MM_ICONSET(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "Wygl¹d ikon";
  text[1] = "Proste: dawne rysunki. W³asne: twoje pliki .png.";
};
instance MM_ICONSET_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "IconSet";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Proste|Z ramk¹|Bez ramki|W³asne";
};
instance MM_SIZENPC(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "Wielkoœæ postaci";
  text[1] = "Wielkoœæ ikon ludzi, potworów i zwierz¹t.";
};
instance MM_SIZENPC_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "IconSizeNpc";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%";
};
instance MM_SIZEITEM(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Wielkoœæ przedmiotów";
  text[1] = "Wielkoœæ ikon przedmiotów le¿¹cych na ziemi.";
};
instance MM_SIZEITEM_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "IconSizeItem";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%";
};
instance MM_SIZEMARK(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 4;
  text[0] = "Wielkoœæ znaczników";
  text[1] = "Skrzynie, miejsca, meble, dŸwignie, teleporty.";
};
instance MM_SIZEMARK_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 4;
  onchgsetoption        = "IconSizeMark";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%";
};
instance MM_I_HINT(C_MENU_ITEM_DEF)
{
  type     = MENU_ITEM_TEXT;
  posx     = 0;
  posy     = MM_START_Y + MM_DY * 9;
  dimx     = 8100;
  dimy     = MM_TEXT_DY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
  fontname = MENU_FONT_SMALL;
  text[0]  = "W³asne: podmieñ pliki .png w folderze gry MiniMap/icons/custom";
};

instance MENU_OPT_MINIMAP_COLORS(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_C_TITLE";
  items[1]      = "MM_C_ENEMY";
  items[2]      = "MM_C_ENEMY_VAL";
  items[3]      = "MM_C_NEUTRAL";
  items[4]      = "MM_C_NEUTRAL_VAL";
  items[5]      = "MM_C_FRIEND";
  items[6]      = "MM_C_FRIEND_VAL";
  items[7]      = "MM_C_PLAYER";
  items[8]      = "MM_C_PLAYER_VAL";
  items[9]      = "MM_C_TRADER";
  items[10]     = "MM_C_TRADER_VAL";
  items[11]     = "MM_C_CORPSE";
  items[12]     = "MM_C_CORPSE_VAL";
  items[13]     = "MM_C_ITEM1H";
  items[14]     = "MM_C_ITEM1H_VAL";
  items[15]     = "MM_C_ITEM2H";
  items[16]     = "MM_C_ITEM2H_VAL";
  items[17]     = "MM_C_RANGED";
  items[18]     = "MM_C_RANGED_VAL";
  items[19]     = "MM_C_POTION";
  items[20]     = "MM_C_POTION_VAL";
  items[21]     = "MM_C_PLANT";
  items[22]     = "MM_C_PLANT_VAL";
  items[23]     = "MM_C_MAGIC";
  items[24]     = "MM_C_MAGIC_VAL";
  items[25]     = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MM_C_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - KOLORY NA MAPIE";
};

instance MM_C_ENEMY(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "Wrogowie";
  text[1] = "Kolor wrogich postaci i potworów.";
};
instance MM_C_ENEMY_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "ColEnemy";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_NEUTRAL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "Neutralni";
  text[1] = "Kolor neutralnych postaci i zwierz¹t.";
};
instance MM_C_NEUTRAL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "ColNeutral";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_FRIEND(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Przyjaciele";
  text[1] = "Kolor przyjaznych postaci.";
};
instance MM_C_FRIEND_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "ColFriend";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_PLAYER(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 4;
  text[0] = "Bohater";
  text[1] = "Kolor strza³ki bohatera.";
};
instance MM_C_PLAYER_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 4;
  onchgsetoption        = "ColPlayer";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_TRADER(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 5;
  text[0] = "Handlarze";
  text[1] = "Kolor wagi handlarza.";
};
instance MM_C_TRADER_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 5;
  onchgsetoption        = "ColTrader";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_CORPSE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 6;
  text[0] = "Zw³oki z ³upem";
  text[1] = "Kolor krzy¿a przy zw³okach z ³upem.";
};
instance MM_C_CORPSE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 6;
  onchgsetoption        = "ColCorpse";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_ITEM1H(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 7;
  text[0] = "Broñ jednorêczna";
  text[1] = "Miecze, topory i maczugi jednorêczne.";
};
instance MM_C_ITEM1H_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 7;
  onchgsetoption        = "ColItem1H";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_ITEM2H(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 8;
  text[0] = "Broñ dwurêczna";
  text[1] = "Miecze, topory i kostury dwurêczne.";
};
instance MM_C_ITEM2H_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 8;
  onchgsetoption        = "ColItem2H";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_RANGED(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 9;
  text[0] = "£uki i kusze";
  text[1] = "£uki, kusze, strza³y i be³ty.";
};
instance MM_C_RANGED_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 9;
  onchgsetoption        = "ColRanged";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_POTION(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 10;
  text[0] = "Mikstury";
  text[1] = "Kolor mikstur.";
};
instance MM_C_POTION_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 10;
  onchgsetoption        = "ColPotion";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_PLANT(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 11;
  text[0] = "Roœliny i jedzenie";
  text[1] = "Zio³a, grzyby i jedzenie.";
};
instance MM_C_PLANT_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 11;
  onchgsetoption        = "ColPlant";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};

instance MM_C_MAGIC(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 12;
  text[0] = "Magia";
  text[1] = "Runy, zwoje, amulety i pierœcienie.";
};
instance MM_C_MAGIC_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 12;
  onchgsetoption        = "ColMagic";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Domyœlny|Czerwony|Pomarañcz|¯ó³ty|Zielony|Turkus|Niebieski|Fiolet|Ró¿owy|Bia³y|Szary|Br¹zowy";
};
instance MM_FURNITURE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "Meble";
  text[1] = "£ó¿ka, krzes³a, ³awki, fotele i trony.";
};
instance MM_FURNITURE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "ShowFurniture";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_CRAFT(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "KuŸnie";
  text[1] = "Kowad³a, paleniska, ose³ki, wiadra do hartowania.";
};
instance MM_CRAFT_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "ShowCraft";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_ALCHEMY(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Alchemia";
  text[1] = "Sto³y alchemiczne i ubijaki zió³.";
};
instance MM_ALCHEMY_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "ShowAlchemy";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_RUNES(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 4;
  text[0] = "Sto³y runiczne";
  text[1] = "Sto³y do run i przemiany magii.";
};
instance MM_RUNES_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 4;
  onchgsetoption        = "ShowRunes";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_SCROLLS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 5;
  text[0] = "Pulpity do zwojów";
  text[1] = "Pulpity do pisania magicznych zwojów.";
};
instance MM_SCROLLS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 5;
  onchgsetoption        = "ShowScrolls";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_DEMON(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 6;
  text[0] = "Demonologia";
  text[1] = "Sto³y przywo³ywaczy demonów.";
};
instance MM_DEMON_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 6;
  onchgsetoption        = "ShowDemon";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_KITCHEN(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 7;
  text[0] = "Kuchnie";
  text[1] = "Kuchenki, kot³y, patelnie, ogniska do pieczenia.";
};
instance MM_KITCHEN_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 7;
  onchgsetoption        = "ShowKitchen";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_WORKSHOP(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 8;
  text[0] = "Warsztaty";
  text[1] = "Pi³y, ³uki, garbarz, z³otnik, kartograf, wino.";
};
instance MM_WORKSHOP_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 8;
  onchgsetoption        = "ShowWorkshop";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_FIRE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 9;
  text[0] = "Ogieñ i œwiat³o";
  text[1] = "Pochodnie, kominki i zwyk³e ogniska.";
};
instance MM_FIRE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 9;
  onchgsetoption        = "ShowFire";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_CLUTTER(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 10;
  text[0] = "Drobiazgi";
  text[1] = "Beczki, studnie, deski do naprawy i reszta.";
};
instance MM_CLUTTER_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 10;
  onchgsetoption        = "ShowClutter";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};
instance MM_LADDERS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 11;
  text[0] = "Drabiny";
  text[1] = "Drabiny, po których mo¿na siê wspi¹æ.";
};
instance MM_LADDERS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 11;
  onchgsetoption        = "ShowLadders";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_CHESTS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 6;
  text[0] = "Wykrywanie skrzyñ na mapie";
  text[1] = "Pokazuj wszystkie skrzynie na mapie zale¿y od zasiêgu.";
};
instance MM_CHESTS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 6;
  onchgsetoption        = "ShowChests";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryte|W zasiêgu|Max";
};

instance MM_AUTHOR(C_MENU_ITEM_DEF)
{
  type     = MENU_ITEM_TEXT;
  posx     = MM_TEXT_X;
  posy     = MM_START_Y + MM_DY * 12;
  dimx     = 5800;
  dimy     = MM_TEXT_DY;
  fontname = MENU_FONT_SMALL;
  flags    = flags & ~IT_SELECTABLE;
  text[0]  = "Autor: PEN996";
};

instance MM_VERSION(C_MENU_ITEM_DEF)
{
  type     = MENU_ITEM_TEXT;
  posx     = MM_TEXT_X;
  posy     = MM_START_Y + MM_DY * 13;
  dimx     = 5800;
  dimy     = MM_TEXT_DY;
  fontname = MENU_FONT_SMALL;
  flags    = flags & ~IT_SELECTABLE;
  text[0]  = "Wersja: 2.0a";
};

instance MM_LANG(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 10;
  text[0] = "Jêzyk / Language";
  text[1] = "Napisy w menu i na mapie.";
};
instance MM_LANG_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 10;
  onchgsetoption        = "Language";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Polski|English|Deutsch|Cestina";
};

instance MM_LOG(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 11;
  text[0] = "Zapis logu";
  text[1] = "Czy tworzyæ MiniMap.log?";
};
instance MM_LOG_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 11;
  onchgsetoption        = "Log";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MENU_OPT_MINIMAP_GEN(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_G_TITLE";
  items[1]       = "MM_DETAIL";
  items[2]       = "MM_DETAIL_VAL";
  items[3]       = "MM_DSPEED";
  items[4]       = "MM_DSPEED_VAL";
  items[5]       = "MM_DQUAL";
  items[6]       = "MM_DQUAL_VAL";
  items[7]       = "MM_SMOOTH";
  items[8]       = "MM_SMOOTH_VAL";
  items[9]       = "MM_SCULPT";
  items[10]      = "MM_SCULPT_VAL";
  items[11]      = "MM_FILL";
  items[12]      = "MM_FILL_VAL";
  items[13]      = "MM_OUTSIDE";
  items[14]      = "MM_OUTSIDE_VAL";
  items[15]      = "MM_PATHS";
  items[16]      = "MM_PATHS_VAL";
  items[17]      = "MM_INDOOR";
  items[18]      = "MM_INDOOR_VAL";
  items[19]      = "MM_TEXCOL";
  items[20]      = "MM_TEXCOL_VAL";
  items[21]      = "MM_TREES";
  items[22]      = "MM_TREES_VAL";
  items[23]      = "MM_FOG";
  items[24]      = "MM_FOG_VAL";
  items[25]      = "MM_G_HINT";
  items[26]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MM_G_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - WYGL¥D TERENU";
};

instance MM_DETAIL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "Detale mapy";
  text[1] = "Ostroœæ przy przybli¿eniu. Wy¿sze buduj¹ siê d³u¿ej.";
};
instance MM_DETAIL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "GenDetailLvl";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "S³abe|Normalne|Maksymalne";
};

instance MM_DSPEED(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "Szybkoœæ szczegó³ów";
  text[1] = "Szybciej = ostra mapa wczeœniej, ale chwilowo mniej FPS.";
};
instance MM_DSPEED_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "DetailSpeed";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Normalna|Szybsza|Mocniejsza|Maksymalna|Ekstremalna";
};

instance MM_DQUAL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Jakoœæ szczegó³ów";
  text[1] = "Dok³adniejsze budynki, krawêdzie i cienie. Buduje d³u¿ej.";
};
instance MM_DQUAL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "DetailQuality";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Normalna|Wysoka|Bardzo wysoka|Maksymalna";
};

instance MM_SMOOTH(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 4;
  text[0] = "Wyg³adzanie terenu";
  text[1] = "Teren g³adszy, ale mniej szczegó³ów. Zalecane: Brak.";
};
instance MM_SMOOTH_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 4;
  onchgsetoption        = "GenSmooth";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Brak|Lekkie|Normalne";
};

instance MM_SCULPT(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 5;
  text[0] = "Cieniowanie gór";
  text[1] = "Grzbiety jaœniej, doliny ciemniej - góry wyraŸnie widaæ.";
};
instance MM_SCULPT_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 5;
  onchgsetoption        = "GenSculpt";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "S³abe|WyraŸne|Mocne";
};

instance MM_FILL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 6;
  text[0] = "Wype³nianie dziur";
  text[1] = "Puste plamy na mapie zakrywa terenem z okolicy.";
};
instance MM_FILL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 6;
  onchgsetoption        = "GenFillHoles";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_OUTSIDE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 7;
  text[0] = "Za krawêdzi¹ œwiata";
  text[1] = "Co rysowaæ tam, gdzie koñczy siê œwiat gry.";
};
instance MM_OUTSIDE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 7;
  onchgsetoption        = "GenOutside";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nic|Morze|Góry|Jak brzeg";
};

instance MM_PATHS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 8;
  text[0] = "Œcie¿ki na mapie";
  text[1] = "Drogi, którymi chodz¹ postacie, jako jasne œcie¿ki.";
};
instance MM_PATHS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 8;
  onchgsetoption        = "GenPaths";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_INDOOR(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 9;
  text[0] = "Wnêtrza i jaskinie";
  text[1] = "Pod dachem i w jaskini: widok wnêtrza.";
};
instance MM_INDOOR_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 9;
  onchgsetoption        = "GenIndoorLvl";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Mocne|Normalne|Tylko wnêtrza";
};

instance MM_TEXCOL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 10;
  text[0] = "Kolory mapy";
  text[1] = "Proste, jak w grze albo naturalne (¿ywsza zieleñ, ska³y).";
};
instance MM_TEXCOL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 10;
  onchgsetoption        = "GenTexColors";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Proste|Jak w grze|Naturalne";
};

instance MM_DBAR(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 9;
  text[0] = "Pasek szczegó³ów";
  text[1] = "Ma³y pasek pod minimap¹, gdy buduje siê ostry teren.";
};
instance MM_DBAR_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 9;
  onchgsetoption        = "ShowDetailBar";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryty|Widoczny";
};

instance MM_TREES(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 11;
  text[0] = "Drzewa na mapie";
  text[1] = "Bez drzew: bez koron i drzew, widaæ ziemiê i œcie¿ki.";
};
instance MM_TREES_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 11;
  onchgsetoption        = "GenTreeShapes";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Brak|Proste|Dok³adne|Bez drzew";
};

instance MM_G_HINT(C_MENU_ITEM_DEF)
{
  type     = MENU_ITEM_TEXT;
  posx     = 0;
  posy     = MM_START_Y + MM_DY * 13;
  dimx     = 8100;
  dimy     = MM_TEXT_DY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
  fontname = MENU_FONT_SMALL;
  text[0]  = "Po zmianie mapa narysuje siê od nowa po wyjœciu z menu.";
};

instance MENU_OPT_MINIMAP_KEYS(C_MENU_DEF)
{
  backpic        = MM_BACK_PIC;
  items[0]       = "MM_K_TITLE";
  items[1]       = "MM_KEYFULL";
  items[2]       = "MM_KEYFULL_VAL";
  items[3]       = "MM_KEYIN";
  items[4]       = "MM_KEYIN_VAL";
  items[5]       = "MM_KEYOUT";
  items[6]       = "MM_KEYOUT_VAL";
  items[7]       = "MM_KEYTOGGLE";
  items[8]       = "MM_KEYTOGGLE_VAL";
  items[9]       = "MM_KEYSIZEUP";
  items[10]      = "MM_KEYSIZEUP_VAL";
  items[11]      = "MM_KEYSIZEDN";
  items[12]      = "MM_KEYSIZEDN_VAL";
  items[13]      = "MM_KEYROTATE";
  items[14]      = "MM_KEYROTATE_VAL";
  items[15]      = "MM_KEYALIGN";
  items[16]      = "MM_KEYALIGN_VAL";
  items[17]      = "MM_KEYICON";
  items[18]      = "MM_KEYICON_VAL";
  items[19]      = "MM_KEYSEARCH";
  items[20]      = "MM_KEYSEARCH_VAL";
  items[21]      = "MM_KEYFSCR";
  items[22]      = "MM_KEYFSCR_VAL";
  items[23]      = "MM_KEYILIST";
  items[24]      = "MM_KEYILIST_VAL";
  items[25]      = "MM_K_HINT";
  items[26]      = "UNION_MENUITEM_BACK";
  defaultoutgame = 0;
  defaultingame  = 0;
  flags          = flags | MENU_SHOW_INFO;
};

instance MM_K_TITLE(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = MM_TITLE_Y;
  dimx    = 8100;
  dimy    = MM_TEXT_DY;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = "MiniMap - KLAWISZE";
};

instance MM_FULLMAP(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "Du¿a mapa";
  text[1] = "Mapa ca³ego œwiata na ca³y ekran, z notatkami.";
};
instance MM_FULLMAP_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "FullMap";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_WHEEL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 7;
  text[0] = "Kó³ko myszy na minimapie";
  text[1] = "Kó³ko myszy przybli¿a i oddala minimapê.";
};
instance MM_WHEEL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 7;
  onchgsetoption        = "WheelMini";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nic|Przybli¿anie";
};

instance MM_WHEELFULL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 8;
  text[0] = "Kó³ko myszy na du¿ej mapie";
  text[1] = "Kó³ko zmienia ikonê znacznika albo przybli¿a mapê.";
};
instance MM_WHEELFULL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 8;
  onchgsetoption        = "WheelFull";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nic|Zmiana ikony|Przybli¿anie";
};

instance MM_FULLCENTER(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 6;
  text[0] = "Œrodek du¿ej mapy";
  text[1] = "Obszar gry: l¹d na œrodku, bez pustego morza.";
};
instance MM_FULLCENTER_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 6;
  onchgsetoption        = "FullCenter";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ca³y œwiat|Obszar gry";
};

instance MM_MARKS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 12;
  text[0] = "W³asne znaczniki";
  text[1] = "Na du¿ej mapie: lewy klik stawia, prawy kasuje.";
};
instance MM_MARKS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 12;
  onchgsetoption        = "Marks";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_SWITCHES(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 8;
  text[0] = "DŸwignie i prze³¹czniki";
  text[1] = "DŸwignie, ko³owroty i p³yty otwieraj¹ce bramy i mosty.";
};
instance MM_SWITCHES_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 8;
  onchgsetoption        = "ShowSwitches";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryte|W zasiêgu|Max";
};

instance MM_PLACES(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 9;
  text[0] = "Miejsca i interakcje";
  text[1] = "Kapliczki, tablice, kowad³a, z³o¿a rudy, skarby.";
};
instance MM_PLACES_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 9;
  onchgsetoption        = "ShowPlaces";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryte|W zasiêgu|Wa¿ne|Max";
};

instance MM_GUIDE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 10;
  text[0] = "Strza³ka do znacznika";
  text[1] = "Na minimapie: kierunek do wybranego znacznika.";
};
instance MM_GUIDE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 10;
  onchgsetoption        = "MarkGuide";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Strza³ka|Z odleg³oœci¹";
};

instance MM_EXITS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 11;
  text[0] = "Przejœcia do innych map";
  text[1] = "Miejsca, z których przechodzi siê do innego œwiata gry.";
};
instance MM_EXITS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 11;
  onchgsetoption        = "ShowExits";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryte|W zasiêgu|Max";
};

instance MM_TRADERS(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Handlarze na mapie";
  text[1] = "Znak wagi u ka¿dego, kto handluje. Towar w podpowiedzi.";
};
instance MM_TRADERS_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "ShowTraders";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryte|W zasiêgu|Max";
};

instance MM_CORPSES(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 4;
  text[0] = "Zw³oki na mapie";
  text[1] = "Krzy¿ przy zw³okach, przy których jest coœ do zabrania.";
};
instance MM_CORPSES_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 4;
  onchgsetoption        = "ShowCorpses";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryte|W zasiêgu|Max";
};

instance MM_HIDEEMPTY(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 7;
  text[0] = "Ukryj puste";
  text[1] = "Chowa puste skrzynie, beczki, worki i ograbione zw³oki.";
};
instance MM_HIDEEMPTY_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 7;
  onchgsetoption        = "HideEmpty";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_HOVER(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 1;
  text[0] = "PodpowiedŸ pod kursorem";
  text[1] = "Na du¿ej mapie: kto albo co jest pod kursorem.";
};
instance MM_HOVER_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 1;
  onchgsetoption        = "HoverInfo";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Max|Postacie|Przedmioty";
};

instance MM_HOVERDET(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 2;
  text[0] = "Szczegó³y podpowiedzi";
  text[1] = "Sama nazwa lub: hp, mana, gildia, ¿ycie, broñ, cena.";
};
instance MM_HOVERDET_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 2;
  onchgsetoption        = "HoverDetail";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Sama nazwa|Podstawowe|Max";
};

instance MM_KEYFULL(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 1;
  text[0] = "Du¿a mapa";
  text[1] = "Otwiera i zamyka du¿¹ mapê.";
};
instance MM_KEYFULL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 1;
  onchgsetoption = "KeyFullMap";
};

instance MM_KEYIN(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 2;
  text[0] = "Przybli¿anie";
  text[1] = "Przybli¿a minimapê i du¿¹ mapê.";
};
instance MM_KEYIN_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 2;
  onchgsetoption = "KeyZoomIn";
};

instance MM_KEYOUT(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 3;
  text[0] = "Oddalanie";
  text[1] = "Oddala minimapê i du¿¹ mapê.";
};
instance MM_KEYOUT_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 3;
  onchgsetoption = "KeyZoomOut";
};

instance MM_KEYTOGGLE(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 4;
  text[0] = "Poka¿ / ukryj minimapê";
  text[1] = "Chowa i pokazuje minimapê.";
};
instance MM_KEYTOGGLE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 4;
  onchgsetoption = "KeyToggle";
};

instance MM_KEYSIZEUP(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 5;
  text[0] = "Wiêksza minimapa";
  text[1] = "Powiêksza okienko minimapy.";
};
instance MM_KEYSIZEUP_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 5;
  onchgsetoption = "KeySizeUp";
};

instance MM_KEYSIZEDN(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 6;
  text[0] = "Mniejsza minimapa";
  text[1] = "Zmniejsza okienko minimapy.";
};
instance MM_KEYSIZEDN_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 6;
  onchgsetoption = "KeySizeDown";
};

instance MM_KEYROTATE(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 7;
  text[0] = "Obrót minimapy";
  text[1] = "Prze³¹cza: pó³noc u góry albo za kamer¹.";
};
instance MM_KEYROTATE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 7;
  onchgsetoption = "KeyRotate";
};

instance MM_KEYALIGN(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 8;
  text[0] = "Obrót obrazu mapy";
  text[1] = "Obraca (i odbija) obraz mapy, krok po kroku.";
};
instance MM_KEYALIGN_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 8;
  onchgsetoption = "KeyAlign";
};

instance MM_KEYICON(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 9;
  text[0] = "Ikona znacznika";
  text[1] = "Zmienia ikonê znacznika na du¿ej mapie.";
};
instance MM_KEYICON_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 9;
  onchgsetoption = "KeyMarkIcon";
};

instance MM_KEYSEARCH(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 10;
  text[0] = "Szukanie";
  text[1] = "Na du¿ej mapie wpisz nazwê i Enter. Esc czyœci.";
};
instance MM_KEYSEARCH_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 10;
  onchgsetoption = "KeySearch";
};

instance MM_KEYFSCR(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 11;
  text[0] = "Pe³ny ekran";
  text[1] = "Otwarta du¿a mapa: ca³y ekran albo okno.";
};
instance MM_KEYFSCR_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 11;
  onchgsetoption = "KeyFullScreen";
};

instance MM_KEYILIST(C_MENU_ITEM_DEF)
{
  C_MM_KEYLABEL();
  posy   += MM_DY * 12;
  text[0] = "Tabela przedmiotów";
  text[1] = "Otwarta du¿a mapa: pokazuje i chowa tabelê.";
};
instance MM_KEYILIST_VAL(C_MENU_ITEM_DEF)
{
  C_MM_INPUT();
  posy          += MM_DY * 12;
  onchgsetoption = "KeyItemList";
};

instance MM_SEARCHOTH(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 5;
  text[0] = "Reszta przy szukaniu";
  text[1] = "Co z rzeczami i postaciami, które nie pasuj¹ do szukania.";
};
instance MM_SEARCHOTH_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 5;
  onchgsetoption        = "SearchOthers";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryj|Przyciemnij";
};

instance MM_FULLPAUSE(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 7;
  text[0] = "Pauza na du¿ej mapie";
  text[1] = "Gra stoi w miejscu, gdy du¿a mapa jest otwarta.";
};
instance MM_FULLPAUSE_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 7;
  onchgsetoption        = "FullPause";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_CURSPEED(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 9;
  text[0] = "Szybkoœæ kursora";
  text[1] = "Jak szybko kursor myszy jeŸdzi po du¿ej mapie.";
};
instance MM_CURSPEED_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 9;
  onchgsetoption        = "CursorSpeedLvl";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "50%|75%|100%|125%|150%|200%";
};

instance MM_PRING(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 6;
  text[0] = "Z³ota obwódka bohatera";
  text[1] = "Na du¿ej mapie migaj¹cy pierœcieñ wokó³ bohatera.";
};
instance MM_PRING_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 6;
  onchgsetoption        = "PlayerRing";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_HOVERINV(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 3;
  text[0] = "Ekwipunek w podpowiedzi";
  text[1] = "Pokazuje listê przedmiotów: handlarze i zw³oki albo ka¿de.";
};
instance MM_HOVERINV_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 3;
  onchgsetoption        = "HoverInv";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Handlarze|Max";
};

instance MM_SEARCHINV(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 6;
  text[0] = "Szukanie w ekwipunku";
  text[1] = "Szukaj postaci po przedmiocie, który ma przy sobie.";
};
instance MM_SEARCHINV_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 6;
  onchgsetoption        = "SearchInv";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Handlarze|Max";
};

instance MM_EDGESCROLL(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 10;
  text[0] = "Przewijanie przy krawêdzi";
  text[1] = "Mapa jedzie, gdy kursor dotknie krawêdzi okna.";
};
instance MM_EDGESCROLL_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 10;
  onchgsetoption        = "FullEdgeScroll";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_COMBO(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 4;
  text[0] = "Kombinacja zamka";
  text[1] = "PodpowiedŸ skrzyni: ruchy wytrycha albo ca³a kombinacja.";
};
instance MM_COMBO_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 4;
  onchgsetoption        = "ChestCombo";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Ukryj|Iloœæ ruchów|Pe³na";
};

instance MM_DRAGMAP(C_MENU_ITEM_DEF)
{
  C_MM_LABEL();
  posy   += MM_DY * 11;
  text[0] = "Przeci¹ganie mapy";
  text[1] = "Du¿a mapa: przytrzymaj lewy przycisk i przesuñ mapê.";
};
instance MM_DRAGMAP_VAL(C_MENU_ITEM_DEF)
{
  C_MM_CHOICE();
  posy                 += MM_DY * 11;
  onchgsetoption        = "FullDragMap";
  onchgsetoptionsection = "MINIMAP";
  text[0]               = "Nie|Tak";
};

instance MM_K_HINT(C_MENU_ITEM_DEF)
{
  type     = MENU_ITEM_TEXT;
  posx     = 0;
  posy     = MM_START_Y + MM_DY * 13;
  dimx     = 8100;
  dimy     = MM_TEXT_DY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
  fontname = MENU_FONT_SMALL;
  text[0]  = "Enter na wierszu, nowy klawisz. Delete usuwa, Esc anuluje.";
};
