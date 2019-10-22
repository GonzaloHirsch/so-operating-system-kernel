#!/bin/bash
if [[ "$1" == "gdb" ]]; then
	qemu-system-x86_64 -soundhw pcspk -hda Image/x64BareBonesImage.qcow2 -rtc base=localtime -m 512 -s -S -d int | grep v=13
else
qemu-system-x86_64 -soundhw pcspk -hda Image/x64BareBonesImage.qcow2 -rtc base=localtime -m 512 -d int | grep v=13
fi
