# C3XL rollback materials and order

Device identity: comma-9c13c9f5; profile c3xl; internal H725 UID 1d002d000a51303436323838. These paths are specific to the 2026-10-09 deployment. Recheck identity, detached/offroad status, controls/ignition false and exclusive access before any future recovery.

## Preserved state

- Original system: slot `_a`, AGNOS 19.7. Its first 4,718,592,000 bytes were hashed before and after the inactive-slot write: `7b3a1303c2638f3f65732e510ca55d676defe2cbee94eec91578f6411445c944`, unchanged.
- Original running checkout: `/data/devsp-rollback-20261009`, original `dev-sp@0f694538fa8f84190ce9751f106f879a62550f8a` plus its existing prebuilt marker.
- Original startup entry: `/data/tesla-port-preflight/continue.sh.before`.
- Original H7 signed application: `/data/lean-tesla-panda-20261009/panda_h7-before.bin.signed`, SHA256 `f9a93028e57858b5e1a1b716e23a95f15db3599891f4e1fb0d5ffd2d5b11f0ce`.
- Fresh whole-MCU preflash backup: `/data/lean-tesla-panda-20261009/flash-before.bin`, 1 MiB, SHA256 `877c0be89741c2bbc68cbf6d9fae80b433ff48d0f7f543bcc1631e7b7bb8dac8`.
- Current signed application: SHA256 `13c99877629f0ebe4662634de2fb9011cab68458cf81532e23caffddf57708c4`; whole verified postflash image SHA256 `c8ddbf0101f3f86a0492a0bb0fd34255e0c29571199d45be889321977996c957`.
- Protected Mac copies: `/Users/mile/work/tesla-port/private-rollback/`. This directory and parameter/persist archives are deliberately outside Git. Do not publish them.

## Order matters

1. Stop comma and keep startup in maintenance while recovering. Never start old pandad against an unverified firmware state.
2. Restore the old H7 application using a newly reviewed copy of the guarded procedure in `PANDA_FLASH_HANDOFF.md`, with the old/current candidate roles reversed. Use a new audit directory, fresh full readback, exact UID/chip/SHA guards, sector 1 only, one write attempt and complete preservation proof. The original October 7 script must not be run unchanged. Read-only ACK verification may need the separately documented 100 ms floor; never respond to an ACK error by repeating an erase/program operation.
3. After full readback and running signature verification, restore the original checkout to `/data/openpilot` and restore `continue.sh.before`. Preserve the current release directory. Update only the updater target back to `dev-sp`; do not blindly overwrite later calibration with the entire parameter archive.
4. Select old slot with `sudo abctl --set_active 0`, reboot, and verify `/VERSION`, active slot, profile, Panda signature, camera streams and offroad process health. Do not use `agnos.py --verify` as a read-only command: it can switch slots.

If only recovering shell access, keep the startup entry in maintenance first, then switch to `_a`. The application, firmware and OS restoration are separate steps. The preserved data is rollback material, not a claim that rollback was executed during this task.
