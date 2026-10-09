from collections.abc import Mapping, Sequence
from dataclasses import dataclass

from openpilot.sunnypilot.hardware.profile import HardwareProfile, get_hardware_profile


@dataclass(frozen=True)
class BootChainImage:
  hash: str
  hash_raw: str
  size: int


class UnsafeBootChainManifest(RuntimeError):
  pass


# Generated from docs/tesla/ref/agnos by tools/tesla/gen_bootchain_allowlist.py.
C3_BOOT_CHAIN_ALLOWLIST: Mapping[str, BootChainImage] = {
  "xbl": BootChainImage("e8acf2a9cc7f0ce84cb803bfea9477f765c0d7b4daf26048e59651b9e6a7bfbb", "e8acf2a9cc7f0ce84cb803bfea9477f765c0d7b4daf26048e59651b9e6a7bfbb", 3282256),
  "xbl_config": BootChainImage("758552ecf92b5569677197783bf0ccb73d7f961685308e45d3276ac9dd974f85", "758552ecf92b5569677197783bf0ccb73d7f961685308e45d3276ac9dd974f85", 98124),
  "abl": BootChainImage("29fd7ed1c012e599420764840f9f11286d34dbff4adaf102a447f06d8c5e0b35", "29fd7ed1c012e599420764840f9f11286d34dbff4adaf102a447f06d8c5e0b35", 274432),
  "aop": BootChainImage("78b2287ca219a0811b3004c523fa0f4749e4d1fd92be3aba61699305b7943ad1", "78b2287ca219a0811b3004c523fa0f4749e4d1fd92be3aba61699305b7943ad1", 184364),
  "devcfg": BootChainImage("f71df3a86958c093ba3969254c4db025187eef9385427f1ade946742939b43cc", "f71df3a86958c093ba3969254c4db025187eef9385427f1ade946742939b43cc", 40336),
  "boot": BootChainImage("6ecf6f987cd11968104abcccabbe268485d329cdb73012dfd3c381a6b8deb27d", "6ecf6f987cd11968104abcccabbe268485d329cdb73012dfd3c381a6b8deb27d", 46897152),
}

C3XL_BOOT_CHAIN_ALLOWLIST: Mapping[str, BootChainImage] = {
  "xbl": BootChainImage("e8acf2a9cc7f0ce84cb803bfea9477f765c0d7b4daf26048e59651b9e6a7bfbb", "e8acf2a9cc7f0ce84cb803bfea9477f765c0d7b4daf26048e59651b9e6a7bfbb", 3282256),
  "xbl_config": BootChainImage("758552ecf92b5569677197783bf0ccb73d7f961685308e45d3276ac9dd974f85", "758552ecf92b5569677197783bf0ccb73d7f961685308e45d3276ac9dd974f85", 98124),
  "abl": BootChainImage("32a2174b5f764e95dfc54cf358ba01752943b1b3b90e626149c3da7d5f1830b6", "32a2174b5f764e95dfc54cf358ba01752943b1b3b90e626149c3da7d5f1830b6", 274432),
  "aop": BootChainImage("78b2287ca219a0811b3004c523fa0f4749e4d1fd92be3aba61699305b7943ad1", "78b2287ca219a0811b3004c523fa0f4749e4d1fd92be3aba61699305b7943ad1", 184364),
  "devcfg": BootChainImage("f71df3a86958c093ba3969254c4db025187eef9385427f1ade946742939b43cc", "f71df3a86958c093ba3969254c4db025187eef9385427f1ade946742939b43cc", 40336),
  "boot": BootChainImage("0191529aa97d90d1fa04b472d80230b777606459e1e1e9e2323c9519839827b4", "0191529aa97d90d1fa04b472d80230b777606459e1e1e9e2323c9519839827b4", 18515968),
}



def validate_agnos_manifest(partitions: Sequence[dict], profile: HardwareProfile | None = None) -> None:
  selected = profile or get_hardware_profile()
  if selected == HardwareProfile.STANDARD:
    return
  allowlist = C3XL_BOOT_CHAIN_ALLOWLIST if selected == HardwareProfile.C3XL else C3_BOOT_CHAIN_ALLOWLIST

  by_name = {partition.get("name"): partition for partition in partitions}
  if len(by_name) != len(partitions):
    raise UnsafeBootChainManifest("AGNOS manifest contains duplicate or unnamed partitions")

  for name, allowed in allowlist.items():
    partition = by_name.get(name)
    if partition is None:
      raise UnsafeBootChainManifest(f"{selected.value} AGNOS manifest is missing required boot-chain partition {name!r}")

    actual = BootChainImage(
      hash=str(partition.get("hash", "")).lower(),
      hash_raw=str(partition.get("hash_raw", "")).lower(),
      size=partition.get("size", -1),
    )
    if actual != allowed:
      raise UnsafeBootChainManifest(
        f"{selected.value} refuses unvalidated {name!r} image: expected hash={allowed.hash} size={allowed.size}, got hash={actual.hash} size={actual.size}"
      )

    if partition.get("has_ab", True) is not True or partition.get("full_check", False) is not True:
      raise UnsafeBootChainManifest(f"{selected.value} requires full A/B verification for boot-chain partition {name!r}")
