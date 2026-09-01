	vrangesd xmm0, xmm5, qword [0x507], 0x70
	vrangesd xmm0, qword [0x507], 0x70
	vrangesd xmm5, xmm7, xmm3, 0x76

%ifdef ERROR
	vrangesd xmm13, xmm10, xmm14, 0x77
	vrangesd xmm16, xmm30, xmm18, 0x54
%endif
