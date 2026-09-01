default rel
	vpermi2pd xmm1, xmm2, oword [0x857]
	vpermi2pd xmm2, xmm2, xmm7
	vpermi2pd ymm1, ymm3, ymm7
	vpermi2pd ymm4, ymm1, ymm4
	vpermi2pd zmm0, zmm5, zword [0x1e2]
	vpermi2pd zmm5, zmm6, zmm2
	vpermi2pd xmm15, xmm14, xmm10
	vpermi2pd ymm10, ymm9, ymm9
	vpermi2pd zmm8, zmm10, zmm11
	vpermi2pd xmm25, xmm24, xmm21
	vpermi2pd ymm29, ymm22, ymm17
	vpermi2pd zmm26, zmm18, zmm17
	vpermi2pd xmm2{k4}, xmm2, oword [0x3ba]
	vpermi2pd ymm7{k2}, ymm7, ymm4
	vpermi2pd zmm6{k6}, zmm4, zmm6
	vpermi2pd xmm6{k3}{z}, xmm6, oword [0x46d]
	vpermi2pd ymm2{k4}{z}, ymm4, ymm3
	vpermi2pd zmm0{k5}{z}, zmm1, zmm1
	vpermi2pd xmm6, xmm2, oword [eax+1]
	vpermi2pd xmm6, xmm1, oword [eax+64]
	vpermi2pd ymm2, ymm5, yword [eax+1]
	vpermi2pd ymm0, ymm6, yword [eax+64]
	vpermi2pd zmm0, zmm6, zword [eax+1]
	vpermi2pd zmm2, zmm7, zword [eax+64]
	vpermi2pd xmm2, xmm1, [0xeff]
	vpermi2pd ymm5, ymm5, [0x7ed]
	vpermi2pd zmm5, zmm7, [0x2c8]
