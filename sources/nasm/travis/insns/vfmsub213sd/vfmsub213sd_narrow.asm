	vfmsub213sd xmm2, xmm6, xmm3
	vfmsub213sd xmm1, xmm5, qword [0x9e1]
	vfmsub213sd xmm2, xmm0, qword [0xab6]
	vfmsub213sd xmm0, xmm3, xmm0

%ifdef ERROR
	vfmsub213sd xmm12, xmm13, xmm12
	vfmsub213sd xmm15, xmm13, xmm11
	vfmsub213sd xmm28, xmm30, xmm28
	vfmsub213sd xmm24, xmm30, xmm16
%endif
