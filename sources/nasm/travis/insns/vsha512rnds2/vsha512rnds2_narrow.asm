	vsha512rnds2 ymm2, ymm1, xmm0
	vsha512rnds2 ymm5, ymm3, xmm7

%ifdef ERROR
	vsha512rnds2 ymm12, ymm11, xmm9
%endif
