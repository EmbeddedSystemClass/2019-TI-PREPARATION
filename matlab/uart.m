clear all
num=1000;
s=serial('COM3','BaudRate',115200,'Parity','none','DataBits',8,'StopBits',1); 
fopen(s); 
for j=1:num
    fwrite(s,1);
    high=fread(s,3) ;%�Ӵ��пڶ���3������
    y(num)=high(1)*256*256+high(2)*256+high(3)-65535;
    for i=1:num-1
        y(i)=y(1+i);   
    end  
    x=1:1:num;
    plot(x,y,'r-');
    axis([0 num -10 1]);
    grid on
    pause(0.0001);      
end
fclose(s);
delete(s); % ɾ���ڴ��еĴ����豸����
clear s; % ��������ռ��еĴ����豸����