	vsubss xmm5, xmm0, xmm1
	vsubss xmm5, xmm1
	vsubss xmm2, xmm1, xmm0
	vsubss xmm0, xmm6, xmm0
	vsubss xmm0, xmm0
	vsubss xmm6, xmm6, xmm4

%ifdef ERROR
	vsubss xmm9, xmm12, xmm12
	vsubss xmm13, xmm9, xmm10
	vsubss xmm19, xmm16, xmm17
	vsubss xmm31, xmm18, xmm24
%endif
