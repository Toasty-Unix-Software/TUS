	vcmpge_oqss xmm5, xmm4, xmm6
	vcmpge_oqss xmm5, xmm6
	vcmpge_oqss xmm1, xmm2, xmm7
	vcmpge_oqss k2, xmm1, xmm3
	vcmpge_oqss k7, xmm2, dword [0x125]

%ifdef ERROR
	vcmpge_oqss xmm12, xmm11, xmm14
	vcmpge_oqss k3, xmm9, xmm9
%endif
