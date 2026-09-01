default rel
	vfmsub132ps xmm0, xmm5, xmm0
	vfmsub132ps xmm0, xmm0, xmm3
	vfmsub132ps ymm5, ymm7, yword [0x5c4]
	vfmsub132ps ymm3, ymm7, ymm7
	vfmsub132ps xmm0, xmm0, xmm6
	vfmsub132ps xmm4, xmm5, xmm3
	vfmsub132ps ymm3, ymm5, yword [0x2b8]
	vfmsub132ps ymm7, ymm3, ymm6
	vfmsub132ps xmm12, xmm10, xmm11
	vfmsub132ps ymm11, ymm10, ymm13
	vfmsub132ps xmm8, xmm10, xmm9
	vfmsub132ps ymm12, ymm15, ymm14
	vfmsub132ps xmm28, xmm16, xmm20
	vfmsub132ps ymm27, ymm19, ymm20
	vfmsub132ps xmm23, xmm25, xmm28
	vfmsub132ps ymm22, ymm26, ymm17
	vfmsub132ps xmm2{k5}, xmm5, xmm0
	vfmsub132ps ymm5{k7}, ymm5, ymm3
	vfmsub132ps zmm4{k6}, zmm7, zword [0xe9b]
	vfmsub132ps xmm0{k5}{z}, xmm3, oword [0x5e8]
	vfmsub132ps ymm6{k7}{z}, ymm2, ymm7
	vfmsub132ps zmm4{k3}{z}, zmm2, zmm4
	vfmsub132ps zmm7, zmm7, zmm6, {rn-sae}
	vfmsub132ps xmm4, xmm2, oword [eax+1]
	vfmsub132ps xmm1, xmm3, oword [eax+64]
	vfmsub132ps ymm5, ymm6, yword [eax+1]
	vfmsub132ps ymm3, ymm0, yword [eax+64]
	vfmsub132ps xmm4, xmm3, oword [eax+1]
	vfmsub132ps xmm2, xmm3, oword [eax+64]
	vfmsub132ps ymm4, ymm6, yword [eax+1]
	vfmsub132ps ymm3, ymm2, yword [eax+64]
	vfmsub132ps zmm3, zmm7, zword [eax+1]
	vfmsub132ps zmm4, zmm4, zword [eax+64]
	vfmsub132ps xmm6, xmm6, [0x48c]
	vfmsub132ps ymm6, ymm3, [0xfc4]
	vfmsub132ps xmm5, xmm4, [0x5a4]
	vfmsub132ps ymm1, ymm7, [0x9c6]
	vfmsub132ps zmm2, zmm0, [0x4a7]
