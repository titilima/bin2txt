-- bin2txt ��ʽת���ű�
-- ��ʽ˵����������Ը�ʽ
-- ���ߣ�����

function fmt(ch)
    return string.format("%02xh, ", string.byte(ch))
end

function bin2txt(s)
    ret = string.gsub(s, ".", fmt);
    -- �޼����һ���ո񣬲�����
    return string.sub(ret, 1, -2) .. "\r\n"
end
