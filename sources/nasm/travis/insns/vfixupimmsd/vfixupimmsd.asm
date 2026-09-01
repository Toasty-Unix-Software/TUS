default rel
	vfixupimmsd xmm2, xmm2, xmm6, 0xa5
	vfixupimmsd xmm2, xmm6, 0xa5
	vfixupimmsd xmm6, xmm1, qword [0xa3e], 0xbb
	vfixupimmsd xmm15, xmm11, xmm13, 0x32
	vfixupimmsd xmm17, xmm28, xmm19, 0xc8
	vfixupimmsd xmm0{k5}, xmm4, qword [0xb51], 0x30
	vfixupimmsd xmm2{k6}{z}, xmm1, xmm1, 0x55
	vfixupimmsd xmm5, xmm1, qword [eax+1], 0xae
	vfixupimmsd xmm2, xmm7, qword [eax+64], 0xbe
	vfixupimmsd xmm1, xmm3, [0x8db], 0x10
