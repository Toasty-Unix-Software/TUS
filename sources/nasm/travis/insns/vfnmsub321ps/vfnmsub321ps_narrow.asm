	vfnmsub321ps xmm0, xmm5, xmm6
	vfnmsub321ps xmm7, xmm0, xmm5
	vfnmsub321ps ymm7, ymm4, ymm3
	vfnmsub321ps ymm2, ymm0, ymm7

%ifdef ERROR
	vfnmsub321ps xmm11, xmm11, xmm9
	vfnmsub321ps ymm12, ymm11, ymm13
%endif
