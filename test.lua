local luash = require './luash/luash.lua'

local similarityTable = torch.load('../data/SimilarityTable.1.m')

dataTensor = luash.getDataTensor(similarityTable)

local indexParams = {K = 5, R = 0.9}

local index = luash.init(dataTensor, {doTune = false, indexFile = './data/similarity.1.index', indexParams = indexParams})

result = index.query()


                                 


