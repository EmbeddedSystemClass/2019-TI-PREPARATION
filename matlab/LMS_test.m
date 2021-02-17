
%function main()
close  all

% �����źŵĲ��� 
% t=0:99;
% xs=3*sin(t);
xs = signal1;
figure;
subplot(2,1,1);
plot(xs);grid;
ylabel('��ֵ');
title('it{�����������ź�}');

% �����źŵĲ���
% t=0:99;
% xn=3*sin(0.5*t);
xn = signal2;
subplot(2,1,2);
plot(xn);grid;
ylabel('��ֵ');
xlabel('ʱ��');
title('it{��������ź�}');

% �ź��˲�
xn = xs+xn;
xn = xn.' ;   % �����ź�����
dn = xs.' ;   % Ԥ�ڽ������
M  = 20 ;   % �˲����Ľ���

rho_max = max(eig(xn*xn.'));   % �����ź���ؾ�����������ֵ
mu = (1/rho_max) ;    % �������� 0 < mu < 1/rho
[yn,W,en] = LMS(xn,dn,M,mu);

% �����˲��������ź�
figure;
subplot(2,1,1);
plot(xn);grid;
ylabel('��ֵ');
xlabel('ʱ��');
title('it{�˲��������ź�}');

% ��������Ӧ�˲�������ź�
subplot(2,1,2);
plot(yn);grid;
ylabel('��ֵ');
xlabel('ʱ��');
title('it{����Ӧ�˲�������ź�}');

t = 1:N;
% ��������Ӧ�˲�������ź�,Ԥ������źź����ߵ����
figure 
plot(t,yn,'b',t,dn,'g',t,dn-yn,'r');grid;
legend('����Ӧ�˲������','Ԥ�����','���');
ylabel('��ֵ');
xlabel('ʱ��');
title('it{����Ӧ�˲���}');
