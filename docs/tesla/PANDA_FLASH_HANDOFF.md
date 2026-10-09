# Inspected H725 application-only updater handoff

No Panda reset/write/recovery or service change was performed by this worker. Finish the parent AGNOS operation before any firmware operation.

## Exact reusable implementation

Device: `/data/dev-sp-c3xl-audit-20261007/flash_application.py`, SHA256 `e702476974cf04dc52f6784142c5247e79cd24594620e31ee7cdb91b961109c1`. An unmodified archival copy is `docs/tesla/ref/panda/flash_application_20261007.py.txt` (non-executable reference).

It uses `panda.python.spi.STBootloaderSPIHandle`, checks offroad/disengaged, enters ROM via `HARDWARE.recover_internal_panda()`, verifies chip `0x483` and UID `1d002d000a51303436323838`, reads all 1,048,576 flash bytes in 256-byte chunks, compares current application to rollback bytes, sets `spi.MAX_XFER_RETRY_COUNT=1`, erases only sector 1, programs `0x08020000`, rereads all flash, proves bootstub and other sectors unchanged, candidate exact and sector padding all FF, then resets only after successful proof.

The old script is NOT directly runnable for this deployment: its root, candidate hash and rollback filenames are hardcoded, and its old rollback file was deleted during the previous approved cleanup. Do not overwrite the October 7 audit directory or run that original script unchanged.

## Available rollback material, freshly checked read-only

- Current application backup: `/data/dev-sp-c3xl-audit-20261007/panda_h7-candidate.bin.signed`, 106,024 bytes, SHA256 `f9a93028e57858b5e1a1b716e23a95f15db3599891f4e1fb0d5ffd2d5b11f0ce`.
- Its previous full readback: `/data/dev-sp-c3xl-audit-20261007/flash-after.bin`, 1,048,576 bytes, SHA256 `7430a3722b808c6993cfe2abfd0ae07a29f14d17d11e6cacbcaf373d4a4652ae`.
- Stored full readback contains exactly that application at offset 0x20000, and the rest of sector 1 through offset 0x40000 is all FF. This is archival proof, not a new MCU backup. The new updater must still read and save the current full flash before erase.
- Previous source/library: `/data/dev-sp-c3xl-20261007`, retaining the matching 106,024-byte firmware. Prefer its known-working ROM updater library for the same guarded procedure; record its resolved source HEAD and hashes before running.

## Concrete parent-run preparation and invocation

1. Create a fresh audit directory for this deployment, preserve the old archive, copy the retained current application above as the new `panda_h7-before.bin.signed`, and copy the final **device-built** H7 candidate as `panda_h7-candidate.bin.signed`. Record both lengths/SHA256/signatures and candidate Panda/opendbc/main source identities. Candidate SHA is intentionally not filled from the Mac build: device build is still pending.
2. Copy the inspected updater into the new directory. Change only its `root` literal and candidate SHA literal to these newly recorded values. Keep chip/UID, <=0x20000 bound, address, sector, full readback and one-attempt guard unchanged. Add/check rollback file SHA against the value above before entering ROM. Review the exact diff.
3. Immediately before invocation verify detached spare, service/manager/pandad stopped, IsOffroad=1, IsEngaged=0, actual current profile c3xl, expected UID/type09/H7, ignition/control flags false, current application signature equals retained rollback signature. Do not run in parallel with AGNOS writes or any Panda owner.
4. From the retained source directory, the established invocation shape is `cd /data/dev-sp-c3xl-20261007 && PYTHONPATH=$PWD:$PWD/opendbc_repo:$PWD/msgq_repo /usr/local/venv/bin/python <NEW_AUDIT>/flash_application.py`. The explicit new audit path and candidate hash must be resolved before execution, never use the original October 7 script path.
5. On any erase/program/readback exception: stop, do not automatically rerun or reset. Preserve full backup and output. On successful full receipt, verify running signature equals candidate, type/UID/version, faults=0 and controls/ignition false; stable SPI counters are a separate runtime check.

## Rollback using the same proven sequence

Rollback is another explicit operation, never an automatic retry. Use the new full `flash-before.bin` as the preservation reference and the retained 106,024-byte signed application as rollback candidate. Review a new copy of the same script with reversed application identities and a new output directory. If the failed candidate is partial, the original application's equality assertion cannot be reused blindly; inspect full readback first and authorize the bounded recovery separately. Only sector 1 may be erased; after restoring, require full 1MiB equality to the new preflash backup (not just signature) before reset. Bootstub and other sectors are not rewritten.

Normal Panda.flash may now be protocol-compatible, but that alone does not supply full pre/post readback proof. The existing inspected application-only path already supplies the requested invariants and avoids changing the bootstub.
