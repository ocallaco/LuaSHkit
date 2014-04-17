#include <boost/program_options.hpp>
#include <boost/progress.hpp>
#include <boost/format.hpp>
#include <boost/timer.hpp>
#include "luash.h"
#include "mplsh-fit-tune.h"

using namespace std;
using namespace lshkit;
namespace po = boost::program_options; 


Environment *init(int dim, int N, float *data_block, unsigned K, float R, bool doTuning, const char *indexFile, int L, int T, int M, float W){
    Environment *env = new Environment;
    Index *index = new Index();
    
    env->data = new FloatMatrix(dim,N,data_block);

    FloatMatrix *data = env->data;

    env->metric = new metric::l2<float>(data->getDim());
    env->accessor = new FloatMatrix::Accessor(*data);
    env->R = R;
    env->K = K;
    env->dim = dim;
    
    unsigned H = 1017881;
    int default_M = 15;
    float default_W = 11;
    
    bool do_benchmark = true;
    bool use_index = false; // load the index from a file

    bool index_loaded = false;

    if(!doTuning){
        ifstream is(indexFile, ios_base::binary);
        if (is) {
            is.exceptions(ios_base::eofbit | ios_base::failbit | ios_base::badbit);
            cout << "LOADING INDEX..." << endl;
            index->load(is);
            BOOST_VERIFY(is);
            index_loaded = true;
        }
    }
    else {
        double cost = -1;
        
        M = -1;
        W = -1;

        try
        {
            cost = mplsh_fit_tune(*(data), *(env->metric), L, T, M, W, R, K);
            default_M = M;
            default_W = W;
        }
        catch(...)
        {
            std::cout << "Fit-Tune Crash !!" << std::endl;
            M = default_M;
            W = default_W;
        }

        cout << "TUNING DONE!" << endl;
        cout << "cost " << cost << endl;
        cout << "L " << L << endl;
        cout << "T " << T << endl;
        cout << "M " << M << endl;
        cout << "W " << W << endl;
    }


    if(!index_loaded){
        Index::Parameter param;
        param.W = W;
        param.range = H; // See H in the program parameters.  You can just use the default value.
        param.repeat = M;
        param.dim = data->getDim();
        DefaultRng rng;

        index->init(param, rng, L);

        cout << "BUILDING THE INDEX" << endl;
        for (unsigned i = 0; i < data->getSize(); ++i)
        {
            // Insert an item to the hash table.
            // Note that only the key is passed in here.
            // MPLSH will get the feature from the accessor.
            index->insert(i, (*data)[i]);
        }

        //Write the index
        cout << "SAVING INDEX..." << endl;
        {
            ofstream os(indexFile, ios_base::binary);
            os.exceptions(ios_base::eofbit | ios_base::failbit | ios_base::badbit);
            index->save(os);
        }
    }

    cout << "DONE" << endl;

    env->index = index;
    return env;
}

void query(Environment *env, float *queryData, int *response){
    unsigned K = env->K;
    float R = env->R;

    cout << "WTF?" << queryData[0] << endl;
    
    TopkScanner<FloatMatrix::Accessor, metric::l2<float> > query(*(env->accessor), *(env->metric), K, R);

    query.reset(queryData);
    
    (env->index)->query(queryData, 5, query);

    for(int i = 0; i < K; i++){
        response[i] = query.topk()[i].key;
        cout << query.topk()[i].key << endl;
    }
}

