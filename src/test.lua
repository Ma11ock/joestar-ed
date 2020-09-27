local joestar = require 'init'

joestar.undo_keep = 40
io.stderr:write("Undo keep is " .. joestar.undo_keep .. "\n")



joestar.undo_keep = 20
io.stderr:write("Undo keep is " .. joestar.undo_keep .. "\n")

joestar.undo_keep = 27
io.stderr:write("Undo keep is " .. joestar.undo_keep .. "\n")

