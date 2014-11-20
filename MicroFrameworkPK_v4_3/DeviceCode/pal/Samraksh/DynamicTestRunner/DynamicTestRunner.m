
% Michael McGrath, michael.mcgrath@samraksh.com, The Samraksh Company
% Date:  2014-10-23
% Brief: Connect to PAL library over serial port on eMote DotNow.
% Details:
% The first part of the script replicates .NET MF connection management
% that I previously implemented in C#.
% This script requires custom Microsoft.SPOT.Debugger.dll that contains
% Emote message client functions.
% I created a separate app note for adding Wire Protocol messages to the
% CLR Debugger.
%
% Setup:
% 1. Obtain the custom Samraksh Microsoft.SPOT.Debugger.dll and update the
% NET.addAssembly path string below.  If you need the
% Microsoft.SPOT.Debugger.dll, on a machine set up for .NET Micro Framework
% Porting Kit development, go to %SPOCLIENT%\Framework and execute
% 'msbuild /t:clean build.dirproj' and 'msbuild /t:build build.dirproj',
% then use the result in %SPOCLIENT%\BuildOutput\public\Debug\Server\dll\
% with this script.
% 2. Connect the Emote DotNow and set up the correct COM port number below
% in the portName variable.
% 3. Edit the bottom of this script to pass your MATLAB data as part of the
% custom commands.  You can find the custom commands by executing the "DBG
% = NET.addAssembly(..." line and then executing "DBG.Classes" to get a
% list of WireProtocol messages.  These include messages normally sent by
% Visual Studio and MF Deploy.  In Samraksh's case, I've created the
% message namespace "Emote_FunctionName", for example, you will find
% 'Microsoft.SPOT.Debugger.WireProtocol.Commands+Emote_DynamicTestRunner',
% which my be executed in this script with
% m_eng.DynamicTestRunner<Command>(...).  A better way to find the prototypes
% for functions is to open (or reference) the custom
% Microsoft.SPOT.Debugger.dll in Visual Studio and use the object explorer
% to find the function prototypes.
%
% Troubleshooting:
% Ensure the DotNow is running the Debug flavor of TinyCLR.
% Try disconnecting and reconnecting the DotNow device.
%
% Summary:
% We can now, from MATLAB, call source code on the DotNow and pass data.
% We can also use traditional Microsoft.SPOT.Debugger functions to do
% things like write memory values.
%
% GUI showing functions and their signatures:
% methodsview('Microsoft.SPOT.Debugger.Engine')
% MATLAB can change the signatures to accomodate C# in/out parameter
% decorations
%
% TODO: locally save input and output data to a file.
% TODO: set up event callbacks to record other debugger messages or output
% them to the console.
% TODO: matlab function that, given function name, finds the function
% address in the map file and returns the function address.  This would
% allow arbitrary function execution.
% TODO: test limits of wire protocol packet size, etc.
% TODO: automatically connect to the correct COM port or list COM ports.
% TODO: ultimately, I want to separate the business logic of calling a
% test, from the Microsoft.SPOT.Debugger.dll, so the DLL does not have to
% be re-compiled for a new test.  We can use MSBUILD to help automate
% compile-time glue between test functions in libraries, and the list of
% test functions made available to MATLAB.
%
% Example:
% I write a native library on the DotNow.  As part of the native library,
% I include a C/C++ test function that takes input data, runs the code,
% and produces output data.  I add glue code to the DynamicTestRunner
% library, and then edit the bottom of this script to pass the
% MATLAB-generated test data to the DotNow and execute the test function.
% At the end of the test, I use MATLAB to verify the results.
%
%% Example Commands:
% Transfer execution to a function at address 0x802a7b0, with 0 arguments,
% stored at address [0]
% [m_success, resultAddr] = m_eng.DynamicTestRunnerProcess(uint32(hex2dec('802a7b1')),0,[0])
%
% Allocate a byte buffer from MATLAB:
% [m_success m_bufferAddr] = m_eng.DynamicTestRunnerMallocByteBuffer(bufferSize);
%
% Write to a byte buffer that MATLAB allocated:
% [m_success] = m_eng.DynamicTestRunnerWriteByteBuffer(bufferAddr, byteOffset, bufferLength, inputData);
% Read byte buffer:
% [m_success, m_data, m_readLength] = m_eng.DynamicTestRunnerReadByteBuffer(bufferAddr, byteOffset, bufferLength);
%
% Show byte buffers that MATLAB allocated:
% [m_success, m_bufferAddrs] = m_eng.DynamicTestRunnerShowByteBuffers(unused_uint);
%
% Free all byte buffers that MATLAB allocated:
% [m_success] = m_eng.DynamicTestRunnerFreeAllBuffers(unused_uint);


%% Import custom Samraksh Microsoft.SPOT.Debugger
%TODO: compare file date and only reload if DBG does not exist or dll is
%newer.
DBG = NET.addAssembly('C:\repo\DotNet-MF\MicroFrameworkPK_v4_3\BuildOutput\public\Debug\Server\dll\Microsoft.SPOT.Debugger.dll');

%% Port setup
portName = 'COM37';  % ON SOME SYSTEMS, MAY NEED A SPACE CHARACTER AFTER 'COM'.
%portNameSys = strcat('\\\\.\\',portName);
portNameSys = strcat('\\.\',portName);

serialBitRate = uint32(115200);
m_port = Microsoft.SPOT.Debugger.PortDefinition.CreateInstanceForSerial(portName, portNameSys, serialBitRate);

% method used by Data Collector Exfiltrator Host to read output stream
%serialStream = portDef.TryToOpen();

%% Connect to device
% function ConnectTo(int timeout_ms, bool tryToConnect, Microsoft.SPOT.Debugger.ConnectionSource target)
if(exist('m_eng','var') == 0)
    m_eng = Microsoft.SPOT.Debugger.Engine(m_port);

    %TODO: m_eng.OnNoise += new _DBG.NoiseEventHandler(OnNoiseHandler);
    %TODO: m_eng.OnMessage += new _DBG.MessageEventHandler(OnMessage);

    m_eng.Start();
end

timeout_ms = int32(5000);
retries = int32(10);
for j = 0:1:retries
    if (m_eng.TryToConnect( ...
            int32(0), ...
            int32(timeout_ms / retries), ...
            logical(true), ...
            Microsoft.SPOT.Debugger.ConnectionSource.Unknown ...
            ) > 0)
        if (m_eng.ConnectionSource == Microsoft.SPOT.Debugger.ConnectionSource.TinyCLR)
            fprintf('Connected to TinyCLR on %s \n',portName);
            %TODO: m_eng.UnlockDevice(m_data); % might not be necessary if no key is set.
        end
        break;
    end
    fprintf('Connect attempt %d to %s failed\n',j,portName);
end

%% Verify we're connected
if (m_eng.ConnectionSource ~= Microsoft.SPOT.Debugger.ConnectionSource.TinyCLR)
    disp('Could not detect TinyCLR running on device. exiting.');
    m_eng.Stop();
    m_eng.Dispose();
    clear m_eng;
    return;
end

error_count = 0;


%% Print out device info
% TODO: get free RAM from GC. Get IDs, etc.

%% Send custom messages, pass test input data, etc, between the DotNow and MATLAB.

% Example: create a test input array on the DotNow

% Example: call a native function on the DotNow
% Description: Edit
% %SPOCLIENT%\DeviceCode\pal\Samraksh\DynamicTestRunner\DynamicTestRunner.cpp
% so that it calls the test function of your library.
% command = uint32(1);
%result = m_eng.DynamicTestRunnerProcess(command);
% Example: fill a static array on the DotNow

%% TEST FreeAllBuffers
% If we don't first free all buffers, the length 1 buffer from last test
% will throw an m_success=0 when we later try to read all buffers to length.
str_commandUnderTest = 'FreeAllBuffers';
fprintf('\n    ## TEST %s ######\n', str_commandUnderTest);
command = uint32(0);
[m_success] = m_eng.DynamicTestRunnerFreeAllBuffers(command);
if (m_success == 0)
    fprintf('ERROR %s returned m_success = %d \n',str_commandUnderTest, m_success);
    error_count = error_count + 1;
else
    %fprintf('device indicates successful %s \n', str_commandUnderTest);
end


%% TEST malloc
str_commandUnderTest = 'MallocByteBuffer';
fprintf('    ## TEST %s ######\n',str_commandUnderTest);
bufferSize = uint32(10);
[m_success m_bufferAddr] = m_eng.DynamicTestRunnerMallocByteBuffer(bufferSize);

if (m_success == 1)
    fprintf('device indicates successful %s with %.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    disp('did not create buffer on device. might be out of memory.');
    error_count = error_count + 1;
end
fprintf('m_buffferAddr = 0x%X \n', m_bufferAddr);

%% TEST write
str_commandUnderTest = 'WriteByteBuffer';
fprintf('    ## TEST %s ######\n',str_commandUnderTest);
bufferAddr = uint32(m_bufferAddr);
byteOffset = uint32(0);
bufferLength = uint32(bufferSize);
inputData = uint8((1+byteOffset):1:bufferSize);
[m_success] = m_eng.DynamicTestRunnerWriteByteBuffer(bufferAddr, byteOffset, bufferLength, inputData);

if (m_success == 1)
    fprintf('device indicates successful %s.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    error_count = error_count + 1;
end
fprintf('m_success = %d \n',m_success);


%% TEST read
% Note: DynamicTestRunnerReadByteBuffer does not return WireProtocol.IncomingMessage
str_commandUnderTest = 'ReadByteBuffer';
fprintf('    ## TEST %s ######\n',str_commandUnderTest);
bufferAddr = uint32(m_bufferAddr);
byteOffset = uint32(0);
bufferLength = uint32(bufferSize);
[m_success, m_data, m_readLength] = m_eng.DynamicTestRunnerReadByteBuffer(bufferAddr, byteOffset, bufferLength);

if ( isempty(m_success) == 0)
    if (m_success == 1)
        fprintf('device indicates successful %s.\n',str_commandUnderTest);
    else
        fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
        error_count = error_count + 1;
    end
    fprintf('m_success = %d \n',m_success);
else
    fprintf('ERROR did not receive ACK from %s command. no message to process.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

if ( isempty(m_readLength) == 0)
    if (m_readLength == bufferLength)
        fprintf('device indicates %s read all %d bytes of %d bytes.\n', str_commandUnderTest, m_readLength, bufferLength);
    else
        fprintf('ERROR device indicates %s failed to read %d bytes.\n', str_commandUnderTest, bufferLength - m_readLength);
        error_count = error_count + 1;
    end
    fprintf('m_readLength = %d \n',m_readLength);
else
    fprintf('ERROR did not receive m_readLength from %s command.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

if ( isempty(m_data) == 0)
    outputData = m_data.uint8;
    if (isequal(outputData, inputData) == 1)
        fprintf('returned data matches input data. successful %s.\n',str_commandUnderTest);
    else
        fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
        error_count = error_count + 1;
        inputData
    end
    outputData
else
    fprintf('ERROR did not receive data from %s command.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

%% TEST over-reading array on device...
str_testDescription = 'test bad input to ReadBytebuffer';
fprintf('    ## TEST %s ######\n',str_testDescription);
errorCountStart = error_count;
str_commandUnderTest = 'ReadByteBuffer';
itr_test_byteOffset = 0;
itr_test_readLength = bufferSize + 1;
bufferAddr = uint32(m_bufferAddr);     %buffer address that should now contain data we just wrote.
test_byteOffset = uint32(itr_test_byteOffset);
test_readLength = uint32(itr_test_readLength); %test by requesting too-long of a sequence
[m_success, m_data, m_readLength] = m_eng.DynamicTestRunnerReadByteBuffer(bufferAddr, test_byteOffset, test_readLength);

if ( isempty(m_success) == 0)
    if (m_success == 0)
        fprintf('PASSED test for %s %X.\n',str_testDescription,bufferAddr);
    else
        fprintf('##############################################################\n');
        fprintf('FAILED test %s.##############################################################\n',str_testDescription);
        fprintf('ERROR device indicates %s passed when it should have failed.\n', str_commandUnderTest);
        error_count = error_count + 1;
    end
    %fprintf('m_success = %d \n',m_success);
else
    fprintf('ERROR did not receive ACK from %s command. no message to process.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

if ( isempty(m_readLength) == 0)
    if ((m_readLength == test_readLength) && (itr_test_byteOffset + itr_test_readLength > bufferSize))
        fprintf('##############################################################\n');
        fprintf('TEST %s failed.##############################################################\n',str_testDescription);
        fprintf('ERROR device indicates %s read %d bytes of requested %d bytes when it should have failed.\n',...
            str_commandUnderTest, m_readLength, bufferLength);
        error_count = error_count + 1;
    else
        fprintf('PASSED test %s. device indicates %s failed to read %d bytes at offset %d in %d sized buffer.\n',...
            str_testDescription, str_commandUnderTest, bufferLength - m_readLength, test_byteOffset, bufferSize);
    end
    %fprintf('   m_readLength = %d \n',m_readLength);
else
    fprintf('ERROR did not receive m_readLength from %s command.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

if ( isempty(m_data) == 0)
    outputData = m_data.uint8;
    if (length(outputData) ~= itr_test_readLength)
        fprintf('ERROR command %s returned outputData with length = %d, requested length of %d, and device reported read length of %d\n',...
            str_commandUnderTest,length(outputData), itr_test_readLength, m_readLength);
        error_count = error_count + 1;
    end
    if ((itr_test_byteOffset < bufferSize) && (itr_test_byteOffset+itr_test_readLength <= bufferSize))
        expected_outputData = inputData((itr_test_byteOffset+1):(itr_test_byteOffset+itr_test_readLength));
        if (isequal(outputData, expected_outputData) ~= 1)
            fprintf('ERROR device indicates %s returned all data when it should not have.\n', str_commandUnderTest);
            error_count = error_count + 1;
            outputData
            expectedOutputData
        end
    end
else
    fprintf('ERROR did not receive data from %s command.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

if(error_count == errorCountStart)
    fprintf('PASSED test %s.\n',str_testDescription);
else
    fprintf('##############################################################\n');
    fprintf('FAILED test %s.\n##############################################################\n',str_testDescription);
end

%% TEST ShowByteBuffers
% Note: DynamicTestRunnerShowByteBuffers does not return WireProtocol.IncomingMessage
str_commandUnderTest = 'ShowByteBuffers';
fprintf('    ## TEST %s ######\n',str_commandUnderTest);
command = uint32(0); % don't care
[m_success, m_bufferAddrs] = m_eng.DynamicTestRunnerShowByteBuffers(command);

if ( isempty(m_success) == 0)
    if (m_success == 1)
        %fprintf('device indicates successful %s.\n',str_commandUnderTest);
    else
        fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
        error_count = error_count + 1;
    end
    %fprintf('m_success = %d \n',m_success);
else
    fprintf('ERROR did not receive ACK from %s command. no message to process.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

if ( isempty(m_bufferAddrs) == 0)
    outputData = m_bufferAddrs.uint32;
    % TODO: keep track of allocations and compare to this.
    dec2hex(outputData)
else
    fprintf('ERROR did not receive data from %s command.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

m_bufferCount = m_bufferAddrs.Length;
if ( isempty(m_bufferCount) == 0)
    fprintf('device indicates it has %d buffer(s) with %s.\n', m_bufferCount, str_commandUnderTest);
    fprintf('m_bufferCount = %d \n',m_bufferCount);
else
    fprintf('ERROR did not receive m_bufferCount from %s command.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

%% TEST print all buffers
str_testDescription = 'printing all buffers';
str_commandUnderTest = 'ReadByteBuffer';
fprintf('\n    ## TEST %s with %s ######\n', str_testDescription, str_commandUnderTest);
command = uint32(0); % don't care
[m_success, m_bufferAddrs] = m_eng.DynamicTestRunnerShowByteBuffers(command);

m_bufferAddrs = m_bufferAddrs.uint32 %convert from NET to MATLAB
if (length(m_bufferAddrs) == 0)
    disp('no buffers on device!');
end
for itr=1:length(m_bufferAddrs)
    bufferAddr = uint32(m_bufferAddrs(itr));
    byteOffset = uint32(0);
    bufferLength = uint32(bufferSize); %TODO: get the size from the device.
    [m_success, m_data, m_readLength] = m_eng.DynamicTestRunnerReadByteBuffer(bufferAddr, byteOffset, bufferLength);
    if (m_success ~= 1)
        fprintf('ERROR ReadByteBuffer returned m_success = %d for input %X %d %d\n',m_success, bufferAddr,byteOffset,bufferLength);
        error_count = error_count + 1;
    end
    fprintf('buffer at address 0x%X :\n',m_bufferAddrs(itr));
    m_data.uint8
end

%% Test SEQUENCE free all buffers
str_testDescription = 'FreeAllBuffers with verification';
errorCountStart = error_count;
str_commandUnderTest = 'FreeAllBuffers';
fprintf('\n    ## TEST %s ######\n', str_commandUnderTest);
command = uint32(0);
[m_success] = m_eng.DynamicTestRunnerFreeAllBuffers(command);
if (m_success == 0)
    fprintf('ERROR %s returned m_success = %d \n',str_commandUnderTest, m_success);
    error_count = error_count + 1;
else
    %fprintf('device indicates successful %s \n', str_commandUnderTest);
end

%% TEST verify no buffersprint all buffers after free
str_commandUnderTest = 'ShowByteBuffers';
fprintf('\n    ## TEST %s with %s ######\n', str_testDescription, str_commandUnderTest);
command = uint32(0); % don't care
[m_success, m_bufferAddrs] = m_eng.DynamicTestRunnerShowByteBuffers(command);
if (m_success == 0)
    fprintf('ERROR %s returned m_success = %d \n',str_commandUnderTest,m_success);
    error_count = error_count + 1;
end
m_bufferAddrs = m_bufferAddrs.int32; %convert from NET to MATLAB
if (length(m_bufferAddrs) ~= 0)
    disp('ERROR:  buffers on device after FreeAllBuffers!');
    error_count = error_count + 1;
end
for itr=1:length(m_bufferAddrs)
    str_commandUnderTest = 'ReadByteBuffer';
    bufferAddr = uint32(m_bufferAddrs(itr));
    byteOffset = uint32(0);
    bufferLength = uint32(bufferSize); %TODO: get the size from the device.
    [m_success, m_data, m_readLength] = m_eng.DynamicTestRunnerReadByteBuffer(bufferAddr, byteOffset, bufferLength);
    if (m_success ~= 1)
        fprintf('ERROR %s returned m_success = %d \n',str_commandUnderTest, m_success);
        error_count = error_count + 1;
    end
    m_data.uint8
end

if(error_count == errorCountStart)
    fprintf('PASSED test %s.\n',str_testDescription);
else
    fprintf('##############################################################\n');
    fprintf('FAILED test %s.\n##############################################################\n',str_testDescription);
end


%% TEST SEQUENCE allocate more buffers
str_testDescription = 'allocate more buffers';
str_commandUnderTest = 'MallocByteBuffer';
fprintf('\n    ## TEST %s with %s ######\n', str_testDescription, str_commandUnderTest);
clear mallocAddrs;
clear bufferAddrs;
clear inputData;
clear outputData;
errorCountStart = error_count;
times = 2;
bufferSize = 10;
fprintf('    ...using %d buffers of size %d\n',times,bufferSize);
for itr=1:times
    fprintf(' . ');
    %disp('/////////////////////////////////////////////////////////////');
    param_bufferSize = uint32(10);
    [m_success, m_bufferAddr] = m_eng.DynamicTestRunnerMallocByteBuffer(param_bufferSize);
    %fprintf('%d: 0x%X \n', itr,m_bufferAddr);
    mallocAddrs(itr) = m_bufferAddr;
    str_commandUnderTest = 'ShowByteBuffers';
    command = uint32(0); % don't care
    [m_success, m_bufferAddrs] = m_eng.DynamicTestRunnerShowByteBuffers(command);

    if ( isempty(m_success) == 0)
        if (m_success == 1)
            %fprintf('device indicates successful %s.\n',str_commandUnderTest);
        else
            fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
            error_count = error_count + 1;
        end
        %fprintf('m_success = %d \n',m_success);
    else
        fprintf('ERROR did not receive m_bufferCount from %s command.\n', str_commandUnderTest);
        error_count = error_count + 1;
    end
    bufferAddrs = m_bufferAddrs.uint32;
    bufferCount = m_bufferAddrs.Length;
    expectedBufferCount = itr;
    if (bufferCount ~= (expectedBufferCount))
        fprintf('ERROR expected %d buffers on device but %s indicates %d buffers on device. \n',expectedBufferCount, str_commandUnderTest, bufferCount);
        error_count = error_count + 1;
    end
    if ( isequal(mallocAddrs,bufferAddrs) == 0)
        fprintf('ERROR malloc addresses do not match ShowByteBuffers addresses\n');
        error_count = error_count + 1;
    end

    %dec2hex(bufferAddrs(itr))

    for itrInner=1:itr
        str_commandUnderTest = 'WriteByteBuffer';
        bufferAddr = uint32(bufferAddrs(itrInner));
        byteOffset = uint32(0);
        bufferLength = uint32(bufferSize);
        inputData(itrInner,1:bufferSize) = uint8(randperm(256,bufferSize));
        [m_success] = m_eng.DynamicTestRunnerWriteByteBuffer(bufferAddr, byteOffset, bufferLength, inputData(itrInner,1:bufferSize));
        if (m_success == 1)
            %fprintf('device indicates successful %s.\n',str_commandUnderTest);
        else
            fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
            error_count = error_count + 1;
        end
    end



    for itrInner=1:itr
        bufferAddr = uint32(bufferAddrs(itrInner));
        byteOffset = uint32(0);
        bufferLength = uint32(bufferSize); %TODO: get the size from the device.
        [m_success, m_data, m_readLength] = m_eng.DynamicTestRunnerReadByteBuffer(bufferAddr, byteOffset, bufferLength);
        if (m_success ~= 1)
            fprintf('ERROR ReadByteBuffer returned m_success = %d, m_readLength = %d \n',m_success, m_readLength);
            error_count = error_count + 1;
            return;
        end
        if (isequal(inputData(itrInner,1:bufferSize),m_data.uint8) == 0)
            fprintf('ERROR buffer outputData does not match inputData \n')
            error_count = error_count + 1;
            fprintf('buffer at address 0x%X :\n',bufferAddrs(itrInner));
            m_data.uint8
        end

    end

end
fprintf('\n');
if(error_count == errorCountStart)
    fprintf('PASSED test %s.\n',str_testDescription);
else
    fprintf('##############################################################\n');
    fprintf('FAILED test %s.\n##############################################################\n',str_testDescription);
end

%% TEST COMMAND free all buffers
str_commandUnderTest = 'FreeAllBuffers';
fprintf('\n    ## TEST %s ######\n', str_commandUnderTest);
command = uint32(0);
[m_success] = m_eng.DynamicTestRunnerFreeAllBuffers(command);
if (m_success == 0)
    fprintf('ERROR %s returned m_success = %d \n',str_commandUnderTest, m_success);
    error_count = error_count + 1;
else
    %fprintf('device indicates successful %s \n', str_commandUnderTest);
end

%% TEST COMMAND ShowByteBuffers
%TODO: verify no buffers.

%% TEST COMMAND show available command addresses
str_commandUnderTest = 'DynamicTestRunnerShowTests';
fprintf('\n    ## TEST %s ######\n', str_commandUnderTest);
command = uint32(0);
[m_success, m_testAddrs] = m_eng.DynamicTestRunnerShowTests(command);
if (m_success == 0)
    fprintf('ERROR %s returned m_success = %d \n',str_commandUnderTest, m_success);
    error_count = error_count + 1;
else
    %fprintf('device indicates successful %s \n', str_commandUnderTest);
end
testAddrs = m_testAddrs.uint32;
dec2hex(testAddrs)

%% TEST SEQUENCE Adding two buffers into a third buffer.
str_testDescription = 'Subtract<UINT8>';
fprintf('    ## TEST %s ######\n',str_testDescription);
errorCountStart = error_count;
str_commandUnderTest = 'MallocByteBuffer';
bufferSize = uint32(4);
[m_success, m_bufferAddr_len] = m_eng.DynamicTestRunnerMallocByteBuffer(bufferSize);
if (m_success == 1)
    %fprintf('device indicates successful %s.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    disp('did not create buffer on device. might be out of memory.');
    error_count = error_count + 1;
end

str_commandUnderTest = 'MallocByteBuffer';
bufferSize = uint32(10);
[m_success, m_bufferAddr_A] = m_eng.DynamicTestRunnerMallocByteBuffer(bufferSize);
if (m_success == 1)
    %fprintf('device indicates successful %s.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    disp('did not create buffer on device. might be out of memory.');
    error_count = error_count + 1;
end

str_commandUnderTest = 'MallocByteBuffer';
bufferSize = uint32(10);
[m_success, m_bufferAddr_B] = m_eng.DynamicTestRunnerMallocByteBuffer(bufferSize);
if (m_success == 1)
    %fprintf('device indicates successful %s.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    disp('did not create buffer on device. might be out of memory.');
    error_count = error_count + 1;
end

str_commandUnderTest = 'MallocByteBuffer';
bufferSize = 10;
param_bufferSize = uint32(bufferSize);
[m_success, m_bufferAddr_C] = m_eng.DynamicTestRunnerMallocByteBuffer(param_bufferSize);
if (m_success == 1)
    %fprintf('device indicates successful %s.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    disp('did not create buffer on device. might be out of memory.');
    error_count = error_count + 1;
end

% fprintf('m_buffferAddrs: 0x%X 0x%X 0x%X  \n', m_bufferAddr_A, m_bufferAddr_B, m_bufferAddr_C);

str_commandUnderTest = 'WriteByteBuffer';
bufferAddr = uint32(m_bufferAddr_len);
byteOffset = uint32(0);
writeLength = 4; %sizeof(UINT32)
inputData = 10;
param_writeLength = uint32(writeLength);
param_inputData_len = typecast(uint32(inputData),'uint8'); % assumes endianness matches between host PC and target device.
[m_success] = m_eng.DynamicTestRunnerWriteByteBuffer(bufferAddr, byteOffset, param_writeLength, param_inputData_len);
if (m_success == 1)
    %fprintf('device indicates successful %s.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

str_commandUnderTest = 'WriteByteBuffer';
bufferAddr = uint32(m_bufferAddr_A);
byteOffset = uint32(0);
writeLength = bufferSize;
inputData = zeros([1,writeLength]);
inputData = inputData + 129;
param_writeLength = uint32(writeLength);
param_inputData_A = uint8(inputData);
[m_success] = m_eng.DynamicTestRunnerWriteByteBuffer(bufferAddr, byteOffset, param_writeLength, param_inputData_A);
if (m_success == 1)
    %fprintf('device indicates successful %s.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

str_commandUnderTest = 'WriteByteBuffer';
bufferAddr = uint32(m_bufferAddr_B);
byteOffset = uint32(0);
writeLength = bufferSize;
inputData = zeros([1,writeLength]);
inputData = inputData + 30;
param_writeLength = uint32(writeLength);
param_inputData_B = uint8(inputData);
[m_success] = m_eng.DynamicTestRunnerWriteByteBuffer(bufferAddr, byteOffset, param_writeLength, param_inputData_B);
if (m_success == 1)
    %fprintf('device indicates successful %s.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

str_commandUnderTest = 'Process SUBTRACT<UINT8>';
testAddr = uint32(2); % TestIndex::SubUINT8 = 2, %hex2dec('802adc5')
testArgv = uint32([m_bufferAddr_len m_bufferAddr_A m_bufferAddr_B m_bufferAddr_C]);
testArgc = length(testArgv);
[m_success, m_resultAddr] = m_eng.DynamicTestRunnerProcess(testAddr,testArgc,testArgv);
if (m_success == 1)
    %fprintf('device indicates successful %s.\n',str_commandUnderTest);
else
    fprintf('ERROR device indicates %s failed.\n', str_commandUnderTest);
    error_count = error_count + 1;
end

resultAddr = m_resultAddr;
if(resultAddr ~= m_bufferAddr_C+8) %specific to function.  SUBTRACT gives the data address of the buffer which is 8 bytes away from the MATLAB address.
    fprintf('ERROR device returned different result than what %s requested.\n',str_commandUnderTest);
    error_count = error_count + 1;
end

str_commandUnderTest = 'ReadByteBuffer';
bufferAddr = uint32(m_bufferAddr_C);
byteOffset = uint32(0);
bufferLength = uint32(bufferSize); %TODO: get the size from the device.
[m_success, m_data, m_readLength] = m_eng.DynamicTestRunnerReadByteBuffer(bufferAddr, byteOffset, bufferLength);
if (m_success ~= 1)
    fprintf('ERROR %s returned m_success = %d \n',str_commandUnderTest, m_success);
    error_count = error_count + 1;
end

expectedResult_C = param_inputData_A - param_inputData_B;
if(isequal(expectedResult_C, m_data.uint8) == 0)
    fprintf('ERROR %s returned unexpected result \n');
    error_count = error_count + 1;
    expectedResult_C
    m_data.uint8
end

if(error_count == errorCountStart)
    fprintf('PASSED test %s.\n',str_testDescription);
else
    fprintf('##############################################################\n');
    fprintf('FAILED test %s.\n##############################################################\n',str_testDescription);
end

%% TEST CreateCLR_RT_TypedArray_INT16
% PROBLEM! functions execute in a different workspace and cannot see the
% loaded DLL.
str_commandUnderTest = 'CreateCLR_RT_TypedArray_INT16';
fprintf('\n    ## TEST %s ######\n', str_commandUnderTest);
%inputData = [hex2dec('CAFE') hex2dec('BABE') hex2dec('BADD') hex2dec('F00D')]
inputData = [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16]
buffer = inputData;
CreateCLR_RT_TypedArray_INT16;
m_INT16addr;
dec2hex(m_INT16addr)
ReadCLR_RT_TypedArray_INT16;
if (isequal(inputData, m_INT16buffer) == 0)
    fprintf('ERROR reading back %s \n',str_commandUnderTest);
    error_count = error_count + 1;
    inputData
    m_INT16buffer
end


%% report
fprintf('\nerrors: %d \n',error_count);


%% Cleanup
m_eng.Stop();
m_eng.Dispose();
clear m_eng;


