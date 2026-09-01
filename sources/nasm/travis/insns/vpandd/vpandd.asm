default rel
	vpandd xmm3, xmm5, xmm4
	vpandd xmm3, xmm4
	vpandd xmm2, xmm0, xmm5
	vpandd ymm0, ymm4, yword [0xc9c]
	vpandd ymm0, yword [0xc9c]
	vpandd ymm2, ymm2, ymm1
	vpandd zmm5, zmm1, zmm3
	vpandd zmm5, zmm3
	vpandd zmm6, zmm2, zmm1
	vpandd xmm8, xmm12, xmm13
	vpandd ymm15, ymm15, ymm10
	vpandd zmm9, zmm14, zmm14
	vpandd xmm24, xmm18, xmm29
	vpandd ymm19, ymm30, ymm22
	vpandd zmm24, zmm20, zmm19
	vpandd xmm2{k1}, xmm0, oword [0x84a]
	vpandd ymm7{k5}, ymm1, yword [0x8ba]
	vpandd zmm7{k2}, zmm5, zword [0xd84]
	vpandd xmm5{k1}{z}, xmm4, xmm7
	vpandd ymm7{k3}{z}, ymm5, ymm2
	vpandd zmm0{k4}{z}, zmm3, zword [0x133]
	vpandd xmm6, xmm5, oword [eax+1]
	vpandd xmm6, xmm5, oword [eax+64]
	vpandd ymm2, ymm0, yword [eax+1]
	vpandd ymm2, ymm2, yword [eax+64]
	vpandd zmm5, zmm3, zword [eax+1]
	vpandd zmm3, zmm3, zword [eax+64]
	vpandd xmm3, xmm4, [0xc2c]
	vpandd ymm5, ymm2, [0xdde]
	vpandd zmm1, zmm5, [0x66f]
