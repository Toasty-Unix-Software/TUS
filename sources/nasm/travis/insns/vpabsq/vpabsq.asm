default rel
	vpabsq xmm3, oword [0x567]
	vpabsq xmm6, oword [0xad6]
	vpabsq ymm3, ymm6
	vpabsq ymm6, ymm5
	vpabsq zmm2, zword [0xd27]
	vpabsq zmm3, zmm1
	vpabsq xmm14, xmm9
	vpabsq ymm14, ymm12
	vpabsq zmm15, zmm12
	vpabsq xmm28, xmm31
	vpabsq ymm26, ymm16
	vpabsq zmm16, zmm20
	vpabsq xmm6{k2}, oword [0x204]
	vpabsq ymm4{k4}, ymm7
	vpabsq zmm5{k2}, zmm6
	vpabsq xmm0{k2}{z}, xmm2
	vpabsq ymm4{k1}{z}, yword [0xc67]
	vpabsq zmm1{k6}{z}, zmm7
	vpabsq xmm0, oword [eax+1]
	vpabsq xmm6, oword [eax+64]
	vpabsq ymm5, yword [eax+1]
	vpabsq ymm2, yword [eax+64]
	vpabsq zmm1, zword [eax+1]
	vpabsq zmm6, zword [eax+64]
	vpabsq xmm5, [0x2e9]
	vpabsq ymm3, [0x430]
	vpabsq zmm7, [0xea0]
