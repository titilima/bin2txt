-- bin2txt ��ʽת���ű�
-- ��ʽ˵����C/C++ �����ʽ
-- ���ߣ�����

function fmt(ch)
    return string.format("0x%02x, ", string.byte(ch))
end

function bin2txt(s)
    ret = string.gsub(s, ".", fmt);
    -- �޼����һ���ո񣬲�����
    return string.sub(ret, 1, -2) .. "\r\n"
end
