default rel
	vfnmsub132ps xmm0, xmm4, xmm1
	vfnmsub132ps xmm0, xmm1, oword [0x58b]
	vfnmsub132ps ymm2, ymm7, ymm6
	vfnmsub132ps ymm2, ymm6, yword [0xd2b]
	vfnmsub132ps xmm7, xmm4, xmm4
	vfnmsub132ps xmm7, xmm3, xmm4
	vfnmsub132ps ymm1, ymm2, yword [0xc7d]
	vfnmsub132ps ymm4, ymm6, ymm5
	vfnmsub132ps xmm11, xmm15, xmm11
	vfnmsub132ps ymm8, ymm13, ymm12
	vfnmsub132ps xmm13, xmm9, xmm14
	vfnmsub132ps ymm13, ymm11, ymm13
	vfnmsub132ps xmm22, xmm30, xmm22
	vfnmsub132ps ymm30, ymm25, ymm20
	vfnmsub132ps xmm25, xmm18, xmm17
	vfnmsub132ps ymm29, ymm18, ymm31
	vfnmsub132ps xmm2{k2}, xmm4, xmm5
	vfnmsub132ps ymm0{k2}, ymm1, ymm4
	vfnmsub132ps zmm1{k6}, zmm7, zmm2
	vfnmsub132ps xmm6{k1}{z}, xmm2, xmm4
	vfnmsub132ps ymm3{k2}{z}, ymm5, ymm7
	vfnmsub132ps zmm0{k6}{z}, zmm4, zmm7
	vfnmsub132ps zmm4, zmm2, zmm4, {rd-sae}
	vfnmsub132ps xmm1, xmm3, oword [eax+1]
	vfnmsub132ps xmm2, xmm3, oword [eax+64]
	vfnmsub132ps ymm7, ymm2, yword [eax+1]
	vfnmsub132ps ymm6, ymm6, yword [eax+64]
	vfnmsub132ps xmm7, xmm3, oword [eax+1]
	vfnmsub132ps xmm2, xmm2, oword [eax+64]
	vfnmsub132ps ymm7, ymm5, yword [eax+1]
	vfnmsub132ps ymm5, ymm2, yword [eax+64]
	vfnmsub132ps zmm2, zmm3, zword [eax+1]
	vfnmsub132ps zmm1, zmm4, zword [eax+64]
	vfnmsub132ps xmm3, xmm0, [0xa99]
	vfnmsub132ps ymm7, ymm1, [0xeca]
	vfnmsub132ps xmm7, xmm2, [0xfd4]
	vfnmsub132ps ymm5, ymm1, [0x19c]
	vfnmsub132ps zmm6, zmm4, [0x6e1]
