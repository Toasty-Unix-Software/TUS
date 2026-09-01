default rel
	ldmxcsr dword [0xc54]
	ldmxcsr dword [0xd15]
	ldmxcsr dword [eax+1]
	ldmxcsr dword [eax+64]
	ldmxcsr [0xbf1]
