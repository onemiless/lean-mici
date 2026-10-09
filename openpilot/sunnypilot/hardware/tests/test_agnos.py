import json
from pathlib import Path

import pytest

from openpilot.sunnypilot.hardware.agnos import UnsafeBootChainManifest, validate_agnos_manifest
from openpilot.sunnypilot.hardware.profile import HardwareProfile


REPO_ROOT = Path(__file__).parents[4]
STANDARD_MANIFEST = Path(__file__).parents[3] / "common/hardware/comma/agnos.json"
C3XL_MANIFEST = Path(__file__).parents[3] / "common/hardware/comma/agnos-c3xl.json"


def read_manifest(path: Path = C3XL_MANIFEST) -> list[dict]:
  with path.open() as manifest_file:
    return json.load(manifest_file)


def test_c3xl_manifest_matches_allowlist() -> None:
  validate_agnos_manifest(read_manifest(), HardwareProfile.C3XL)


@pytest.mark.parametrize("name", ["xbl", "xbl_config", "abl", "aop", "devcfg", "boot"])
def test_c3xl_rejects_changed_boot_chain_image(name: str) -> None:
  manifest = read_manifest()
  partition = next(partition for partition in manifest if partition["name"] == name)
  partition["hash"] = "0" * 64

  with pytest.raises(UnsafeBootChainManifest, match=name):
    validate_agnos_manifest(manifest, HardwareProfile.C3XL)


def test_standard_profile_does_not_apply_c3xl_allowlist() -> None:
  manifest = read_manifest(STANDARD_MANIFEST)
  next(partition for partition in manifest if partition["name"] == "boot")["hash"] = "0" * 64
  validate_agnos_manifest(manifest, HardwareProfile.STANDARD)
