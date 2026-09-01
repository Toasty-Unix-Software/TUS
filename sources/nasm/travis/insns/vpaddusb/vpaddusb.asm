default rel
	vpaddusb xmm6, xmm5, xmm1
	vpaddusb xmm6, xmm1
	vpaddusb xmm1, xmm7, oword [0xf1e]
	vpaddusb ymm1, ymm6, ymm0
	vpaddusb ymm1, ymm0
	vpaddusb ymm7, ymm5, yword [0xc57]
	vpaddusb xmm2, xmm6, xmm7
	vpaddusb xmm2, xmm7
	vpaddusb xmm0, xmm5, xmm6
	vpaddusb ymm1, ymm2, yword [0x14a]
	vpaddusb ymm1, yword [0x14a]
	vpaddusb ymm2, ymm2, yword [0xd80]
	vpaddusb xmm12, xmm14, xmm10
	vpaddusb ymm14, ymm9, ymm12
	vpaddusb xmm11, xmm10, xmm14
	vpaddusb ymm8, ymm12, ymm11
	vpaddusb xmm18, xmm16, xmm27
	vpaddusb ymm29, ymm18, ymm20
	vpaddusb xmm23, xmm21, xmm23
	vpaddusb ymm26, ymm18, ymm24
	vpaddusb xmm5{k1}, xmm1, xmm0
	vpaddusb ymm1{k7}, ymm5, ymm4
	vpaddusb zmm4{k3}, zmm1, zmm7
	vpaddusb xmm0{k5}{z}, xmm3, xmm5
	vpaddusb ymm7{k7}{z}, ymm2, ymm3
	vpaddusb zmm2{k7}{z}, zmm4, zmm1
	vpaddusb xmm4, xmm7, oword [eax+1]
	vpaddusb xmm0, xmm5, oword [eax+64]
	vpaddusb ymm0, ymm4, yword [eax+1]
	vpaddusb ymm3, ymm7, yword [eax+64]
	vpaddusb xmm5, xmm1, oword [eax+1]
	vpaddusb xmm5, xmm6, oword [eax+64]
	vpaddusb ymm0, ymm7, yword [eax+1]
	vpaddusb ymm7, ymm5, yword [eax+64]
	vpaddusb zmm2, zmm1, zword [eax+1]
	vpaddusb zmm6, zmm5, zword [eax+64]
	vpaddusb xmm1, xmm5, [0x1db]
	vpaddusb ymm6, ymm0, [0xa32]
	vpaddusb xmm2, xmm1, [0x202]
	vpaddusb ymm4, ymm1, [0x438]
	vpaddusb zmm2, zmm5, [0xfc8]
