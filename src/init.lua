 setmetatable(joestar, {
--    __index = function(t, n, v)
--        jsync_l(n, v)
--        rawset(joestar, n, v)
--    end,
    __newindex = function(t, n, v)
        -- get var check what type of error (if any)
        joestar.jsync_l(n, v)
        rawset(joestar, n, v)
    end,
})



return joestar