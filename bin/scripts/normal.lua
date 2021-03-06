-- bin2txt 格式转换脚本
-- 格式说明：普通十六进制格式
-- 作者：李马

function fmt(ch)
    return string.format("%02x ", string.byte(ch))
end

function bin2txt(s)
    ret = string.gsub(s, ".", fmt);
    -- 修剪最后一个空格，并换行
    return string.sub(ret, 1, -2) .. "\r\n"
end
