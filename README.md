# general
Dit is de evolutie van mijn engine doorheen de systemen, de grootste aanpassingen worden hier genoteerd.
Thomas Dooms - s0181389 - 1Ba Informatica

# Versie 1 (l_systemen)

 - l_renderer  : classe die alle generatie en rendering van l-systmeen omvat
 - primitives  : simpele structs/functies voor eenvoudige zaken zoals Line2D
 - l_parser    : extra classe ReplacementRule om gemakkelijker stochastische l-systemen te parsen

voor stochastische l-systemen is de syntax als volgt:

```bash
F -> "F-G+F" = 0.5 & "G-F+G" = 0.5
```

waarbij er oneindig veel verschillende rules mogen maar de som is altijd 1, anders geeft hij een error

# Stochastische

in de subdirectory "stochastische" zie je 3 ini en L2D files, ze zijn respectievelijk:
  - een soort plant
  - een cirkel achtig iets
  - een lange lijn die cirkels maakt

# Versie 2 (3D lijntekeningen)

math folder     : zelfgeschreven math library omdat ik die toch al had.
geometry folder : alle klasses dat hebben te maken met de geometrische data/manipulatie


# Versie 3 (3D platonische lichamen)

l_renderer      : ondersteuning 3D l_systemen adhv de mesh classe.

Ik heb bij de L2D systemen het probleem van 'dubbele lijnen' opgelost door 2 line insert cases te maken.
  - Als sinds de laatste lijn iets is veranderd (pop/push/angle change) maakt hij een nieuwe lijn aan.
  - Zoniet wordt het tweede punt van de laatste lijn aangepast.

zware aanpassingen aan de structuur:
  - alles rond 3D in mesh.h/mesh.cpp
  - typedefs weggehaald, hierdoor ligt de onderliggende datastructuur bloot maar kan ik later makkelijker drawfuncties templaten indien nodig
  - drawFigures voor 3D  / drawLines voor 2D
  - easy_image lichtjes aangepast en ook alle .cc vervangen door .cpp

drawFigures haalt ook dubbele lijnen eruit door de indices door een set te duwen.
Hierdoor worden er 2 keer minder lijnen gemaakt/getekend, spijtig genoeg gebruikt de rood-zwart boom van c++ veel memory.
Later ga ik eens kijken naar std::unordered_set voor hogere efficientie.

# Versie 4 (Z-Buffered lines)

zBuffer     : klasse die een bitmap is met wat overloaded operatoren

Ik heb sinds de laatste keer mijn memory gebruik zwaar verminderd,en mijn snelheid verbeterd:
 - unordered_set ipv set: 30% minder memory en 2-5x sneller O(n*logn) -> O(n).
 - foutje in mijn sphere vertex size voorspelling (er zijn nu geen resizes meer): 10% minder memory 
 
het totale gealloceerde geheugen met valgrind voor alle testen tesamen test is nu 817 mb, ipv 1150 mb
    
Verdere optimisaties die kunnen gebeuren:
 - Elke opdeling van een sphere dupliceerd vertices. Dit lijkt me wel geen triviaal op te lossen probleem :(
 
 