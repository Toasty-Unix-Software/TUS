default rel
	vfixupimmss xmm4, xmm2, dword [0x998], 0x9c
	vfixupimmss xmm4, dword [0x998], 0x9c
	vfixupimmss xmm0, xmm0, xmm6, 0x77
	vfixupimmss xmm15, xmm14, xmm11, 0xf6
	vfixupimmss xmm18, xmm30, xmm16, 0x6a
	vfixupimmss xmm0{k3}, xmm4, xmm6, 0x5e
	vfixupimmss xmm3{k4}{z}, xmm1, dword [0x473], 0xb
	vfixupimmss xmm0, xmm3, dword [eax+1], 0xa2
	vfixupimmss xmm0, xmm6, dword [eax+64], 0x4b
	vfixupimmss xmm2, xmm2, [0xa3f], 0x8b
