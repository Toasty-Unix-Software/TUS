default rel
	vpmovsdb xmm4, xmm4
	vpmovsdb xmm4, xmm3
	vpmovsdb xmm6, ymm3
	vpmovsdb xmm4, ymm5
	vpmovsdb xmm6, zmm5
	vpmovsdb xmm4, zmm6
	vpmovsdb dword [0x2ad], xmm7
	vpmovsdb dword [0x281], xmm4
	vpmovsdb xmm9, xmm11
	vpmovsdb xmm11, ymm13
	vpmovsdb xmm10, zmm13
	vpmovsdb dword [0x1b5], xmm15
	vpmovsdb xmm19, xmm26
	vpmovsdb xmm25, ymm16
	vpmovsdb xmm31, zmm30
	vpmovsdb dword [0x4f6], xmm23
	vpmovsdb xmm0{k5}, xmm6
	vpmovsdb xmm5{k5}, ymm5
	vpmovsdb xmm7{k5}, zmm2
	vpmovsdb dword [0xcac]{k2}, xmm3
	vpmovsdb qword [0x78a]{k5}, ymm6
	vpmovsdb oword [0x52a]{k3}, zmm2
	vpmovsdb dword [eax+1], xmm0
	vpmovsdb dword [eax+64], xmm7
	vpmovsdb qword [eax+1], ymm5
	vpmovsdb qword [eax+64], ymm0
	vpmovsdb oword [eax+1], zmm5
	vpmovsdb oword [eax+64], zmm4
	vpmovsdb [0x2e0], xmm0
	vpmovsdb [0xefe], ymm5
	vpmovsdb [0xbdb], zmm0
