-- bin2txt 格式转换脚本
-- 格式说明：十六进制映像格式
-- 作者：李马

function fmt(ch)
    return string.format("%02x ", string.byte(ch))
end

function bin2txt(str)
    s1 = string.gsub(str, ".", fmt)
    s2 = string.gsub(str, "[^%a%d%p]", ".")
    if (string.len(str) < 16) then
        -- 用空格补齐最后一行
        s1 = s1 .. string.rep("   ", 16 - string.len(str))
    end
    return s1 .. "  " .. s2 .. "\r\n"
end
