	vpsubusb xmm7, xmm5, xmm4
	vpsubusb xmm7, xmm4
	vpsubusb xmm6, xmm6, xmm3
	vpsubusb ymm4, ymm2, ymm6
	vpsubusb ymm4, ymm6
	vpsubusb ymm2, ymm6, ymm6
	vpsubusb xmm4, xmm0, xmm6
	vpsubusb xmm4, xmm6
	vpsubusb xmm2, xmm2, oword [0x859]
	vpsubusb ymm5, ymm3, yword [0xec2]
	vpsubusb ymm5, yword [0xec2]
	vpsubusb ymm3, ymm4, ymm4

%ifdef ERROR
	vpsubusb xmm9, xmm13, xmm9
	vpsubusb ymm8, ymm9, ymm15
	vpsubusb xmm11, xmm9, xmm13
	vpsubusb ymm10, ymm12, ymm10
	vpsubusb xmm16, xmm30, xmm18
	vpsubusb ymm21, ymm25, ymm23
	vpsubusb xmm30, xmm25, xmm22
	vpsubusb ymm28, ymm28, ymm31
%endif
