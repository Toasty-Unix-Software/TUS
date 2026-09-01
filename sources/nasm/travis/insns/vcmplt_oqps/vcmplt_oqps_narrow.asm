	vcmplt_oqps xmm1, xmm2, oword [0x103]
	vcmplt_oqps xmm1, oword [0x103]
	vcmplt_oqps xmm4, xmm6, oword [0xe41]
	vcmplt_oqps ymm6, ymm5, ymm2
	vcmplt_oqps ymm6, ymm2
	vcmplt_oqps ymm4, ymm2, ymm7
	vcmplt_oqps k2, xmm6, oword [0x8f7]
	vcmplt_oqps k2, xmm3, xmm7
	vcmplt_oqps k5, ymm4, ymm3
	vcmplt_oqps k4, ymm4, yword [0xd19]

%ifdef ERROR
	vcmplt_oqps xmm12, xmm9, xmm14
	vcmplt_oqps ymm14, ymm13, ymm14
	vcmplt_oqps k5, xmm14, xmm12
	vcmplt_oqps k1, ymm10, ymm11
%endif
