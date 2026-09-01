default rel
	vfmadd132ph xmm4, xmm5, xmm5
	vfmadd132ph xmm4, xmm5
	vfmadd132ph xmm7, xmm3, oword [0x1f3]
	vfmadd132ph ymm2, ymm7, yword [0xf14]
	vfmadd132ph ymm2, yword [0xf14]
	vfmadd132ph ymm5, ymm3, ymm3
	vfmadd132ph zmm5, zmm6, zmm0
	vfmadd132ph zmm5, zmm0
	vfmadd132ph zmm4, zmm0, zmm5
	vfmadd132ph xmm12, xmm11, xmm9
	vfmadd132ph ymm10, ymm10, ymm15
	vfmadd132ph zmm8, zmm11, zmm13
	vfmadd132ph xmm25, xmm29, xmm18
	vfmadd132ph ymm19, ymm20, ymm30
	vfmadd132ph zmm16, zmm18, zmm27
	vfmadd132ph xmm7{k1}, xmm1, xmm7
	vfmadd132ph ymm0{k4}, ymm0, ymm3
	vfmadd132ph zmm4{k1}, zmm4, zmm2
	vfmadd132ph xmm0{k7}{z}, xmm4, oword [0xd76]
	vfmadd132ph ymm5{k2}{z}, ymm0, ymm4
	vfmadd132ph zmm0{k1}{z}, zmm6, zword [0xb60]
	vfmadd132ph zmm2, zmm7, zmm0, {rn-sae}
	vfmadd132ph xmm6, xmm7, oword [eax+1]
	vfmadd132ph xmm5, xmm3, oword [eax+64]
	vfmadd132ph ymm0, ymm7, yword [eax+1]
	vfmadd132ph ymm7, ymm1, yword [eax+64]
	vfmadd132ph zmm6, zmm3, zword [eax+1]
	vfmadd132ph zmm2, zmm0, zword [eax+64]
	vfmadd132ph xmm2, xmm5, [0x745]
	vfmadd132ph ymm1, ymm7, [0xb0c]
	vfmadd132ph zmm0, zmm2, [0x45b]
