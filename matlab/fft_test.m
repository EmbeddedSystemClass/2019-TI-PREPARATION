%���������360��fs�Ļ�����λ��Ϣ���й̶�ƫ��
f = 512;
w = 2 * pi * f;
phi = 53.2/360*2*pi;
A = 1;
B = 2;

N = 4096;

t =  0:N-1;
fs = 40960;

signal1  = A*cos(w*t/fs);
signal2  = B*cos(w*t/fs+phi) +2;
plot(signal1)
plot(abs(fft(signal2)));
 