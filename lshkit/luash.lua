local ffi = require "ffi"
ffi.cdef
[[
   int run(void)
]]

local clib = ffi.load('./luash.so')
local lshkit = ffi.load('./liblshkit.so')

clib.run()

