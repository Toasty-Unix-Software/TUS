default rel
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
	vpsubusb xmm9, xmm13, xmm9
	vpsubusb ymm8, ymm9, ymm15
	vpsubusb xmm11, xmm9, xmm13
	vpsubusb ymm10, ymm12, ymm10
	vpsubusb xmm16, xmm30, xmm18
	vpsubusb ymm21, ymm25, ymm23
	vpsubusb xmm30, xmm25, xmm22
	vpsubusb ymm28, ymm28, ymm31
	vpsubusb xmm5{k2}, xmm7, xmm1
	vpsubusb ymm7{k1}, ymm3, ymm5
	vpsubusb zmm3{k7}, zmm0, zmm4
	vpsubusb xmm0{k6}{z}, xmm1, oword [0xd47]
	vpsubusb ymm6{k4}{z}, ymm6, ymm1
	vpsubusb zmm0{k5}{z}, zmm4, zmm3
	vpsubusb xmm5, xmm5, oword [eax+1]
	vpsubusb xmm1, xmm0, oword [eax+64]
	vpsubusb ymm5, ymm6, yword [eax+1]
	vpsubusb ymm4, ymm1, yword [eax+64]
	vpsubusb xmm3, xmm0, oword [eax+1]
	vpsubusb xmm2, xmm7, oword [eax+64]
	vpsubusb ymm4, ymm4, yword [eax+1]
	vpsubusb ymm0, ymm6, yword [eax+64]
	vpsubusb zmm5, zmm7, zword [eax+1]
	vpsubusb zmm2, zmm7, zword [eax+64]
	vpsubusb xmm1, xmm1, [0x4a7]
	vpsubusb ymm3, ymm1, [0x4ac]
	vpsubusb xmm7, xmm4, [0xc23]
	vpsubusb ymm4, ymm4, [0x452]
	vpsubusb zmm5, zmm1, [0x656]
