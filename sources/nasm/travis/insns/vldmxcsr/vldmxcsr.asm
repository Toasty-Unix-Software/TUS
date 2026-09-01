default rel
	vldmxcsr dword [0xa77]
	vldmxcsr dword [0x3ec]
	vldmxcsr dword [eax+1]
	vldmxcsr dword [eax+64]
	vldmxcsr [0x4af]
