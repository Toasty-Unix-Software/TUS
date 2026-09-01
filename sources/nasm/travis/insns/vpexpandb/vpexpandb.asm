default rel
	vpexpandb xmm0, oword [0x8fc]
	vpexpandb xmm4, oword [0x222]
	vpexpandb ymm0, yword [0x292]
	vpexpandb ymm0, yword [0xdcc]
	vpexpandb zmm7, zmm3
	vpexpandb zmm4, zword [0xbcb]
	vpexpandb xmm10, xmm12
	vpexpandb ymm13, ymm11
	vpexpandb zmm10, zmm9
	vpexpandb xmm31, xmm23
	vpexpandb ymm24, ymm30
	vpexpandb zmm23, zmm22
	vpexpandb xmm0{k7}, xmm6
	vpexpandb ymm6{k2}, yword [0x908]
	vpexpandb zmm7{k5}, zmm0
	vpexpandb xmm3{k2}{z}, xmm2
	vpexpandb ymm6{k1}{z}, ymm6
	vpexpandb zmm0{k2}{z}, zword [0x4b2]
	vpexpandb xmm5, oword [eax+1]
	vpexpandb xmm7, oword [eax+64]
	vpexpandb ymm7, yword [eax+1]
	vpexpandb ymm0, yword [eax+64]
	vpexpandb zmm4, zword [eax+1]
	vpexpandb zmm5, zword [eax+64]
	vpexpandb xmm7, [0x9c0]
	vpexpandb ymm1, [0x6f3]
	vpexpandb zmm7, [0x5fe]
