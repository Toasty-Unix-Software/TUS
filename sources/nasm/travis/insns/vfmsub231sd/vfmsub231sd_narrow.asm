	vfmsub231sd xmm0, xmm0, xmm1
	vfmsub231sd xmm4, xmm6, xmm1
	vfmsub231sd xmm3, xmm5, xmm6
	vfmsub231sd xmm7, xmm1, qword [0xc75]

%ifdef ERROR
	vfmsub231sd xmm12, xmm13, xmm13
	vfmsub231sd xmm8, xmm11, xmm14
	vfmsub231sd xmm18, xmm18, xmm21
	vfmsub231sd xmm21, xmm30, xmm19
%endif
