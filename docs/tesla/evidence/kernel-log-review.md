# Kernel log comparison boundary

The old-system snapshot and final boot do not have identical error-text sets. The raw comparison is retained in kernel-error-comparison.json; it must not be reported as a strict zero-new-category pass.

The final CAM-IRQ-CTRL invalid-bottom-half subscription messages occur at boot times 2.395–2.403 seconds, before application/model execution. Later final live-camera inference starts around monotonic 704 seconds and passes. Other additions include USB/TYPEC messages after the USB cable was attached, Bluetooth power-state messages, modem diagnostic channels, fuel-gauge status and system_b mount-probe messages. The final root is successfully mounted read-only as ext4; the ext2/ext3 probe warnings do not mean ext4 mount failed.

These facts separate boot/probe messages from the verified successful camera/model/runtime window. They do not establish the cause of every new message. Functional acceptance passed; the task list's strict old-versus-new kernel-category gate remains open for a like-for-like cold-boot investigation. The prior build stall also has no proven root cause; stopping runtime before build is a deliberate isolation measure, not a claim to have repaired the kernel.
