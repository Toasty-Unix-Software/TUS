	vcmpless xmm7, xmm1, xmm0
	vcmpless xmm7, xmm0
	vcmpless xmm1, xmm7, xmm5
	vcmpless k5, xmm5, xmm1
	vcmpless k3, xmm7, xmm5

%ifdef ERROR
	vcmpless xmm12, xmm11, xmm15
	vcmpless k6, xmm10, xmm10
%endif
