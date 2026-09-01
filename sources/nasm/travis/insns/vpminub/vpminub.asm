default rel
	vpminub xmm6, xmm6, xmm2
	vpminub xmm6, xmm2
	vpminub xmm5, xmm2, xmm1
	vpminub ymm0, ymm0, ymm0
	vpminub ymm0, ymm0
	vpminub ymm7, ymm2, ymm5
	vpminub xmm7, xmm7, xmm5
	vpminub xmm7, xmm5
	vpminub xmm3, xmm1, xmm2
	vpminub ymm3, ymm0, ymm4
	vpminub ymm3, ymm4
	vpminub ymm7, ymm0, yword [0x198]
	vpminub xmm11, xmm9, xmm9
	vpminub ymm9, ymm9, ymm12
	vpminub xmm8, xmm9, xmm11
	vpminub ymm13, ymm15, ymm11
	vpminub xmm20, xmm17, xmm25
	vpminub ymm31, ymm19, ymm25
	vpminub xmm21, xmm16, xmm30
	vpminub ymm26, ymm27, ymm16
	vpminub xmm3{k5}, xmm2, xmm5
	vpminub ymm6{k4}, ymm4, yword [0x8a0]
	vpminub zmm0{k1}, zmm6, zword [0xf33]
	vpminub xmm7{k7}{z}, xmm4, xmm6
	vpminub ymm6{k3}{z}, ymm0, yword [0x166]
	vpminub zmm4{k7}{z}, zmm4, zmm3
	vpminub xmm5, xmm1, oword [eax+1]
	vpminub xmm5, xmm7, oword [eax+64]
	vpminub ymm0, ymm7, yword [eax+1]
	vpminub ymm5, ymm1, yword [eax+64]
	vpminub xmm0, xmm4, oword [eax+1]
	vpminub xmm4, xmm3, oword [eax+64]
	vpminub ymm0, ymm7, yword [eax+1]
	vpminub ymm6, ymm3, yword [eax+64]
	vpminub zmm1, zmm3, zword [eax+1]
	vpminub zmm5, zmm6, zword [eax+64]
	vpminub xmm3, xmm3, [0x934]
	vpminub ymm0, ymm7, [0x89d]
	vpminub xmm1, xmm1, [0x7fa]
	vpminub ymm4, ymm3, [0x3ce]
	vpminub zmm1, zmm1, [0xe02]
