default rel
	vpermi2w xmm3, xmm0, oword [0x4ea]
	vpermi2w xmm6, xmm4, xmm0
	vpermi2w ymm3, ymm1, yword [0x5a7]
	vpermi2w ymm4, ymm4, ymm5
	vpermi2w zmm3, zmm0, zmm7
	vpermi2w zmm7, zmm6, zmm3
	vpermi2w xmm11, xmm9, xmm14
	vpermi2w ymm14, ymm11, ymm9
	vpermi2w zmm10, zmm10, zmm10
	vpermi2w xmm21, xmm28, xmm31
	vpermi2w ymm24, ymm21, ymm27
	vpermi2w zmm23, zmm22, zmm21
	vpermi2w xmm4{k2}, xmm0, oword [0x64f]
	vpermi2w ymm2{k2}, ymm5, ymm6
	vpermi2w zmm6{k2}, zmm5, zword [0x951]
	vpermi2w xmm6{k3}{z}, xmm4, xmm7
	vpermi2w ymm1{k1}{z}, ymm3, yword [0x50e]
	vpermi2w zmm7{k1}{z}, zmm4, zmm2
	vpermi2w xmm3, xmm1, oword [eax+1]
	vpermi2w xmm2, xmm5, oword [eax+64]
	vpermi2w ymm1, ymm5, yword [eax+1]
	vpermi2w ymm3, ymm7, yword [eax+64]
	vpermi2w zmm4, zmm5, zword [eax+1]
	vpermi2w zmm5, zmm0, zword [eax+64]
	vpermi2w xmm2, xmm6, [0x1b7]
	vpermi2w ymm1, ymm6, [0x25c]
	vpermi2w zmm5, zmm2, [0x987]
