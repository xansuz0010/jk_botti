#!/usr/bin/env python3
import pefile, sys

REQUIRED_EXPORTS = {
    b'GetEngineFunctions', b'GetEngineFunctions_POST',
    b'GetEntityAPI2', b'GetEntityAPI2_POST',
    b'GiveFnptrsToDll', b'GiveFnptrsToDll@8',
    b'Meta_Attach', b'Meta_Detach', b'Meta_Query',
}
ALLOWED_IMPORTS = {'kernel32.dll', 'msvcrt.dll', 'wsock32.dll', 'ws2_32.dll'}

dlls = ['jk_botti_mm.dll', 'jk_botti_mm_x87.dll',
        'jk_botti_mm_sse3.dll', 'jk_botti_mm_avx2fma.dll']
fail = False
for dll in dlls:
    print(f'=== {dll} ===')
    pe = pefile.PE(dll)

    exports = set()
    if hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
        for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
            if exp.name:
                exports.add(exp.name)
    print(f'  Exports: {sorted(e.decode() for e in exports)}')
    for sym in sorted(REQUIRED_EXPORTS):
        if sym not in exports:
            print(f'  FAIL: missing export {sym.decode()}')
            fail = True

    imports = set()
    if hasattr(pe, 'DIRECTORY_ENTRY_IMPORT'):
        for entry in pe.DIRECTORY_ENTRY_IMPORT:
            imports.add(entry.dll.decode().lower())
    print(f'  Imports: {sorted(imports)}')
    for dep in imports:
        if dep not in ALLOWED_IMPORTS:
            print(f'  FAIL: unexpected import {dep}')
            fail = True

    pe.close()

sys.exit(1 if fail else 0)
