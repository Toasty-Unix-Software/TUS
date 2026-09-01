default rel
	vfmsub231pd xmm3, xmm7, xmm0
	vfmsub231pd xmm4, xmm4, xmm5
	vfmsub231pd ymm4, ymm5, ymm3
	vfmsub231pd ymm1, ymm5, ymm2
	vfmsub231pd xmm5, xmm3, xmm7
	vfmsub231pd xmm4, xmm5, xmm4
	vfmsub231pd ymm4, ymm7, ymm2
	vfmsub231pd ymm3, ymm3, ymm7
	vfmsub231pd xmm12, xmm15, xmm8
	vfmsub231pd ymm10, ymm10, ymm13
	vfmsub231pd xmm8, xmm8, xmm12
	vfmsub231pd ymm12, ymm11, ymm13
	vfmsub231pd xmm16, xmm21, xmm22
	vfmsub231pd ymm31, ymm16, ymm25
	vfmsub231pd xmm24, xmm20, xmm17
	vfmsub231pd ymm29, ymm28, ymm27
	vfmsub231pd xmm5{k5}, xmm3, xmm1
	vfmsub231pd ymm6{k3}, ymm6, yword [0xfab]
	vfmsub231pd zmm7{k7}, zmm1, zmm0
	vfmsub231pd xmm3{k6}{z}, xmm5, oword [0x1c2]
	vfmsub231pd ymm0{k7}{z}, ymm2, yword [0x104]
	vfmsub231pd zmm7{k7}{z}, zmm2, zmm0
	vfmsub231pd zmm6, zmm2, zmm7, {rd-sae}
	vfmsub231pd xmm6, xmm3, oword [eax+1]
	vfmsub231pd xmm5, xmm1, oword [eax+64]
	vfmsub231pd ymm3, ymm3, yword [eax+1]
	vfmsub231pd ymm7, ymm6, yword [eax+64]
	vfmsub231pd xmm4, xmm4, oword [eax+1]
	vfmsub231pd xmm6, xmm0, oword [eax+64]
	vfmsub231pd ymm5, ymm3, yword [eax+1]
	vfmsub231pd ymm5, ymm7, yword [eax+64]
	vfmsub231pd zmm2, zmm2, zword [eax+1]
	vfmsub231pd zmm7, zmm0, zword [eax+64]
	vfmsub231pd xmm6, xmm5, [0xd09]
	vfmsub231pd ymm2, ymm1, [0xa61]
	vfmsub231pd xmm6, xmm4, [0xe24]
	vfmsub231pd ymm4, ymm4, [0x56a]
	vfmsub231pd zmm1, zmm4, [0xd2c]
