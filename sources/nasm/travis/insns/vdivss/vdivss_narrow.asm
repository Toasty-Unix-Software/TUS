	vdivss xmm6, xmm4, xmm6
	vdivss xmm6, xmm6
	vdivss xmm0, xmm6, xmm2
	vdivss xmm1, xmm4, xmm6
	vdivss xmm1, xmm6
	vdivss xmm6, xmm3, xmm1

%ifdef ERROR
	vdivss xmm11, xmm14, xmm12
	vdivss xmm12, xmm15, xmm10
	vdivss xmm29, xmm22, xmm26
	vdivss xmm21, xmm30, xmm27
%endif
