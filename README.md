# SolteszViktorF2UJS6Grafika

A gyakorlatok gyak02 részben a "Stack" lényegében a verem típusú feladatokkal kiegészített "matrices"

# Wizard Game – 3D grafikus projekt

Ez a projekt egy egyszerű, interaktív 3D világban játszódó játék alapját képezi, ahol a játékos egy varázslót irányít. A cél egy könnyen bővíthető, jól strukturált grafikus motor létrehozása OpenGL + SDL használatával.

## 🔧 Követelmények

- C fordító (pl. GCC)
- CMake vagy saját `Makefile`
- SDL2
- SDL2_image
- SDL2_ttf
- GLEW
- OpenGL

## 📁 Könyvtárstruktúra

```
assets/
├── models/         # 3D modellek (.obj, .mtl)
├── textures/       # Textúrák (PNG, JPG)
├── maps/           # Térképfájlok (.txt)
├── shaders/        # GLSL shader fájlok

src/
├── main.c          # Program belépési pontja
├── engine.c        # SDL + OpenGL inicializálás, futtatás
├── renderer.c      # Kirajzolás
├── camera.c        # Kamera kezelés
├── model_loader.c  # OBJ + MTL fájl betöltés
├── model_registry.c # Előregisztrált modellek
├── map_loader.c    # Térképről modellek példányosítása
```

## 🎮 Funkciók (verzió 1)

### ✅ Minimális követelmények

- [x] **Kamerakezelés** – egérrel körbeforgatható, billentyűvel mozgatható kamera (third-person mód).
- [x] **Térbeli objektumok** – OBJ modellek betöltése külső fájlból.
- [x] **Textúrák** – MTL alapján PNG textúrák betöltése a modellekhez.
- [x] **Animáció, mozgatás** – játékos mozgatható billentyűzettel, kamera körbeforgatható egérrel.
- [x] **Használati útmutató** – `README.md` fájl (ez).
- [x] **Fények** – (alap shader szinten, + és - billentyűkkel még nem állítható)

### 🔜 Tervek (későbbi verziókban)

- [ ] Több fényforrás kezelése és mozgatása
- [ ] Árnyék, köd és részecskerendszer
- [ ] AI és ütközésvizsgálat
- [ ] UI elemek megjelenítése (életerő, gombok)

## 📦 Fordítás

A program `make` paranccsal fordítható, ha elérhető a Makefile:

```bash
make
./wizard_game
```

Ha Windows alatt dolgozol VS Code-ban, célszerű a `tasks.json` fájlt is beállítani.

## 🗺️ Térkép fájl

A pályát egy egyszerű szöveges térképfájl írja le, például:

```
0 0 0 2 0 0
0 3 0 0 4 0
0 0 1 0 0 0
```

Itt a számok különböző modelleket jelentenek:

- 1 = Wizard
- 2 = Crystal1
- 3-7 = Rock1 - Rock5

## 📌 Megjegyzések

- A modellek nem lehetnek túl részletesek, hogy a teljes projekt ne haladja meg a 200MB-ot.
- Textúrák: PNG vagy JPG, egyelőre MTL fájl alapján töltődnek be.
- A program a `main.c`-ből indul, ne felejtsd el lefordítani az összes `.c` fájlt!

# 🧙 Wizard Game 0.2 – Verzió újdonságai
## 🌄 Terrain rendszer
-A domborzat (heightmap) és a textúratérkép (texturemap) már PNG fájlokból töltődik be.
-A terep egy folyamatos, lépcsőzésmentes terrain_mesh-ként generálódik.
-Bizonyos tile-típusok között jelenleg átfolyás (seam) figyelhető meg – ezek javítása a következő verziók egyik célja lesz.

## 🧱 Modellek
- A korábbi player modell eltávolításra került – a jövőben .glb fájlban, animációval együtt kerül be.
- .obj modellek elhelyezése ~80%-ban kész: random forgatás még nem tökéletes, de nem sürgető.
- Különböző típusú modellek támogatottak:

Színes (vertex color alapú)

Egyszerű, egy textúrás modellek

Több anyagos / több textúrás modellek (.obj + .mtl alapján)

- Az elhelyezés .png típusú "biome" térkép alapján történik, ami nem fix pozíciókat, hanem valószínűségi eloszlást definiál egy adott területen.

## 🎲 Saját véletlenszám-generátor
- Bevezetésre került egy saját random rendszer, így mindig ugyanaz a világ generálódik azonos seed esetén.
- Jelenleg a seed fix (konstans), a későbbiekben mentéskor fog tárolódni.

## 🎥 Kamera és vezérlés
- A kamera mozgása és a játékos pozíciója immár jobban összehangolt.
- Cheat mód (P billentyű): szabadon mozgatható kamera (free-fly).(Még nem működik rendesen)
- A Wireframe mód (F2), és a kamera állapot kiírása (F3) is elérhető.

## ✨ Átlátszóság
- A kristálymodelleknél bevezetésre került az átlátszóság (alpha blending).
- Jelenleg egységes átlátszóság (pl. 0.6f) van alkalmazva, de a sorrend szerinti rajzolás és finomhangolás még hátra van.

## 🛠 Build rendszer
- A fordítás mostantól Makefile alapú.
- A tasks.json automatikusan ezt hívja meg VS Code-ban, így egyszerűen és gyorsan lehet fordítani.

## ⚠️ Megjegyzés
- A projektet sajnos egy komoly hiba miatt újra kellett kezdeni. Emiatt előfordulhatnak olyan elemek, amelyek korábban már működtek, de a mostani verzióban még nincsenek implementálva.

##📦 Assets
-A szükséges assetek letölthetők innen:
https://drive.google.com/file/d/1flusGZMYSA_hpVMtx_4PKbfYPpjnSGxJ/view?usp=sharing


