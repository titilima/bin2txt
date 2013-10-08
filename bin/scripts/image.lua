-- bin2txt ��ʽת���ű�
-- ��ʽ˵����ʮ������ӳ���ʽ
-- ���ߣ�����

function fmt(ch)
    return string.format("%02x ", string.byte(ch))
end

function bin2txt(str)
    s1 = string.gsub(str, ".", fmt)
    s2 = string.gsub(str, "[^%a%d%p]", ".")
    if (string.len(str) < 16) then
        -- �ÿո������һ��
        s1 = s1 .. string.rep("   ", 16 - string.len(str))
    end
    return s1 .. "  " .. s2 .. "\r\n"
end
