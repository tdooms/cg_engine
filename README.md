## Aanpassingen

l_renderer  : classe die alle generatie en rendering van l-systmeen omvat
primitives  : simpele structs/functies voor eenvoudige zaken zoals Line2D
l_parser    : extra classe ReplacementRule om gemakkelijker stochastische l-systemen te parsen

voor stochastische l-systemen is de syntax als volgt: 

```bash
F -> "F-G+F" = 0.5 & "G-F+G" = 0.5
```

waarbij er oneindig veel verschillende rules mogen maar de som is altijd 1, anders geeft hij een error

## Installatie

In de directory van deze file type je:

```bash
make
sh runall.sh
```

nu kan je de .bmp bestanden zien in de config subdir




