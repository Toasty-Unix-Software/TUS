default rel
	vfmsubadd231ps xmm7, xmm7, xmm4
	vfmsubadd231ps xmm5, xmm0, xmm6
	vfmsubadd231ps ymm2, ymm1, ymm5
	vfmsubadd231ps ymm3, ymm0, yword [0x5c3]
	vfmsubadd231ps xmm3, xmm1, xmm7
	vfmsubadd231ps xmm3, xmm7, oword [0xa20]
	vfmsubadd231ps ymm7, ymm7, yword [0x3f0]
	vfmsubadd231ps ymm4, ymm0, ymm3
	vfmsubadd231ps xmm10, xmm10, xmm10
	vfmsubadd231ps ymm11, ymm14, ymm10
	vfmsubadd231ps xmm11, xmm8, xmm11
	vfmsubadd231ps ymm15, ymm9, ymm11
	vfmsubadd231ps xmm29, xmm17, xmm21
	vfmsubadd231ps ymm27, ymm29, ymm18
	vfmsubadd231ps xmm23, xmm26, xmm31
	vfmsubadd231ps ymm29, ymm24, ymm30
	vfmsubadd231ps xmm4{k5}, xmm3, xmm6
	vfmsubadd231ps ymm5{k7}, ymm3, ymm3
	vfmsubadd231ps zmm4{k7}, zmm3, zmm7
	vfmsubadd231ps xmm1{k1}{z}, xmm0, xmm7
	vfmsubadd231ps ymm5{k6}{z}, ymm5, yword [0xc98]
	vfmsubadd231ps zmm6{k5}{z}, zmm6, zmm5
	vfmsubadd231ps zmm3, zmm2, zmm0, {rd-sae}
	vfmsubadd231ps xmm7, xmm2, oword [eax+1]
	vfmsubadd231ps xmm0, xmm0, oword [eax+64]
	vfmsubadd231ps ymm1, ymm7, yword [eax+1]
	vfmsubadd231ps ymm2, ymm0, yword [eax+64]
	vfmsubadd231ps xmm5, xmm6, oword [eax+1]
	vfmsubadd231ps xmm2, xmm2, oword [eax+64]
	vfmsubadd231ps ymm0, ymm6, yword [eax+1]
	vfmsubadd231ps ymm3, ymm3, yword [eax+64]
	vfmsubadd231ps zmm0, zmm2, zword [eax+1]
	vfmsubadd231ps zmm1, zmm1, zword [eax+64]
	vfmsubadd231ps xmm3, xmm3, [0x4cd]
	vfmsubadd231ps ymm2, ymm5, [0x931]
	vfmsubadd231ps xmm1, xmm4, [0x268]
	vfmsubadd231ps ymm1, ymm6, [0xa19]
	vfmsubadd231ps zmm1, zmm1, [0x886]
