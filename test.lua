local luash = require './lshkit'

local similarityTable = torch.load('./data/SimilarityTable.400k.m')

dataTensor = luash.getDataTensor(similarityTable)

local indexParams = {K = 10, R = 0.9, L = 10, T = 15, M = 20, W = 386.476}

local N = similarityTable.public_vectors:size(1)
local dim = similarityTable.public_vectors:size(2)

local index = luash.init(dataTensor, {N = N, dim = dim, doTune = true, indexFile = './data/similarity.1.400k.index', indexParams = indexParams, libdir = "./lib/"})

result = index.query(dataTensor[454])

print(result)

                                 


