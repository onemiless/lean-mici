#!/usr/bin/env python3
from build_agnos_manifest import CHAIN, REF, load

for profile in ("c3", "c3xl"):
  print(f"{profile.upper()}_BOOT_CHAIN_ALLOWLIST: Mapping[str, BootChainImage] = {{")
  for name, part in load(REF / f"devsp-{profile}.json").items():
    if name in CHAIN:
      print(f'  "{name}": BootChainImage("{part["hash"]}", "{part["hash_raw"]}", {part["size"]}),')
  print("}\n")
