default rel
	vpmovsxbd xmm4, xmm3
	vpmovsxbd xmm5, dword [0x676]
	vpmovsxbd ymm5, qword [0x194]
	vpmovsxbd ymm6, qword [0xced]
	vpmovsxbd ymm0, xmm5
	vpmovsxbd ymm0, xmm2
	vpmovsxbd xmm7, xmm2
	vpmovsxbd xmm5, xmm6
	vpmovsxbd xmm14, xmm14
	vpmovsxbd ymm8, qword [0x6d7]
	vpmovsxbd ymm11, xmm10
	vpmovsxbd xmm15, xmm12
	vpmovsxbd xmm20, xmm24
	vpmovsxbd ymm17, qword [0xd9b]
	vpmovsxbd ymm19, xmm24
	vpmovsxbd xmm19, xmm29
	vpmovsxbd xmm1{k6}, xmm5
	vpmovsxbd ymm2{k5}, qword [0x245]
	vpmovsxbd zmm2{k7}, xmm1
	vpmovsxbd xmm6{k3}{z}, xmm3
	vpmovsxbd ymm1{k6}{z}, xmm1
	vpmovsxbd zmm5{k4}{z}, xmm4
	vpmovsxbd xmm4, dword [eax+1]
	vpmovsxbd xmm0, dword [eax+64]
	vpmovsxbd ymm3, qword [eax+1]
	vpmovsxbd ymm7, qword [eax+64]
	vpmovsxbd xmm2, dword [eax+1]
	vpmovsxbd xmm5, dword [eax+64]
	vpmovsxbd ymm6, qword [eax+1]
	vpmovsxbd ymm7, qword [eax+64]
	vpmovsxbd zmm3, oword [eax+1]
	vpmovsxbd zmm6, oword [eax+64]
	vpmovsxbd xmm2, [0xeed]
	vpmovsxbd ymm0, [0xa0d]
	vpmovsxbd xmm1, [0x621]
	vpmovsxbd ymm6, [0x460]
	vpmovsxbd zmm6, [0xa2d]
