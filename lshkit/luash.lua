local ffi = require "ffi"

require 'torchffi'
local async = require 'async'

ffi.cdef
[[ 
   typedef void Environment;
    
    Environment *init(int dim, int N, float *data_block);
    void query(Environment *env, int queryIndex);
]]

local clib = ffi.load('./luash.so')

local similarityTable = torch.load('../../data/SimilarityTable.1.m')

local N = similarityTable.public_vectors:size(1)
local dim = similarityTable.public_vectors:size(2)

local data_tensor = torch.FloatTensor(dim * N):copy(similarityTable.public_vectors):resize(similarityTable.public_vectors:size())
local multipliers = similarityTable.public_multipliers:resize(N,1)

data_tensor:cmul(multipliers:expandAs(data_tensor))
   
print("STARTING")

local environ = clib.init(dim, N, torch.data(data_tensor))

local time = async.hrtime()
for i=565,665 do
--   print("A")
--   io.flush()
   clib.query(environ, i)
--   print("B")
--   io.flush()
end

local second_time = async.hrtime()
print("Completed 100 queries in ", (second_time - time) / 1000, " seconds")
print((second_time - time) / 100, " ms per query")

