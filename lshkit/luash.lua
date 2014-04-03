local ffi = require "ffi"
ffi.cdef
[[
   int run(void)
]]

local clib = ffi.load('./luash.so')
local lshkit = ffi.load('./liblshkit.so')

local similarityTable = torch.load('../../data/SimilarityTable.1.m')

local index = clib.init()

for i=1,similarityTable.public_vectors:size(1) do
   clib.insert(index, i, similarityTable.public_vectors[i] * similarityTable.public_multipliers[i])
end

print(clib.query(index, similarityTable.public_vectors[44] * similarityTable.public_multipliers[44])

