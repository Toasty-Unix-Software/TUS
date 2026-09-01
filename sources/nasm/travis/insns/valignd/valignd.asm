default rel
	valignd xmm2, xmm5, oword [0xa65], 0x52
	valignd xmm2, oword [0xa65], 0x52
	valignd xmm4, xmm6, xmm2, 0xcd
	valignd ymm3, ymm0, ymm5, 0x61
	valignd ymm3, ymm5, 0x61
	valignd ymm2, ymm2, ymm2, 0xc5
	valignd zmm7, zmm1, zmm4, 0xdf
	valignd zmm7, zmm4, 0xdf
	valignd zmm4, zmm2, zmm5, 0x80
	valignd xmm9, xmm8, xmm14, 0xd0
	valignd ymm13, ymm13, ymm9, 0x2c
	valignd zmm13, zmm11, zmm13, 0x5d
	valignd xmm26, xmm26, xmm28, 0xdb
	valignd ymm25, ymm23, ymm18, 0x91
	valignd zmm25, zmm25, zmm29, 0xd5
	valignd xmm5{k5}, xmm6, oword [0xf52], 0xb3
	valignd ymm3{k3}, ymm7, yword [0xc62], 0xd6
	valignd zmm0{k5}, zmm4, zmm5, 0xa6
	valignd xmm2{k6}{z}, xmm5, xmm4, 0x25
	valignd ymm3{k2}{z}, ymm5, ymm7, 0x45
	valignd zmm1{k2}{z}, zmm5, zword [0xc01], 0x22
	valignd xmm7, xmm0, oword [eax+1], 0xc7
	valignd xmm3, xmm4, oword [eax+64], 0x73
	valignd ymm1, ymm1, yword [eax+1], 0x99
	valignd ymm6, ymm6, yword [eax+64], 0xd
	valignd zmm7, zmm2, zword [eax+1], 0x63
	valignd zmm5, zmm5, zword [eax+64], 0x86
	valignd xmm6, xmm6, [0x45b], 0xd0
	valignd ymm0, ymm7, [0x323], 0xd6
	valignd zmm1, zmm4, [0xdde], 0x7d
