default rel
	vfmaddsub132ps xmm0, xmm0, xmm4
	vfmaddsub132ps xmm3, xmm4, xmm5
	vfmaddsub132ps ymm1, ymm4, yword [0xe42]
	vfmaddsub132ps ymm4, ymm1, yword [0xf1b]
	vfmaddsub132ps xmm2, xmm1, xmm3
	vfmaddsub132ps xmm3, xmm0, xmm1
	vfmaddsub132ps ymm7, ymm0, yword [0xd72]
	vfmaddsub132ps ymm7, ymm4, ymm6
	vfmaddsub132ps xmm10, xmm14, xmm13
	vfmaddsub132ps ymm14, ymm11, ymm15
	vfmaddsub132ps xmm10, xmm15, xmm11
	vfmaddsub132ps ymm10, ymm9, ymm11
	vfmaddsub132ps xmm17, xmm16, xmm17
	vfmaddsub132ps ymm31, ymm17, ymm25
	vfmaddsub132ps xmm29, xmm31, xmm18
	vfmaddsub132ps ymm26, ymm22, ymm19
	vfmaddsub132ps xmm2{k6}, xmm2, oword [0x951]
	vfmaddsub132ps ymm2{k6}, ymm6, ymm0
	vfmaddsub132ps zmm6{k5}, zmm7, zword [0xbbd]
	vfmaddsub132ps xmm2{k4}{z}, xmm2, xmm6
	vfmaddsub132ps ymm0{k1}{z}, ymm4, ymm0
	vfmaddsub132ps zmm5{k4}{z}, zmm5, zmm0
	vfmaddsub132ps zmm6, zmm3, zmm3, {rz-sae}
	vfmaddsub132ps xmm0, xmm6, oword [eax+1]
	vfmaddsub132ps xmm4, xmm5, oword [eax+64]
	vfmaddsub132ps ymm3, ymm6, yword [eax+1]
	vfmaddsub132ps ymm1, ymm2, yword [eax+64]
	vfmaddsub132ps xmm4, xmm4, oword [eax+1]
	vfmaddsub132ps xmm1, xmm1, oword [eax+64]
	vfmaddsub132ps ymm1, ymm2, yword [eax+1]
	vfmaddsub132ps ymm5, ymm1, yword [eax+64]
	vfmaddsub132ps zmm2, zmm0, zword [eax+1]
	vfmaddsub132ps zmm3, zmm6, zword [eax+64]
	vfmaddsub132ps xmm3, xmm5, [0x528]
	vfmaddsub132ps ymm0, ymm1, [0xf29]
	vfmaddsub132ps xmm2, xmm4, [0x255]
	vfmaddsub132ps ymm2, ymm7, [0xf93]
	vfmaddsub132ps zmm2, zmm7, [0x492]
