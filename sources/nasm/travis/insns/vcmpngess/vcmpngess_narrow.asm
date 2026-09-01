	vcmpngess xmm2, xmm6, xmm6
	vcmpngess xmm2, xmm6
	vcmpngess xmm2, xmm5, qword [0xf47]
	vcmpngess k4, xmm6, xmm4
	vcmpngess k1, xmm3, xmm0

%ifdef ERROR
	vcmpngess xmm12, xmm9, xmm13
	vcmpngess k1, xmm15, xmm8
%endif
