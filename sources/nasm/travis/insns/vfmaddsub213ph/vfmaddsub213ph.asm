default rel
	vfmaddsub213ph xmm3, xmm2, oword [0xcb1]
	vfmaddsub213ph xmm3, oword [0xcb1]
	vfmaddsub213ph xmm2, xmm3, xmm1
	vfmaddsub213ph ymm7, ymm1, ymm5
	vfmaddsub213ph ymm7, ymm5
	vfmaddsub213ph ymm2, ymm4, ymm5
	vfmaddsub213ph zmm7, zmm1, zmm3
	vfmaddsub213ph zmm7, zmm3
	vfmaddsub213ph zmm3, zmm0, zword [0x440]
	vfmaddsub213ph xmm13, xmm12, xmm13
	vfmaddsub213ph ymm9, ymm15, ymm14
	vfmaddsub213ph zmm8, zmm15, zmm8
	vfmaddsub213ph xmm22, xmm25, xmm28
	vfmaddsub213ph ymm23, ymm27, ymm17
	vfmaddsub213ph zmm24, zmm17, zmm19
	vfmaddsub213ph xmm5{k7}, xmm6, xmm7
	vfmaddsub213ph ymm6{k2}, ymm5, ymm2
	vfmaddsub213ph zmm1{k5}, zmm0, zmm4
	vfmaddsub213ph xmm7{k6}{z}, xmm3, oword [0xccf]
	vfmaddsub213ph ymm3{k5}{z}, ymm5, yword [0x731]
	vfmaddsub213ph zmm2{k3}{z}, zmm7, zword [0x5e2]
	vfmaddsub213ph zmm7, zmm0, zmm1, {rz-sae}
	vfmaddsub213ph xmm6, xmm6, oword [eax+1]
	vfmaddsub213ph xmm6, xmm2, oword [eax+64]
	vfmaddsub213ph ymm7, ymm6, yword [eax+1]
	vfmaddsub213ph ymm1, ymm1, yword [eax+64]
	vfmaddsub213ph zmm4, zmm6, zword [eax+1]
	vfmaddsub213ph zmm1, zmm4, zword [eax+64]
	vfmaddsub213ph xmm7, xmm4, [0xe8d]
	vfmaddsub213ph ymm3, ymm3, [0x349]
	vfmaddsub213ph zmm5, zmm3, [0x40c]
