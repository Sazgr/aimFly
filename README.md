# aimfly
Valorant aim trainer

## Compiling

```
g++ *.cpp -o main.exe -Ilib\raylib\src -Llib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm -Wl,--subsystem,windows
```