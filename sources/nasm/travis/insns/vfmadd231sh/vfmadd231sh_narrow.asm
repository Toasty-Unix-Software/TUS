	vfmadd231sh xmm0, xmm6, xmm0
	vfmadd231sh xmm0, xmm0
	vfmadd231sh xmm4, xmm1, xmm5

%ifdef ERROR
	vfmadd231sh xmm8, xmm10, xmm11
	vfmadd231sh xmm16, xmm29, xmm27
%endif
