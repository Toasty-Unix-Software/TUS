	vbroadcastf32x4 ymm5, oword [0x22d]
	vbroadcastf32x4 ymm7, oword [0x27e]
	vbroadcastf32x4 zmm5, oword [0x289]
	vbroadcastf32x4 zmm5, oword [0x240]

%ifdef ERROR
	vbroadcastf32x4 ymm8, oword [0x747]
	vbroadcastf32x4 zmm13, oword [0xdf6]
	vbroadcastf32x4 ymm27, oword [0xbe6]
	vbroadcastf32x4 zmm23, oword [0x790]
%endif
