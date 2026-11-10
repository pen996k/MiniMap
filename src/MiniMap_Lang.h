#pragma once
}
namespace MiniMapCommon {
  enum ELang { LANG_PL = 0, LANG_EN = 1, LANG_DE = 2, LANG_CS = 3, LANG_COUNT = 4 };
  inline constexpr const char* const LANG_NAME[LANG_COUNT] = { "Polish", "English", "German", "Czech" };
  enum ETextId {
    TX_ICON_TARGET, TX_ICON_TREASURE, TX_ICON_WARNING, TX_ICON_STAR, TX_ICON_POINT,
    TX_NOTE_PREFIX, TX_NOTE_TIP, TX_MARK_HINT, TX_MARK_HINT_KEYS,
    TX_OF_WORLD,
    TX_KEY_NONE,
    TX_ICON_FIGHT, TX_ICON_HOUSE, TX_ICON_CAVE, TX_ICON_HERB, TX_ICON_QUEST,
    TX_HOV_LEVEL, TX_HOV_GUILD, TX_HOV_HOSTILE, TX_HOV_NEUTRAL, TX_HOV_FRIENDLY,
    TX_HOV_HP, TX_HOV_VALUE,
    TX_HOV_MANA, TX_HOV_STR, TX_HOV_DEX, TX_HOV_WEAPON, TX_HOV_ARMOR, TX_HOV_PROT,
    TX_HOV_P_ARROW, TX_HOV_P_MAGIC, TX_HOV_P_FIRE, TX_HOV_IMMUNE, TX_HOV_DAMAGE,
    TX_HOV_RANGE, TX_HOV_NEEDS, TX_HOV_AMOUNT,
    TX_CAT_MELEE, TX_CAT_RANGED, TX_CAT_AMMO, TX_CAT_ARMOR, TX_CAT_FOOD, TX_CAT_DOCS,
    TX_CAT_POTION, TX_CAT_LIGHT, TX_CAT_RUNE, TX_CAT_MAGIC,
    TX_HOV_CHEST, TX_HOV_LOCKED, TX_HOV_UNLOCKED,
    TX_HOV_LOCK_KEY, TX_HOV_LOCK_PICK, TX_HOV_LOCK_BOTH, TX_HOV_ITEMS,
    TX_HOV_KEY, TX_HOV_LOCKPICK, TX_HOV_EMPTY, TX_HOV_MORE,
    TX_SEARCH_PREFIX, TX_SEARCH_FOUND, TX_SEARCH_NPCS, TX_SEARCH_INCHEST, TX_SEARCH_TIP,
    TX_SCAT_ALL, TX_SCAT_MELEE, TX_SCAT_RANGED, TX_SCAT_ARMOR, TX_SCAT_POTION,
    TX_SCAT_FOOD, TX_SCAT_MAGIC, TX_SCAT_DOCS, TX_SCAT_GOLD,
    TX_SCAT_NPC_ALL, TX_SCAT_HUMAN, TX_SCAT_MONSTER, TX_SCAT_FOE, TX_SCAT_FRIEND,
    TX_SCAT_OTHER,
    TX_GEN_BUSY,
    TX_GEN_PROG, TX_DETAIL_PROG, TX_LEFT_ABOUT,
    TX_GEN_PAUSED,
    TX_GEN_AFTER_TALK,
    TX_HOV_SW_LEVER, TX_HOV_SW_WHEEL, TX_HOV_SW_TRIGGER, TX_HOV_SW_ON, TX_HOV_SW_OFF,
    TX_HOV_PL_SHRINE, TX_HOV_PL_BOARD, TX_HOV_PL_CRAFT, TX_HOV_PL_ORE, TX_HOV_PL_OTHER,
    TX_HOV_PL_REST, TX_SCAT_PLACES,
    TX_HOV_PL_TREASURE, TX_HOV_PL_BED, TX_HOV_PL_CHAIR, TX_HOV_PL_BENCH,
    TX_HOV_PL_THRONE, TX_HOV_PL_FIRE,
    TX_HOV_EXIT, TX_HOV_EXIT_TO,
    TX_MAP_TITLE,
    TX_SCAT_TRADER, TX_HOV_TRADER, TX_HOV_CORPSE, TX_HOV_GOODS, TX_HOV_LOOT, TX_HOV_INV,
    TX_HOV_NONE,
    TX_HOV_MOVES, TX_HOV_COMBO, TX_HOV_KEYNAME,
    TX_IL_KINDS, TX_IL_PIECES, TX_IL_EMPTY, TX_IL_TIP,
    TX_TP_TITLE,
    TX_TP_MENU,
    TX_EXP_PROG, TX_EXP_SAVED, TX_EXP_FAIL, TX_EXP_ESC, TX_EXP_AFTER_MENU,
    TX_HOV_PL_ALCHEMY, TX_HOV_PL_KITCHEN, TX_HOV_PL_WORKSHOP,
    TX_HOV_PL_LADDER,
    TX_HOV_WTP,
    TX_HOV_PL_RUNE, TX_HOV_PL_SCROLL, TX_HOV_PL_DEMON,
    TX_COUNT
  };
  inline constexpr const char* const TEXT[TX_COUNT][LANG_COUNT] = {
    { "Cel", "Target", "Ziel", "Cil" },
    { "Skarb", "Treasure", "Schatz", "Poklad" },
    { "Uwaga", "Warning", "Achtung", "Pozor" },
    { "Gwiazda", "Star", "Stern", "Hvezda" },
    { "Punkt", "Point", "Punkt", "Bod" },
    { "Notatka: ", "Note: ", "Notiz: ", "Poznamka: " },
    { "Enter zapisz   Esc anuluj", "Enter save   Esc cancel", "Enter speichern   Esc abbrechen", "Enter ulozit   Esc zrusit" },
    { "Lewy: znacznik/cel   Prawy: usu\xF1   k\xF3\xB3ko/1-0 ikona: ", "Left: marker/target   Right: delete   wheel/1-0 icon: ", "Links: Markierung/Ziel   Rechts: loeschen   Rad/1-0 Symbol: ", "Levy: znacka/cil   Pravy: smazat   kolecko/1-0 ikona: " },
    { "Lewy: znacznik/cel   Prawy: usu\xF1   1-0 ikona: ", "Left: marker/target   Right: delete   1-0 icon: ", "Links: Markierung/Ziel   Rechts: loeschen   1-0 Symbol: ", "Levy: znacka/cil   Pravy: smazat   1-0 ikona: " },
    { "% \x9Cwiata", "% of the world", "% der Welt", "% sveta" },
    { "Brak", "None", "Keine", "Zadna" },
    { "Walka", "Fight", "Kampf", "Boj" },
    { "Dom / Handlarz", "House / Trader", "Haus / Haendler", "Dum / Obchodnik" },
    { "Jaskinia", "Cave", "Hoehle", "Jeskyne" },
    { "Zio\xB3o", "Herb", "Kraut", "Bylina" },
    { "Zadanie", "Quest", "Auftrag", "Ukol" },
    { "Poziom", "Level", "Stufe", "Uroven" },
    { "Gildia", "Guild", "Gilde", "Gilda" },
    { "Nastawienie: wrogie", "Hostile", "Feindlich", "Postoj: nepratelsky" },
    { "Nastawienie: neutralne", "Neutral", "Neutral", "Postoj: neutralni" },
    { "Nastawienie: przyjazne", "Friendly", "Freundlich", "Postoj: pratelsky" },
    { "\xAFycie", "Health", "Leben", "Zivot" },
    { "Warto\x9C\xE6", "Value", "Wert", "Hodnota" },
    { "Mana", "Mana", "Mana", "Mana" },
    { "Si\xB3" "a", "Strength", "Staerke", "Sila" },
    { "Zr\xEA" "czno\x9C\xE6", "Dexterity", "Geschick", "Obratnost" },
    { "Bro\xF1", "Weapon", "Waffe", "Zbran" },
    { "Zbroja", "Armour", "Ruestung", "Zbroj" },
    { "Ochrona", "Protection", "Schutz", "Ochrana" },
    { "Strza\xB3y", "Arrows", "Pfeile", "Sipy" },
    { "Magia", "Magic", "Magie", "Magie" },
    { "Ogie\xF1", "Fire", "Feuer", "Ohen" },
    { "bez obra\xBF" "e\xF1", "Immune", "Immun", "bez poskozeni" },
    { "Obra\xBF" "enia", "Damage", "Schaden", "Poskozeni" },
    { "Zasi\xEAg", "Range", "Reichweite", "Dosah" },
    { "Wymaga", "Requires", "Benoetigt", "Vyzaduje" },
    { "Ilo\x9C\xE6", "Amount", "Anzahl", "Pocet" },
    { "Bro\xF1 bia\xB3" "a", "Melee weapon", "Nahkampfwaffe", "Zbran na blizko" },
    { "Bro\xF1 dystansowa", "Ranged weapon", "Fernkampfwaffe", "Zbran na dalku" },
    { "Amunicja", "Ammunition", "Munition", "Munice" },
    { "Zbroja", "Armour", "Ruestung", "Zbroj" },
    { "Jedzenie", "Food", "Nahrung", "Jidlo" },
    { "Dokument", "Document", "Dokument", "Dokument" },
    { "Mikstura", "Potion", "Trank", "Lektvar" },
    { "\x8Cwiat\xB3o", "Light", "Licht", "Svetlo" },
    { "Runa / zw\xF3j", "Rune / scroll", "Rune / Spruchrolle", "Runa / svitek" },
    { "Magiczny przedmiot", "Magic item", "Magischer Gegenstand", "Magicky predmet" },
    { "Pojemnik", "Container", "Behaelter", "Schranka" },
    { "Zamek: zamkni\xEAty", "Locked", "Verschlossen", "Zamek: zamceny" },
    { "Zamek: otwarty", "Unlocked", "Offen", "Zamek: odemceny" },
    { "Zamek: tylko na klucz", "Locked - key only", "Verschlossen - nur Schluessel", "Zamek: jen na klic" },
    { "Zamek: tylko na wytrych", "Locked - lockpick only", "Verschlossen - nur Dietrich", "Zamek: jen na paklic" },
    { "Zamek: klucz lub wytrych", "Locked - key or lockpick", "Verschlossen - Schluessel oder Dietrich", "Zamek: klic nebo paklic" },
    { "Przedmiot\xF3w", "Items", "Gegenstaende", "Predmetu" },
    { "Klucz", "Key", "Schluessel", "Klic" },
    { "Wytrych", "Lockpick", "Dietrich", "Paklic" },
    { "Brak", "None", "Nichts", "Nic" },
    { "I jeszcze", "More:", "Weitere:", "Dalsi:" },
    { "Szukaj: ", "Search: ", "Suche: ", "Hledat: " },
    { "znaleziono: ", "found: ", "gefunden: ", "nalezeno: " },
    { "postacie: ", "characters: ", "Figuren: ", "postavy: " },
    { "w pojemnikach: ", "in containers: ", "in Behaeltern: ", "ve schrankach: " },
    { "<- -> rodzaj   g\xF3ra/d\xF3\xB3 wyb\xF3r   Enter szukaj   Esc wyczy\x9C\xE6", "<- -> type   up/down pick   Enter search   Esc clear", "<- -> Art   auf/ab Wahl   Enter suchen   Esc leeren", "<- -> druh   nahoru/dolu vyber   Enter hledat   Esc smazat" },
    { "Wszystko", "Everything", "Alles", "Vsechno" },
    { "Bro\xF1 bia\xB3" "a", "Melee weapons", "Nahkampfwaffen", "Zbrane na blizko" },
    { "Bro\xF1 dystansowa", "Ranged weapons", "Fernkampfwaffen", "Zbrane na dalku" },
    { "Zbroje", "Armour", "Ruestungen", "Zbroje" },
    { "Mikstury", "Potions", "Traenke", "Lektvary" },
    { "Jedzenie i ro\x9Cliny", "Food and plants", "Essen und Pflanzen", "Jidlo a rostliny" },
    { "Magia", "Magic", "Magie", "Magie" },
    { "Listy i dokumenty", "Letters and documents", "Briefe und Dokumente", "Dopisy a dokumenty" },
    { "Z\xB3oto", "Gold", "Gold", "Zlato" },
    { "Postacie", "Characters", "Figuren", "Postavy" },
    { "Ludzie", "People", "Menschen", "Lide" },
    { "Potwory", "Monsters", "Monster", "Monstra" },
    { "Wrogowie", "Enemies", "Feinde", "Nepratele" },
    { "Przyjaciele", "Friends", "Freunde", "Pratele" },
    { "Inne", "Other", "Sonstiges", "Ostatni" },
    { "Budowanie mapy...", "Building map...", "Karte wird aufgebaut...", "Stavim mapu..." },
    { "Budowanie mapy \x9Cwiata: ", "Building world map: ", "Weltkarte wird aufgebaut: ", "Stavim mapu sveta: " },
    { "Szczeg\xF3\xB3y: ", "Details: ", "Details: ", "Detaily: " },
    { ", ok. ", ", ~", ", ca. ", ", cca " },
    { "Gra wstrzymana, a\xBF mapa b\xEA" "dzie gotowa", "Game paused until the map is ready", "Spiel pausiert, bis die Karte fertig ist", "Hra stoji, dokud neni mapa hotova" },
    { "Mapa \x9Cwiata: zbuduj\xEA po rozmowie", "World map: will build after the talk", "Weltkarte: wird nach dem Gespraech aufgebaut", "Mapa sveta: postavim po rozhovoru" },
    { "D\x9Fwignia / prze\xB3\xB9" "cznik", "Lever / switch", "Hebel / Schalter", "Paka / spinac" },
    { "Ko\xB3owr\xF3t", "Winch", "Winde", "Navijak" },
    { "Co\x9C uruchamia", "Triggers something", "Loest etwas aus", "Neco spousti" },
    { "Stan: przestawiony", "State: switched", "Zustand: umgelegt", "Stav: prepnuto" },
    { "Stan: nieprzestawiony", "State: not switched", "Zustand: nicht umgelegt", "Stav: neprepnuto" },
    { "Kapliczka / pos\xB9g", "Shrine / statue", "Schrein / Statue", "Svatyne / socha" },
    { "Tablica / ksi\xEAga", "Board / book", "Tafel / Buch", "Tabule / kniha" },
    { "Ku\x9Fnia", "Smithy", "Schmiede", "Kovarna" },
    { "Z\xB3o\xBF" "e / zbieranie", "Ore vein / gathering", "Erzader / Sammeln", "Zila rudy / sber" },
    { "Interakcja", "Interaction", "Interaktion", "Interakce" },
    { "Miejsce odpoczynku", "Rest spot", "Rastplatz", "Misto k odpocinku" },
    { "Miejsca", "Places", "Orte", "Mista" },
    { "Skarb do wykopania", "Buried treasure", "Vergrabener Schatz", "Zakopany poklad" },
    { "\xA3\xF3\xBFko", "Bed", "Bett", "Postel" },
    { "Krzes\xB3o", "Chair", "Stuhl", "Zidle" },
    { "\xA3" "awka", "Bench", "Bank", "Lavice" },
    { "Fotel / tron", "Armchair / throne", "Sessel / Thron", "Kreslo / trun" },
    { "Ognisko / \x9Cwiat\xB3o", "Fire / light", "Feuer / Licht", "Ohniste / svetlo" },
    { "Przej\x9C" "cie", "Exit", "Uebergang", "Pruchod" },
    { "Do:", "To:", "Nach:", "Do:" },
    { "MAPA", "MAP", "KARTE", "MAPA" },
    { "Handlarze", "Traders", "Haendler", "Obchodnici" },
    { "Handlarz", "Trader", "Haendler", "Obchodnik" },
    { "Zw\xB3oki", "Corpse", "Leiche", "Mrtvola" },
    { "Towar", "Goods", "Waren", "Zbozi" },
    { "Do zabrania", "Loot", "Beute", "K sebrani" },
    { "Ekwipunek", "Inventory", "Inventar", "Inventar" },
    { "Brak", "None", "Nichts", "Nic" },
    { "Ruchy wytrycha: ", "Lockpick moves: ", "Dietrich-Zuege: ", "Tahy paklicem: " },
    { "Kombinacja: ", "Combination: ", "Kombination: ", "Kombinace: " },
    { "Klucz: ", "Key: ", "Schluessel: ", "Klic: " },
    { "Rodzaj\xF3w: ", "Kinds: ", "Arten: ", "Druhu: " },
    { "sztuk: ", "pieces: ", "Stueck: ", "kusu: " },
    { "Nic w tej kategorii", "Nothing in this category", "Nichts in dieser Kategorie", "Nic v teto kategorii" },
    { "A/D kategoria, W/S wyb\xF3r", "A/D category, W/S select", "A/D Kategorie, W/S Wahl", "A/D kategorie, W/S vyber" },
    { "Teleport: ", "Teleport: ", "Teleport: ", "Teleport: " },
    { "Odblokowany cel - teleport run\xB9 w grze", "Unlocked destination - use the rune in game", "Freigeschaltetes Ziel - Rune im Spiel benutzen", "Odemceny cil - pouzij runu ve hre" },
    { "Eksport mapy: ", "Exporting map: ", "Karte wird exportiert: ", "Export mapy: " },
    { "Zapisano: ", "Saved: ", "Gespeichert: ", "Ulozeno: " },
    { "Eksport nieudany (szczeg\xF3\xB3y w MiniMap.log)", "Export failed (details in MiniMap.log)", "Export fehlgeschlagen (siehe MiniMap.log)", "Export se nezdaril (viz MiniMap.log)" },
    { "Gra wstrzymana. Esc przerywa.", "Game paused. Esc cancels.", "Spiel pausiert. Esc bricht ab.", "Hra stoji. Esc rusi." },
    { "Eksport ruszy po wyj\x9C" "ciu z menu", "Export starts after leaving the menu", "Export startet nach Verlassen des Menues", "Export zacne po opusteni menu" },
    { "Alchemia", "Alchemy", "Alchemie", "Alchymie" },
    { "Kuchnia", "Kitchen", "Kueche", "Kuchyne" },
    { "Warsztat", "Workshop", "Werkstatt", "Dilna" },
    { "Drabina", "Ladder", "Leiter", "Zebrik" },
    { "Teleport", "Teleport", "Teleport", "Teleport" },
    { "St\xF3\xB3 runiczny", "Rune table", "Runentisch", "Runovy stul" },
    { "Pulpit do zwoj\xF3w", "Scroll desk", "Schreibpult", "Pult na svitky" },
    { "Demonologia", "Demonology", "Daemonologie", "Demonologie" },
  };
}
namespace GOTHIC_NAMESPACE {
namespace MiniMap {
  using namespace MiniMapCommon;
  static int g_lang = LANG_PL;
  static int  g_textFontPL = -1;
  static char g_textFold[TX_COUNT][96];
  static bool g_textFoldHave[TX_COUNT];
  inline void MenuFoldPL( char* s );
  static const char* g_textTxt[TX_COUNT];
  static int         g_textTxtN = 0;
  inline void LangTxtEnsure();
  inline const char* T( int id ) {
    if( id < 0 || id >= TX_COUNT ) return "?";
    LangTxtEnsure();
    if( g_textTxtN > 0 && g_textTxt[id] ) return g_textTxt[id];
    const int l = ( g_lang >= 0 && g_lang < LANG_COUNT ) ? g_lang : LANG_PL;
    if( l == LANG_PL && g_textFontPL == 0 ) {
      if( !g_textFoldHave[id] ) {
        strncpy_s( g_textFold[id], sizeof( g_textFold[id] ), TEXT[id][l] ? TEXT[id][l] : "", _TRUNCATE );
        MenuFoldPL( g_textFold[id] );
        g_textFoldHave[id] = true;
      }
      return g_textFold[id];
    }
    return TEXT[id][l];
  }
}
}
namespace MiniMapCommon {
  struct TMenuLine {
    const char* t[LANG_COUNT];
  };
  struct TMenuText {
    const char* item;
    TMenuLine   text0;
    TMenuLine   text1;
  };
  inline constexpr TMenuText MENU_TR[] = {
    { "MENUITEM_UNION_AUTO_MINIMAP", { 0, "MiniMap", "MiniMap", "MiniMap" }, { 0, "Minimap and full-screen world map.", "Minikarte und Weltkarte im Vollbild.", "Minimapa a velka mapa sveta." } },
    { "MM_TITLE", { 0, "MiniMap - SETTINGS", "MiniMap - EINSTELLUNGEN", "MiniMap - NASTAVENI" } },
    { "MM_ENABLED", { 0, "MiniMap", "MiniMap", "MiniMap" }, { 0, "Do you want to turn on the whole minimap plugin?", "Willst du das ganze Minikarten-Plugin einschalten?", "Chces zapnout cely plugin minimapy?" } },
    { "MM_ENABLED_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_SHAPE", { 0, "Minimap shape", "Form der Minikarte", "Tvar minimapy" }, { 0, "A square shows a little more terrain than a circle.", "Das Quadrat zeigt etwas mehr Gelaende als der Kreis.", "Ctverec ukaze o neco vic terenu nez kruh." } },
    { "MM_SHAPE_VAL", { 0, "Square|Circle", "Quadrat|Kreis", "Ctverec|Kruh" } },
    { "MM_SIZE", { 0, "Minimap size", "Groesse der Minikarte", "Velikost minimapy" }, { 0, "Minimap side as a percentage of screen height.", "Seitenlaenge in Prozent der Bildschirmhoehe.", "Strana minimapy v procentech vysky obrazovky." } },
    { "MM_SIZE_VAL", { 0, "10%|12%|14%|16%|18%|20%|22%|25%|28%|32%|36%|40%", "10%|12%|14%|16%|18%|20%|22%|25%|28%|32%|36%|40%", "10%|12%|14%|16%|18%|20%|22%|25%|28%|32%|36%|40%" } },
    { "MM_CORNER", { 0, "Minimap position", "Position der Minikarte", "Poloha minimapy" }, { 0, "Which corner of the screen holds the minimap.", "In welcher Bildschirmecke die Minikarte sitzt.", "Ve kterem rohu obrazovky je minimapa." } },
    { "MM_CORNER_VAL", { 0, "Top right|Top left|Bottom right|Bottom left", "Oben rechts|Oben links|Unten rechts|Unten links", "Vpravo nahore|Vlevo nahore|Vpravo dole|Vlevo dole" } },
    { "MM_ROTATE", { 0, "Minimap rotation", "Drehung der Minikarte", "Otaceni minimapy" }, { 0, "Follow camera: the minimap turns with the hero.", "Mit Kamera: die Minikarte dreht sich mit dem Helden.", "Podle kamery: minimapa se otaci s hrdinou." } },
    { "MM_ROTATE_VAL", { 0, "North up|Follow camera", "Norden oben|Mit Kamera", "Sever nahore|Podle kamery" } },
    { "MM_ALIGN", { 0, "Map image rotation", "Kartenbild drehen", "Otoceni obrazu mapy" }, { 0, "Turns the map image.", "Dreht das Kartenbild.", "Otoci obraz mapy." } },
    { "MM_ALIGN_VAL", { 0, "Auto|North|90 Degrees|180 Degrees|270 Degrees", "Auto|Norden|90 Grad|180 Grad|270 Grad", "Auto|Sever|90 Stupnu|180 Stupnu|270 Stupnu" } },
    { "MM_FALIGN", { 0, "Full map rotation", "Grosse Karte drehen", "Otoceni velke mapy" }, { 0, "Same: the full map is turned like the minimap.", "Gleich: grosse Karte gedreht wie die Minikarte.", "Stejne: velka mapa otocena jako minimapa." } },
    { "MM_FALIGN_VAL", { 0, "Same|North|90 Degrees|180 Degrees|270 Degrees", "Gleich|Norden|90 Grad|180 Grad|270 Grad", "Stejne|Sever|90 Stupnu|180 Stupnu|270 Stupnu" } },
    { "MM_GOTO_ICONS", { 0, "Icons and colours ...", "Symbole und Farben ...", "Ikony a barvy ..." }, { 0, "Look, size and colours of icons on both maps.", "Aussehen, Groesse und Farben der Symbole.", "Vzhled, velikost a barvy ikon na obou mapach." } },
    { "MM_I_TITLE", { 0, "MiniMap - ICONS AND COLOURS", "MiniMap - SYMBOLE UND FARBEN", "MiniMap - IKONY A BARVY" } },
    { "MM_ICONSET", { 0, "Icon look", "Symbolaussehen", "Vzhled ikon" }, { 0, "Simple: old drawings. Own: your .png files.", "Einfach: alte Zeichnungen. Eigene: deine .png.", "Jednoduche: stare kresby. Vlastni: tvoje .png." } },
    { "MM_ICONSET_VAL", { 0, "Simple|Framed|No frame|Own", "Einfach|Mit Rahmen|Ohne Rahmen|Eigene", "Jednoduche|S ramem|Bez ramu|Vlastni" } },
    { "MM_SIZENPC", { 0, "Character size", "Figurengroesse", "Velikost postav" }, { 0, "Size of people, monster and animal icons.", "Groesse der Symbole fuer Menschen und Monster.", "Velikost ikon lidi, nestvur a zvirat." } },
    { "MM_SIZENPC_VAL", { 0, "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%", "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%", "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%" } },
    { "MM_SIZEITEM", { 0, "Item size", "Gegenstandsgroesse", "Velikost predmetu" }, { 0, "Size of icons of items lying on the ground.", "Groesse der Symbole liegender Gegenstaende.", "Velikost ikon predmetu lezicich na zemi." } },
    { "MM_SIZEITEM_VAL", { 0, "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%", "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%", "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%" } },
    { "MM_SIZEMARK", { 0, "Marker size", "Markergroesse", "Velikost znacek" }, { 0, "Chests, places, furniture, levers, teleports.", "Truhen, Orte, Moebel, Hebel, Teleporter.", "Truhly, mista, nabytek, paky, teleporty." } },
    { "MM_SIZEMARK_VAL", { 0, "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%", "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%", "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%" } },
    { "MM_I_HINT", { 0, "Own: replace .png files in the game folder MiniMap/icons/custom", "Eigene: .png-Dateien im Spielordner MiniMap/icons/custom ersetzen", "Vlastni: nahrad soubory .png ve slozce hry MiniMap/icons/custom" } },
    { "MM_MINISTYLE", { 0, "Minimap style", "Stil der Minikarte", "Styl minimapy" }, { 0, "Minimap frame: classic, parchment, iron or gold.", "Rahmen der Minikarte: klassisch, Pergament, Eisen, Gold.", "Ramecek minimapy: klasicky, pergamen, zelezo, zlato." } },
    { "MM_MINISTYLE_VAL", { 0, "Classic|Parchment|Iron|Gold", "Klassisch|Pergament|Eisen|Gold", "Klasicky|Pergamen|Zelezo|Zlato" } },
    { "MM_FULLSIZE", { 0, "Full map size", "Groesse der grossen Karte", "Velikost velke mapy" }, { 0, "Screen share of the full map (100% = default).", "Bildanteil der grossen Karte (100% = Standard).", "Kolik obrazovky zabira velka mapa (100% = vychozi)." } },
    { "MM_FULLSIZE_VAL", { 0, "60%|70%|80%|90%|100%", "60%|70%|80%|90%|100%", "60%|70%|80%|90%|100%" } },
    { "MM_FULLSTYLE", { 0, "Full map style", "Stil der grossen Karte", "Styl velke mapy" }, { 0, "Frame and title plate: classic, parchment, iron, gold.", "Rahmen und Schild: klassisch, Pergament, Eisen, Gold.", "Ramecek a stitek: klasicky, pergamen, zelezo, zlato." } },
    { "MM_FULLSTYLE_VAL", { 0, "Classic|Parchment|Iron|Gold", "Klassisch|Pergament|Eisen|Gold", "Klasicky|Pergamen|Zelezo|Zlato" } },
    { "MM_FULLSCR", { 0, "Full map fills screen", "Grosse Karte im Vollbild", "Velka mapa pres obrazovku" }, { 0, "Full map fills the screen, or set a key in the menu.", "Grosse Karte fuellt den Bildschirm, oder Taste im Menue.", "Velka mapa pres celou obrazovku, nebo klavesa v menu." } },
    { "MM_FULLSCR_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_NPC", { 0, "Characters on the map", "Figuren auf der Karte", "Postavy na mape" }, { 0, "Character detection on the map.", "Erkennung von Figuren auf der Karte.", "Detekce postav na mape." } },
    { "MM_NPC_VAL", { 0, "Hidden|In range|Max", "Aus|In Reichweite|Max", "Skryte|V dosahu|Max" } },
    { "MM_NPCKIND", { 0, "Character type", "Art der Figuren", "Druh postav" }, { 0, "Range of sight: people or monsters (orcs too).", "Sichtweite: Menschen oder Monster (auch Orks).", "Dohled: lide nebo monstra (i orkove)." } },
    { "MM_NPCKIND_VAL", { 0, "Max|People|Monsters|Nobody", "Max|Menschen|Monster|Keine", "Max|Lide|Monstra|Nikdo" } },
    { "MM_ITEMS", { 0, "Item detection", "Gegenstaende erkennen", "Detekce predmetu" }, { 0, "Item detection on the ground.", "Erkennung von Gegenstaenden am Boden.", "Detekce predmetu na zemi." } },
    { "MM_ITEMS_VAL", { 0, "Hidden|In range|Max", "Aus|In Reichweite|Max", "Skryte|V dosahu|Max" } },
    { "MM_SCALE", { 0, "Scale under the minimap", "Massstab unter Minikarte", "Meritko pod minimapou" }, { 0, "How many metres the minimap shows (e.g. 60 m).", "Wie viele Meter die Minikarte zeigt (z. B. 60 m).", "Kolik metru minimapa ukazuje (napr. 60 m)." } },
    { "MM_SCALE_VAL", { 0, "Hidden|Always|On zoom", "Aus|Immer|Beim Zoomen", "Skryte|Vzdy|Pri zoomu" } },
    { "MM_GOTO_GEN", { 0, "Terrain appearance ...", "Aussehen des Gelaendes ...", "Vzhled terenu ..." }, { 0, "Sharpness, shading, colours, trees, caves and fog.", "Schaerfe, Schatten, Farben, Baeume, Hoehlen, Nebel.", "Ostrost, stinovani, barvy, stromy, jeskyne a mlha." } },
    { "MM_GOTO_KEYS", { 0, "Keys ...", "Tasten ...", "Klavesy ..." }, { 0, "Keys for both maps: open, zoom, search.", "Tasten fuer beide Karten: Oeffnen, Zoom, Suche.", "Klavesy obou map: otevreni, zoom, hledani." } },
    { "MM_GOTO_LOOK", { 0, "Minimap ...", "Minikarte ...", "Minimapa ..." }, { 0, "Shape, size, position, style and rotation.", "Form, Groesse, Position, Stil und Drehung.", "Tvar, velikost, poloha, styl a otoceni minimapy." } },
    { "MM_GOTO_SHOW", { 0, "What the map shows ...", "Was die Karte zeigt ...", "Co ukazat na mape ..." }, { 0, "Characters, items, chests, places, teleports.", "Figuren, Gegenstaende, Truhen, Orte, Teleporter.", "Postavy, predmety, truhly, mista, teleporty." } },
    { "MM_GOTO_FULL", { 0, "Full map ...", "Grosse Karte ...", "Velka mapa ..." }, { 0, "Size, style, rotation, mouse and own markers.", "Groesse, Stil, Drehung, Maus und Markierungen.", "Velikost, styl, otoceni, mys a vlastni znacky." } },
    { "MM_L_TITLE", { 0, "MiniMap - MINIMAP", "MiniMap - MINIKARTE", "MiniMap - MINIMAPA" } },
    { "MM_S_TITLE", { 0, "MiniMap - WHAT THE MAP SHOWS", "MiniMap - WAS DIE KARTE ZEIGT", "MiniMap - CO UKAZAT NA MAPE" } },
    { "MM_F_TITLE", { 0, "MiniMap - FULL MAP", "MiniMap - GROSSE KARTE", "MiniMap - VELKA MAPA" } },
    { "MM_VERSION", { 0, "Version: 2.0a", "Version: 2.0a", "Verze: 2.0a" } },
    { "MM_ITEMCOL", { 0, "Item colours", "Farben der Gegenstaende", "Barvy predmetu" }, { 0, "Item type by colour: weapons, potions, plants...", "Art als Farbe: Waffen, Traenke, Pflanzen...", "Druh predmetu barvou: zbrane, lektvary, rostliny..." } },
    { "MM_ITEMCOL_VAL", { 0, "Uniform|Coloured", "Einfarbig|Bunt", "Jednobarevne|Barevne" } },
    { "MM_GOTO_COLORS", { 0, "Map colours ...", "Farben der Karte ...", "Barvy na mape ..." }, { 0, "Colours of characters and items on the map.", "Farben von Figuren und Gegenstaenden.", "Barvy postav a predmetu na mape." } },
    { "MM_C_TITLE", { 0, "MiniMap - MAP COLOURS", "MiniMap - FARBEN DER KARTE", "MiniMap - BARVY NA MAPE" } },
    { "MM_C_ENEMY", { 0, "Enemies", "Feinde", "Nepratele" }, { 0, "Colour of hostile characters and monsters.", "Farbe feindlicher Figuren und Monster.", "Barva nepratelskych postav a priser." } },
    { "MM_C_ENEMY_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_NEUTRAL", { 0, "Neutral", "Neutrale", "Neutralni" }, { 0, "Colour of neutral characters and animals.", "Farbe neutraler Figuren und Tiere.", "Barva neutralnich postav a zvirat." } },
    { "MM_C_NEUTRAL_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_FRIEND", { 0, "Friends", "Freunde", "Pratele" }, { 0, "Colour of friendly characters.", "Farbe freundlicher Figuren.", "Barva pratelskych postav." } },
    { "MM_C_FRIEND_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_PLAYER", { 0, "Hero", "Held", "Hrdina" }, { 0, "Colour of the hero arrow.", "Farbe des Heldenpfeils.", "Barva sipky hrdiny." } },
    { "MM_C_PLAYER_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_TRADER", { 0, "Traders", "Haendler", "Obchodnici" }, { 0, "Colour of the trader scale.", "Farbe der Haendlerwaage.", "Barva vahy obchodnika." } },
    { "MM_C_TRADER_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_CORPSE", { 0, "Corpses with loot", "Leichen mit Beute", "Mrtvoly s koristi" }, { 0, "Colour of the cross on corpses with loot.", "Farbe des Kreuzes bei Leichen mit Beute.", "Barva krize u mrtvol s koristi." } },
    { "MM_C_CORPSE_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_ITEM1H", { 0, "One-handed weapons", "Einhandwaffen", "Jednorucni zbrane" }, { 0, "One-handed swords, axes and maces.", "Einhandschwerter, -aexte und Keulen.", "Jednorucni mece, sekery a palcaty." } },
    { "MM_C_ITEM1H_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_ITEM2H", { 0, "Two-handed weapons", "Zweihandwaffen", "Obourucni zbrane" }, { 0, "Two-handed swords, axes and staffs.", "Zweihandschwerter, -aexte und Staebe.", "Obourucni mece, sekery a hole." } },
    { "MM_C_ITEM2H_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_RANGED", { 0, "Bows and crossbows", "Boegen und Armbrueste", "Luky a kuse" }, { 0, "Bows, crossbows, arrows and bolts.", "Boegen, Armbrueste, Pfeile und Bolzen.", "Luky, kuse, sipy a sipky." } },
    { "MM_C_RANGED_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_POTION", { 0, "Potions", "Traenke", "Lektvary" }, { 0, "Colour of potions.", "Farbe der Traenke.", "Barva lektvaru." } },
    { "MM_C_POTION_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_PLANT", { 0, "Plants and food", "Pflanzen und Essen", "Rostliny a jidlo" }, { 0, "Herbs, mushrooms and food.", "Kraeuter, Pilze und Essen.", "Byliny, houby a jidlo." } },
    { "MM_C_PLANT_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_C_MAGIC", { 0, "Magic", "Magie", "Magie" }, { 0, "Runes, scrolls, amulets and rings.", "Runen, Schriftrollen, Amulette, Ringe.", "Runy, svitky, amulety a prsteny." } },
    { "MM_C_MAGIC_VAL", { 0, "Default|Red|Orange|Yellow|Green|Turquoise|Blue|Purple|Pink|White|Grey|Brown", "Standard|Rot|Orange|Gelb|Gruen|Tuerkis|Blau|Violett|Rosa|Weiss|Grau|Braun", "Vychozi|Cervena|Oranzova|Zluta|Zelena|Tyrkysova|Modra|Fialova|Ruzova|Bila|Seda|Hneda" } },
    { "MM_FURNITURE", { 0, "Furniture", "Moebel", "Nabytek" }, { 0, "Beds, chairs, benches, armchairs and thrones.", "Betten, Stuehle, Baenke, Sessel und Throne.", "Postele, zidle, lavice, kresla a truny." } },
    { "MM_FURNITURE_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_CRAFT", { 0, "Smithies", "Schmieden", "Kovarny" }, { 0, "Anvils, forges, grindstones, quench buckets.", "Ambosse, Essen, Schleifsteine, Kuehleimer.", "Kovadla, vyhne, brusy, kbeliky na kaleni." } },
    { "MM_CRAFT_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ALCHEMY", { 0, "Alchemy", "Alchemie", "Alchymie" }, { 0, "Alchemy benches and herb stompers.", "Alchemietische und Kraeuterstampfer.", "Alchymisticke stoly a drtice bylin." } },
    { "MM_ALCHEMY_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_RUNES", { 0, "Rune tables", "Runentische", "Runove stoly" }, { 0, "Tables to make runes and transform magic.", "Tische fuer Runen und Magiewandlung.", "Stoly na runy a premenu magie." } },
    { "MM_RUNES_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_SCROLLS", { 0, "Scroll desks", "Schreibpulte", "Pulty na svitky" }, { 0, "Desks to write magic scrolls.", "Pulte zum Schreiben von Spruchrollen.", "Pulty na psani kouzelnych svitku." } },
    { "MM_SCROLLS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_DEMON", { 0, "Demonology", "Daemonologie", "Demonologie" }, { 0, "Demonologist tables.", "Tische der Daemonenbeschwoerer.", "Stoly vyvolavacu demonu." } },
    { "MM_DEMON_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_KITCHEN", { 0, "Kitchens", "Kuechen", "Kuchyne" }, { 0, "Stoves, cauldrons, pans, roasting fires.", "Herde, Kessel, Pfannen, Bratfeuer.", "Sporaky, kotle, panve, ohniste na peceni." } },
    { "MM_KITCHEN_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_WORKSHOP", { 0, "Workshops", "Werkstaetten", "Dilny" }, { 0, "Saws, bows, tanner, goldsmith, maps, wine.", "Saegen, Boegen, Gerber, Goldschmied, Wein.", "Pily, luky, kozeluh, zlatnik, mapy, vino." } },
    { "MM_WORKSHOP_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_FIRE", { 0, "Fire and light", "Feuer und Licht", "Ohen a svetlo" }, { 0, "Torches, fireplaces and plain campfires.", "Fackeln, Kamine und einfache Lagerfeuer.", "Pochodne, krby a obycejna ohniste." } },
    { "MM_FIRE_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_CLUTTER", { 0, "Clutter", "Kleinkram", "Drobnosti" }, { 0, "Barrels, wells, planks to repair and the rest.", "Faesser, Brunnen, Bretter zum Flicken, Rest.", "Sudy, studny, prkna k oprave a zbytek." } },
    { "MM_CLUTTER_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_LADDERS", { 0, "Ladders", "Leitern", "Zebriky" }, { 0, "Ladders you can climb.", "Leitern, auf die man klettern kann.", "Zebriky, po kterych se da vylezt." } },
    { "MM_LADDERS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_GOTO_PLACES", { 0, "Places on the map ...", "Orte auf der Karte ...", "Mista na mape ..." }, { 0, "Furniture, smithies, alchemy, kitchens, fire.", "Moebel, Schmieden, Alchemie, Kuechen, Feuer.", "Nabytek, kovarny, alchymie, kuchyne, ohen." } },
    { "MM_P_TITLE", { 0, "MiniMap - PLACES ON THE MAP", "MiniMap - ORTE AUF DER KARTE", "MiniMap - MISTA NA MAPE" } },
    { "MM_CHESTS", { 0, "Chest detection on map", "Truhen auf Karte erkennen", "Detekce truhel na mape" }, { 0, "Shows all chests on the map, depending on range.", "Zeigt alle Truhen auf der Karte, je nach Reichweite.", "Ukaze vsechny truhly na mape podle dosahu." } },
    { "MM_CHESTS_VAL", { 0, "Hidden|In range|Max", "Aus|In Reichweite|Max", "Skryte|V dosahu|Max" } },
    { "MM_AUTHOR", { 0, "Author: PEN996", "Autor: PEN996", "Autor: PEN996" } },
    { "MM_LANG", { 0, "Language", "Sprache / Language", "Jazyk / Language" }, { 0, "Menu and map texts.", "Texte in Menue und Karte.", "Texty v menu a na mape." } },
    { "MM_LANG_VAL", { 0, "Polski|English|Deutsch|Cestina", "Polski|English|Deutsch|Cestina", "Polski|English|Deutsch|Cestina" } },
    { "MM_LOG", { 0, "Log file", "Log-Datei", "Zapis logu" }, { 0, "Create MiniMap.log?", "MiniMap.log erstellen?", "Vytvaret MiniMap.log?" } },
    { "MM_LOG_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_G_TITLE", { 0, "MiniMap - TERRAIN APPEARANCE", "MiniMap - AUSSEHEN DES GELAENDES", "MiniMap - VZHLED TERENU" } },
    { "MM_DETAIL", { 0, "Detail level", "Detailgrad", "Uroven detailu" }, { 0, "Sharpness when zoomed in. Higher takes longer.", "Schaerfe beim Zoomen. Hoeher dauert laenger.", "Ostrost pri priblizeni. Vyssi trva dele." } },
    { "MM_DETAIL_VAL", { 0, "Low|Normal|Maximum", "Niedrig|Normal|Maximal", "Nizka|Normalni|Maximalni" } },
    { "MM_SCULPT", { 0, "Mountain shading", "Bergschattierung", "Stinovani hor" }, { 0, "Ridges lighter, valleys darker - mountains stand out.", "Grate heller, Taeler dunkler - Berge treten hervor.", "Hrebeny svetlejsi, udoli tmavsi - hory vyniknou." } },
    { "MM_SCULPT_VAL", { 0, "Light|Clear|Strong", "Leicht|Deutlich|Stark", "Slabe|Vyrazne|Silne" } },
    { "MM_SMOOTH", { 0, "Terrain smoothing", "Gelaende glaetten", "Vyhlazeni terenu" }, { 0, "Smoother terrain, fewer details. Recommended: None.", "Glatter, aber weniger Details. Empfohlen: Aus.", "Hladsi teren, mene detailu. Doporuceno: Zadne." } },
    { "MM_SMOOTH_VAL", { 0, "None|Light|Normal", "Aus|Leicht|Normal", "Zadne|Lehke|Normalni" } },
    { "MM_FILL", { 0, "Fill terrain holes", "Loecher fuellen", "Zaplnit diry" }, { 0, "Covers empty spots on the map with nearby terrain.", "Fuellt leere Stellen mit Gelaende aus der Naehe.", "Prazdna mista zakryje okolnim terenem." } },
    { "MM_FILL_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_OUTSIDE", { 0, "Beyond the world edge", "Hinter dem Weltrand", "Za okrajem sveta" }, { 0, "What to draw where the game world ends.", "Was dort erscheint, wo die Spielwelt endet.", "Co kreslit tam, kde svet hry konci." } },
    { "MM_OUTSIDE_VAL", { 0, "Nothing|Sea|Mountains|Like shore", "Nichts|Meer|Berge|Wie Ufer", "Nic|More|Hory|Jako breh" } },
    { "MM_PATHS", { 0, "Paths on the map", "Wege auf der Karte", "Cesty na mape" }, { 0, "Roads the characters walk, shown as light paths.", "Wege der Figuren als helle Pfade.", "Cesty, kudy chodi postavy, jako svetle stezky." } },
    { "MM_PATHS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_INDOOR", { 0, "Interiors and caves", "Innenraeume und Hoehlen", "Interiery a jeskyne" }, { 0, "Under roofs and in caves: inside view.", "Unter Dach, in Hoehlen: Innenansicht.", "Pod strechou a v jeskyni: vnitrek." } },
    { "MM_INDOOR_VAL", { 0, "No|Strong|Normal|Indoors only", "Nein|Stark|Normal|Nur innen", "Ne|Silna|Normalni|Jen interiery" } },
    { "MM_TEXCOL", { 0, "Map colours", "Kartenfarben", "Barvy mapy" }, { 0, "Plain, as in game or natural (livelier green, rocks).", "Schlicht, wie im Spiel oder natuerlich (frisches Gruen).", "Jednoduche, jako ve hre nebo prirodni (zivejsi zelen)." } },
    { "MM_TEXCOL_VAL", { 0, "Plain|As in game|Natural", "Schlicht|Wie im Spiel|Natuerlich", "Jednoduche|Jako ve hre|Prirodni" } },
    { "MM_DSPEED", { 0, "Detail speed", "Detailtempo", "Rychlost detailu" }, { 0, "Faster = sharp map sooner, but fewer FPS meanwhile.", "Schneller = scharfe Karte eher, aber weniger FPS dabei.", "Rychleji = ostra mapa driv, ale mezitim mene FPS." } },
    { "MM_DSPEED_VAL", { 0, "Normal|Faster|Stronger|Maximum|Extreme", "Normal|Schneller|Staerker|Maximal|Extrem", "Normalni|Rychlejsi|Silnejsi|Maximalni|Extremni" } },
    { "MM_DQUAL", { 0, "Detail quality", "Detailqualitaet", "Kvalita detailu" }, { 0, "Sharper buildings, edges and shadows. Takes longer.", "Genauere Gebaeude, Kanten, Schatten. Dauert laenger.", "Presnejsi budovy, hrany a stiny. Trva dele." } },
    { "MM_DQUAL_VAL", { 0, "Normal|High|Very high|Maximum", "Normal|Hoch|Sehr hoch|Maximal", "Normalni|Vysoka|Velmi vysoka|Maximalni" } },
    { "MM_ITEMLIST", { 0, "Item table", "Gegenstandsliste", "Tabulka predmetu" }, { 0, "Full map: list on the left, A/D category, W/S select.", "Grosse Karte: Liste links, A/D Kategorie, W/S Wahl.", "Velka mapa: seznam vlevo, A/D kategorie, W/S vyber." } },
    { "MM_ITEMLIST_VAL", { 0, "Hidden|Visible", "Aus|Sichtbar", "Skryta|Viditelna" } },
    { "MM_TELEPORTS", { 0, "Teleports", "Teleporter", "Teleporty" }, { 0, "Teleport destinations on the map.", "Teleportziele auf der Karte.", "Cile teleportu na mape." } },
    { "MM_TELEPORTS_VAL", { 0, "Hidden|Visible", "Aus|Sichtbar", "Skryte|Viditelne" } },
    { "MM_FOG", { 0, "Fog of war", "Kartennebel", "Mlha na mape" }, { 0, "The map reveals only where you have been.", "Die Karte zeigt nur, wo du schon warst.", "Mapa se odkryva jen tam, kde uz jsi byl." } },
    { "MM_FOG_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_X_TITLE", { 0, "MiniMap - TOOLTIPS AND SEARCH", "MiniMap - INFOS UND SUCHE", "MiniMap - NAPOVEDA A HLEDANI" } },
    { "MM_E_TITLE", { 0, "MiniMap - MAP EXPORT", "MiniMap - KARTENEXPORT", "MiniMap - EXPORT MAPY" } },
    { "MM_GOTO_EXTRA", { 0, "Tooltips and search ...", "Infos und Suche ...", "Napoveda a hledani ..." }, { 0, "Info under the cursor, search and item table.", "Info unter dem Zeiger, Suche, Gegenstandsliste.", "Popis pod kurzorem, hledani a tabulka predmetu." } },
    { "MM_GOTO_EXP", { 0, "Map export ...", "Kartenexport ...", "Export mapy ..." }, { 0, "Save the world map to an image file (JPG, PNG, BMP).", "Weltkarte als Bilddatei (JPG, PNG, BMP) speichern.", "Ulozit mapu sveta do obrazku (JPG, PNG, BMP)." } },
    { "MM_EXPZOOM", { 0, "Map zoom", "Kartenzoom", "Zoom mapy" }, { 0, "Bigger zoom = more pixels and a bigger file.", "Mehr Zoom = mehr Pixel und groessere Datei.", "Vetsi zoom = vic pixelu a vetsi soubor." } },
    { "MM_EXPZOOM_VAL", { 0, "1x|2x|4x", "1x|2x|4x", "1x|2x|4x" } },
    { "MM_EXPFMT", { 0, "File format", "Dateiformat", "Format souboru" }, { 0, "JPG smallest, PNG and BMP lossless (BMP is huge).", "JPG am kleinsten, PNG und BMP verlustfrei (BMP riesig).", "JPG nejmensi, PNG a BMP bezztratove (BMP obrovsky)." } },
    { "MM_EXPFMT_VAL", { 0, "BMP|PNG|JPG", "BMP|PNG|JPG", "BMP|PNG|JPG" } },
    { "MM_EXPQ", { 0, "JPG quality", "JPG-Qualitaet", "Kvalita JPG" }, { 0, "Higher = sharper icons and a bigger file.", "Hoeher = schaerfere Symbole, groessere Datei.", "Vyssi = ostrejsi ikony a vetsi soubor." } },
    { "MM_EXPQ_VAL", { 0, "70%|80%|90%|95%", "70%|80%|90%|95%", "70%|80%|90%|95%" } },
    { "MM_GOTO_EXPDET", { 0, "Map details ...", "Kartendetails ...", "Detaily na mape ..." }, { 0, "What to save with the map: chests, NPCs, enemies...", "Was mit der Karte: Truhen, Figuren, Gegner...", "Co ulozit s mapou: truhly, postavy, nepratele..." } },
    { "MM_D_TITLE", { 0, "MiniMap - EXPORT DETAILS", "MiniMap - EXPORTDETAILS", "MiniMap - DETAILY EXPORTU" } },
    { "MM_ED_CHESTS", { 0, "Chests", "Truhen", "Truhly" }, { 0, "Chests and containers in lock colour.", "Truhen und Behaelter in Schlossfarbe.", "Truhly a nadoby v barve zamku." } },
    { "MM_ED_CHESTS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_ITEMS", { 0, "Items", "Gegenstaende", "Predmety" }, { 0, "Items lying in the world (no plants).", "Gegenstaende in der Welt (ohne Pflanzen).", "Predmety lezici ve svete (bez rostlin)." } },
    { "MM_ED_ITEMS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_PLANTS", { 0, "Plants", "Pflanzen", "Rostliny" }, { 0, "Herbs and food lying in the world.", "Kraeuter und Essen in der Welt.", "Byliny a jidlo lezici ve svete." } },
    { "MM_ED_PLANTS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_NPCS", { 0, "Neutral characters", "Neutrale Figuren", "Neutralni postavy" }, { 0, "Neutral characters (white head) and traders (scales).", "Neutrale Figuren (weisser Kopf) und Haendler (Waage).", "Neutralni postavy (bila hlava) a obchodnici (vaha)." } },
    { "MM_ED_NPCS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_FOES", { 0, "Hostile characters", "Feindliche Figuren", "Nepratelske postavy" }, { 0, "Enemies and monsters (skull), state at export.", "Gegner und Monster (Schaedel), Stand beim Export.", "Nepratele a monstra (lebka), stav pri exportu." } },
    { "MM_ED_FOES_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_SWITCHES", { 0, "Switches", "Schalter", "Spinace" }, { 0, "Levers, winches, plates.", "Hebel, Winden, Platten.", "Paky, navijaky, desky." } },
    { "MM_ED_SWITCHES_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_PLACES", { 0, "Places", "Orte", "Mista" }, { 0, "Shrines, crafting, ore, treasures.", "Schreine, Handwerk, Erz, Schaetze.", "Svatyne, remesla, ruda, poklady." } },
    { "MM_ED_PLACES_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_EXITS", { 0, "Exits", "Uebergaenge", "Prechody" }, { 0, "Passages to other worlds.", "Uebergaenge in andere Welten.", "Prechody do jinych svetu." } },
    { "MM_ED_EXITS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_TELEPORTS", { 0, "Teleport targets", "Teleportziele", "Cile teleportu" }, { 0, "Rune targets from inventory and teleports in the world.", "Runenziele aus dem Inventar und Teleporter in der Welt.", "Cile run z inventare a teleporty ve svete." } },
    { "MM_ED_TELEPORTS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_MARKS", { 0, "Own markers", "Eigene Markierungen", "Vlastni znacky" }, { 0, "Markers placed on the full map.", "Markierungen auf der grossen Karte.", "Znacky polozene na velke mape." } },
    { "MM_ED_MARKS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_PLAYER", { 0, "Hero", "Held", "Hrdina" }, { 0, "Arrow of the hero.", "Pfeil des Helden.", "Sipka hrdiny." } },
    { "MM_ED_PLAYER_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_ED_FRIENDS", { 0, "Friendly characters", "Freundliche Figuren", "Pratelske postavy" }, { 0, "Friends and companions (two green heads).", "Freunde und Begleiter (zwei gruene Koepfe).", "Pratele a spolecnici (dve zelene hlavy)." } },
    { "MM_ED_FRIENDS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_EXP_GO", { 0, "Confirm export", "Export bestaetigen", "Potvrdit export" }, { 0, "Enter, then leave the menu. Esc cancels.", "Enter, dann Menue verlassen. Esc bricht ab.", "Enter, pak opust menu. Esc rusi." } },
    { "MM_E_HINT", { 0, "File in the game folder: MiniMap/export", "Datei im Spielordner: MiniMap/export", "Soubor ve slozce hry: MiniMap/export" } },
    { "MM_HIDEEMPTY", { 0, "Hide empty", "Leere ausblenden", "Skryt prazdne" }, { 0, "Hides empty chests, barrels, sacks and looted bodies.", "Blendet leere Truhen, Faesser, Saecke, Leichen aus.", "Skryje prazdne truhly, sudy, pytle a vykradena tela." } },
    { "MM_HIDEEMPTY_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_COMBO", { 0, "Lock combination", "Schlosskombination", "Kombinace zamku" }, { 0, "Chest tooltip: lockpick moves or full combination.", "Truhen-Info: Dietrich-Zuege oder ganze Kombination.", "Info truhly: tahy paklicem nebo cela kombinace." } },
    { "MM_COMBO_VAL", { 0, "Hide|Moves|Full", "Aus|Zuege|Voll", "Skryt|Tahy|Plna" } },
    { "MM_DRAGMAP", { 0, "Drag the map", "Karte ziehen", "Tazeni mapy" }, { 0, "Full map: hold the left button and move the map.", "Weltkarte: linke Taste halten und Karte verschieben.", "Velka mapa: drz leve tlacitko a posun mapu." } },
    { "MM_DRAGMAP_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_DBAR", { 0, "Detail bar", "Detailbalken", "Pruh detailu" }, { 0, "Small bar under the minimap while sharp terrain loads.", "Kleiner Balken unter der Minikarte beim Detailaufbau.", "Maly pruh pod minimapou, kdyz se stavi ostry teren." } },
    { "MM_DBAR_VAL", { 0, "Hidden|Visible", "Aus|Sichtbar", "Skryty|Viditelny" } },
    { "MM_TREES", { 0, "Trees on the map", "Baeume auf der Karte", "Stromy na mape" }, { 0, "No trees: no crowns or trees, ground and paths visible.", "Ohne Baeume: keine Kronen, Boden und Wege sichtbar.", "Bez stromu: zadne koruny ani stromy, je videt zem a cesty." } },
    { "MM_TREES_VAL", { 0, "None|Simple|Exact|No trees", "Keine|Einfach|Genau|Ohne Baeume", "Zadne|Jednoduche|Presne|Bez stromu" } },
    { "MM_G_HINT", { 0, "After a change, the map is redrawn when you leave the menu.", "Nach einer Aenderung wird die Karte beim Verlassen neu gezeichnet.", "Po zmene se mapa po opusteni menu prekresli." } },
    { "MM_K_TITLE", { 0, "MiniMap - KEYS", "MiniMap - TASTEN", "MiniMap - KLAVESY" } },
    { "MM_FULLMAP", { 0, "Full map", "Grosse Karte", "Velka mapa" }, { 0, "Map of the whole world on screen, with notes.", "Karte der ganzen Welt im Vollbild, mit Notizen.", "Mapa celeho sveta pres obrazovku, s poznamkami." } },
    { "MM_FULLMAP_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_WHEEL", { 0, "Mouse wheel on minimap", "Mausrad auf Minikarte", "Kolecko na minimape" }, { 0, "The mouse wheel zooms the minimap in and out.", "Das Mausrad zoomt die Minikarte hinein und heraus.", "Kolecko mysi priblizuje a oddaluje minimapu." } },
    { "MM_WHEEL_VAL", { 0, "Nothing|Zoom", "Nichts|Zoom", "Nic|Zoom" } },
    { "MM_WHEELFULL", { 0, "Mouse wheel on full map", "Mausrad auf grosser Karte", "Kolecko na velke mape" }, { 0, "The wheel changes the marker icon or zooms the map.", "Das Rad wechselt das Markierungssymbol oder zoomt.", "Kolecko meni ikonu znacky nebo priblizuje mapu." } },
    { "MM_WHEELFULL_VAL", { 0, "Nothing|Marker icon|Zoom", "Nichts|Symbol|Zoom", "Nic|Ikona znacky|Zoom" } },
    { "MM_FULLCENTER", { 0, "Full map centre", "Mitte der grossen Karte", "Stred velke mapy" }, { 0, "Play area: land in the middle, no empty sea.", "Spielbereich: Land in der Mitte, kein leeres Meer.", "Herni oblast: sous uprostred, bez prazdneho more." } },
    { "MM_FULLCENTER_VAL", { 0, "Whole world|Play area", "Ganze Welt|Spielbereich", "Cely svet|Herni oblast" } },
    { "MM_MARKS", { 0, "Custom markers", "Eigene Markierungen", "Vlastni znacky" }, { 0, "On the full map: left click places, right click deletes.", "Grosse Karte: Linksklick setzt, Rechtsklick loescht.", "Na velke mape: levy klik pridava, pravy maze." } },
    { "MM_MARKS_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_SWITCHES", { 0, "Levers and switches", "Hebel und Schalter", "Paky a spinace" }, { 0, "Levers, winches and plates that open gates or bridges.", "Hebel, Winden und Platten fuer Tore und Bruecken.", "Paky, navijaky a desky, ktere otviraji brany a mosty." } },
    { "MM_SWITCHES_VAL", { 0, "Hidden|In range|Max", "Aus|In Reichweite|Max", "Skryte|V dosahu|Max" } },
    { "MM_PLACES", { 0, "Places and interactions", "Orte und Interaktionen", "Mista a interakce" }, { 0, "Shrines, boards, anvils, ore veins, treasures.", "Schreine, Tafeln, Ambosse, Erzadern, Schaetze.", "Svatyne, tabule, kovadliny, zily rudy, poklady." } },
    { "MM_PLACES_VAL", { 0, "Hidden|In range|Important|Max", "Aus|In Reichweite|Wichtige|Max", "Skryte|V dosahu|Dulezite|Max" } },
    { "MM_HOVER", { 0, "Tooltip under cursor", "Info unter dem Zeiger", "Napoveda pod kurzorem" }, { 0, "On the full map: who or what is under the cursor.", "Grosse Karte: wer oder was unter dem Zeiger ist.", "Na velke mape: kdo nebo co je pod kurzorem." } },
    { "MM_HOVER_VAL", { 0, "No|Max|Characters|Items", "Nein|Max|Figuren|Gegenstaende", "Ne|Max|Postavy|Predmety" } },
    { "MM_HOVERDET", { 0, "Tooltip detail", "Umfang der Infos", "Podrobnost napovedy" }, { 0, "Name only or: HP, mana, guild, life, weapon, price.", "Nur Name oder: LP, Mana, Gilde, Leben, Waffe, Preis.", "Jen jmeno nebo: HP, mana, gilda, zivot, zbran, cena." } },
    { "MM_HOVERDET_VAL", { 0, "Name only|Basic|Max", "Nur Name|Einfach|Max", "Jen jmeno|Zakladni|Max" } },
    { "MM_SEARCHOTH", { 0, "Non-matches in search", "Unpassendes bei der Suche", "Ostatni pri hledani" }, { 0, "What happens to things and people that do not match.", "Was mit Dingen und Figuren passiert, die nicht passen.", "Co delat s vecmi a postavami, ktere nesedi." } },
    { "MM_SEARCHOTH_VAL", { 0, "Hide|Dim", "Ausblenden|Abdunkeln", "Skryt|Ztlumit" } },
    { "MM_FULLPAUSE", { 0, "Pause on full map", "Pause bei grosser Karte", "Pauza na velke mape" }, { 0, "The game stands still while the full map is open.", "Das Spiel ruht, solange die grosse Karte offen ist.", "Hra stoji, dokud je velka mapa otevrena." } },
    { "MM_FULLPAUSE_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_CURSPEED", { 0, "Cursor speed", "Zeigertempo", "Rychlost kurzoru" }, { 0, "How fast the mouse cursor moves on the full map.", "Wie schnell sich der Zeiger auf der Karte bewegt.", "Jak rychle se kurzor hybe po velke mape." } },
    { "MM_CURSPEED_VAL", { 0, "50%|75%|100%|125%|150%|200%", "50%|75%|100%|125%|150%|200%", "50%|75%|100%|125%|150%|200%" } },
    { "MM_GUIDE", { 0, "Arrow to marker", "Pfeil zur Markierung", "Sipka ke znacce" }, { 0, "On the minimap: direction to the chosen marker.", "Auf der Minikarte: Richtung zur gewaehlten Markierung.", "Na minimape: smer k vybrane znacce." } },
    { "MM_GUIDE_VAL", { 0, "No|Arrow|With distance", "Nein|Pfeil|Mit Distanz", "Ne|Sipka|+ Vzdalenost" } },
    { "MM_EXITS", { 0, "Exits to other maps", "Wege in andere Welten", "Vychody do jinych map" }, { 0, "Spots that lead to another game world.", "Stellen, die in eine andere Spielwelt fuehren.", "Mista, odkud vede cesta do jineho sveta hry." } },
    { "MM_EXITS_VAL", { 0, "Hidden|In range|Max", "Aus|In Reichweite|Max", "Skryte|V dosahu|Max" } },
    { "MM_TRADERS", { 0, "Traders on the map", "Haendler auf der Karte", "Obchodnici na mape" }, { 0, "Scales icon on everyone who trades. Goods in the tooltip.", "Waage bei allen, die handeln. Ware in der Info.", "Vaha u kazdeho, kdo obchoduje. Zbozi v napovede." } },
    { "MM_TRADERS_VAL", { 0, "Hidden|In range|Max", "Aus|In Reichweite|Max", "Skryte|V dosahu|Max" } },
    { "MM_CORPSES", { 0, "Corpses on the map", "Leichen auf der Karte", "Mrtvoly na mape" }, { 0, "A cross on corpses that still have loot.", "Kreuz bei Leichen, die noch Beute haben.", "Kriz u mrtvol, u kterych je jeste co vzit." } },
    { "MM_CORPSES_VAL", { 0, "Hidden|In range|Max", "Aus|In Reichweite|Max", "Skryte|V dosahu|Max" } },
    { "MM_PRING", { 0, "Gold ring around hero", "Goldring um den Helden", "Zlaty kruh kolem hrdiny" }, { 0, "A blinking ring around the hero on the full map.", "Blinkender Ring um den Helden auf der grossen Karte.", "Blikajici kruh kolem hrdiny na velke mape." } },
    { "MM_PRING_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_HOVERINV", { 0, "Inventory in tooltip", "Inventar in der Info", "Inventar v napovede" }, { 0, "Shows the item list: traders and corpses, or everyone.", "Zeigt die Liste: Haendler und Leichen oder alle.", "Ukaze seznam veci: obchodnici a mrtvoly, nebo vsichni." } },
    { "MM_HOVERINV_VAL", { 0, "No|Traders|Max", "Nein|Haendler|Max", "Ne|Obchodnici|Max" } },
    { "MM_SEARCHINV", { 0, "Search inventories", "Suche im Inventar", "Hledani v inventari" }, { 0, "Find characters by an item they carry.", "Figuren nach einem Gegenstand finden, den sie tragen.", "Najde postavu podle predmetu, ktery nosi." } },
    { "MM_SEARCHINV_VAL", { 0, "No|Traders|Max", "Nein|Haendler|Max", "Ne|Obchodnici|Max" } },
    { "MM_EDGESCROLL", { 0, "Scroll at window edge", "Rand-Bildlauf", "Posun u okraje" }, { 0, "Map moves when the cursor touches the window edge.", "Karte bewegt sich am Fensterrand.", "Mapa se posune, kdyz kurzor dotkne okraje okna." } },
    { "MM_EDGESCROLL_VAL", { 0, "No|Yes", "Nein|Ja", "Ne|Ano" } },
    { "MM_KEYFULL", { 0, "Full map", "Grosse Karte", "Velka mapa" }, { 0, "Opens and closes the full map.", "Oeffnet und schliesst die grosse Karte.", "Otevre a zavre velkou mapu." } },
    { "MM_KEYIN", { 0, "Zoom in", "Hineinzoomen", "Priblizeni" }, { 0, "Zooms in on the minimap and full map.", "Zoomt Minikarte und grosse Karte heran.", "Priblizi minimapu i velkou mapu." } },
    { "MM_KEYOUT", { 0, "Zoom out", "Herauszoomen", "Oddaleni" }, { 0, "Zooms out the minimap and full map.", "Zoomt Minikarte und grosse Karte heraus.", "Oddali minimapu i velkou mapu." } },
    { "MM_KEYTOGGLE", { 0, "Show / hide minimap", "Minikarte ein/aus", "Ukazat / skryt minimapu" }, { 0, "Hides and shows the minimap.", "Blendet die Minikarte aus und ein.", "Skryje a ukaze minimapu." } },
    { "MM_KEYSIZEUP", { 0, "Bigger minimap", "Groessere Minikarte", "Vetsi minimapa" }, { 0, "Makes the minimap window bigger.", "Vergroessert das Fenster der Minikarte.", "Zvetsi okno minimapy." } },
    { "MM_KEYSIZEDN", { 0, "Smaller minimap", "Kleinere Minikarte", "Mensi minimapa" }, { 0, "Makes the minimap window smaller.", "Verkleinert das Fenster der Minikarte.", "Zmensi okno minimapy." } },
    { "MM_KEYROTATE", { 0, "Rotate minimap", "Minikarte drehen", "Otaceni minimapy" }, { 0, "Switches: north up or follow camera.", "Wechselt: Norden oben oder mit Kamera.", "Prepne: sever nahore nebo podle kamery." } },
    { "MM_KEYALIGN", { 0, "Rotate map image", "Kartenbild drehen", "Otoceni obrazu mapy" }, { 0, "Turns (and mirrors) the map image, step by step.", "Dreht (und spiegelt) das Kartenbild, Schritt fuer Schritt.", "Otaci (i zrcadli) obraz mapy, krok za krokem." } },
    { "MM_KEYICON", { 0, "Marker icon", "Markierungssymbol", "Ikona znacky" }, { 0, "Changes the marker icon on the full map.", "Wechselt das Markierungssymbol auf der Karte.", "Meni ikonu znacky na velke mape." } },
    { "MM_KEYSEARCH", { 0, "Search", "Suche", "Hledani" }, { 0, "On the full map type a name and press Enter. Esc clears.", "Auf der grossen Karte Namen tippen, Enter. Esc loescht.", "Na velke mape napis jmeno a Enter. Esc maze." } },
    { "MM_KEYFSCR", { 0, "Full screen", "Vollbild", "Cela obrazovka" }, { 0, "Open full map: whole screen or window.", "Offene grosse Karte: Vollbild oder Fenster.", "Otevrena velka mapa: cela obrazovka nebo okno." } },
    { "MM_KEYILIST", { 0, "Item table", "Gegenstandsliste", "Tabulka predmetu" }, { 0, "Open full map: shows and hides the table.", "Offene grosse Karte: Liste zeigen und verbergen.", "Otevrena velka mapa: ukaze a skryje tabulku." } },
    { "MM_K_HINT", { 0, "Enter on a row, then the new key. Delete clears, Esc cancels.", "Enter auf Zeile, neue Taste. Entf loescht, Esc bricht ab.", "Enter na radku, nova klavesa. Delete maze, Esc rusi." } },
  };
  inline constexpr int MENU_TR_COUNT = sizeof( MENU_TR ) / sizeof( MENU_TR[0] );
  inline constexpr const char* const MENU_TR_PL[][2] = {
    { 0, 0 },
    { "MiniMap - USTAWIENIA", 0 },
    { "MiniMap", "Czy chcesz w\xB3\xB9" "czy\xE6 ca\xB3y plugin minimapy?" },
    { "Nie|Tak", 0 },
    { "Kszta\xB3t minimapy", "Kwadrat pokazuje troch\xEA wi\xEA" "cej terenu ni\xBF ko\xB3o." },
    { "Kwadrat|Ko\xB3o", 0 },
    { "Rozmiar minimapy", "Bok minimapy jako procent wysoko\x9C" "ci ekranu." },
    { "10%|12%|14%|16%|18%|20%|22%|25%|28%|32%|36%|40%", 0 },
    { "Po\xB3o\xBF" "enie minimapy", "W kt\xF3rym rogu ekranu jest minimapa." },
    { "Prawy g\xF3rny|Lewy g\xF3rny|Prawy dolny|Lewy dolny", 0 },
    { "Obr\xF3t minimapy", "Za kamer\xB9: minimapa obraca si\xEA razem z bohaterem." },
    { "P\xF3\xB3noc u g\xF3ry|Za kamer\xB9", 0 },
    { "Obr\xF3t obrazu mapy", "Obraca obraz mapy." },
    { "Auto|P\xF3\xB3noc|90 Stopni|180 Stopni|270 Stopni", 0 },
    { "Obr\xF3t du\xBF" "ej mapy", "Tak samo: du\xBF" "a mapa obr\xF3" "cona jak minimapa." },
    { "Tak samo|P\xF3\xB3noc|90 Stopni|180 Stopni|270 Stopni", 0 },
    { "Ikony i kolory ...", "Wygl\xB9" "d, wielko\x9C\xE6 i kolory ikon na obu mapach." },
    { "MiniMap - IKONY I KOLORY", 0 },
    { "Wygl\xB9" "d ikon", "Proste: dawne rysunki. W\xB3" "asne: twoje pliki .png." },
    { "Proste|Z ramk\xB9|Bez ramki|W\xB3" "asne", 0 },
    { "Wielko\x9C\xE6 postaci", "Wielko\x9C\xE6 ikon ludzi, potwor\xF3w i zwierz\xB9t." },
    { "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%", 0 },
    { "Wielko\x9C\xE6 przedmiot\xF3w", "Wielko\x9C\xE6 ikon przedmiot\xF3w le\xBF\xB9" "cych na ziemi." },
    { "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%", 0 },
    { "Wielko\x9C\xE6 znacznik\xF3w", "Skrzynie, miejsca, meble, d\x9Fwignie, teleporty." },
    { "50%|75%|100%|125%|150%|175%|200%|250%|300%|400%", 0 },
    { "W\xB3" "asne: podmie\xF1 pliki .png w folderze gry MiniMap/icons/custom", 0 },
    { "Styl minimapy", "Ramka minimapy: klasyczna, pergamin, \xBF" "elazo albo z\xB3oto." },
    { "Klasyczny|Pergamin|\xAF" "elazo|Z\xB3oto", 0 },
    { "Wielko\x9C\xE6 du\xBF" "ej mapy", "Ile ekranu zajmuje du\xBF" "a mapa (100% = domy\x9Clnie)." },
    { "60%|70%|80%|90%|100%", 0 },
    { "Styl du\xBF" "ej mapy", "Ramka i tabliczka: klasyczna, pergamin, \xBF" "elazo, z\xB3oto." },
    { "Klasyczny|Pergamin|\xAF" "elazo|Z\xB3oto", 0 },
    { "Du\xBF" "a mapa na ca\xB3y ekran", "Du\xBF" "a mapa zajmuje ca\xB3y ekran lub w menu pod klawiszem." },
    { "Nie|Tak", 0 },
    { "Postacie na mapie", "Wykrywanie postaci na mapie." },
    { "Ukryte|W zasi\xEAgu|Max", 0 },
    { "Rodzaj postaci", "Zasi\xEAg widzenia ludzi albo potwory (tak\xBF" "e orkowie)." },
    { "Max|Ludzie|Potwory|Nikt", 0 },
    { "Wykrywanie przedmiot\xF3w", "Wykrywanie przedmiot\xF3w na ziemi." },
    { "Ukryte|W zasi\xEAgu|Max", 0 },
    { "Skala pod minimap\xB9", "Ile metr\xF3w wida\xE6 na minimapie (np. 60 m)." },
    { "Ukryta|Zawsze|Przy zmianie", 0 },
    { "Wygl\xB9" "d terenu ...", "Ostro\x9C\xE6, cienie, kolory, drzewa, jaskinie i mg\xB3" "a." },
    { "Klawisze ...", "Klawisze obu map: otwieranie, przybli\xBF" "anie, szukanie." },
    { "Minimapa ...", "Kszta\xB3t, rozmiar, po\xB3o\xBF" "enie, styl i obr\xF3t minimapy." },
    { "Co wida\xE6 na mapie ...", "Postacie, przedmioty, skrzynie, miejsca, teleporty." },
    { "Du\xBF" "a mapa ...", "Wielko\x9C\xE6, styl, obr\xF3t, mysz i w\xB3" "asne znaczniki." },
    { "MiniMap - MINIMAPA", 0 },
    { "MiniMap - CO WIDA\xC6 NA MAPIE", 0 },
    { "MiniMap - DU\xAF" "A MAPA", 0 },
    { 0, 0 },
    { "Kolory przedmiot\xF3w", "Rodzaj przedmiotu kolorem: bro\xF1, mikstury, ro\x9Cliny..." },
    { "Jednolite|Kolorowe", 0 },
    { "Kolory na mapie ...", "Kolory postaci i przedmiot\xF3w na mapie." },
    { "MiniMap - KOLORY NA MAPIE", 0 },
    { "Wrogowie", "Kolor wrogich postaci i potwor\xF3w." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Neutralni", "Kolor neutralnych postaci i zwierz\xB9t." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Przyjaciele", "Kolor przyjaznych postaci." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Bohater", "Kolor strza\xB3ki bohatera." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Handlarze", "Kolor wagi handlarza." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Zw\xB3oki z \xB3upem", "Kolor krzy\xBF" "a przy zw\xB3okach z \xB3upem." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Bro\xF1 jednor\xEA" "czna", "Miecze, topory i maczugi jednor\xEA" "czne." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Bro\xF1 dwur\xEA" "czna", "Miecze, topory i kostury dwur\xEA" "czne." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "\xA3uki i kusze", "\xA3uki, kusze, strza\xB3y i be\xB3ty." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Mikstury", "Kolor mikstur." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Ro\x9Cliny i jedzenie", "Zio\xB3" "a, grzyby i jedzenie." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Magia", "Runy, zwoje, amulety i pier\x9C" "cienie." },
    { "Domy\x9Clny|Czerwony|Pomara\xF1" "cz|\xAF\xF3\xB3ty|Zielony|Turkus|Niebieski|Fiolet|R\xF3\xBFowy|Bia\xB3y|Szary|Br\xB9zowy", 0 },
    { "Meble", "\xA3\xF3\xBFka, krzes\xB3" "a, \xB3" "awki, fotele i trony." },
    { "Nie|Tak", 0 },
    { "Ku\x9Fnie", "Kowad\xB3" "a, paleniska, ose\xB3ki, wiadra do hartowania." },
    { "Nie|Tak", 0 },
    { "Alchemia", "Sto\xB3y alchemiczne i ubijaki zi\xF3\xB3." },
    { "Nie|Tak", 0 },
    { "Sto\xB3y runiczne", "Sto\xB3y do run i przemiany magii." },
    { "Nie|Tak", 0 },
    { "Pulpity do zwoj\xF3w", "Pulpity do pisania magicznych zwoj\xF3w." },
    { "Nie|Tak", 0 },
    { "Demonologia", "Sto\xB3y przywo\xB3ywaczy demon\xF3w." },
    { "Nie|Tak", 0 },
    { "Kuchnie", "Kuchenki, kot\xB3y, patelnie, ogniska do pieczenia." },
    { "Nie|Tak", 0 },
    { "Warsztaty", "Pi\xB3y, \xB3uki, garbarz, z\xB3otnik, kartograf, wino." },
    { "Nie|Tak", 0 },
    { "Ogie\xF1 i \x9Cwiat\xB3o", "Pochodnie, kominki i zwyk\xB3" "e ogniska." },
    { "Nie|Tak", 0 },
    { "Drobiazgi", "Beczki, studnie, deski do naprawy i reszta." },
    { "Nie|Tak", 0 },
    { "Drabiny", "Drabiny, po kt\xF3rych mo\xBFna si\xEA wspi\xB9\xE6." },
    { "Nie|Tak", 0 },
    { "Miejsca na mapie ...", "Meble, ku\x9Fnie, alchemia, kuchnie, warsztaty, ogie\xF1." },
    { "MiniMap - MIEJSCA NA MAPIE", 0 },
    { "Wykrywanie skrzy\xF1 na mapie", "Pokazuj wszystkie skrzynie na mapie zale\xBFy od zasi\xEAgu." },
    { "Ukryte|W zasi\xEAgu|Max", 0 },
    { 0, 0 },
    { "J\xEAzyk / Language", "Napisy w menu i na mapie." },
    { "Polski|English|Deutsch|Cestina", 0 },
    { "Zapis logu", "Czy tworzy\xE6 MiniMap.log?" },
    { "Nie|Tak", 0 },
    { "MiniMap - WYGL\xA5" "D TERENU", 0 },
    { "Detale mapy", "Ostro\x9C\xE6 przy przybli\xBF" "eniu. Wy\xBFsze buduj\xB9 si\xEA d\xB3u\xBF" "ej." },
    { "S\xB3" "abe|Normalne|Maksymalne", 0 },
    { "Cieniowanie g\xF3r", "Grzbiety ja\x9Cniej, doliny ciemniej - g\xF3ry wyra\x9Fnie wida\xE6." },
    { "S\xB3" "abe|Wyra\x9Fne|Mocne", 0 },
    { "Wyg\xB3" "adzanie terenu", "Teren g\xB3" "adszy, ale mniej szczeg\xF3\xB3\xF3w. Zalecane: Brak." },
    { "Brak|Lekkie|Normalne", 0 },
    { "Wype\xB3nianie dziur", "Puste plamy na mapie zakrywa terenem z okolicy." },
    { "Nie|Tak", 0 },
    { "Za kraw\xEA" "dzi\xB9 \x9Cwiata", "Co rysowa\xE6 tam, gdzie ko\xF1" "czy si\xEA \x9Cwiat gry." },
    { "Nic|Morze|G\xF3ry|Jak brzeg", 0 },
    { "\x8C" "cie\xBFki na mapie", "Drogi, kt\xF3rymi chodz\xB9 postacie, jako jasne \x9C" "cie\xBFki." },
    { "Nie|Tak", 0 },
    { "Wn\xEAtrza i jaskinie", "Pod dachem i w jaskini: widok wn\xEAtrza." },
    { "Nie|Mocne|Normalne|Tylko wn\xEAtrza", 0 },
    { "Kolory mapy", "Proste, jak w grze albo naturalne (\xBFywsza ziele\xF1, ska\xB3y)." },
    { "Proste|Jak w grze|Naturalne", 0 },
    { "Szybko\x9C\xE6 szczeg\xF3\xB3\xF3w", "Szybciej = ostra mapa wcze\x9Cniej, ale chwilowo mniej FPS." },
    { "Normalna|Szybsza|Mocniejsza|Maksymalna|Ekstremalna", 0 },
    { "Jako\x9C\xE6 szczeg\xF3\xB3\xF3w", "Dok\xB3" "adniejsze budynki, kraw\xEA" "dzie i cienie. Buduje d\xB3u\xBF" "ej." },
    { "Normalna|Wysoka|Bardzo wysoka|Maksymalna", 0 },
    { "Tabela przedmiot\xF3w", "Du\xBF" "a mapa: lista po lewej, A/D kategoria, W/S wyb\xF3r." },
    { "Ukryta|Widoczna", 0 },
    { "Teleporty", "Cele teleport\xF3w na mapie." },
    { "Ukryte|Widoczne", 0 },
    { "Mg\xB3" "a na mapie", "Mapa odkrywa si\xEA tylko tam, gdzie ju\xBF by\xB3" "e\x9C." },
    { "Nie|Tak", 0 },
    { "MiniMap - PODPOWIEDZI I SZUKANIE", 0 },
    { "MiniMap - EKSPORT MAPY", 0 },
    { "Podpowiedzi i szukanie ...", "Opis pod kursorem, szukanie i tabela przedmiot\xF3w." },
    { "Eksport mapy ...", "Zapis mapy \x9Cwiata do pliku obrazu (JPG, PNG, BMP)." },
    { "Zoom mapy", "Wi\xEAkszy zoom = wi\xEA" "cej pikseli i wi\xEAkszy plik." },
    { "1x|2x|4x", 0 },
    { "Format pliku", "JPG najmniejszy, PNG i BMP bez strat (BMP ogromny)." },
    { "BMP|PNG|JPG", 0 },
    { "Jako\x9C\xE6 JPG", "Wy\xBFsza = ostrzejsze ikony i wi\xEAkszy plik." },
    { "70%|80%|90%|95%", 0 },
    { "Detale na mapie ...", "Co zapisa\xE6 z map\xB9: skrzynie, postacie, przeciwnicy..." },
    { "MiniMap - DETALE EKSPORTU", 0 },
    { "Skrzynie", "Skrzynie i pojemniki w kolorze zamka." },
    { "Nie|Tak", 0 },
    { "Przedmioty", "Przedmioty le\xBF\xB9" "ce w \x9Cwiecie (bez ro\x9Clin)." },
    { "Nie|Tak", 0 },
    { "Ro\x9Cliny", "Zio\xB3" "a i jedzenie le\xBF\xB9" "ce w \x9Cwiecie." },
    { "Nie|Tak", 0 },
    { "Postacie neutralne", "Neutralne postacie (bia\xB3" "a g\xB3owa) i handlarze (waga)." },
    { "Nie|Tak", 0 },
    { "Postacie wrogie", "Wrogowie i potwory (czaszka), stan z chwili eksportu." },
    { "Nie|Tak", 0 },
    { "Prze\xB3\xB9" "czniki", "D\x9Fwignie, ko\xB3owroty, p\xB3yty." },
    { "Nie|Tak", 0 },
    { "Miejsca", "Kapliczki, rzemios\xB3o, z\xB3o\xBF" "a, skarby." },
    { "Nie|Tak", 0 },
    { "Przej\x9C" "cia", "Przej\x9C" "cia do innych \x9Cwiat\xF3w." },
    { "Nie|Tak", 0 },
    { "Cele teleportu", "Cele run z ekwipunku i teleporty w \x9Cwiecie." },
    { "Nie|Tak", 0 },
    { "W\xB3" "asne znaczniki", "Znaczniki postawione na du\xBF" "ej mapie." },
    { "Nie|Tak", 0 },
    { "Bohater", "Strza\xB3ka bohatera." },
    { "Nie|Tak", 0 },
    { "Postacie przyjazne", "Przyjaciele i towarzysze (dwie zielone g\xB3owy)." },
    { "Nie|Tak", 0 },
    { "Potwierd\x9F eksport", "Enter, potem wyjd\x9F z menu. Esc w trakcie przerywa." },
    { "Plik w folderze gry: MiniMap/export", 0 },
    { "Ukryj puste", "Chowa puste skrzynie, beczki, worki i ograbione zw\xB3oki." },
    { "Nie|Tak", 0 },
    { "Kombinacja zamka", "Podpowied\x9F skrzyni: ruchy wytrycha albo ca\xB3" "a kombinacja." },
    { "Ukryj|Ilo\x9C\xE6 ruch\xF3w|Pe\xB3na", 0 },
    { "Przeci\xB9ganie mapy", "Du\xBF" "a mapa: przytrzymaj lewy przycisk i przesu\xF1 map\xEA." },
    { "Nie|Tak", 0 },
    { "Pasek szczeg\xF3\xB3\xF3w", "Ma\xB3y pasek pod minimap\xB9, gdy buduje si\xEA ostry teren." },
    { "Ukryty|Widoczny", 0 },
    { "Drzewa na mapie", "Bez drzew: bez koron i drzew, wida\xE6 ziemi\xEA i \x9C" "cie\xBFki." },
    { "Brak|Proste|Dok\xB3" "adne|Bez drzew", 0 },
    { "Po zmianie mapa narysuje si\xEA od nowa po wyj\x9C" "ciu z menu.", 0 },
    { "MiniMap - KLAWISZE", 0 },
    { "Du\xBF" "a mapa", "Mapa ca\xB3" "ego \x9Cwiata na ca\xB3y ekran, z notatkami." },
    { "Nie|Tak", 0 },
    { "K\xF3\xB3ko myszy na minimapie", "K\xF3\xB3ko myszy przybli\xBF" "a i oddala minimap\xEA." },
    { "Nic|Przybli\xBF" "anie", 0 },
    { "K\xF3\xB3ko myszy na du\xBF" "ej mapie", "K\xF3\xB3ko zmienia ikon\xEA znacznika albo przybli\xBF" "a map\xEA." },
    { "Nic|Zmiana ikony|Przybli\xBF" "anie", 0 },
    { "\x8Crodek du\xBF" "ej mapy", "Obszar gry: l\xB9" "d na \x9Crodku, bez pustego morza." },
    { "Ca\xB3y \x9Cwiat|Obszar gry", 0 },
    { "W\xB3" "asne znaczniki", "Na du\xBF" "ej mapie: lewy klik stawia, prawy kasuje." },
    { "Nie|Tak", 0 },
    { "D\x9Fwignie i prze\xB3\xB9" "czniki", "D\x9Fwignie, ko\xB3owroty i p\xB3yty otwieraj\xB9" "ce bramy i mosty." },
    { "Ukryte|W zasi\xEAgu|Max", 0 },
    { "Miejsca i interakcje", "Kapliczki, tablice, kowad\xB3" "a, z\xB3o\xBF" "a rudy, skarby." },
    { "Ukryte|W zasi\xEAgu|Wa\xBFne|Max", 0 },
    { "Podpowied\x9F pod kursorem", "Na du\xBF" "ej mapie: kto albo co jest pod kursorem." },
    { "Nie|Max|Postacie|Przedmioty", 0 },
    { "Szczeg\xF3\xB3y podpowiedzi", "Sama nazwa lub: hp, mana, gildia, \xBFycie, bro\xF1, cena." },
    { "Sama nazwa|Podstawowe|Max", 0 },
    { "Reszta przy szukaniu", "Co z rzeczami i postaciami, kt\xF3re nie pasuj\xB9 do szukania." },
    { "Ukryj|Przyciemnij", 0 },
    { "Pauza na du\xBF" "ej mapie", "Gra stoi w miejscu, gdy du\xBF" "a mapa jest otwarta." },
    { "Nie|Tak", 0 },
    { "Szybko\x9C\xE6 kursora", "Jak szybko kursor myszy je\x9F" "dzi po du\xBF" "ej mapie." },
    { "50%|75%|100%|125%|150%|200%", 0 },
    { "Strza\xB3ka do znacznika", "Na minimapie: kierunek do wybranego znacznika." },
    { "Nie|Strza\xB3ka|Z odleg\xB3o\x9C" "ci\xB9", 0 },
    { "Przej\x9C" "cia do innych map", "Miejsca, z kt\xF3rych przechodzi si\xEA do innego \x9Cwiata gry." },
    { "Ukryte|W zasi\xEAgu|Max", 0 },
    { "Handlarze na mapie", "Znak wagi u ka\xBF" "dego, kto handluje. Towar w podpowiedzi." },
    { "Ukryte|W zasi\xEAgu|Max", 0 },
    { "Zw\xB3oki na mapie", "Krzy\xBF przy zw\xB3okach, przy kt\xF3rych jest co\x9C do zabrania." },
    { "Ukryte|W zasi\xEAgu|Max", 0 },
    { "Z\xB3ota obw\xF3" "dka bohatera", "Na du\xBF" "ej mapie migaj\xB9" "cy pier\x9C" "cie\xF1 wok\xF3\xB3 bohatera." },
    { "Nie|Tak", 0 },
    { "Ekwipunek w podpowiedzi", "Pokazuje list\xEA przedmiot\xF3w: handlarze i zw\xB3oki albo ka\xBF" "de." },
    { "Nie|Handlarze|Max", 0 },
    { "Szukanie w ekwipunku", "Szukaj postaci po przedmiocie, kt\xF3ry ma przy sobie." },
    { "Nie|Handlarze|Max", 0 },
    { "Przewijanie przy kraw\xEA" "dzi", "Mapa jedzie, gdy kursor dotknie kraw\xEA" "dzi okna." },
    { "Nie|Tak", 0 },
    { "Du\xBF" "a mapa", "Otwiera i zamyka du\xBF\xB9 map\xEA." },
    { "Przybli\xBF" "anie", "Przybli\xBF" "a minimap\xEA i du\xBF\xB9 map\xEA." },
    { "Oddalanie", "Oddala minimap\xEA i du\xBF\xB9 map\xEA." },
    { "Poka\xBF / ukryj minimap\xEA", "Chowa i pokazuje minimap\xEA." },
    { "Wi\xEAksza minimapa", "Powi\xEAksza okienko minimapy." },
    { "Mniejsza minimapa", "Zmniejsza okienko minimapy." },
    { "Obr\xF3t minimapy", "Prze\xB3\xB9" "cza: p\xF3\xB3noc u g\xF3ry albo za kamer\xB9." },
    { "Obr\xF3t obrazu mapy", "Obraca (i odbija) obraz mapy, krok po kroku." },
    { "Ikona znacznika", "Zmienia ikon\xEA znacznika na du\xBF" "ej mapie." },
    { "Szukanie", "Na du\xBF" "ej mapie wpisz nazw\xEA i Enter. Esc czy\x9C" "ci." },
    { "Pe\xB3ny ekran", "Otwarta du\xBF" "a mapa: ca\xB3y ekran albo okno." },
    { "Tabela przedmiot\xF3w", "Otwarta du\xBF" "a mapa: pokazuje i chowa tabel\xEA." },
    { "Enter na wierszu, nowy klawisz. Delete usuwa, Esc anuluje.", 0 },
  };
  static_assert( sizeof( MENU_TR_PL ) / sizeof( MENU_TR_PL[0] ) == sizeof( MENU_TR ) / sizeof( MENU_TR[0] ), "MENU_TR_PL count mismatch" );
  inline constexpr int MENU_PL_MAX = 96;
}
namespace GOTHIC_NAMESPACE {
namespace MiniMap {
  static char g_menuPL0[MENU_TR_COUNT][MENU_PL_MAX];
  static char g_menuPL1[MENU_TR_COUNT][MENU_PL_MAX];
  static bool g_menuPLHave[MENU_TR_COUNT];
  static int  g_menuLangApplied = -1;
  static const char* g_menuTxt[MENU_TR_COUNT][2];
  static int         g_menuTxtGen = 0, g_menuTxtApplied = 0;
  inline void LangTxtEnsure();
  inline bool MenuLocked( int i ) {
    return i >= 0 && i < MENU_TR_COUNT
        && ( !strcmp( MENU_TR[i].item, "MM_AUTHOR" ) || !strcmp( MENU_TR[i].item, "MM_VERSION" ) );
  }
  static char g_menuInfoCur[MENU_TR_COUNT][MENU_PL_MAX];
  inline const char* MenuTxt( int i, int k ) {
    if( MenuLocked( i ) ) return 0;
    if( i < 0 || i >= MENU_TR_COUNT || k < 0 || k >= 2 ) return 0;
    if( g_menuTxt[i][k] ) return g_menuTxt[i][k];
    return ( k == 1 && g_menuInfoCur[i][0] ) ? g_menuInfoCur[i] : 0;
  }
  static int g_menuFontPL = -1;
  inline bool FontHasPL( const zCFont* f ) {
    return f && f->width[0x8F] > 0 && f->width[0x9F] > 0 && f->width[0xB9] > 0
             && f->width[0x83] == 0 && f->width[0x88] == 0;
  }
  inline void MenuFoldPL( char* s ) {
    static const char from[] = "\xA5\xC6\xCA\xA3\xD1\xD3\x8C\x8F\xAF\xB9\xE6\xEA\xB3\xF1\xF3\x9C\x9F\xBF";
    static const char to[]   = "ACELNOSZZacelnoszz";
    for( ; s && *s; s++ )
      for( int k = 0; from[k]; k++ )
        if( *s == from[k] ) {
          *s = to[k];
          break;
        }
  }
  inline void MenuPLCopy( char* dst, const zSTRING& src ) { MiniMapCfg::Copy( dst, MENU_PL_MAX, src.ToChar() ); }
  inline void MenuInfoSync( zCMenuItem* it, const zSTRING& info ) {
    if( !it || it->m_parText[1] == info ) return;
    it->m_parText[1] = info;
    zCMenu* const menu = zCMenu::GetActive();
    if( menu && menu->GetActiveItem() == it )
      menu->PrintInformation( info );
  }
  inline void ApplyMenuLanguage() {
    LangTxtEnsure();
    const int want = ( g_lang >= 0 && g_lang < LANG_COUNT ) ? g_lang : LANG_PL;
    if( g_menuLangApplied == want && g_menuTxtGen == g_menuTxtApplied ) return;
    g_menuTxtApplied = g_menuTxtGen;
    int touched = 0, missing = 0;
    for( int i = 0; i < MENU_TR_COUNT; i++ ) {
      zCMenuItem* it = zCMenuItem::GetByName( zSTRING( MENU_TR[i].item ) );
      if( !it ) {
        missing++;
        continue;
      }
      if( g_menuFontPL < 0 ) {
        const zCFont* f = it->m_pFont ? it->m_pFont : ( screen ? screen->font : 0 );
        if( f ) {
          g_menuFontPL = FontHasPL( f ) ? 1 : 0;
          MM_LOG( g_menuFontPL ? "menu font has Polish letters"
                                        : "menu font without Polish letters - menu text without diacritics" );
        }
      }
      if( !g_menuPLHave[i] ) {
        MenuPLCopy( g_menuPL0[i], it->m_parText[0] );
        MenuPLCopy( g_menuPL1[i], it->m_parText[1] );
        if( g_menuFontPL == 0 ) {
          MenuFoldPL( g_menuPL0[i] );
          MenuFoldPL( g_menuPL1[i] );
        }
        g_menuPLHave[i] = true;
      }
      const char* t0 = MENU_TR[i].text0.t[want];
      const char* t1 = MENU_TR[i].text1.t[want];
      const char* f0 = MenuTxt( i, 0 );
      const char* f1 = MenuTxt( i, 1 );
      it->SetText( zSTRING( f0 ? f0 : ( want != LANG_PL && t0 ) ? t0 : g_menuPL0[i] ), 0, 1 );
      const zSTRING s1( f1 ? f1 : ( want != LANG_PL && t1 ) ? t1 : g_menuPL1[i] );
      it->SetText( s1, 1, 1 );
      MenuInfoSync( it, s1 );
      touched++;
    }
    if( touched > 0 && missing == 0 ) g_menuLangApplied = want;
  }
  struct TKeyField {
    const char* item;
    const char* option;
    const char* label;
  };
  static const TKeyField KEY_FIELDS[] = {
    { "MM_KEYFULL_VAL",   "KeyFullMap",  "MM_KEYFULL" },
    { "MM_KEYIN_VAL",     "KeyZoomIn",   "MM_KEYIN" },
    { "MM_KEYOUT_VAL",    "KeyZoomOut",  "MM_KEYOUT" },
    { "MM_KEYTOGGLE_VAL", "KeyToggle",   "MM_KEYTOGGLE" },
    { "MM_KEYSIZEUP_VAL", "KeySizeUp",   "MM_KEYSIZEUP" },
    { "MM_KEYSIZEDN_VAL", "KeySizeDown", "MM_KEYSIZEDN" },
    { "MM_KEYROTATE_VAL", "KeyRotate",   "MM_KEYROTATE" },
    { "MM_KEYALIGN_VAL",  "KeyAlign",    "MM_KEYALIGN" },
    { "MM_KEYICON_VAL",   "KeyMarkIcon", "MM_KEYICON" },
    { "MM_KEYSEARCH_VAL", "KeySearch",   "MM_KEYSEARCH" },
    { "MM_KEYFSCR_VAL",   "KeyFullScreen", "MM_KEYFSCR" },
    { "MM_KEYILIST_VAL",  "KeyItemList", "MM_KEYILIST" },
  };
  const int KEY_FIELDS_COUNT = sizeof( KEY_FIELDS ) / sizeof( KEY_FIELDS[0] );
  inline zSTRING KeyToOptHex( int code ) {
    char b[5];
    snprintf( b, sizeof( b ), "%02x%02x", code & 0xFF, ( code >> 8 ) & 0xFF );
    return zSTRING( b );
  }
  static int  g_keyCaptureRow   = -1;
  static bool g_keyEnterWasDown = false;
  static bool  g_capWas[256];
  static DWORD g_menuSeenAt = 0;
  inline int KeyFromOptHex( const zSTRING& s ) { return ( s.Length() == 4 ) ? MiniMapCfg::DecodeKeyHex4( s.ToChar() ) : -1; }
  static const char* GAME_SCREEN_KEYS[] = {
    "keyShowStatus", "keyShowLog", "keyInventory", "keyShowMap"
  };
  const int GAME_SCREEN_KEYS_COUNT = 4;
  static int   g_gameKeys[GAME_SCREEN_KEYS_COUNT * 2];
  static DWORD g_gameKeysAt = 0;
  static bool  g_gameKeysRead = false;
  inline void ReadGameKeys() {
    const DWORD now = GetTickCount();
    if( g_gameKeysRead && now - g_gameKeysAt < 2000 ) return;
    g_gameKeysRead = true;
    g_gameKeysAt   = now;
    memset( g_gameKeys, 0, (size_t)( GAME_SCREEN_KEYS_COUNT * 2 ) * sizeof( g_gameKeys[0] ) );
    if( !zoptions ) return;
    for( int i = 0; i < GAME_SCREEN_KEYS_COUNT; i++ ) {
      const zSTRING raw = zoptions->ReadString( zSTRING( "KEYS" ), GAME_SCREEN_KEYS[i], "" );
      const char* p = raw.ToChar();
      const int len = raw.Length();
      for( int k = 0; k < 2 && p && ( k + 1 ) * 4 <= len; k++ ) {
        const int c = MiniMapCfg::DecodeKeyHex4( p + k * 4 );
        if( c > 0 && c < 0x100 )
          g_gameKeys[i * 2 + k] = c;
      }
    }
  }
  inline bool IsGameScreenKey( int code ) {
    if( code <= 0 ) return false;
    ReadGameKeys();
    for( int i = 0; i < GAME_SCREEN_KEYS_COUNT * 2; i++ )
      if( g_gameKeys[i] == code ) return true;
    return false;
  }
  inline bool CaptureKey() {
    if( !zinput || !zoptions ) return false;
    zCMenu* menu = zCMenu::GetActive();
    if( !menu ) return false;
    const zSTRING sec( "MINIMAP" );
    const DWORD now = GetTickCount();
    if( g_menuSeenAt == 0 || now - g_menuSeenAt > 300 ) {
      if( g_keyCaptureRow >= 0 )
        MM_LOG( "menu: key capture from previous visit cancelled" );
      g_keyCaptureRow   = -1;
      g_keyEnterWasDown = zinput->KeyPressed( KEY_RETURN ) != 0;
    }
    g_menuSeenAt = now;
    if( g_keyCaptureRow < 0 ) {
      const bool enterDown = zinput->KeyPressed( KEY_RETURN ) != 0;
      const bool enterEdge = enterDown && !g_keyEnterWasDown;
      g_keyEnterWasDown = enterDown;
      if( !enterEdge ) return false;
      zCMenuItem* act = menu->GetActiveItem();
      if( !act ) return false;
      for( int i = 0; i < KEY_FIELDS_COUNT; i++ ) {
        if( act->GetName() == zSTRING( KEY_FIELDS[i].label )
            || act->GetName() == zSTRING( KEY_FIELDS[i].item ) ) {
          g_keyCaptureRow = i;
          zCMenuItem* fld = zCMenuItem::GetByName( zSTRING( KEY_FIELDS[i].item ) );
          if( fld )
            fld->SetText( zSTRING( "..." ), 0, 1 );
          for( int k = 0; k < 256; k++ )
            g_capWas[k] = zinput->KeyPressed( k ) != 0;
          MM_LOG( zSTRING( "menu: waiting for key for " ) + zSTRING( KEY_FIELDS[i].option ) );
          return false;
        }
      }
      return false;
    }
    const int i = g_keyCaptureRow;
    int k = 0;
    for( int c = 1; c < 256; c++ ) {
      const bool down = zinput->KeyPressed( c ) != 0;
      const bool edge = down && !g_capWas[c];
      g_capWas[c] = down;
      if( edge && c != KEY_RETURN && !k ) k = c;
    }
    if( k == 0 ) return false;
    zinput->ClearKeyBuffer();
    g_keyCaptureRow   = -1;
    g_keyEnterWasDown = true;
    zCMenuItem* fld = zCMenuItem::GetByName( zSTRING( KEY_FIELDS[i].item ) );
    if( k == KEY_ESCAPE ) {
      if( fld )
        fld->SetText( zSTRING( "" ), 0, 1 );
      MM_LOG( "menu: key change cancelled" );
      return false;
    }
    if( k != KEY_DELETE && IsGameScreenKey( k ) ) {
      if( fld )
        fld->SetText( zSTRING( "" ), 0, 1 );
      MM_LOG( zSTRING( "menu: key " ) + zCInput::GetNameByControlValue( (unsigned short)k ) + " rejected - it is a game screen key from [KEYS]" );
      return false;
    }
    const int code = ( k == KEY_DELETE ) ? 0 : (int)k;
    const int NOT_TEMP = 0;
    if( code ) {
      for( int j = 0; j < KEY_FIELDS_COUNT; j++ ) {
        if( j == i ) continue;
        if( KeyFromOptHex( zoptions->ReadString( sec, KEY_FIELDS[j].option, "" ) ) == code ) {
          zoptions->WriteString( sec, KEY_FIELDS[j].option, KeyToOptHex( 0 ), NOT_TEMP );
          MM_LOG( zSTRING( "menu: " ) + zSTRING( KEY_FIELDS[j].option ) + " cleared (key taken by " + zSTRING( KEY_FIELDS[i].option ) + ")" );
        }
      }
    }
    zoptions->WriteString( sec, KEY_FIELDS[i].option, KeyToOptHex( code ), NOT_TEMP );
    if( fld )
      fld->SetText( zSTRING( "" ), 0, 1 );
    MM_LOG( zSTRING( "menu: " ) + zSTRING( KEY_FIELDS[i].option ) + " = "
            + KeyToOptHex( code ) + " (" + ( code ? zCInput::GetNameByControlValue( (unsigned short)code ) : zSTRING( "none" ) ) + ")" );
    return true;
  }
  inline void ApplyKeyNames() {
    if( !zoptions ) return;
    const zSTRING sec( "MINIMAP" );
    for( int i = 0; i < KEY_FIELDS_COUNT; i++ ) {
      if( i == g_keyCaptureRow ) continue;
      zCMenuItem* it = zCMenuItem::GetByName( zSTRING( KEY_FIELDS[i].item ) );
      if( !it ) continue;
      const zSTRING raw = zoptions->ReadString( sec, KEY_FIELDS[i].option, "" );
      const int code = raw.IsEmpty() ? 0 : KeyFromOptHex( raw );
      zSTRING name;
      if( code == 0 )
        name = T( TX_KEY_NONE );
      else if( code > 0 && code < 0x100 )
        name = zCInput::GetNameByControlValue( (unsigned short)code );
      if( name.IsEmpty() ) continue;
      if( it->m_parText[0] != name )
        it->SetText( name, 0, 1 );
    }
  }
}
