local joestar_globals = {}

 setmetatable(_G, {
    __newindex = function(t, n, v)
        jsync_l(n, v)
        rawset(joestar_globals, n, v)
    end,
    __index = function(_, n)
        if not joestar_globals[n] then
            error("Attempt to read from undeclared variable: " .. n)
        else
            return joestar_globals[n]
        end
    end,
})

joestar = {}

function get_variable(name)
    return joestar_globals[name]
end
 
function variable_sync(name, value)
    rawset(joestar_globals, name, value)
end

joestar.get_variable = get_variable
joestar.variable_sync = variable_sync

return joestar