default rel
	vfmaddsub213ps xmm3, xmm1, oword [0xf24]
	vfmaddsub213ps xmm6, xmm7, xmm7
	vfmaddsub213ps ymm7, ymm1, yword [0x8a0]
	vfmaddsub213ps ymm1, ymm4, ymm5
	vfmaddsub213ps xmm1, xmm1, xmm5
	vfmaddsub213ps xmm1, xmm5, xmm4
	vfmaddsub213ps ymm3, ymm7, ymm0
	vfmaddsub213ps ymm0, ymm1, yword [0xefb]
	vfmaddsub213ps xmm13, xmm8, xmm9
	vfmaddsub213ps ymm11, ymm12, ymm13
	vfmaddsub213ps xmm15, xmm10, xmm11
	vfmaddsub213ps ymm15, ymm11, ymm11
	vfmaddsub213ps xmm22, xmm30, xmm17
	vfmaddsub213ps ymm26, ymm29, ymm27
	vfmaddsub213ps xmm22, xmm17, xmm20
	vfmaddsub213ps ymm27, ymm31, ymm26
	vfmaddsub213ps xmm3{k7}, xmm2, xmm6
	vfmaddsub213ps ymm0{k7}, ymm2, yword [0x3fd]
	vfmaddsub213ps zmm4{k6}, zmm6, zmm0
	vfmaddsub213ps xmm2{k1}{z}, xmm1, oword [0x9ad]
	vfmaddsub213ps ymm5{k5}{z}, ymm3, yword [0x2db]
	vfmaddsub213ps zmm5{k6}{z}, zmm6, zmm3
	vfmaddsub213ps zmm3, zmm0, zmm2, {rd-sae}
	vfmaddsub213ps xmm4, xmm4, oword [eax+1]
	vfmaddsub213ps xmm6, xmm3, oword [eax+64]
	vfmaddsub213ps ymm0, ymm0, yword [eax+1]
	vfmaddsub213ps ymm5, ymm6, yword [eax+64]
	vfmaddsub213ps xmm7, xmm2, oword [eax+1]
	vfmaddsub213ps xmm0, xmm5, oword [eax+64]
	vfmaddsub213ps ymm7, ymm4, yword [eax+1]
	vfmaddsub213ps ymm3, ymm6, yword [eax+64]
	vfmaddsub213ps zmm2, zmm1, zword [eax+1]
	vfmaddsub213ps zmm0, zmm7, zword [eax+64]
	vfmaddsub213ps xmm2, xmm0, [0x257]
	vfmaddsub213ps ymm2, ymm2, [0x7cb]
	vfmaddsub213ps xmm5, xmm3, [0x53b]
	vfmaddsub213ps ymm2, ymm5, [0xbc2]
	vfmaddsub213ps zmm2, zmm3, [0x1f1]
