	vptest xmm6, xmm1
	vptest xmm5, xmm0
	vptest ymm7, ymm5
	vptest ymm5, ymm4

%ifdef ERROR
	vptest xmm13, xmm15
	vptest ymm11, ymm10
%endif
