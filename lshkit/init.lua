local ffi = require "ffi"
require 'torchffi'
local async = require 'async'

ffi.cdef
[[ 
    typedef void Environment;
    
    Environment *init(int dim, int N, float *data_block, unsigned K, float R, bool doTuning, const char *indexFile, int L, int T, int M, float W);
    void query(Environment *env, float *queryData, int *response);
]]


local luash = {}

-- must have a dataTensor
-- options.indexParams must provide 

--TODO: make the similarity table just have the proper float tensor in it instead of converting here, to allow shared RAM
luash.getDataTensor = function(similarityTable)
   local N = similarityTable.public_vectors:size(1)
   local dim = similarityTable.public_vectors:size(2)

   local data_tensor = torch.FloatTensor(dim * N):copy(similarityTable.public_vectors):resize(similarityTable.public_vectors:size())
   local multipliers = similarityTable.public_multipliers:resize(N,1)

   data_tensor:cmul(multipliers:expandAs(data_tensor))

   return data_tensor, similarityTable
end

luash.init = function(dataTensor, options)
  
   local clib = ffi.load(options.libdir .. "luash.so")

   if not dataTensor then error("Must provide a data tensor") end

   options = options or {}

   local indexFile = options.indexFile or './similarity.1.index'
   local doTune = (options.doTune and true) or false;
   local indexParams = options.indexParams or {}

   -- TODO: check that these vars are actually properly described...
   indexParams.K = indexParams.K or 5 -- number of results we want returned
   indexParams.R = indexParams.R or 0.9 -- precision (0.9 means if image I has KNN including image I', I' has a probability 0.9 of being returned
   indexParams.L = indexParams.L or 3 -- number of hashes
   indexParams.T = indexParams.T or 5 -- number of probes done in multiprobe LSH (multiprobe does multiple searches to avoid hitting edges of buckets)

   if doTune then
      indexParams.M = indexParams.M or -1 -- number of buckets per hash
      indexParams.W = indexParams.W or -1 -- window size 
   else
      if indexParams.M == nil or indexParams.M == -1 or indexParams.W == nil or indexParams.W == -1 then
         error("Must either provide M and W or set doTune = true")
      end
   end

   local environ = clib.init(options.dim, options.N, torch.data(dataTensor), 
                             indexParams.K, indexParams.R, doTune,
                             indexFile, indexParams.L, indexParams.T,
                             indexParams.M, indexParams.W)

   local index = {
      query = function(queryTensor)
         local responseTensor = torch.IntTensor(indexParams.K)
         
         local response = clib.query(environ, torch.data(queryTensor), torch.data(responseTensor));

         local similar_indexes = {}

         -- TODO: don't know what happens when we get no response... is it possible to get fewer than K results? 
         for i = 1,responseTensor:size()[1] do
            table.insert(similar_indexes, responseTensor[i] + 1)
         end

         return similar_indexes
      end
   }

   return index
end

return luash

