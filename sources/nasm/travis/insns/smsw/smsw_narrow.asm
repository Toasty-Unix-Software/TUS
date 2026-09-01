	smsw dx
	smsw cx
	smsw dx
	smsw cx
	smsw eax
	smsw edx

%ifdef ERROR
	smsw rbp
	smsw rcx
	smsw r11w
	smsw r14
	smsw r15w
	smsw r12d
	smsw r23w
	smsw r21
	smsw r30w
	smsw r29d
%endif
