# General
Dit is de evolutie van mijn engine doorheen de opdrachten. De grootste aanpassingen worden hier weergegeven.

Thomas Dooms - s0181389 - 1Ba Informatica

# Features

 1. Verscheidene optimalisaties:
    * std::forward_list ipv std::list voor 12% minder memory.
    * Kijkt na of huidige lijn een extensie is van de vorige bij l-systemen en slaagt ze op als een lijn.
    * Slaagt punten niet op om d, dx en dy te berekenen om de driehoeken te tekenen.
    * Reserved elke vector zodat er maar een allocatie moet gebeuren (dit gebeurt door overal eerst het aantal elementen te berekenen).
    * Dit allemaal zorgt voor gemiddeld meer dan 10x betere performance en meer dan 5x minder memory allocatated.
    * Alles runnen (exclusief raytracing) duurt 4 min op mijn laptop met een i5 aan 3.4 GHz (zie onderaan).
    
 2. Backface culling
    * Hierdoor wordt een groot deel van de driehoeken niet eens getekend.
    * 2-3x betere performance
    
 3. Duplicate lijnen eruit bij faces
    * Haalt dubbele lijnen eruit adhv een hashmap op de indices in O(n).
    * Perfecte hashfunctie t.e.m 65 000 vertices zorgt dat lijnen worden verwijderd met maar een vergelijking.
    
 4. Mengerspons optimalisatie
    * Haalt elke soort duplicaat of onzichtbare face eruit in O(n*log(n)) .
    * Dit is nog een extra uitbreding op de uitbreiding van de opdracht.
 
 5. Raytracing extensie
    * Kan verschillende materialen aan.
    * Geeft vrij snel zeer realistische beelden met reflectie/schaduw/color bleeding.
    * Zie sectie ray tracing voor meer info.
    
 6. Sterke fractaal optimalisatie
    * Aanmaken van de figuur wordt een keer gedaan, direct op juiste schaal.
    * Figuur wordt n keer gekopieerd en direct op de juiste plek geplaatst.
    * Meer iteraties zorgt alleen voor meer kopien, en leidt niet tot meer berekeningen (1 vector lookup en 1 optelling per iteratie).
    * Geen nodeloze kopien / recursie stackframes.
    
 7. Heel lage peak usage
    * Peak usage van alle tests is 114980 kb, ver onder de limiet van 2 gb (zie onderaan)
   
# Subfolders

 * config-extra: raytracing ini files
 * config2: stochastische l_systemen
 * config12: texture mapping
 * src: source code

# Versie 1 (l_systemen)

Voor stochastische l-systemen is de syntax als volgt:

```bash
F -> "F-G+F" = 0.5 & "G-F+G" = 0.5
```

waarbij er oneindig veel verschillende rules mogen maar de som is altijd 1, anders geeft hij een error.

In de subdirectory "config2" zie je 3 ini en L2D files, ze zijn respectievelijk:

 - een soort plant
 - een cirkel achtig iets
 - een lange lijn die cirkels maakt

# Versie 2 (3D lijntekeningen)

 - math folder     : zelfgeschreven math library omdat ik die toch al had.
 - geometry folder : alle classes die te maken hebben met de geometrische data/manipulatie


# Versie 3 (3D platonische lichamen)

Ik heb bij de L2D systemen het probleem van 'dubbele lijnen' opgelost door 2 line insert cases te maken.

 - Als sinds de laatste lijn iets is veranderd (pop/push/angle change) maakt hij een nieuwe lijn aan.
 - Zoniet wordt het tweede punt van de laatste lijn aangepast.

drawFigures haalt ook dubbele lijnen eruit door de indices door een set te duwen.
Hierdoor worden er 2 keer minder lijnen gemaakt/getekend, spijtig genoeg gebruikt de rood-zwart boom van c++ veel memory.
Later ga ik eens kijken naar std::unordered_set voor hogere efficientie.

# Versie 4 (ZBuffered lines)

Ik heb sinds de laatste keer mijn memory gebruik zwaar verminderd en mijn snelheid verbeterd:

 - unordered_set ipv set: 30% minder memory en 2-5x sneller O(n*logn) -> O(n).
 - foutje in mijn sphere vertex size voorspelling (er zijn nu geen resizes meer): 10% minder memory 
 
Het totale gealloceerde geheugen met valgrind voor alle testen tesamen test is nu 817 mb, ipv 1150 mb
 
# Versie 5 (ZBuffered Triangles)
 
 Enige verschil met de cursus is dat ik een aparte functie maak die gewoon de minima en maxima haalt uit de 3D punten, zonder alles dubbel op te slaan.
 
# Versie 6 (Fractals)
 
 Ik heb ook de optimalisatie van de Mengerspons gemaakt.
 Wilt men de figuur zonder de optimalisatie kan men naar Mesh.cpp gaan, 
 naar createMengerSponge en daar ipv mergeMengerSponge, gewoon mergeMeshes returnen.
 
# Versie 7 (Lighting)
 
 Ik heb wat kleine optimalisaties geschreven zodat er geen enkele berekening dubbel gebeurt.
 bv: hij gaat de color al converten naar een img::color buiten de loop als er geen puntlichten zijn.
 
# Versie 8 (Shadows)

 Niets speciaals.
 
# Versie 9 (Texture mapping)

 Ik heb de willekeurige oppervlakken gedaan, zie subdirectory "config12" voor de voorbeelden
 Alle afbeeldingen zijn wat 'speciaal' want ik vond geen .bmp files en als ik die converte, werkte die niet.
 
 Hier is hoe het werkt:
 
 ```bash
[General]
texture = TRUE

[Texture]
path = "texture.bmp"
p = (-2,-2,-2)
a = (4, 0, 0)
b = (0, 4, 0)
 ```
 
# Versie 10 (Thick figures)
 
  Gebruikmakende van de code die alle dubbele lijnen eruit haalt, is deze functie zeer performant.
  De bol wordt maar een keer aangemaakt, dan gewoon gekopieerd en verschoven zodat deze niet elke keer opnieuw moet gemaakt worden.
 
# Uitbreiding (Ray tracing)

 Ik heb als uitbreiding een simpele ray tracer geschreven.
 Zie "config-extra" voor de voorbeelden van de files.
 
## Uitleg ini files
 - ambient: belichting als een ray geen oppervlak raakt
 - samples, aantal random rays per pixel (best boven de 10)
 - reflectance: licht dat gereflecteerd wordt
 - emmitance: licht dat uitgestraald wordt (kan > 1 voor instense lichten)
 - optioneel - fuzzyness: zorgt voor een meer mat gevoel bij metalen

## Materialen:
 - Lambertian: mat, licht gaat naar overal waardoor het mat is
 - Metal:  metaal, licht wordt mooi rond de normaal gespiegeld 
 
## Uileg 1ste afbeedling:
 Linkse rode bol is metaal.
 Middenste bol is een lamp.
 Rechtse blauwe bol is mat.
 
 Men ziet mooi de schaduwen en color bleeding.
 Men ziet zelfs een andere blauwe bol die achter de camera staat in de reflectie van de rode bol
 en het licht dat op de grond weerkaatst wordt in de reflectie van de metalen bol.
 
 
## Uitgevoerde tests
* performance:

```bash
[thomas@laptop engine]$ cd everything/
[thomas@laptop everything]$ time ../engine *.ini
      
real    4m12.232s
user    3m58.569s
sys     0m12.074s
```

* peak memory:

```bash
[thomas@laptop ~]$ grep VmPeak /proc/3281/status
VmPeak:   114980 kB
```

* allocated memory:

```bash
[thomas@laptop ~]$ valgrind ../engine *.ini
```
 
 
 
 
