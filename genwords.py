import json

with open("scrabble.json") as fp:
    c = fp.read()
    words = json.loads(c)
    scrabble = set(w for w in words if len(w) > 3)

ALL_WORDS = sorted(scrabble)

with open("words.h", "w") as fp:
    fp.write("#pragma once\n\n")
    fp.write("#include <stdint.h>\n")
    fp.write("\n")
    fp.writelines(
        [
            "typedef struct word {\n",
            "    const char* w;\n",
            "    const uint32_t mask;\n",
            "} word;\n\n",
        ]
    )

    fp.write("word words[] = {\n")
    wc = 0
    for word in ALL_WORDS:
        mask = 0
        for c in word:
            m = 1 << (ord(c) - ord("a"))
            mask |= m
        fp.write(f'    {{"{word}", {mask}U}},\n')
        wc += 1
    fp.write("};\n\n")
    fp.write(f"const int wordCount = {wc};\n")
