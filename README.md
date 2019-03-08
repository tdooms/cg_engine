## Aanpassingen

l_renderer  : classe die alle generatie en rendering van l-systmeen omvat
primitives  : simpele structs/functies voor eenvoudige zaken zoals Line2D
l_parser    : extra classe ReplacementRule om gemakkelijker stochastische l-systemen te parsen

voor stochastische l-systemen is de syntax als volgt: 

```bash
F -> "F-G+F" = 0.5 & "G-F+G" = 0.5
```

waarbij er oneindig veel verschillende rules mogen maar de som is altijd 1, anders geeft hij een error

## stochastische

in de subdirectory "stochastische" zie je 3 ini en L2D files, ze zijn respectievelijk:
  -> een soort plant
  -> een cirkel achtig iets
  -> een lange lijn die cirkels maakt




