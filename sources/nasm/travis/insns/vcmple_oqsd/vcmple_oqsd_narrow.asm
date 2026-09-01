	vcmple_oqsd xmm2, xmm0, xmm2
	vcmple_oqsd xmm2, xmm2
	vcmple_oqsd xmm1, xmm7, qword [0xdf6]
	vcmple_oqsd k1, xmm6, qword [0xac5]
	vcmple_oqsd k5, xmm6, qword [0xbb2]

%ifdef ERROR
	vcmple_oqsd xmm12, xmm15, xmm8
	vcmple_oqsd k5, xmm15, xmm8
%endif
