	vcmpgt_osss xmm6, xmm5, qword [0xf31]
	vcmpgt_osss xmm6, qword [0xf31]
	vcmpgt_osss xmm3, xmm6, qword [0xa52]
	vcmpgt_osss k6, xmm6, xmm6
	vcmpgt_osss k4, xmm5, xmm2

%ifdef ERROR
	vcmpgt_osss xmm10, xmm12, xmm13
	vcmpgt_osss k2, xmm11, xmm11
%endif
