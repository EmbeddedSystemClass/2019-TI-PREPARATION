%ֻ����ʽ��txt�ļ�
file_t = fopen('output.txt','r');
% file_t = fopen('dpsk_demod2.txt','r');

%��ʮ���ƶ�ȡ,�Ҷ�ȡ�������Զ��ų�һ�У��ŵ�˳��Ϊ���ȴӵ�һ����ߵ���һ���ұߣ�Ȼ���ŵڶ���
raw2 = fscanf(file_t,'%d');
%�ر��ļ�
fclose(file_t);

plot(raw2)