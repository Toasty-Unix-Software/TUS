	vfnmadd231sd xmm3, xmm4, xmm3
	vfnmadd231sd xmm3, xmm3, xmm5
	vfnmadd231sd xmm6, xmm6, xmm1
	vfnmadd231sd xmm3, xmm3, qword [0x165]

%ifdef ERROR
	vfnmadd231sd xmm13, xmm12, xmm15
	vfnmadd231sd xmm15, xmm11, xmm13
	vfnmadd231sd xmm30, xmm17, xmm18
	vfnmadd231sd xmm27, xmm18, xmm18
%endif
