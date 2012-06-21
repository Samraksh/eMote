% SERVER Write a message over the specified port
% 
% Usage - server(message, output_port, number_of_retries)
%function server(message, server_port, number_of_retries)

    import java.net.ServerSocket
    import java.io.*

    %if (nargin <>
    %    number_of_retries = 20; % set to -1 for infinite
    %end
    retry             = 0;

    server_socket  = [];
    output_socket  = [];
    
    server_port = 45000;
    number_of_retries = 20;

    while true

        retry = retry + 1;

        try
            if ((number_of_retries > 0) && (retry > number_of_retries))
                fprintf(1, 'Too many retries\n');
                break;
            end

            disp('hey');
            disp( [ 'Try ' int2str(retry) 'waiting for client to connect to this ...ost on port : ' int2str(output_port)]);

            % wait for 1 second for client to connect server socket
            server_socket = ServerSocket(server_port);
            server_socket.setSoTimeout(1000);

            client_socket = server_socket.accept;

            disp('Client connected\n');

            output_stream   = client_socket.getOutputStream;
            input_stream = client_socket.getInputStream;
            d_output_stream = DataOutputStream(output_stream);
            d_input_stream = DataInputStream(input_stream);

            % output the data over the DataOutputStream
            % Convert to stream of bytes
            disp('Reading from client\n');
            input_buf =d_input_stream.ReadBytes();
            
            %reverse input
            ouput_buf=input_buf(end:1);
            
            disp(['Writing '  int2str(length(output_buf)) ' bytes']);
            d_output_stream.writeBytes(output_buf);
            d_output_stream.flush;
            
            % clean up
            server_socket.close;
            output_socket.close;
            break;
            
        catch
            if ~isempty(server_socket)
                server_socket.close
            end

            if ~isempty(output_socket)
                output_socket.close
            end

            % pause before retrying
            pause(1);
        end
    end
%end