	vfmaddsub321ps xmm3, xmm2, xmm6
	vfmaddsub321ps xmm2, xmm2, oword [0x4cc]
	vfmaddsub321ps ymm7, ymm2, ymm6
	vfmaddsub321ps ymm6, ymm2, ymm4

%ifdef ERROR
	vfmaddsub321ps xmm12, xmm13, xmm15
	vfmaddsub321ps ymm12, ymm13, ymm15
%endif
