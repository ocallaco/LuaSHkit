#include <boost/program_options.hpp>
#include <boost/progress.hpp>
#include <boost/format.hpp>
#include <boost/timer.hpp>
#include "luash.h"
#include "mplsh-fit-tune.h"

using namespace std;
using namespace lshkit;
namespace po = boost::program_options; 


Environment *init(int dim, int N, float *data_block){
    Environment *env = new Environment;

    env->dim = dim;
    Index *index = new Index();
    env->data = new FloatMatrix(dim,N,data_block);

    FloatMatrix data = *(env->data);

    env->metric = new metric::l1<float>(data.getDim());
    env->accessor = new FloatMatrix::Accessor(data);
    
    string index_file = "./test.index";

    unsigned H = 1017881;
    unsigned K = 10;
    int L = 3, T = 5;
    float R =  0.9;
    int default_M = 15;
    float default_W = 11;
    
    bool do_benchmark = true;
    bool use_index = false; // load the index from a file

    bool index_loaded = false;

    ifstream is(index_file.c_str(), ios_base::binary);
    if (is) {
        is.exceptions(ios_base::eofbit | ios_base::failbit | ios_base::badbit);
        cout << "LOADING INDEX..." << endl;
        timer.restart();
        index->load(is);
        BOOST_VERIFY(is);
        cout << boost::format("LOAD TIME: %1%s.") % timer.elapsed() << endl;
        index_loaded = true;
    }

    if (!index_loaded) {

        int M = -1;
        float W = -1;
        double cost = -1;
        try
        {
            cout << "TRYING HERE " << endl;
            cout << flush;
            cost = mplsh_fit_tune(*(env->data), *(env->metric), L, T, M, W, R, K);
            cout << "SUCCESS? " << endl;
            cout << flush;
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
        cout << "T" << T << endl;
        cout << "M " << M << endl;
        cout << "W " << W << endl;


        Index::Parameter param;
        param.W = W;
        param.range = H; // See H in the program parameters.  You can just use the default value.
        param.repeat = M;
        param.dim = data.getDim();
        DefaultRng rng;

        index->init(param, rng, L);

        cout << "BUILDING THE INDEX" << endl;
        for (unsigned i = 0; i < data.getSize(); ++i)
        {
            // Insert an item to the hash table.
            // Note that only the key is passed in here.
            // MPLSH will get the feature from the accessor.
            index->insert(i, data[i]);
        }

        //Write the index
        cout << "SAVING INDEX..." << endl;
        {
            ofstream os(index_file.c_str(), ios_base::binary);
            os.exceptions(ios_base::eofbit | ios_base::failbit | ios_base::badbit);
            index->save(os);
        }

    }
    cout << "DONE" << endl;

    env->index = index;
    return env;
}

void tune(Environment *env){

}

void query(Environment *env, float *queryData){
    unsigned K = 10;
    float R = 0.9;
    
    cout << "TEST 0" << endl;
    cout << flush;

    //FloatMatrix data = *(env->data);
    
    cout << "TEST 1" << endl;
    cout << flush;
    //vector<float> *q = new vector<float>();      
    //q->assign(queryData, queryData + data.getDim());
    
    TopkScanner<FloatMatrix::Accessor, metric::l1<float> > query(*(env->accessor), *(env->metric), K, R);
    
    cout << "TEST 2" << endl;
    cout << flush;

    query.reset(queryData);
    
    cout << "TEST 3" << endl;
    cout << flush;
    
    (*(env->index))->query(queryData, 5, query);
    
    cout << "TEST 4" << endl;
    cout << flush;
    
//    cout << query.topk().recall(query.topk()) << endl;
//    cout << "ANSWER" << endl;
//    cout << query.topk().size()<< endl;
//    cout << query.topk()[1].dist << endl;
//    cout << flush;
}

void query(Environment *env, int queryIndex){
//    FloatMatrix data = *(env->data);
//    query(env, data[queryIndex]);
    cout << "SOMETHING ELSE" << endl;
    cout << flush;
}

