	vfnmsubss xmm2, xmm1, xmm1, xmm1
	vfnmsubss xmm2, xmm1, xmm1
	vfnmsubss xmm4, xmm3, xmm1, xmm5
	vfnmsubss xmm5, xmm3, xmm2, dword [0x1f8]
	vfnmsubss xmm5, xmm2, dword [0x1f8]
	vfnmsubss xmm0, xmm3, xmm1, xmm2

%ifdef ERROR
	vfnmsubss xmm14, xmm14, xmm15, xmm10
	vfnmsubss xmm10, xmm8, xmm8, xmm12
%endif
