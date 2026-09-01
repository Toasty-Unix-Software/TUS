default rel
	vpmovsxdq xmm0, xmm4
	vpmovsxdq xmm3, qword [0x64e]
	vpmovsxdq ymm5, xmm6
	vpmovsxdq ymm1, xmm6
	vpmovsxdq xmm3, qword [0x708]
	vpmovsxdq xmm1, qword [0x423]
	vpmovsxdq ymm0, xmm6
	vpmovsxdq ymm4, oword [0x60a]
	vpmovsxdq xmm15, xmm13
	vpmovsxdq ymm11, xmm8
	vpmovsxdq xmm15, xmm8
	vpmovsxdq ymm11, xmm13
	vpmovsxdq xmm29, xmm28
	vpmovsxdq ymm23, xmm31
	vpmovsxdq xmm19, xmm29
	vpmovsxdq ymm17, xmm27
	vpmovsxdq xmm5{k5}, xmm0
	vpmovsxdq ymm7{k6}, xmm0
	vpmovsxdq zmm4{k4}, ymm3
	vpmovsxdq xmm6{k5}{z}, qword [0x667]
	vpmovsxdq ymm6{k5}{z}, xmm5
	vpmovsxdq zmm7{k6}{z}, yword [0xd95]
	vpmovsxdq xmm5, qword [eax+1]
	vpmovsxdq xmm2, qword [eax+64]
	vpmovsxdq ymm0, oword [eax+1]
	vpmovsxdq ymm5, oword [eax+64]
	vpmovsxdq xmm7, qword [eax+1]
	vpmovsxdq xmm6, qword [eax+64]
	vpmovsxdq ymm7, oword [eax+1]
	vpmovsxdq ymm7, oword [eax+64]
	vpmovsxdq zmm6, yword [eax+1]
	vpmovsxdq zmm5, yword [eax+64]
	vpmovsxdq xmm0, [0xba7]
	vpmovsxdq ymm2, [0xbcf]
	vpmovsxdq xmm5, [0x6b4]
	vpmovsxdq ymm4, [0x7a4]
	vpmovsxdq zmm5, [0x8a3]
