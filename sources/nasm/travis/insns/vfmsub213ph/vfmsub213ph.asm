default rel
	vfmsub213ph xmm5, xmm4, xmm5
	vfmsub213ph xmm5, xmm5
	vfmsub213ph xmm0, xmm7, xmm5
	vfmsub213ph ymm5, ymm7, ymm2
	vfmsub213ph ymm5, ymm2
	vfmsub213ph ymm2, ymm5, yword [0xa62]
	vfmsub213ph zmm3, zmm1, zmm6
	vfmsub213ph zmm3, zmm6
	vfmsub213ph zmm1, zmm3, zword [0x5ca]
	vfmsub213ph xmm10, xmm10, xmm12
	vfmsub213ph ymm15, ymm15, ymm14
	vfmsub213ph zmm14, zmm13, zmm10
	vfmsub213ph xmm28, xmm22, xmm30
	vfmsub213ph ymm29, ymm23, ymm31
	vfmsub213ph zmm18, zmm21, zmm26
	vfmsub213ph xmm2{k4}, xmm7, xmm0
	vfmsub213ph ymm7{k1}, ymm7, ymm1
	vfmsub213ph zmm7{k4}, zmm2, zword [0xef3]
	vfmsub213ph xmm0{k3}{z}, xmm7, oword [0xde3]
	vfmsub213ph ymm7{k7}{z}, ymm2, ymm7
	vfmsub213ph zmm0{k3}{z}, zmm0, zmm4
	vfmsub213ph zmm7, zmm4, zmm2, {rz-sae}
	vfmsub213ph xmm4, xmm3, oword [eax+1]
	vfmsub213ph xmm2, xmm6, oword [eax+64]
	vfmsub213ph ymm4, ymm3, yword [eax+1]
	vfmsub213ph ymm5, ymm3, yword [eax+64]
	vfmsub213ph zmm2, zmm4, zword [eax+1]
	vfmsub213ph zmm1, zmm4, zword [eax+64]
	vfmsub213ph xmm1, xmm4, [0x4b3]
	vfmsub213ph ymm0, ymm6, [0xb07]
	vfmsub213ph zmm3, zmm5, [0x2ef]
