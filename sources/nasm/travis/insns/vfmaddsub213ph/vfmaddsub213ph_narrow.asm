	vfmaddsub213ph xmm3, xmm2, oword [0xcb1]
	vfmaddsub213ph xmm3, oword [0xcb1]
	vfmaddsub213ph xmm2, xmm3, xmm1
	vfmaddsub213ph ymm7, ymm1, ymm5
	vfmaddsub213ph ymm7, ymm5
	vfmaddsub213ph ymm2, ymm4, ymm5
	vfmaddsub213ph zmm7, zmm1, zmm3
	vfmaddsub213ph zmm7, zmm3
	vfmaddsub213ph zmm3, zmm0, zword [0x440]

%ifdef ERROR
	vfmaddsub213ph xmm13, xmm12, xmm13
	vfmaddsub213ph ymm9, ymm15, ymm14
	vfmaddsub213ph zmm8, zmm15, zmm8
	vfmaddsub213ph xmm22, xmm25, xmm28
	vfmaddsub213ph ymm23, ymm27, ymm17
	vfmaddsub213ph zmm24, zmm17, zmm19
%endif
