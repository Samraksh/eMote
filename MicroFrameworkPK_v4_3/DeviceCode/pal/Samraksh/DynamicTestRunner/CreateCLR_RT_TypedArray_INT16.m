%function [ m_INT16addr ] = CreateCLR_RT_TypedArray_INT16(buffer)
% create a CLR_RT_TypedArray<UINT16> on the device.
% This is represented by a pointer to the data, the length, and the data,
% so we need sizeof(buffer) + 8 bytes.

%TODO: verify GCC4.7.3 doesn't have extra information at the top of the
%struct and our padding works with whatever variable it currently is.

%assumes DBG connected and created m_eng object to the
%Microsoft.SPOT.Debugger.dll

buffer_real_offset_from_reported_address = 8; %depends on MatlabCom array implementation because real array starting addresses are not used as IDs.
data_offset_from_CLR_struct_start = 8;
sizeof_int16 = 2;

[m_success, m_INT16addr] = m_eng.DynamicTestRunnerMallocByteBuffer(uint32((length(buffer)*sizeof_int16)+8));
if (m_success == 0)
    fprintf('ERROR MallocByteBuffer failed\n');
end
dec2hex(m_INT16addr)

% write pointer to data
[m_success] = m_eng.DynamicTestRunnerWriteByteBuffer(uint32(m_INT16addr), uint32(0), uint32(4),  typecast(uint32(m_INT16addr + buffer_real_offset_from_reported_address + data_offset_from_CLR_struct_start),'uint8'));
if (m_success == 0)
    fprintf('ERROR WriteByteBuffer failed\n');
end

% write length of data
[m_success] = m_eng.DynamicTestRunnerWriteByteBuffer(uint32(m_INT16addr), uint32(4), uint32(4),  typecast(uint32(length(buffer)),'uint8'));
if (m_success == 0)
    fprintf('ERROR WriteByteBuffer failed\n');
end

% write data
[m_success] = m_eng.DynamicTestRunnerWriteByteBuffer(uint32(m_INT16addr), uint32(8), uint32(length(buffer)*sizeof_int16),  typecast(int16(buffer),'uint8'));
if (m_success == 0)
    fprintf('ERROR WriteByteBuffer failed\n');
end

