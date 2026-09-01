	vbroadcastf64x2 ymm6, oword [0xdc5]
	vbroadcastf64x2 ymm6, oword [0xadc]
	vbroadcastf64x2 zmm4, oword [0x9ef]
	vbroadcastf64x2 zmm0, oword [0xcb6]

%ifdef ERROR
	vbroadcastf64x2 ymm15, oword [0x108]
	vbroadcastf64x2 zmm9, oword [0xb5c]
	vbroadcastf64x2 ymm20, oword [0x39f]
	vbroadcastf64x2 zmm30, oword [0xb7d]
%endif
