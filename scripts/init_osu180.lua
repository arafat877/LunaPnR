clear()
load_lef("test/files/iit_stdcells/lib/tsmc018/lib/iit018_stdcells.lef")
load_lib("test/files/iit_stdcells/lib/tsmc018/signalstorm/iit018_stdcells.lib")
load_layers("scripts/osu180_layers.json")

add_hatch(16, 16,
    "*..............*" ..
    "......*...*....." ..
    "..*..........*.." ..
    "*.......*......." ..
    "....*......*...." ..
    "..*...........*." ..
    "......*..*......" ..
    "*...*.......*..." ..
    "................" ..
    "..*...*..*....*." ..
    "................" ..
    "*...*...*..*...." ..
    "................" ..
    "..*...*...*..*.." ..
    "................" ..
    "*...*...*...*..*"
)

add_hatch(16, 16,
    "...*.*.....*.*.." ..
    "..*****...*****." ..
    "...*.*.....*.*.." ..
    "................" ..
    "................" ..
    "................" ..
    ".*.....*.*.....*" ..
    "***...*****...**" ..
    ".*.....*.*.....*" ..
    "***...*****...**" ..
    ".*.....*.*.....*" ..
    "................" ..
    "................" ..
    "................" ..
    "...*.*.....*.*.." ..
    "..*****...*****."
)

add_hatch(16, 16,
    ".......*........" ..
    "......*........." ..
    ".....*.........." ..
    "....*..........." ..
    "...*............" ..
    "..*............." ..
    ".*.............." ..
    "*..............." ..
    "...............*" ..
    "..............*." ..
    ".............*.." ..
    "............*..." ..
    "...........*...." ..
    "..........*....." ..
    ".........*......" ..
    "........*......." 
)

add_hatch(16, 16,
    "........*......." ..
    ".........*......" ..
    "..........*....." ..
    "...........*...." ..
    "............*..." ..
    ".............*.." ..
    "..............*." ..
    "...............*" ..
    "*..............." ..
    ".*.............." ..
    "..*............." ..
    "...*............" ..
    "....*..........." ..
    ".....*.........." ..
    "......*........." ..
    ".......*........"
)

print("OSU180 technology loaded")
