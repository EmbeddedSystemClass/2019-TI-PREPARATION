close all
%ֻ����ʽ��txt�ļ�
file_t = fopen('noise.txt','r');
% file_t = fopen('dpsk_demod2.txt','r');

%��ʮ���ƶ�ȡ,�Ҷ�ȡ�������Զ��ų�һ�У��ŵ�˳��Ϊ���ȴӵ�һ����ߵ���һ���ұߣ�Ȼ���ŵڶ���
input = fscanf(file_t,'%f');

%ֻ����ʽ��txt�ļ�
file_t = fopen('output.txt','r');
% file_t = fopen('dpsk_demod2.txt','r');

%��ʮ���ƶ�ȡ,�Ҷ�ȡ�������Զ��ų�һ�У��ŵ�˳��Ϊ���ȴӵ�һ����ߵ���һ���ұߣ�Ȼ���ŵڶ���
out = fscanf(file_t,'%f');

%ֻ����ʽ��txt�ļ�
file_t = fopen('LMS_input.txt','r');
% file_t = fopen('dpsk_demod2.txt','r');

%��ʮ���ƶ�ȡ,�Ҷ�ȡ�������Զ��ų�һ�У��ŵ�˳��Ϊ���ȴӵ�һ����ߵ���һ���ұߣ�Ȼ���ŵڶ���
ref = fscanf(file_t,'%f');
%�ر��ļ�
fclose(file_t);

%ֻ����ʽ��txt�ļ�
file_t = fopen('error_output.txt','r');
% file_t = fopen('dpsk_demod2.txt','r');

%��ʮ���ƶ�ȡ,�Ҷ�ȡ�������Զ��ų�һ�У��ŵ�˳��Ϊ���ȴӵ�һ����ߵ���һ���ұߣ�Ȼ���ŵڶ���
error = fscanf(file_t,'%f');
%�ر��ļ�
fclose(file_t);

plot(ref);
figure

plot(input);
figure
plot(error);
figure
plot(out);