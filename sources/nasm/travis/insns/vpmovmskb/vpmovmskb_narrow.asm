	vpmovmskb ebx, xmm6
	vpmovmskb ebx, xmm5
	vpmovmskb ebx, ymm2
	vpmovmskb edx, ymm5

%ifdef ERROR
	vpmovmskb rsi, xmm1
	vpmovmskb rsi, xmm6
	vpmovmskb rcx, ymm4
	vpmovmskb rax, ymm4
	vpmovmskb r13, xmm12
	vpmovmskb r9d, xmm12
	vpmovmskb r13d, ymm8
	vpmovmskb r13, ymm10
%endif
