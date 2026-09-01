	vcmpgt_ossd xmm5, xmm5, xmm5
	vcmpgt_ossd xmm5, xmm5
	vcmpgt_ossd xmm2, xmm6, qword [0x51b]
	vcmpgt_ossd k5, xmm6, qword [0x9bb]
	vcmpgt_ossd k4, xmm6, xmm2

%ifdef ERROR
	vcmpgt_ossd xmm11, xmm8, xmm12
	vcmpgt_ossd k2, xmm11, xmm14
%endif
