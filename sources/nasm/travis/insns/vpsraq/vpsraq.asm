default rel
	vpsraq xmm5, xmm4, xmm3
	vpsraq xmm5, xmm3
	vpsraq xmm2, xmm4, oword [0x1b8]
	vpsraq ymm7, ymm2, xmm2
	vpsraq ymm7, xmm2
	vpsraq ymm7, ymm3, xmm7
	vpsraq zmm1, zmm5, xmm6
	vpsraq zmm1, xmm6
	vpsraq zmm1, zmm3, xmm2
	vpsraq xmm5, xmm2, 0xf9
	vpsraq xmm5, 0xf9
	vpsraq xmm3, xmm6, 0xa2
	vpsraq xmm8, xmm10, xmm13
	vpsraq ymm15, ymm8, xmm15
	vpsraq zmm10, zmm13, xmm14
	vpsraq xmm10, xmm15, 0x53
	vpsraq xmm23, xmm30, xmm30
	vpsraq ymm20, ymm25, xmm28
	vpsraq zmm31, zmm19, xmm16
	vpsraq xmm19, xmm25, 0x4a
	vpsraq xmm0{k3}, xmm7, oword [0x7da]
	vpsraq ymm3{k2}, ymm7, xmm1
	vpsraq zmm1{k7}, zmm0, oword [0xdc3]
	vpsraq xmm2{k2}, xmm4, 0xf3
	vpsraq ymm2{k7}, ymm5, 0xca
	vpsraq zmm0{k5}, zmm2, 0xec
	vpsraq xmm3{k7}{z}, xmm5, xmm1
	vpsraq ymm7{k5}{z}, ymm7, oword [0xc7d]
	vpsraq zmm2{k6}{z}, zmm0, xmm1
	vpsraq xmm6{k6}{z}, xmm7, 0x2e
	vpsraq ymm2{k6}{z}, ymm0, 0xb3
	vpsraq zmm7{k7}{z}, zword [0xf0d], 0xb9
	vpsraq xmm0, xmm2, oword [eax+1]
	vpsraq xmm0, xmm7, oword [eax+64]
	vpsraq ymm6, ymm3, oword [eax+1]
	vpsraq ymm2, ymm0, oword [eax+64]
	vpsraq zmm3, zmm5, oword [eax+1]
	vpsraq zmm5, zmm1, oword [eax+64]
	vpsraq xmm6, oword [eax+1], 0xc1
	vpsraq xmm2, oword [eax+64], 0x4d
	vpsraq ymm3, yword [eax+1], 0x81
	vpsraq ymm4, yword [eax+64], 0xd9
	vpsraq zmm7, zword [eax+1], 0xe6
	vpsraq zmm7, zword [eax+64], 0x3
	vpsraq xmm5, xmm7, [0x4ec]
	vpsraq ymm6, ymm3, [0x792]
	vpsraq zmm0, zmm6, [0x473]
	vpsraq xmm3, [0x958], 0xda
	vpsraq ymm5, [0xd81], 0x16
	vpsraq zmm0, [0x362], 0x56
