	vfmadd231sd xmm5, xmm2, xmm4
	vfmadd231sd xmm5, xmm5, xmm7
	vfmadd231sd xmm5, xmm3, xmm2
	vfmadd231sd xmm5, xmm7, xmm3

%ifdef ERROR
	vfmadd231sd xmm11, xmm9, xmm9
	vfmadd231sd xmm10, xmm9, xmm12
	vfmadd231sd xmm22, xmm16, xmm24
	vfmadd231sd xmm29, xmm21, xmm21
%endif
