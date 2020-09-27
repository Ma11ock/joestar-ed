local _joestar = joestar_internal
local joestar = {}

setmetatable(joestar, {
    __index = function(t, n, v)
        return _joestar[n]
    end,
    __newindex = function(t, n, v)
        -- get var check what type of error (if any)
        if not _joestar.jsync_l(n, v) then
            error("Could not set value " .. v .. " for var " .. n)
        else
            rawset(_joestar, n, v)        
        end
    end,
})



return joestar