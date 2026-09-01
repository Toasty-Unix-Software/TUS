	vdpbf16ps xmm3, xmm1, xmm3
	vdpbf16ps xmm3, xmm3
	vdpbf16ps xmm2, xmm7, xmm4
	vdpbf16ps ymm4, ymm4, ymm4
	vdpbf16ps ymm4, ymm4
	vdpbf16ps ymm6, ymm6, ymm0
	vdpbf16ps zmm2, zmm7, zword [0xd70]
	vdpbf16ps zmm2, zword [0xd70]
	vdpbf16ps zmm2, zmm2, zword [0xebe]

%ifdef ERROR
	vdpbf16ps xmm15, xmm10, xmm15
	vdpbf16ps ymm15, ymm9, ymm8
	vdpbf16ps zmm10, zmm9, zmm8
	vdpbf16ps xmm24, xmm26, xmm25
	vdpbf16ps ymm17, ymm28, ymm19
	vdpbf16ps zmm18, zmm16, zmm23
%endif
