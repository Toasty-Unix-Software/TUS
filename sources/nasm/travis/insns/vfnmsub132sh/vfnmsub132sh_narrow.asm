	vfnmsub132sh xmm0, xmm1, xmm6
	vfnmsub132sh xmm0, xmm6
	vfnmsub132sh xmm1, xmm1, xmm6

%ifdef ERROR
	vfnmsub132sh xmm10, xmm10, xmm14
	vfnmsub132sh xmm29, xmm16, xmm18
%endif
