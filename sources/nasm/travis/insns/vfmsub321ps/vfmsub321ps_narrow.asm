	vfmsub321ps xmm5, xmm5, oword [0xaed]
	vfmsub321ps xmm7, xmm1, xmm7
	vfmsub321ps ymm7, ymm5, ymm5
	vfmsub321ps ymm0, ymm7, ymm5

%ifdef ERROR
	vfmsub321ps xmm15, xmm8, xmm11
	vfmsub321ps ymm8, ymm10, ymm13
%endif
