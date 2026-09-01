	sha256rnds2 xmm7, xmm7, xmm0
	sha256rnds2 xmm5, xmm0, xmm0
	sha256rnds2 xmm6, xmm2
	sha256rnds2 xmm1, xmm5

%ifdef ERROR
	sha256rnds2 xmm15, xmm9, xmm0
	sha256rnds2 xmm11, xmm15
%endif
