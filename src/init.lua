local joestar = {}

 setmetatable(joestar, {
    __newindex = function(t, n, v)
        jsync_l(n, v)
        rawset(joestar, n, v)
    end,
})
return joestar