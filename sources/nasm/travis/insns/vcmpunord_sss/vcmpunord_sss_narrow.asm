	vcmpunord_sss xmm5, xmm5, xmm0
	vcmpunord_sss xmm5, xmm0
	vcmpunord_sss xmm0, xmm7, qword [0xe69]
	vcmpunord_sss k7, xmm3, xmm2
	vcmpunord_sss k2, xmm2, xmm5

%ifdef ERROR
	vcmpunord_sss xmm13, xmm11, xmm12
	vcmpunord_sss k1, xmm10, xmm12
%endif
