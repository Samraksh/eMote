%function [ m_UINT16buffer ] = ReadCLR_RT_TypedArray_UINT16(m_UINT16addr)
% create a CLR_RT_TypedArray<UINT16> on the device.
% This is represented by a pointer to the data, the length, and the data,
% so we need sizeof(buffer) + 8 bytes.

%TODO: verify GCC4.7.3 doesn't have extra information at the top of the
%struct and our padding works with whatever variable it currently is.

%assumes DBG connected and created m_eng object to the
%Microsoft.SPOT.Debugger.dll

buffer_real_offset_from_reported_address = 8; %depends on MatlabCom array implementation because real array starting addresses are not used as IDs.
data_offset_from_CLR_struct_start = 8;
sizeof_uint16 = 2;

% write pointer to data
clear m_succes;
clear m_data;
clear m_readLength;
%m_data = zeros(1,4);
[m_success, m_data, m_readLength] = m_eng.DynamicTestRunnerReadByteBuffer(uint32(m_UINT16addr), uint32(4), uint32(4));
if (m_success == 0)
    fprintf('ERROR ReadByteBuffer failed\n');
end

bufferSize = typecast(m_data,'uint32');

m_data = zeros(1,bufferSize);
[m_success, m_data, m_readLength] = m_eng.DynamicTestRunnerReadByteBuffer(uint32(m_UINT16addr), uint32(data_offset_from_CLR_struct_start), uint32(bufferSize*sizeof_uint16));
if (m_success == 0)
    fprintf('ERROR ReadByteBuffer failed\n');
end

m_UINT16buffer = typecast(m_data,'uint16');
