tcpipServer = tcpip('127.0.0.0',45000,'NetworkRole','Server');


%while (1)
    disp('Waiting for client connection');
    fopen(tcpipServer)
    
    r=fread(tcpipServer,100);
    disp(r);
    Buffer=(1:10);
    
    fwrite(ClientPort, Buffer);
    
%end