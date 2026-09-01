default rel
	vpmovsxwd xmm1, xmm3
	vpmovsxwd xmm5, xmm5
	vpmovsxwd ymm2, oword [0x344]
	vpmovsxwd ymm5, xmm1
	vpmovsxwd xmm3, xmm6
	vpmovsxwd xmm7, qword [0xb57]
	vpmovsxwd ymm6, xmm6
	vpmovsxwd ymm0, xmm6
	vpmovsxwd xmm9, xmm15
	vpmovsxwd ymm13, xmm11
	vpmovsxwd xmm14, xmm13
	vpmovsxwd ymm9, xmm9
	vpmovsxwd xmm30, xmm17
	vpmovsxwd ymm18, xmm17
	vpmovsxwd xmm29, xmm17
	vpmovsxwd ymm18, xmm27
	vpmovsxwd xmm6{k5}, xmm1
	vpmovsxwd ymm1{k7}, xmm2
	vpmovsxwd zmm0{k7}, yword [0x815]
	vpmovsxwd xmm7{k2}{z}, xmm2
	vpmovsxwd ymm6{k6}{z}, xmm3
	vpmovsxwd zmm5{k4}{z}, yword [0x9fe]
	vpmovsxwd xmm1, qword [eax+1]
	vpmovsxwd xmm7, qword [eax+64]
	vpmovsxwd ymm0, oword [eax+1]
	vpmovsxwd ymm4, oword [eax+64]
	vpmovsxwd xmm6, qword [eax+1]
	vpmovsxwd xmm6, qword [eax+64]
	vpmovsxwd ymm7, oword [eax+1]
	vpmovsxwd ymm1, oword [eax+64]
	vpmovsxwd zmm1, yword [eax+1]
	vpmovsxwd zmm0, yword [eax+64]
	vpmovsxwd xmm0, [0x475]
	vpmovsxwd ymm4, [0xa48]
	vpmovsxwd xmm4, [0x4e8]
	vpmovsxwd ymm3, [0xa05]
	vpmovsxwd zmm5, [0xcb3]
