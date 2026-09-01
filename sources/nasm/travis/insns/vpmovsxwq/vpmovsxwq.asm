default rel
	vpmovsxwq xmm6, xmm3
	vpmovsxwq xmm4, xmm7
	vpmovsxwq ymm0, qword [0x124]
	vpmovsxwq ymm5, qword [0xacc]
	vpmovsxwq ymm5, xmm2
	vpmovsxwq ymm2, xmm6
	vpmovsxwq xmm5, dword [0x4bb]
	vpmovsxwq xmm5, xmm4
	vpmovsxwq xmm12, xmm13
	vpmovsxwq ymm11, qword [0xe51]
	vpmovsxwq ymm10, xmm14
	vpmovsxwq xmm8, xmm14
	vpmovsxwq xmm26, xmm16
	vpmovsxwq ymm18, qword [0x6a4]
	vpmovsxwq ymm26, xmm30
	vpmovsxwq xmm30, xmm26
	vpmovsxwq xmm5{k3}, xmm7
	vpmovsxwq ymm6{k3}, xmm2
	vpmovsxwq zmm2{k4}, xmm2
	vpmovsxwq xmm4{k1}{z}, xmm4
	vpmovsxwq ymm7{k5}{z}, xmm0
	vpmovsxwq zmm4{k6}{z}, xmm4
	vpmovsxwq xmm0, dword [eax+1]
	vpmovsxwq xmm4, dword [eax+64]
	vpmovsxwq ymm4, qword [eax+1]
	vpmovsxwq ymm2, qword [eax+64]
	vpmovsxwq xmm2, dword [eax+1]
	vpmovsxwq xmm5, dword [eax+64]
	vpmovsxwq ymm4, qword [eax+1]
	vpmovsxwq ymm6, qword [eax+64]
	vpmovsxwq zmm7, oword [eax+1]
	vpmovsxwq zmm6, oword [eax+64]
	vpmovsxwq xmm1, [0xe3f]
	vpmovsxwq ymm1, [0xfed]
	vpmovsxwq xmm0, [0x3d2]
	vpmovsxwq ymm6, [0x424]
	vpmovsxwq zmm5, [0x888]
