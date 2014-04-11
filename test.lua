local luash = require './lshkit'

local similarityTable = torch.load('../data/SimilarityTable.1.m')

dataTensor = luash.getDataTensor(similarityTable)

local indexParams = {K = 5, R = 0.9, L = 3, T = 5, M = 20, W = 749.62}

local N = similarityTable.public_vectors:size(1)
local dim = similarityTable.public_vectors:size(2)

local index = luash.init(dataTensor, {N = N, dim = dim, doTune = false, indexFile = './data/similarity.1.index', indexParams = indexParams, libdir = "./lib/"})

result = index.query(dataTensor[454])

print(result)

                                 


