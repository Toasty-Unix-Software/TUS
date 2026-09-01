default rel
	vpmovsxbq xmm7, xmm5
	vpmovsxbq xmm7, xmm5
	vpmovsxbq ymm3, dword [0x898]
	vpmovsxbq ymm4, dword [0x643]
	vpmovsxbq ymm2, xmm5
	vpmovsxbq ymm5, xmm3
	vpmovsxbq xmm0, xmm1
	vpmovsxbq xmm2, xmm7
	vpmovsxbq xmm11, xmm10
	vpmovsxbq ymm10, dword [0x104]
	vpmovsxbq ymm9, xmm10
	vpmovsxbq xmm9, xmm14
	vpmovsxbq xmm24, xmm29
	vpmovsxbq ymm26, dword [0xe37]
	vpmovsxbq ymm20, xmm30
	vpmovsxbq xmm16, xmm16
	vpmovsxbq xmm0{k1}, xmm2
	vpmovsxbq ymm2{k3}, xmm4
	vpmovsxbq zmm3{k7}, xmm2
	vpmovsxbq xmm6{k1}{z}, xmm7
	vpmovsxbq ymm5{k4}{z}, xmm1
	vpmovsxbq zmm5{k2}{z}, qword [0xf8c]
	vpmovsxbq xmm7, word [eax+1]
	vpmovsxbq xmm7, word [eax+64]
	vpmovsxbq ymm2, dword [eax+1]
	vpmovsxbq ymm0, dword [eax+64]
	vpmovsxbq xmm1, word [eax+1]
	vpmovsxbq xmm3, word [eax+64]
	vpmovsxbq ymm5, dword [eax+1]
	vpmovsxbq ymm7, dword [eax+64]
	vpmovsxbq zmm0, qword [eax+1]
	vpmovsxbq zmm3, qword [eax+64]
	vpmovsxbq xmm4, [0x2f3]
	vpmovsxbq ymm3, [0xce8]
	vpmovsxbq xmm5, [0x534]
	vpmovsxbq ymm4, [0x1bc]
	vpmovsxbq zmm2, [0x577]
