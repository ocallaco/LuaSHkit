local ffi = require "ffi"

require 'torchffi'


ffi.cdef
[[
    typedef void Environment;
    
    Environment *init(int dim, int N, float *data_block);
    int query(Environment *env, float *queryData);
]]

local clib = ffi.load('./luash.so')
local lshkit = ffi.load('./liblshkit.so')

local similarityTable = torch.load('../../data/SimilarityTable.1.m')

local N = similarityTable.public_vectors:size(1)
local dim = similarityTable.public_vectors:size(2)

local data_tensor = torch.FloatTensor(dim * N):copy(similarityTable.public_vectors):resize(similarityTable.public_vectors:size())
local multipliers = similarityTable.public_multipliers:resize(N,1)

data_tensor:cmul(multipliers:expandAs(data_tensor))

local environ = clib.init(dim, N, torch.data(data_tensor))

print(clib.query(environ, torch.data(data_tensor[44])))

