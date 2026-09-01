default rel
	vpermilpd xmm2, xmm6, xmm7
	vpermilpd xmm2, xmm7
	vpermilpd xmm1, xmm6, xmm7
	vpermilpd ymm3, ymm7, ymm7
	vpermilpd ymm3, ymm7
	vpermilpd ymm7, ymm7, ymm4
	vpermilpd xmm0, oword [0xde1], 0x2b
	vpermilpd xmm3, xmm3, 0x43
	vpermilpd ymm1, ymm2, 0x4b
	vpermilpd ymm0, yword [0xc84], 0x73
	vpermilpd xmm10, xmm10, xmm14
	vpermilpd ymm9, ymm13, ymm10
	vpermilpd xmm11, xmm9, 0xc
	vpermilpd ymm10, ymm13, 0x49
	vpermilpd xmm30, xmm29, xmm21
	vpermilpd ymm16, ymm19, ymm18
	vpermilpd xmm28, xmm17, 0xb0
	vpermilpd ymm18, ymm18, 0xef
	vpermilpd xmm0{k6}, xmm5, 0x59
	vpermilpd ymm2{k4}, ymm4, 0x28
	vpermilpd zmm6{k4}, zword [0x710], 0x61
	vpermilpd xmm2{k2}, xmm6, xmm0
	vpermilpd ymm1{k1}, ymm0, ymm7
	vpermilpd zmm3{k6}, zmm2, zmm2
	vpermilpd xmm7{k2}{z}, xmm4, 0xcc
	vpermilpd ymm2{k1}{z}, ymm1, 0x4a
	vpermilpd zmm1{k5}{z}, zmm5, 0xdb
	vpermilpd xmm1{k6}{z}, xmm1, xmm7
	vpermilpd ymm7{k6}{z}, ymm2, ymm3
	vpermilpd zmm6{k5}{z}, zmm0, zword [0xf12]
	vpermilpd xmm6, xmm0, oword [eax+1]
	vpermilpd xmm6, xmm5, oword [eax+64]
	vpermilpd ymm3, ymm1, yword [eax+1]
	vpermilpd ymm2, ymm2, yword [eax+64]
	vpermilpd xmm0, oword [eax+1], 0x92
	vpermilpd xmm7, oword [eax+64], 0xbd
	vpermilpd ymm7, yword [eax+1], 0x2a
	vpermilpd ymm4, yword [eax+64], 0x7d
	vpermilpd xmm7, oword [eax+1], 0x62
	vpermilpd xmm5, oword [eax+64], 0xcd
	vpermilpd ymm2, yword [eax+1], 0xd
	vpermilpd ymm5, yword [eax+64], 0xc1
	vpermilpd zmm7, zword [eax+1], 0xe5
	vpermilpd zmm5, zword [eax+64], 0xd2
	vpermilpd xmm0, xmm7, oword [eax+1]
	vpermilpd xmm3, xmm5, oword [eax+64]
	vpermilpd ymm5, ymm5, yword [eax+1]
	vpermilpd ymm0, ymm6, yword [eax+64]
	vpermilpd zmm5, zmm2, zword [eax+1]
	vpermilpd zmm1, zmm2, zword [eax+64]
	vpermilpd xmm7, xmm4, [0x4a2]
	vpermilpd ymm2, ymm0, [0x631]
	vpermilpd xmm4, [0x74a], 0xe1
	vpermilpd ymm0, [0xc93], 0xff
	vpermilpd xmm4, [0xf15], 0xb9
	vpermilpd ymm7, [0xf0e], 0x47
	vpermilpd zmm2, [0x3f2], 0x75
	vpermilpd xmm4, xmm3, [0x8c3]
	vpermilpd ymm5, ymm5, [0xa48]
	vpermilpd zmm2, zmm7, [0x4c1]
