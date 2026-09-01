default rel
	vrcpph xmm3, oword [0xfb1]
	vrcpph xmm3, xmm4
	vrcpph ymm2, ymm7
	vrcpph ymm7, yword [0xfd9]
	vrcpph zmm2, zword [0xa11]
	vrcpph zmm1, zword [0x62b]
	vrcpph xmm8, xmm10
	vrcpph ymm11, ymm8
	vrcpph zmm9, zmm13
	vrcpph xmm24, xmm24
	vrcpph ymm29, ymm22
	vrcpph zmm16, zmm26
	vrcpph xmm0{k7}, xmm1
	vrcpph ymm7{k1}, ymm1
	vrcpph zmm2{k5}, zmm7
	vrcpph xmm0{k4}{z}, oword [0x89f]
	vrcpph ymm0{k7}{z}, yword [0xbff]
	vrcpph zmm0{k5}{z}, zmm1
	vrcpph xmm1, oword [eax+1]
	vrcpph xmm3, oword [eax+64]
	vrcpph ymm3, yword [eax+1]
	vrcpph ymm6, yword [eax+64]
	vrcpph zmm7, zword [eax+1]
	vrcpph zmm3, zword [eax+64]
	vrcpph xmm0, [0x685]
	vrcpph ymm3, [0x9f0]
	vrcpph zmm3, [0xa2e]
